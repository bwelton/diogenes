import hashlib
import json
import os
import subprocess
import os.path
import struct
from sets import Set
from TF_trace import TF_Trace
from LS_TraceBin import LS_TraceBin
from FI_TraceBin import FI_TraceBin

cachedMD5 = {}
stackPoints = {}

def RemoveNewline(line):
    return line.replace("\n","")

def md5(fname):
    global cachedMD5
    if fname in cachedMD5:
        return cachedMD5[fname]

    md5hasher = hashlib.md5()
    try:
        f = open(fname, "rb")
        data = f.read()
        md5hasher.update(data)
        cachedMD5[fname] = md5hasher.hexdigest()
        print fname + " hashed to " + cachedMD5[fname]
        f.close()
    except Exception as e:
        print e
        print "Error: Could not fine file with name - " + fname
        cachedMD5[fname] = None
    return cachedMD5[fname]

def AddStackPoint(libname, offset):
    global stackPoints
    decodeString = libname + "@" + str(offset)
    if decodeString not in stackPoints:
        stackPoints[decodeString] = {"line_num" : 0, "filename" : "", "funcname" : ""}

def GetStackPoint(libname, offset):
    global stackPoints
    decodeString = libname + "@" + str(offset)
    if decodeString in stackPoints:
        return stackPoints[decodeString]
    return None


def GetLineInfo():
    f = open("tmp_encoded_stack.txt","w")
    count = 1
    idToStackpoint = {}
    for x in stackPoints:
        f.write(str(count) + "$" + str(x) +"\n")
        idToStackpoint[count] = x
        count += 1
    f.close()
    try:
        os.remove("tmp_decoded_stack.txt")
    except:
        pass
    subprocess.call("/g/g17/welton2/repo/spack/opt/spack/linux-rhel7-ppc64le/gcc-4.9.3/cudadedup-develop-sfolqw2eykf4ubdm3umxxvnky2ul6k7r/bin/LaunchCUInstriment -d -i tmp_encoded_stack.txt -o tmp_decoded_stack.txt &> debug_symtab.txt", shell=True)
    f = open("tmp_decoded_stack.txt", "r")
    for x in f.readlines():
        entry = {"line_num" : 0, "filename" : "", "funcname" : ""}
        tmp = x.split("$")
        count = tmp[0]
        tmp[1] = "".join(tmp[1:])
        tmp[1] = RemoveNewline(tmp[1])
        index = int(count,16)
        a = tmp[1].split("@")
        if "<Unknown at Offset" in a[1]:
            entry["funcname"] = "Unknown"
            continue
        lines = os.popen("/usr/bin/c++filt " + a[1]).read().splitlines()
        if len(lines) == 0:
            entry["funcname"] = "Unknown"
        else:
            entry["funcname"] = lines[0]
        lineMapSplit = a[3].split("|")
        if len(lineMapSplit) == 2:
            if lineMapSplit[1] != "":
                entry["line_num"] = int(lineMapSplit[1],16)
            if lineMapSplit[0] != "":
                entry["filename"] = lineMapSplit[0]

        if index not in idToStackpoint:
            print "Unknown index of " + str(index)
            print lines
            continue
        print "associating " + a[0] + "@" + str(int(a[2],16)) + " with stack entry " + idToStackpoint[index]
        stackPoints[idToStackpoint[index]] = entry

            #print a[1]

