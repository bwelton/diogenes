import sys
import os
import struct
from  ReadStackFiles import StackReader
from LS_TraceBin import LS_TraceBin
from TF_trace import TF_Trace
from FI_TraceBin import FI_TraceBin

class MatchTimeToLSStack:
    def __init__(self, TF_timekey, LS_stackkey):
        self._tf = TF_timekey
        self._ls = LS_stackkey

    def CompareStacks(self, ls, tf):
        #ls stack will always be longer
        start = 0
        for x in ls:
            found = False
            nextPos = start
            for y in range(start, len(tf)):
                nextPos += 1
                if x == tf[y]:
                    found = True
                    break
                elif start != 0:
                    found = False
                    break
            if found == False:
                return False
            start = nextPos


        # for x in range(0,len(ls)):
        #     found = False
        #     for y in range(0,len(tf)):
        #         if ls[x] == tf[y]:
        #             found = True
        #             break
        #     if (found == False):
        #         return False

            #if str(ls[x]) != str(tf[x]):
            #    return False
        return True


    def PruneLSStack(self,stack):
        ret = []
        for x in range(0, len(stack)):
            if "libcuda.so" in stack[x]._libname:
                continue
            else:
                ret.append(stack[x])
        return ret
    def PruneStack(self, stack):
        ## Remove all elements at the beginning of the stack such as 
        ## startmain.
        removeElements = []
        for x in range(0, len(stack)):
            if "/lib64/libc" in stack[x]._libname or "main" == stack[x]._funcname:
                removeElements.append(x)
            else:
                break

        removeElements = list(set(removeElements))
        ret = []
        for x in range(0, len(stack)):
            if x in removeElements:
                continue
            else:
                ret.append(stack[x])

        return ret

    def GetMatchSet(self):
        # returns [(TFNum,LSNum)]
        ret = []
        tf_stacks = self._tf.GetAllStacks()
        ls_stacks = self._ls.GetAllStacks()
        for x in tf_stacks:
            individaulStack = tf_stacks[x].GetStack()
            pruned = self.PruneStack(individaulStack)
            for y in ls_stacks:
                if self.CompareStacks(self.PruneLSStack(ls_stacks[y].GetStack()), pruned):
                    ret.append([tf_stacks[x]._ident,ls_stacks[y]._ident])
        print "MATCH SET"
        print ret
        return ret


class DuplicateEntry:
    def __init__(self, overwrite, prevTransferHash):
        self._overwrite = overwrite
        self._prevTransferHash = prevTransferHash

    def IsDuplicate(self):
        if self._overwrite == 1 or self._prevTransferHash != 0:
            return True
        return False

    def __str__(self):
        return "Prev Transfer: " + str(self._prevTransferHash) + "| Overwrite: " + str(self._overwrite)


class TimingEntry:
    def __init__(self, useStack, timeval):
        self._useStack = useStack
        self._timeVal = timeval

    def GetUse(self):
        return self._useStack

    def GetTimeVal(self):
        return self._timeVal

