import json
import os
from sets import Set

from TF_trace import TF_Trace
from Driver2 import JSStack, JSStackEntry, BuildMap
from DisplayFormatter import DisplayFormatter, TextRow
from ReadExecTime import ReadExecTime
import copy
FOLD_ID = DisplayFormatter()
TOTAL_TIME = ReadExecTime()
cudaFunctions = os.path.join(os.path.dirname(os.path.realpath(__file__)),"cudaFunctions.txt")
f = open(cudaFunctions, "r")
data = f.readlines()
f.close()

def RemoveNewline(line):
    return line.replace("\n","")

cudaCalls = {}
for x in data:
    cudaCalls[RemoveNewline(x)] = None

mapFromGIDToStackDepth = {}
postProc = {}
findStackDepth = {}
stackMap = {}

def IsCudaCall(funcName):
    global cudaCalls
    if funcName in cudaCalls:
        return True
    return False

class EntryList:
    def __init__(self, nextStackDepth):
        self._data = {"DepthLevel" : nextStackDepth, "IdList" : []}


    def GetDepthID(self):
        return self.Get("DepthLevel")._data["FoldID"]

    def Get(self, ident):
        if ident not in self._data:
            return None
        return self._data[ident]

    def AddStack(self, globalID):
        global mapFromGIDToStackDepth
        if globalID not in mapFromGIDToStackDepth:
            mapFromGIDToStackDepth[globalID] = []

        mapFromGIDToStackDepth[globalID].append(self.GetDepthID())
        self._data["IdList"].append(globalID)


class StackDepthLevel:
    def __init__(self):
        global FOLD_ID
        global findStackDepth
        self._data = {"EntryLists" : {} , "FoldID" : FOLD_ID.GetID()}
        findStackDepth[self._data["FoldID"]] = self
        self._entires = {}

    def __CreateEntryList(self, name):
        if name not in self._data["EntryLists"]:
            self._data["EntryLists"][name] = EntryList(StackDepthLevel())
        
    def AddEntry(self, stack, pos, nextMode = "Up"):
        funcName = stack.StripTemplate(pos)
        if funcName == None:
            return False
        self.__CreateEntryList(funcName)
        ret = False 
        if nextMode == "Up":
            ret = self._data["EntryLists"][funcName].Get("DepthLevel").AddEntry(stack, pos - 1)
        else:
            ret = self._data["EntryLists"][funcName].Get("DepthLevel").AddEntry(stack, pos + 1)

        if ret != False:
            self._data["EntryLists"][funcName].AddStack(stack.GetGlobalId())

        return True


    def GetRecursiveTime(self, entry):
        global stackMap
        global TOTAL_TIME
        ret = [0.0,0.0]
        idList = entry.Get("IdList")
        for x in idList:
            if x == 0:
                continue
            assert x in stackMap
            sav = stackMap[x].TotalSavingsSinglePoint()
            if sav["TransCount"] > 0:
                ret[0] += sav["Total"]
                ret[1] += (sav["Total"] / TOTAL_TIME)
            else:
                ret[0] += sav["OpRemoved"]
                ret[1] += (sav["OpRemoved"] / TOTAL_TIME)
        return ret

    def GetAllTime(self):
        ret = [0.0, 0.0]
        for x in self._data["EntryLists"]:
            tmp = self.GetRecursiveTime(self._data["EntryLists"][x])
            ret[0] += tmp[0]
            ret[1] += tmp[1]
        return ret

    def GetDependencies(self, entry):
        idList = entry.Get("IdList")
        dependencies = Set()
        ret = Set()
        for x in idList:
            dependencies = dependencies | Set(stackMap[x].GetDependency())
        for x in dependencies:
            if x == 1:
                continue
            ret.add(mapFromGIDToStackDepth[x][0])
        # print ret
        return list(ret)


    def GetMyString(self, priors = [], outputData = True):
        global FOLD_ID
        myDepth = len(priors)
        rows = []
        ret = "\n"
        # ret = "=" * 100 + "\n"
        # ret = " " + "FOLD_ID=" + str(self._data["FoldID"]) + "\n"
        for x in range(0,len(priors)):
            rows.append(TextRow([priors[x]],x))
            ret += " " + "  " * x + priors[x] + "\n"
        orderedList = []
        for x in self._data["EntryLists"]:
            orderedList.append([self.GetRecursiveTime(self._data["EntryLists"][x])[0],x])
        orderedList.sort(key=lambda x: x[0],reverse=True)

        for y in orderedList:
            x = y[1]
            times = self.GetRecursiveTime(self._data["EntryLists"][x])
            if times[1] < 0.001:
                continue
            rows.append(TextRow(["{0:3.3f}s({1:2.2f}%) {2:s} (DEBUG ID: {3:d})".format(times[0], times[1] *100, x, self._data["EntryLists"][x].GetDepthID())], myDepth, self._data["EntryLists"][x].GetDepthID()))
            ret += " " + myDepth * "  " + "{0:3.3f}s({1:2.2f}%) {2:s} (DEBUG ID: {3:d})".format(times[0], times[1] *100, x, self._data["EntryLists"][x].GetDepthID()) + "\n"
            tmp = self.GetDependencies(self._data["EntryLists"][x])
            if len(tmp) == 0:
                rows.append(TextRow(["Synchronization and/or Transfer unncessary"], myDepth + 1))
            else:
                conditonID = FOLD_ID.GetID()
                postProc[conditonID] =  tmp        
                rows.append(TextRow(["Conditionally unnecessary synchronization/transfer (see: conditions)"], myDepth + 1,  conditonID))
            #ret += " " + myDepth * "  " + "      Depends on synchronizations/transfers at\n"
            # for nm in self.GetDependencies(self._data["EntryLists"][x]):
            #     ret += " " + myDepth * "  " + "           "  + str(nm)+ "\n"
        self._data["outstring"] = ret
        if outputData == True:
            FOLD_ID.AddElement(self._data["FoldID"], rows)
        return ret
    def GetOuputStrings(self, prior = []):
        ret = [self.GetMyString(prior)]
        for x in self._data["EntryLists"]:
            newList = copy.deepcopy(prior)
            myself = self.GetRecursiveTime(self._data["EntryLists"][x])
            # if myself[1] <  0.001:
            #     continue
            newLine = "{0:3.3f}s({1:2.2f}%) {2:s} (DEBUG ID: {3:d})".format(myself[0], myself[1] *100, x, self._data["EntryLists"][x].GetDepthID())
            newList.append(newLine)
            ret += self._data["EntryLists"][x].Get("DepthLevel").GetOuputStrings(newList)
        return ret