class JSStackEntry:

    def __init__(self, data = None, libname = None, liboffset = None, pos = None):
        self._libName = ""
        self._libOffset = 0
        self._funcName = ""
        self._lineNum = 0
        self._fileName = ""
        self._md5sum = None
        self._pos = 0
        self._compare = None
        if libname != None:
            self._libName = libname
        if liboffset != None:
            self._libOffset = int(liboffset)
        if pos != None:
            self._pos = int(pos)
        if data != None:
            self.from_dict(data)
        else:
            self._md5sum = md5(libname)
            AddStackPoint(libname, liboffset)

    def GetComparitor(self):
        if self._compare != None:
            return self._compare

        if self._md5sum != None:
            self._compare = self._md5sum + "@" + str(self._libOffset)
        else:
            self._compare = self._libName + "@" + str(self._libOffset)
        return self._compare

    def GetFunctionName(self):
        return self._funcName

    def GetLibName(self):
        return self._libName

    def GetNameInfo(self):
        ret = GetStackPoint(self._libName, self._libOffset)
        if ret == None:
            return
        self._funcName = ret["funcname"]
        self._lineNum = ret["line_num"]
        self._fileName = ret["filename"]

    def SetPos(self,pos):
        self._pos = pos

    def to_dict(self):
        return {"libname": self._libName, "liboffset" : self._libOffset, "funcname" : self._funcName,
                "linenum" : self._lineNum, "filename" : self._fileName, "position": self._pos,
                "md5sum" : self._md5sum, "compare" : self._compare}

    def from_dict(self, data):
        global cachedMD5
        self._libName = data["libname"]
        self._libOffset = data["liboffset"]
        self._funcName = data["funcname"]
        self._lineNum = data["linenum"]
        self._fileName = data["filename"]
        self._pos = data["position"]
        self._md5sum = data["md5sum"]
        if self._md5sum != "":
            cachedMD5[self._libName] = self._md5sum