class Synchronization:
    def __init__(self, stack, stackID):
        self._stack = stack
        self._stackID = stackID
        self._timingData = []
        self._useStacks = []
        self._useTimes = []


    def AddTFTime(self, entry):
        self._timingData.append(entry)

    def AddUse(self, stack):
        self._useStacks.append(stack)

    def AddUseTime(self, entry):
        self._useTimes.append(entry)

    def GetCallCount(self):
        return len(self._timingData)

    def GetNumNonUses(self):
        return len(self._timingData) - len(self._useStacks) 

    def GetTimeSaved(self):
        timingDataAvg = 0.0
        useAverage = 0.0
        assert len(self._timingData) >= len(self._useStacks)
        if len(self._timingData) == 0:
            return 0.0
        for x in self._timingData:
            timingDataAvg += float(x[4])
        timingDataAvg = timingDataAvg / len(self._timingData)
        for x in self._useStacks:
            if x.GetTimeVal() > 0:
                useAverage += float(x.GetTimeVal())
            else:
                useAverage += timingDataAvg
        if len(self._useStacks) == 0:
            useAverage = 0.0
        else:
            useAverage = useAverage / len(self._useStacks)

        if useAverage > timingDataAvg:
            return timingDataAvg * float(len(self._timingData))

        totalSaved = 0.0
        return useAverage * float(len(self._useStacks)) + timingDataAvg * float(len(self._timingData) - len(self._useStacks))



    def CopyUses(self, ls_stack):
        self._useStacks = ls_stack._useStacks

    def CompareToDupData(self, dupData):
        dupStack = dupData._stack.GetStack()
        my_entries =  self._stack.GetStack()
        ## We should be a subset of the duplicate data stack
        curPos = 0
        for x in my_entries:
            found = False
            for y in range(curPos,len(dupStack)):
                if str(dupStack[y]) == str(x):
                    curPos = y + 1
                    found = True
                    break
            if found == False:
                return False

        return True

    def GetFITimes(self):
        retList = []
        prevTime = 0.0
        for x in self._useStacks:
            if x.GetUse() == None:
                prevTime = x.GetTimeVal()
            else:
                retList.append(TimingEntry(x.GetUse(), x.GetTimeVal() - prevTime))
                prevTime = x.GetTimeVal()
        return retList
           
    def CompareLStoFI(self, fi_sync):
        ## these stacks should be equal in length, captured via the same exact methods
        my_entries = self._stack.GetStack()
        fi_entries =  fi_sync._stack.GetStack()
        if len(my_entries) != len(fi_entries):
            return False

        for x in range(0, len(my_entries)):
            if str(my_entries[x]) != str(fi_entries[x]):
                return False

        return True

    def CompareLStoTF(self, tf_stack):
        my_entries = self._stack.GetStack()
        tf_stack =  tf_stack._stack.GetStack()

        ## LS stack should be a subset of the TF_stack
        curPos = 0
        for x in my_entries:
            found = False
            for y in range(curPos, len(tf_stack)):
                entryX = str(x)
                entryY = str(tf_stack[y])
                if "libcudart.so" in entryX:
                    entryX = "libcudart.so@" + entryX.split("@")[1]
                if "libcudart.so" in entryY:
                    entryY = "libcudart.so@" +entryX.split("@")[1]
                # print entryX
                # print entryY
                if entryX == entryY:
                    found = True
                    curPos = y + 1
                    break
            if found == False:
                return False
        return True

    def CopyFirstUses(self, fi_sync):
        if len(fi_sync) != len(self._useStacks):
            print "Difference in number of uses between runs - " + str(len(fi_sync)) + " vs " + str(len(self._useStacks))
        curPos = 0

        for x in range(0,len(self._useStacks)):
            for y in range(curPos, len(fi_sync)):
                assert len(fi_sync[y].GetUse().GetStack()) == 1
                assert len(self._useStacks[x].GetUse().GetStack()) == 1
                if str(fi_sync[y].GetUse().GetStack()[0]) == str(self._useStacks[x].GetUse().GetStack()[0]):
                    self._useStacks[x]._timeVal = fi_sync[y]._timeVal
                    curPos = y+1
                    break
        for x in self._useStacks:
            if x._timeVal == 0:
                print "Error: One of the timevals in this stack does not have a matching use"
                print str(x._useStack)
                print str(self._stackID)