class TemplateFolder:
    def __init__(self, stacks):
        global cudaCalls
        global stackMap

        self._data = {"JSONStacks": copy.deepcopy(stacks), "EntryListsBot" : {}}
        for x in self._data["JSONStacks"]:
            x.TruncateToCudaCall(cudaCalls)
            stackMap[x.GetGlobalId()] = x

    def BuildBottomUp(self):
        global cudaCalls
        for x in self._data["JSONStacks"]:
            if x.GetGlobalId() == 1:
                continue
            funcName = x.StripTemplate(x.GetStackLen() - 1)
            assert funcName != None
            if funcName not in cudaCalls:
                print "Unknown func - " + str(funcName)

            if funcName not in self._data["EntryListsBot"]:
                self._data["EntryListsBot"][funcName] =  EntryList(StackDepthLevel())
            self._data["EntryListsBot"][funcName].Get("DepthLevel").AddEntry(x, x.GetStackLen() - 2)


    def BuildTopDown(self):
        for x in self._data["JSONStacks"]:
            funcName = x.StripTemplate(0)
            assert funcName != None
            if funcName not in self._data["EntryListsTop"]:
                self._data["EntryListsTop"][funcName] =  EntryList(StackDepthLevel())
            self._data["EntryListsTop"][funcName].Get("DepthLevel").AddEntry(x, 1, "Down")     

    def Build(self):
        global FOLD_ID
        global postProc
        global TOTAL_TIME
        global findStackDepth
        #self.BuildTopDown()
        self.BuildBottomUp()
        # sequentialEntries = []
        # with open("sequential_pproc.json", "rb") as sequential:
        #     sequentialEntries = json.load(sequential)


        

        f = open("BottomUpTest.txt", "w")
        beforeSort = []
        presentationID = FOLD_ID.GetID()
        # ordering = []
        # for x in self._data["EntryListsBot"]:
        #     ordering.append([self._data["EntryListsBot"][x].Get("DepthLevel").GetAllTime()[0], x])
        # ordering.sort(key=lambda x: x[0],reverse=True)
        for x in self._data["EntryListsBot"]:
            times = self._data["EntryListsBot"][x].Get("DepthLevel").GetAllTime()
            startID = "{0:6.3f}s({1:5.2f}%) Fold on {2:s} (DEBUG ID: {3:d})".format(times[0], times[1] *100, x, self._data["EntryListsBot"][x].GetDepthID())
            beforeSort.append([times[0],TextRow([startID],0,self._data["EntryListsBot"][x].GetDepthID())])
            ret = self._data["EntryListsBot"][x].Get("DepthLevel").GetOuputStrings(prior=[startID])
            for y in ret:
                f.write(y + "\n")
        beforeSort.sort(key=lambda x: x[0],reverse=True)
        outRows = []
        outRows.append(TextRow(["Template and Function Folds"],0))
        outRows.append(TextRow(["==================================="],0))
        for x in beforeSort:
            outRows.append(x[1])

        beforeSort = []
        if os.path.exists("sequential.json"):
            with open("sequential.json", "rb") as seqLoad:
                sequenceData = json.load(seqLoad)
                FOLD_ID.ElementFromFile({"Elements":sequenceData["Elements"]})
                for x in sequenceData["initial"]:
                    beforeSort.append([x["timeData"],TextRow(0,0,data=x)])
                # for x in sequentialEntries:
                #     if x[0] / TOTAL_TIME < 0.001:
                #         continue
                #     rows = [TextRow([x[1]],0)]
                #     myID = FOLD_ID.GetID()
                #     FOLD_ID.AddElement(myID, rows)
                #     beforeSort.append([x[0], TextRow(["{0:6.3f}s({1:5.2f}%) Sequence".format(x[0],(x[0]/TOTAL_TIME) * 100)], 0, myID)])
                beforeSort.sort(key=lambda x: x[0],reverse=True)

                outRows.append(TextRow(["Sequences of Unnecessary Operations"],0))
                outRows.append(TextRow(["==================================="],0))        
                for x in beforeSort:
                    outRows.append(x[1])


        outRows.append(TextRow(["Single Points of Interest"],0))
        outRows.append(TextRow(["==================================="],0))
        beforeSort = []
        if os.path.exists("single_point.json"):
            with open("single_point.json", "rb") as seqLoad:
                sequenceData = json.load(seqLoad)
                FOLD_ID.ElementFromFile({"Elements":sequenceData["Elements"]})
                for x in sequenceData["initial"]:
                    beforeSort.append([x["timeData"],TextRow(0,0,data=x)])
                # for x in sequentialEntries:
                #     if x[0] / TOTAL_TIME < 0.001:
                #         continue
                #     rows = [TextRow([x[1]],0)]
                #     myID = FOLD_ID.GetID()
                #     FOLD_ID.AddElement(myID, rows)
                #     beforeSort.append([x[0], TextRow(["{0:6.3f}s({1:5.2f}%) Sequence".format(x[0],(x[0]/TOTAL_TIME) * 100)], 0, myID)])
                beforeSort.sort(key=lambda x: x[0],reverse=True)  
                for x in beforeSort:
                    outRows.append(x[1])
        # outRows.append(TextRow(["(Coming Soon)"],0))  
        FOLD_ID.AddElement(presentationID, outRows)
        FOLD_ID.SetStart(presentationID)
        for x in postProc:
            rows = [TextRow(["Unnecessary if the following operations are kept"],0)]
            for y in postProc[x]:
                rows.append(TextRow([findStackDepth[y]._data["outstring"]],1))
            FOLD_ID.AddElement(x, rows)

        f.close()

if __name__ == "__main__":
    f = open("combined_stacks.json", "rb")
    data = json.load(f)
    myStacks = []
    for z in data:
        myStacks.append(JSStack(data=z))
    test = TemplateFolder(myStacks)
    test.Build()
    FOLD_ID.BeginDisplay()