class JSStack:
    def __init__(self, data = None):
        self._cachedTime = None
        self._globalID = 0
        self._chopped = False
        self._stack = []
        self._ident = {"tf_id": 0, "ls_id" : 0, "dstime_id" : 0, "fi_id" : 0, "dt_id" : 0}
        self._count = 0
        self._ttime = 0.0
        self._syncUses = 0
        self._useDelay = 0.0
        self._fiCount = 0
        self._duplicates = 0
        self._transTime = 0.0
        self._transferCount = 0
        self._deltaTime = 0.0
        self._deltaCount = 0
        self._syncOnlyTime = 0.0
        self._syncOnlyCount = 0
        self._dependencies = Set()
        self._transferCollisions = []
        self._overwrites = 0
        if data != None:
            self.from_dict(data)
            return

    def GetStackPos(self, spos):
        if spos >= len(self._stack) or spos < 0:
            return None
        return self._stack[spos]

    def TruncateToCudaCall(self, cudaCalls):
        endPos = -1
        for x in range(0,len(self._stack)):
            if self._stack[x].GetFunctionName() in cudaCalls:
                endPos = x + 1
                break
        if endPos == -1:
            return
        if len(self._stack) <= endPos:
            return
        self._stack = self._stack[0:endPos]


    def StripTemplate(self, spos):
        if spos >= len(self._stack) or spos < 0:
            return None

        funcName = self._stack[spos].GetFunctionName()

        pos = funcName.find("(")
        pos2 = funcName.find("<")
        if pos == -1:
            return funcName
        if pos2 < pos:
            return funcName.split("<")[0] + "<...>"
        return funcName

    def GetDependency(self):
        return list(self._dependencies)

    def AddDependency(self,ident):
        self._dependencies.add(ident)

    def GetSyncUses(self):
        return self._syncUses

    def GetCount(self):
        return self._count

    def GetTotalSyncTime(self):
        return self._ttime

    def GetDelta(self):
        return [self._deltaCount, self._deltaTime]

    def AddSyncTime(self, time):
        self._syncOnlyTime += time
        self._syncOnlyCount += 1

    def AddDelta(self, delta): 
        self._deltaCount += 1
        self._deltaTime += delta


    def GetTotalTime(self):
        if self._ttime > 0.0:
            return self._ttime
        if self._transTime > 0.0:
            return self._transTime

        print "Error, we have a stack without any sort of time associated with it!"
        print "Error at global id: " + str(self._globalID)
        return None


    def CheckIfDependsRequired(self):
        return self.GetGlobalId() in self._dependencies

    def GetCount(self):
        if self._count > 0:
            return self._count
        if self._transferCount > 0:
            return self._transferCount
        print "Error, we have a stack without any sort of count associated with it!"
        print "Error at global id: " + str(self._globalID)
        return None           

    def GetUnnecessaryCount(self):
        tranferCount = self._count - self._syncUses
        ddCount = len(self._transferCollisions) - self._overwrites
        if tranferCount > ddCount:
            return tranferCount
        return ddCount
    def GetEstimatedSavings(self):
        deltaAvg = 0.0
        if self._deltaCount > 0:
            deltaAvg = float(self._deltaTime /  float(self._deltaCount))


        fiAvg = 0.0
        avgTime = 0.0
        if self._count > 0:
            avgTime = float(self._ttime) / float(self._count)

        syncSavings = 0.0
        if avgTime < deltaAvg or self._deltaCount == 0:
            syncSavings = self._ttime - (avgTime * float(self._syncUses))
        else:
            syncSavings = (deltaAvg * self._count) - (deltaAvg * float(self._syncUses))
        
        avgTrans = 0.0
        if self._transferCount > 0:
            avgTrans = float(self._transTime) / float(self._transferCount)
        transSavings = avgTrans * (len(self._transferCollisions) + self._overwrites)

        if syncSavings > transSavings:
            if syncSavings > self._ttime:
                return self._ttime
            return syncSavings
        if transSavings > self._ttime:
            return self._ttime
        return transSavings

    def TotalSavingsSinglePoint(self):
        if self._cachedTime  != None:
            return self._cachedTime
        ret = {"Total" : 0.0 ,"CallCount": 0, "SyncRemoved" : 0.0 , "TransRemoved" : 0.0, "TransCount" : 0, "OpRemoved" : 0.0, "RequiredTrans" : 0}

        deltaAvg = 0.0
        if self._deltaCount > 0:
            deltaAvg = float(self._deltaTime /  float(self._deltaCount))    

        syncOnlyTime = 0.0
        if self._syncOnlyTime > 0:
            syncOnlyTime =  float(float(self._syncOnlyTime) / float(self._syncOnlyCount))

        avgTime = 0.0
        if self._count > 0:
            avgTime = float(self._ttime) / float(self._count)       

        opTime = avgTime - syncOnlyTime
        if opTime < 0.0:
            opTime = 0.0
        syncSavings = 0.0

        syncCorrectedTime = 0.0
        deltaUseTime = 0.0
        FIDelayavg = 0.0
        if self._fiCount > 0:
            FIDelayavg = (float(self._useDelay) / float(self._fiCount))

        if self._syncUses > 0:
            syncCorrectedTime = syncOnlyTime * float(self._syncUses)
            deltaUseTime = deltaAvg * float(self._syncUses)
            if FIDelayavg != 0:
                if (FIDelayavg > syncCorrectedTime):
                    syncCorrectedTime = 0.0
                else:
                    syncCorrectedTime = FIDelayavg
     
                if (FIDelayavg > deltaUseTime):
                    deltaUseTime = 0.0
                else:
                    deltaUseTime = FIDelayavg

        if syncOnlyTime < deltaAvg or self._deltaCount == 0:
            if self.CheckIfDependsRequired():
                syncSavings = self._syncOnlyTime - syncCorrectedTime
                #syncSavings = 0.0
            else:
                syncSavings = self._syncOnlyTime
        else:
            if self.CheckIfDependsRequired():
                syncSavings = (deltaAvg * self._count) - deltaUseTime
            else:
                syncSavings = (deltaAvg * self._count)
        avgTrans = 0.0
        if self._transferCount > 0:
            avgTrans = float(opTime)
        transSavings = avgTrans * (len(self._transferCollisions) + self._overwrites)

        if syncSavings + transSavings > self._ttime:
            ret["Total"] = self._ttime
        else:
            ret["Total"] = syncSavings + transSavings

        ret["SyncRemoved"] = syncSavings
        ret["CallCount"] = self._count
        ret["TransRemoved"] = transSavings
        ret["TransCount"] = (len(self._transferCollisions) + self._overwrites)
        if self.CheckIfDependsRequired():
            ret["OpRemoved"] = syncSavings + opTime * self._count
        else:
            ret["OpRemoved"] = syncSavings + opTime * (self._count - self._syncUses)

        ret["RequiredTrans"] = self._transferCount - ret["TransCount"]
        if ret["OpRemoved"] > self._ttime:
            ret["OpRemoved"] = self._ttime

        self._cachedTime  = ret
        return ret

    def GetEstimatedSavings2(self):
        deltaAvg = 0.0
        if self._deltaCount > 0:
            deltaAvg = float(self._deltaTime /  float(self._deltaCount))


        fiAvg = 0.0
        avgTime = 0.0
        if self._count > 0:
            avgTime = float(self._syncOnlyTime) / float(self._count)

        syncSavings = 0.0
        if avgTime < deltaAvg or self._deltaCount == 0:
            syncSavings = self._syncOnlyTime - (avgTime * float(self._syncUses))
        else:
            syncSavings = (deltaAvg * self._count) - (deltaAvg * float(self._syncUses))
        
        avgTrans = 0.0
        if self._transferCount > 0:
            avgTrans = float(self._transTime) / float(self._transferCount)
        transSavings = avgTrans * (len(self._transferCollisions) + self._overwrites)

        if syncSavings > transSavings:
            if syncSavings > self._ttime:
                return self._ttime
            return syncSavings
        return transSavings

    def AddTransferCollision(self, otherGlobalId, overwrite):
        if otherGlobalId != 0:
            self._transferCollisions.append(otherGlobalId)
        else:
            if overwrite == 1:
                self._overwrites += 1

    def GetLineInfo(self):
        for x in self._stack:
            x.GetNameInfo()

    def AddTotalTime(self, time):
        self._count += 1
        self._ttime += time

    def AddSyncUses(self, count):
        self._syncUses += count

    def AddDataTransfer(self, time):
        self._transTime += time
        self._transferCount += 1

    def AddFirstUse(self, uses):
        for x in range(1, len(uses)):
            if uses[x][0] > 0:
                self._useDelay = self._useDelay + (uses[x][1] - uses[x-1][1])
                self._fiCount += 1



    def GetStackCompairtors(self):
        ret = []
        for x in self._stack:
            ret.append(x.GetComparitor())
        return ret

    def AddEntry(self, libname, liboffset):
        # if "cudadedup-develop-" in libname:
        #     return
        self._stack.append(JSStackEntry(data=None,libname=libname, liboffset=int(liboffset), pos=len(self._stack)))

    def SetID(self, idName, value):
        self._ident[idName] = int(value)

    def GetID(self, idName):
        return self._ident[idName]

    def InverseStacks(self):
        tmp = []
        count = 0
        for x in range(len(self._stack) - 1, 0, -1):
            tmp.append(self._stack[x])
            tmp[count].SetPos(count)
            count += 1

        self._stack = tmp


    def DropTimerEntries(self):
        for x in range(0,len(self._stack)):
            if "plugins/libTimeCall.so" in self._stack[x].GetLibName():
                self._stack = self._stack[0:x]
                self._chopped = True
                return        
    def ChopStack(self):
        ## remove diogenes crap from these stacks
        if self._chopped == True:
            return
        for x in range(0,len(self._stack)):
            if "libDriverAPIWrapper" in self._stack[x].GetLibName():
                self._stack = self._stack[0:x]
                self._chopped = True
                return
        self._chopped = True


    def CompareFIToLS(self, other):
        myself = self.GetStackCompairtors()
        otherComps = other.GetStackCompairtors()
        # print otherComps
        # print myself
        if len(myself) != len(otherComps):
            return False
        for x in range(0, len(myself)):
            if myself[x] != otherComps[x]:
                return False
        return True

    def CompareFIToTF(self, other):
        myself = self.GetStackCompairtors()
        other = other.GetStackCompairtors()

        curPos = 0
        for x in myself:
            found = False
            for y in range(curPos, len(other)):
                if x == other[y]:
                    found = True
                    curPos = y + 1
                    break
            if found == False:
                return False
        return True

    def CompareTFToDT(self, other):
        return self.CompareFIToTF(other)

    def CompareDStoDT(self, other):
        return self.CompareFIToLS(other)

    def GetStackLen(self):
        return len(self._stack)


    def GetStacks(self):
        return self._stack
        
    def to_dict(self):
        ret = dict(self._ident)
        ret["GlobalID"] = self._globalID
        ret["count"] = self._count
        ret["total_time"] = float(self._ttime)
        ret["Sync Uses"] = int(self._syncUses)
        ret["Use Delay"] = float(self._useDelay)
        ret["Duplicates"] = int(self._duplicates)
        ret["FICount"] = int(self._fiCount)
        ret["Transfer Time"] = float(self._transTime)
        ret["Transfer Count"] = int(self._transferCount)
        ret["Stack"] = [x.to_dict() for x in self._stack]
        ret["TransferCollisions"] = self._transferCollisions 
        ret["TransferOverwrites"] = self._overwrites
        ret["DeltaTime"] = self._deltaTime
        ret["DeltaCount"] = self._deltaCount
        ret["SyncOnlyTime"] = self._syncOnlyTime
        ret["SyncOnlyCount"] = self._syncOnlyCount
        ret["Dependencies"] = list(self._dependencies)
        return ret

    def HasTransferIssues(self):
        if len(self._transferCollisions) > 0 or self._duplicates > 0:
            return True 
        return False

    def HasSynchronizationIssues(self):
        if self._syncUses < self._count:
            return True
        return False
    def GetGlobalId(self):
        return self._globalID
    def from_dict(self, data):
        for x in self._ident:
            self._ident[x] = data[x]
        self._globalID = data["GlobalID"]
        self._count = data["count"] 
        self._ttime = data["total_time"] 
        self._syncUses = data["Sync Uses"] 
        self._useDelay = data["Use Delay"] 
        self._duplicates = data["Duplicates"]
        self._fiCount = data["FICount"]
        self._transferCount = data["Transfer Count"]
        self._transTime = data["Transfer Time"]
        self._transferCollisions = data["TransferCollisions"]
        self._overwrites = data["TransferOverwrites"]
        self._deltaTime  = data["DeltaTime"]
        self._deltaCount = data["DeltaCount"]
        self._syncOnlyTime = data["SyncOnlyTime"]
        self._syncOnlyCount = data["SyncOnlyCount"]
        self._dependencies = Set(data["Dependencies"])
        for x in data["Stack"]:
            self._stack.append(JSStackEntry(x))