class DataTransfer:
    def __init__(self, hashVal, position, stack):
        self._hash = hashVal
        self._pos = position
        self._stack = stack
        self._preSync = []
        self._totalTime = []
        self._cpuOverhead = []
        self._duplicates = []
        self._inplace = []

    def HashStackDataTransfer(self):
        return self._stack.HashStackDataTransfer()

    def FindSavings(self):
        overwriteIssues = 0
        previousTransfers = 0
        previousTransferList = []
        totalTimeSavings = 0.0
        for x in range(0,len(self._duplicates)):
            if self._duplicates[x].IsDuplicate():
                if len(self._totalTime) > x:
                    totalTimeSavings += self._totalTime[x]
                if self._duplicates[x]._overwrite == 1:
                    overwriteIssues += 1
                else:
                    previousTransfers += 1
                    previousTransferList.append(self._duplicates[x]._prevTransferHash)
        return [totalTimeSavings, overwriteIssues, previousTransferList]

    def Analysis(self):
        ret = {}
        ret["id"] = str(self._pos)
        ret["Total Time Aggragate"] = str(sum(self._totalTime))
        ret["CPU Overhead Aggragate"] = str(sum(self._cpuOverhead))
        ret["Pre-transfer Synchronization Time Aggragate"] = str(sum(self._preSync))
        ret["Call Count"] = str(len(self._preSync))
        ret["Cuda Call"] = self._stack.TransGetFirstLibCuda()
        ret["First User Call"] = self._stack.TransGetFirstUserCall()
        ret["Duplicate Count"] = str(len(self._duplicates))
        savings = self.FindSavings()
        ret["Estimated Savings"] = str(savings[0])
        ret["Overwrite Issues"] = str(savings[1])
        ret["Duplicate Matches Transfer"] = ""
        for x in savings[2]:
            ret["Duplicate Matches Transfer"] += str(x) + " "
        return ret

    def CreateOutput(self, idToHash):
        ret = self.Analysis(idToHash)
        return ret["id"] + "," + ret["Call Count"] + "," + ret["Cuda Call"] + "," + ret["First User Call"] + "," + ret["Duplicate Count"] + "," + ret["Estimated Savings"] + "," + ret["Overwrite Issues"]  + "," +  ret["Duplicate Matches Transfer"]

    def PrintList(self, name, nlist):
        ret = str(name) + " - Count =  " + str(len(nlist)) + " : "
        for i in nlist:
            ret += str(i) + " "
        ret += "\n"
        return ret

    def GetFullOutput(self):
        ret = ""
        ret += str(self._stack)
        ret += self.PrintList("Total Time" , self._totalTime)
        ret += self.PrintList("CPU Overhead" , self._cpuOverhead)
        ret += self.PrintList("Pre Trans Sync" , self._preSync)
        ret += self.PrintList("Duplicates" , self._duplicates)
        #ret += self.PrintList("Copys over Inplace" , self._preSync)
        return ret

    def AddTotalTime(self, time):
        self._totalTime.append(time)

    def AddCPUOverhead(self, time):
        self._cpuOverhead.append(time)


    def FixDuplicateID(self, idMap):
        for x in self._duplicates:
            if x._prevTransferHash > 0:
                x._prevTransferHash = idMap[x._prevTransferHash]

    def AddDuplicate(self, dupEntry):
        self._duplicates.append(dupEntry)

    def InPlace(self, inplace):
        self._inplace.append(inplace);

    def PreTransSynchronization(self, time):
        self._preSync.append(time)

    def CopyDuplicates(self, other):
        self._duplicates = other._duplicates



class ReadTransferCollisions:
    def __init__(self, filename):
        self._filename = filename
        self._pos = 0
        try:
            f = open(self._filename,"rb")
            self._data = f.read()
            f.close()
        except:
            self._data = ""


    def DecodeRecord(self):
        if self._pos >= len(self._data):
            return None
        tmp = struct.unpack_from("QQQQ", self._data, offset=self._pos)
        self._pos += (8*4)
        return list(tmp)