def BuildMap(stacks, key_id):
    ret = {}
    for x in range(0, len(stacks)):
        if stacks[x].GetID(key_id) != 0:
            #print stacks[x].to_dict()
            assert stacks[x].GetID(key_id) not in ret
            ret[stacks[x].GetID(key_id)] = x
    return ret
class ProcessTimeFile:
    def __init__(self, final):
        self._final = final

    def Process(self):
        m = BuildMap(self._final, "tf_id")
        tf_trace = TF_Trace("TF_trace.bin")
        tf_trace.DecodeFile()
        f = open("TF_Trace.txt","w")
        for x in tf_trace._records:
            ## x[0] = format (2 = sync record)
            ## x[1] = dynId
            ## x[2] = stackId
            ## x[3] = count (only used on x86_64)
            ## x[4] = time (double)
            f.write(str(x[1]) + "," + str(x[2]) + "," + str(x[3]) + "," + str(x[4]) + "\n")
            assert x[0] == 1
            if x[2] not in m:
                print "Error: Missing entry for TF Stack ID - " + str(x[2])
            else:
                self._final[m[x[2]]].AddTotalTime(float(x[4]))
        f.close()
        return self._final

class ProcessLSTrace:
    def __init__(self, final):
        self._final = final

    def Process(self):
        if os.path.isfile("LS_trace.bin") == False:
            return self._final

        m = BuildMap(self._final, "ls_id")

        ls_trace = LS_TraceBin( "LS_trace.bin")
        ls_trace.DecodeFile()
        for x in ls_trace._entriesMap:
            if int(x) not in m and int(x) != 0:
                print "Error: Could not find ls id of - " + str(x)
            elif int(x) != 0:
                self._final[m[x]].AddSyncUses(len(ls_trace._entriesMap[x]))
        return self._final