class Driver:
    def __init__(self):
        self._inDir = os.getcwd()
        self._outDir = os.getcwd()

    def GetSynchronizationType(self, stacks):
        ret = {}
        for x in stacks:
            ret[int(x)] = Synchronization(stacks[x], int(x))
        return ret

    def Run(self):
        ## TIme in Seconds
        TIME = 109.00
        self._stackStore = {}
        ## LS_stackkey -> Necessary Synchronization
        ## LS_tracekey -> Locations of first access
        ## TF_timekey -> Timing for each synchronization.
        stack_files = ["LS_stackkey.txt", "LS_tracekey.txt", "TF_timekey.txt","FI_tracekey.txt","FI_stackkey.txt"]

        for x in stack_files:
            self._stackStore[x] = StackReader(os.path.join(self._inDir, x))
        self._stackStore["TF_timekey.txt"].InverseStacks()
        for x in self._stackStore:
            print str(self._stackStore[x])

        ## LS_trace Reader
        ls_trace = LS_TraceBin(os.path.join(self._inDir, "LS_trace.bin"))
        ls_trace.DecodeFile()
        #print str(ls_trace)

        ## TF_Trace Decorder 
        tf_trace = TF_Trace(os.path.join(self._inDir, "TF_trace.bin"))
        tf_trace.DecodeFile()
        #print str(tf_trace)

        self._syncStacks = self.GetSynchronizationType(self._stackStore["TF_timekey.txt"].GetAllStacks())
        for x in tf_trace._records:
            # entry: dynID,stackId,count,time
            if int(x[2]) in self._syncStacks:
                self._syncStacks[x[2]].AddTFTime(x)
            else:
                print "ERROR: Could not find stack id - " + str(x[1])

        ls_stacks = self.GetSynchronizationType(self._stackStore["LS_stackkey.txt"].GetAllStacks())
        ls_usepoints = self.GetSynchronizationType(self._stackStore["LS_tracekey.txt"].GetAllStacks())
        for x in ls_trace._entriesMap:
            if x in ls_stacks:
                for y in ls_trace._entriesMap[x]:
                    ls_stacks[x].AddUse(TimingEntry(ls_usepoints[y]._stack, 0.0))
            else:
                print "Error: Could not find LS StackKey ID of " + str(x)

        fi_trace = FI_TraceBin(os.path.join(self._inDir, "FI_trace.bin"))
        fi_trace.DecodeFile()
        fi_stacks = self.GetSynchronizationType(self._stackStore["FI_stackkey.txt"].GetAllStacks())
        fi_usepoints = self.GetSynchronizationType(self._stackStore["FI_tracekey.txt"].GetAllStacks())

        for x in fi_trace._entriesMap:
            if x in fi_stacks:
                for y in fi_trace._entriesMap[x]:
                    if y[0] in fi_usepoints:
                        fi_stacks[x].AddUse(TimingEntry(fi_usepoints[y[0]]._stack, float(y[1])))
                    else:
                        fi_stacks[x].AddUse(TimingEntry(None, float(y[1])))
            else:
                print "Error: Could not find FI StackKey ID of " + str(x)

        for x in fi_stacks:
            tmp = fi_stacks[x].GetFITimes()
            if len(tmp) > 0:
                for y in ls_stacks:
                    if ls_stacks[y].CompareLStoFI(fi_stacks[x]):
                        ls_stacks[y].CopyFirstUses(tmp)
                        break

        alreadyMatch = {}
        for x in ls_stacks:
            found = False
            for y in self._syncStacks:
                if y in alreadyMatch:
                    continue
                if ls_stacks[x].CompareLStoTF(self._syncStacks[y]):
                    self._syncStacks[y].CopyUses(ls_stacks[x])
                    alreadyMatch[y] = 1
                    found = True
                    break
            if found != True:
                print "ERROR Could not find LS stack - " + str(x) 
                #assert found == True




        # match = MatchTimeToLSStack(self._stackStore["TF_timekey.txt"], self._stackStore["LS_stackkey.txt"])
        # m = match.GetMatchSet()
        # singleSetTF = []
        # for x in m:
        #     singleSetTF.append(x[0])
        # notInSet = tf_trace.GetNotInSet(singleSetTF)
        # #print notInSet
        # timeSavedEstimate = 0.0
        # for x in notInSet:
        #     timeSavedEstimate += x[-1]
        # tmp_sort = {}
        # for x in notInSet:
        #     if x[2] not in tmp_sort:
        #         tmp_sort[x[2]] = x
        #     else:
        #         tmp_sort[x[2]][-1] += x[-1]
        #         tmp_sort[x[2]][-2] += x[-2]
        # notInSet = []
        # for x in tmp_sort:
        #     notInSet.append(tmp_sort[x])
        # print "\n\n\n\n"
        # print "Estimated time savings from unnecessary synchroniations/data transfers: " + str(timeSavedEstimate)
        # print "Table of unnecessary operation\n"
        # print '%-20.20s | %-20.20s | %-20.20s | %-10.10s | %-20.20s | %-10.10s | %-10.10s' % ('App Bin','App Function','Cuda Call','Stack ID','Type','Call Count', 'Est Impact')
        # outFD = open("results_output.csv", "w")
        # outFD.write('%-20.20s,%-20.20s,%-20.20s,%-10.10s,%-20.20s,%-10.10s,%-10.10s\n' % ('App Bin','App Function','Cuda Call','Stack ID','Type','Call Count', 'Est Impact'))
        # for x in notInSet:
        #     i = x[2]
        #     stack = self._stackStore["TF_timekey.txt"].GetStackAtID(int(i))
        #     ucall = stack.FindFirstUserCall()
        #     ccall = stack.FindFirstLibCuda()
        #     print '%-20.20s | %-20.20s | %-20.20s | %-10.10s | %-20.20s | %-10.10s | %-10.10s' % (ucall.GetFilename(),str(ucall._funcname),str(ccall._funcname),str(i),"Sync",str(x[3]), str(x[4]))
        #     outFD.write('%-20.20s,%-20.20s,%-20.20s,%-10.10s,%-20.20s,%-10.10s,%-10.10s\n' % (ucall.GetFilename(),str(ucall._funcname),str(ccall._funcname),str(i),"Sync",str(x[3]), str(x[4])))
        # outFD.close()
        self.DataDeduplication()
        self.Finalize()

    def GetDataTransferType(self, stacks):
        ret = {}
        for x in stacks:
            ret[x] = DataTransfer(stacks[x].HashStackDataTransfer(),int(x), stacks[x])
        return ret

    def DataDeduplication(self):
        ##
        #   Contents of files:
        #   DCPUTIME_trace.bin, DTOTIME_trace.bin, DSTIME_trace.bin -> Binary TFFile (dynId=stackId,stackId,count <uint64_t's>, time <double>)
        #   DSTIME_stacks.bin -> StackFile (standard), associated with the above files.
        #   DT_collisions.txt -> Binary file containing the following stackid,overwrite,previousTransfer, previousTransferID (uint64_t)
        #   DT_stacks.bin -> Stackfile (standard)
        ##
        hashedIssueStacks = {}
        dtstack_idToHash = {}
        hashedStacks = {}
        dstime_idToHash = {}
        stack_files = ["DSTIME_stacks.bin", "DT_stacks.bin"]
        for x in stack_files:
            self._stackStore[x] = StackReader(os.path.join(self._inDir, x))
            self._stackStore[x].InverseStacks()

        timing_files = ["DSTIME_trace.bin", "DCPUTIME_trace.bin", "DTOTIME_trace.bin"]

        dstime_stacks = self.GetDataTransferType(self._stackStore["DSTIME_stacks.bin"].GetAllStacks())
        for x in timing_files:
            transferTime = TF_Trace(os.path.join(self._inDir, x))
            transferTime.DecodeFile()
            for y in transferTime._records:
                hValue = int(y[1])
                if "DSTIME_trace.bin" in x:
                    dstime_stacks[hValue].PreTransSynchronization(y[-1])
                elif "DCPUTIME_trace.bin" in x:
                    dstime_stacks[hValue].AddCPUOverhead(y[-1])
                elif "DTOTIME_trace.bin" in x:
                    dstime_stacks[hValue].AddTotalTime(y[-1])
                else:
                    print "error, should not be here"
                    print y
                    print x   
        # for x in dstime_stacks:
        #     print dstime_stacks[x].GetFullOutput() + "\n"         

        dt_stacks =  self.GetDataTransferType(self._stackStore["DT_stacks.bin"].GetAllStacks())
        readCollisionFile = ReadTransferCollisions(os.path.join(self._inDir, "DT_collisions.txt"))
        while 1:
            rec = readCollisionFile.DecodeRecord()
            print rec
            if rec == None:
                break

            prevHash = 0
            if rec[2] == 1:
                prevHash = int(rec[3])
            if rec[1] == 0 and rec[2] == 0:
                continue
            dt_stacks[int(rec[0])].AddDuplicate(DuplicateEntry(rec[1],prevHash))

        dt_to_ds = {}
        for x in dt_stacks:
            found = False
            for y in dstime_stacks:
                if dt_stacks[x].HashStackDataTransfer() == dstime_stacks[y].HashStackDataTransfer():
                    found = True
                    dt_to_ds[x] = y
                    dstime_stacks[y].CopyDuplicates(dt_stacks[x])
                    break
            if found == False:
                print "COULD NOT FIND STACK "
                print dt_stacks[x].GetFullOutput()
                exit(-1)

        for x in dstime_stacks:
            dstime_stacks[x].FixDuplicateID(dt_to_ds)
            print dstime_stacks[x].GetFullOutput()

        totalTimeSavings = 0.0
        self._transStacks = dstime_stacks

        for x in dstime_stacks:
            tmp = dstime_stacks[x].Analysis()
            print "Stack " + str(x) + " - Estimated Savings: " + str(tmp["Estimated Savings"])
            totalTimeSavings += float(tmp["Estimated Savings"])

        print "Total estimated savings from Duplicate Transfer Issues - "  + str(totalTimeSavings)

    def Finalize(self):
        totalImpact = 0.0
        print '%-20.20s | %-20.20s | %-20.20s | %-10.10s | %-20.20s | %-10.10s | %-10.10s | %-10.10s | %-10.10s' % ('App Bin','App Function','Cuda Call','Stack ID','Type','Call Count', 'No Use', 'Est Impact', "Dup Trans")
        syncToDupData = {}
        dupDataToSync = {}

        for x in self._syncStacks:
            if x == 0:
                continue
            for y in self._transStacks:
                if y in dupDataToSync:
                    continue
                if self._syncStacks[x].CompareToDupData(self._transStacks[y]):
                    if self._transStacks[y].Analysis()["Duplicate Count"] == 0:
                        break
                    syncToDupData[x] = y
                    dupDataToSync[y] = x
                    break

        stackPrinter = []

        for x in self._syncStacks:
            if x == 0:
                continue
            myType = "Sync"
            if x in syncToDupData:
                myType += " + DupData"
            stack = self._stackStore["TF_timekey.txt"].GetStackAtID(int(x))
            if stack.InNvidiaFPLibs():
                continue
            ucall = stack.FindFirstUserCall()
            ccall = stack.FindFirstLibCuda()
            if x not in syncToDupData:
                stackPrinter.append([float(self._syncStacks[x].GetTimeSaved()),'%-20.20s | %-20.20s | %-20.20s | %-10.10s | %-20.20s | %-10.10s | %-10.10s | %-10.10s | %-10.10s' % (ucall.GetFilename(),str(ucall._funcname),str(ccall._funcname),str(x),myType,str(self._syncStacks[x].GetCallCount()), str(self._syncStacks[x].GetNumNonUses()), str(self._syncStacks[x].GetTimeSaved()), "")])
            else: 
                stackPrinter.append([float(self._syncStacks[x].GetTimeSaved()), '%-20.20s | %-20.20s | %-20.20s | %-10.10s | %-20.20s | %-10.10s | %-10.10s | %-10.10s | %-10.10s' % (ucall.GetFilename(),str(ucall._funcname),str(ccall._funcname),str(x),myType,str(self._syncStacks[x].GetCallCount()), str(self._syncStacks[x].GetNumNonUses()), str(self._syncStacks[x].GetTimeSaved()), self._transStacks[syncToDupData[x]].Analysis()["Duplicate Matches Transfer"])])

        for y in self._transStacks:
            if y not in dupDataToSync:
                stack = self._transStacks[y]._stack
                if stack.InNvidiaFPLibs():
                    continue
                ucall = stack.FindFirstUserCall()
                ccall = stack.FindFirstLibCuda() 
                analysis = self._transStacks[y].Analysis()            
                if int(analysis["Duplicate Count"]) == 0:
                    continue
                stackPrinter.append([float(self._syncStacks[x].GetTimeSaved()),'%-20.20s | %-20.20s | %-20.20s | %-10.10s | %-20.20s | %-10.10s | %-10.10s | %-10.10s | %-10.10s' % (ucall.GetFilename(),str(ucall._funcname),str(ccall._funcname),str(y),"Dup Data",str(analysis["Call Count"]), str(analysis["Duplicate Count"]), str(analysis["Estimated Savings"]), str(analysis["Duplicate Matches Transfer"]))])
        stackPrinter.sort(key=lambda tup: tup[0])

        for x in stackPrinter:
            print x[1]


        # for x in stack_files:
        #     print str(self._stackStore[x])

        # tmpStack = self._stackStore["DSTIME_stacks.bin"].GetAllStacks()
        # for x in tmpStack:
        #     hashedStacks[tmpStack[x].HashStackDataTransfer()] = DataTransfer(tmpStack[x].HashStackDataTransfer(),int(x), tmpStack[x])
        #     dstime_idToHash[x] = tmpStack[x].HashStackDataTransfer()

        # timing_files = ["DSTIME_trace.bin", "DCPUTIME_trace.bin", "DTOTIME_trace.bin"]
        # for x in timing_files:
        #     transferTime = TF_Trace(os.path.join(self._inDir, x))
        #     transferTime.DecodeFile()

        # tmpStack = self._stackStore["DT_stacks.bin"].GetAllStacks()
        # for x in tmpStack:
        #     hashedIssueStacks[tmpStack[x].HashStackDataTransfer()] = DataTransfer(tmpStack[x].HashStackDataTransfer(),int(x), tmpStack[x])
        #     dtstack_idToHash[x] = tmpStack[x].HashStackDataTransfer()

        # readCollisionFile = ReadTransferCollisions(os.path.join(self._inDir, "DT_collisions.txt"))
        # while 1:
        #     rec = readCollisionFile.DecodeRecord()
        #     print rec
        #     if rec == None:
        #         break

        #     prevHash = 0
        #     if rec[2] == 1:
        #         prevHash = dtstack_idToHash[rec[3]]
        #     hashedIssueStacks[dtstack_idToHash[int(rec[0])]].AddDuplicate(DuplicateEntry(rec[1],prevHash))

        # ## Map the collision stacks to timing
        # colToTiming = {}
        # for x in hashedIssueStacks:
        #     if x in hashedStacks:
        #         hashedStacks[x].CopyDuplicates(hashedIssueStacks[x])
        #     else:
        #         print "Unknown Transfer - " + str(x)

        # f = open("dtrans_out.csv", "w")
        # for x in hashedStacks:
        #     f.write(str(hashedStacks[x].CreateOutput(dstime_idToHash)) + "\n")
        # f.close()


if __name__ == "__main__":
    d = Driver()
    d.Run()