class ProcessFITrace:
    def __init__(self, final):
        self._final = final

    def Process(self):
        if os.path.isfile("FI_trace.bin") == False:
            return self._final
        m = BuildMap(self._final, "fi_id")
        fi_tracebin = FI_TraceBin("FI_trace.bin")
        fi_tracebin.DecodeFile()
        for x in fi_tracebin._entriesMap:
            if int(x) not in m and int(x) != 0:
                print "Warn: Could not find fi id of - " + str(x)
            elif int(x) != 0:
                self._final[m[x]].AddFirstUse(fi_tracebin._entriesMap[x])
        return self._final    

class ProcessDSTime:
    def __init__(self, final):
        self._final = final

    def Process(self):
        m = BuildMap(self._final, "dstime_id")
        transferTime = TF_Trace("DTOTIME_trace.bin")
        transferTime.DecodeFile()
        for x in transferTime._records:
            assert x[0] == 2
            stackId = int(x[2])
            time = float(x[3])
            if stackId not in m and stackId != 0:
                print "Error could not find dstiming stack with id of - " + str(stackId)
            elif stackId != 0:
                self._final[m[stackId]].AddDataTransfer(time)
        return self._final

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

class ProcessTransCollisions:
    def __init__(self, final):
        self._final = final
    def Process(self):
        m = BuildMap(self._final, "dt_id")
        readCollisionFile = ReadTransferCollisions( "DT_collisions.txt")
        while 1:
            rec = readCollisionFile.DecodeRecord()
            if rec == None:
                break
            if rec[0] == 0:
                continue
            prevHash = 0
            if rec[2] == 1:
                prevHash = int(rec[3])
            if rec[1] == 0 and rec[2] == 0:
                continue
            if prevHash != 0:
                if prevHash not in m:
                    print "Could not find previous hash id of - " + str(prevHash)
                    prevHash = 999999999
                else:
                    prevHash = self._final[m[prevHash]].GetGlobalId()
            if int(rec[0]) not in m:
                print "Could not find transfer with dt_id of - " + str(rec[0])
            else:
                self._final[m[int(rec[0])]].AddTransferCollision(prevHash, rec[1])
        return self._final


class StackContainer:
    def __init__(self):
        self._stacks = []
        self._filenames = {"tf_id": "", "ls_id" : "", "dstime_id" : "", "fi_id" : "", "dt_id" : ""}

    def GetFilenameStack(self):
        return self._filenames


    def ReadSingleFile(self, filename, id_key):
        try:
            f = open(filename, "rb")
            data = f.readlines()
            f.close()
        except:
            return []
        ret = []
        for x in data:
            tmp = RemoveNewline(x).split("$")
            stack = JSStack()
            stack.SetID(id_key, int(tmp[0]))
            for y in tmp[1:]:
                ms = y.split("@")
                stack.AddEntry(ms[0], int(ms[1]))
            if id_key == "tf_id" or id_key == "dt_id" or id_key == "dstime_id":
                stack.InverseStacks()
            ret.append(stack)
        return ret

    def DumpStack(self, stack, outName):
        dump = []
        for y in stack:
            dump.append(y.to_dict())

        j = json.dumps(dump, indent=4)
        f = open(outName +".json", "wb")
        f.write(j)
        f.close()


    def DumpStacks(self, stacks):
        for x in stacks:
            self.DumpStack(stacks[x], x)

    def ReadStackFiles(self, filenames):
        stacks = {}
        for x in filenames:
            stacks[x] = self.ReadSingleFile(filenames[x], x)
        discards = []
        
        if stacks["fi_id"] != None:
            for x in stacks["ls_id"]:
                found = False
                for y in stacks["fi_id"]:
                    if x.CompareFIToLS(y):
                        found = True
                        assert y.GetID("ls_id") == 0
                        y.SetID("ls_id", x.GetID("ls_id"))
                        break
                if found == False:
                    print "Could not find an associated LS stack for LS ID - " + str(x.GetID("ls_id"))
                    discards.append(x)
        else:
            stacks["fi_id"] = stacks["ls_id"]
        self.DumpStacks(stacks)
        # for x in stacks["ls_id"]:
        #     print "ls_id: " + str(x.GetID("fi_id"))
        #     print x.GetStackCompairtors()            
        for x in stacks["tf_id"]:
            x.DropTimerEntries()
        for x in stacks["fi_id"]:
            found = False
            for y in stacks["tf_id"]:
                if x.CompareFIToTF(y):
                    found = True
                    assert y.GetID("ls_id") == 0 and y.GetID("fi_id") == 0
                    y.SetID("ls_id", x.GetID("ls_id"))
                    y.SetID("fi_id", x.GetID("fi_id"))
                    print y.GetID("tf_id")
                    break
            if found == False:
                print "Could not find an associated FI stack for FI ID - " + str(x.GetID("fi_id")) + "," + str(x.GetID("ls_id"))
                print x.to_dict()
                discards.append(x)
        final = []
        for x in stacks["dt_id"]:
            found = False
            x.ChopStack()
            for y in stacks["dstime_id"]:
                y.ChopStack()
                if x.CompareDStoDT(y):
                    found = True
                    assert y.GetID("dt_id") == 0
                    y.SetID("dt_id", x.GetID("dt_id"))
                    break
            if found == False:
                print "Could not find an associated DT stack for DT ID - " + str(x.GetID("dt_id")) 
                discards.append(x)

        idsCopied = {}
        tfMap = {}
        for x in stacks["tf_id"]:
            found = False
            if x.GetID("tf_id") == 0:
                continue
            for y in stacks["dstime_id"]:
                if x.CompareTFToDT(y):
                    found = True
                    print "Found DSTime"
                    assert y.GetID("tf_id") == 0
                    idsCopied[y.GetID("dstime_id")] = True
                    y.SetID("ls_id", x.GetID("ls_id"))
                    y.SetID("fi_id", x.GetID("fi_id"))                    
                    y.SetID("tf_id", x.GetID("tf_id"))

                    final.append(y)
                    break
            if found == False:
                print "Could not find tf_id in transfer log (synchronization only) " + str(x.GetID("tf_id")) 
                if x.GetID("tf_id") == 2:
                    print x.to_dict()
                    print "IN FALSE"
                final.append(x)

        self.DumpStack(final, "combined_stacks")      
        for x in stacks["dstime_id"]:
            if x.GetID("dstime_id") not in idsCopied:
                print "No synchronization for - " + str(x.GetID("dstime_id"))
                assert x.GetID("tf_id") == 0
                final.append(x)
        GetLineInfo()
        gloId = 1
        for x in final:
            x.GetLineInfo()
            x._globalID = gloId
            gloId += 1

 
        proc = ProcessTimeFile(final)
        final = proc.Process()

        proc = ProcessLSTrace(final)
        final = proc.Process()

        proc = ProcessFITrace(final)
        final = proc.Process()


        print final
        proc = ProcessDSTime(final)
        final = proc.Process()

        proc = ProcessTransCollisions(final)
        final = proc.Process()
        curMap = BuildMap(final, "tf_id")
        tfDelta = None

        ## Read Delta
        with open("TF_delta.bin", "rb") as file:
            tfDelta = file.read()
            deltalength = int(int(len(tfDelta) / 8) / 2)
            for x in range(0, deltalength):
                deltatmp = struct.unpack_from("Qd", tfDelta, offset=(x*8*2))
                if int(deltatmp[0]) in curMap:
                    final[curMap[deltatmp[0]]].AddDelta(deltatmp[1])
                else:
                    print "Could not find TF with ID of - " + str(deltatmp[0])

        ## Read Sync Time 
        with open("TF_synctime.bin", "rb") as file:
            tfDelta = file.read()
            deltalength = int(int(len(tfDelta) / 8) / 2)
            for x in range(0, deltalength):
                deltatmp = struct.unpack_from("Qd", tfDelta, offset=(x*8*2))
                if int(deltatmp[0]) in curMap:
                    final[curMap[deltatmp[0]]].AddSyncTime(deltatmp[1])
                else:
                    print "Could not find TF with ID of - " + str(deltatmp[0])

        ## Read Timeline File
        curMap = BuildMap(final, "ls_id")
        with open("LS_syncaccess.bin", "rb") as file:
            tfDelta = file.read()            
            deltalength = int(int(len(tfDelta) / 8) / 3)
            for x in range(0, deltalength):
                deltatmp = struct.unpack_from("QQQ", tfDelta, offset=(x*8*3))
                if deltatmp[0] in curMap:
                    n = deltatmp[2]
                    if n != 0 and n in curMap:
                        n = curMap[n]
                    else:
                        n = 0
                    if deltatmp[1] == 1:
                        final[curMap[deltatmp[0]]].AddDependency(curMap[deltatmp[0]])
                    else:
                        if n == 0:
                            print "Could not find global id for syncaccess - " + str(n)
                        else:
                            final[curMap[deltatmp[0]]].AddDependency(n)
                else:
                    print "Could not find global id for synch access at - " + str(deltatmp[0])
                                
                
        self.DumpStack(final, "combined_stacks")

        

        

if __name__ == "__main__":
    n = StackContainer()
    n.ReadStackFiles({"tf_id": "TF_timekey.txt", "ls_id" : "LS_stackkey.txt", "dstime_id" : "DSTIME_stacks.bin", "fi_id" : "FI_stackkey.txt", "dt_id" : "DT_stacks.bin"})


