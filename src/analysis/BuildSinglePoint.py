import json
import os
import copy
from sets import Set
from Driver2 import JSStack, JSStackEntry, BuildMap
from DisplayFormatter import DisplayFormatter, TextRow
from ReadExecTime import ReadExecTime
FOLD_ID = DisplayFormatter()
EXEC_TIME = ReadExecTime()

cudaFunctions = os.path.join(os.path.dirname(os.path.realpath(__file__)),"cudaFunctions.txt")
f = open(cudaFunctions, "r")
data = f.readlines()
f.close()

def RemoveNewline(line):
    return line.replace("\n","")

cudaCalls = {}
for x in data:
    cudaCalls[RemoveNewline(x)] = None


class Stacks:
    def __init__(self, stackEntry, stackMap):
        global FOLD_ID
        self._data = {"Children" : {}, "ChildGIDs": Set(), "FoldID" : FOLD_ID.GetID(), 
                      "StackEntry" : stackEntry, "Stacks": stackMap}

    def GetLinkID(self):
        return self._data["FoldID"]

    def AddEntry(self, stack, pos):
        self._data["ChildGIDs"].add(stack.GetGlobalId())
        stackItem = stack.GetStackPos(pos-1)
        if stackItem == None:
            return

        comp = stackItem.GetComparitor()
        if comp not in self._data["Children"]:
            self._data["Children"][comp] = Stacks(stackItem, self._data["Stacks"])
        self._data["Children"][comp].AddEntry(stack,pos-1)

    def GetLevelSavings(self):
        savings = 0.0
        for x in list(self._data["ChildGIDs"]):
            sav = self._data["Stacks"][x].TotalSavingsSinglePoint()
            if sav["TransCount"] > 0:
                savings += self._data["Stacks"][x].TotalSavingsSinglePoint()["Total"]
            else:
                savings += self._data["Stacks"][x].TotalSavingsSinglePoint()["OpRemoved"]
            # savings += self._data["Stacks"][x].GetEstimatedSavings()["Total"]
        return savings


    def BuildHeader(self, depth):
        global EXEC_TIME
        savings = self.GetLevelSavings()
        return TextRow(["{0:3.3f}s({1:2.2f}%) {2:s} in {3:s} at line {4:d}".format(savings, (savings / EXEC_TIME) * 100, 
            self._data["StackEntry"].GetFunctionName()[:40], self._data["StackEntry"]._fileName, self._data["StackEntry"]._lineNum)],depth)


    def BuildView(self, prior = []):
        global EXEC_TIME
        print "Building stackview for : " + self._data["StackEntry"].GetFunctionName()

        header = self.BuildHeader(len(prior) + 1)
        childReturns = []
        newPrior = copy.deepcopy(prior)
        newPrior.append(header)
        for x in self._data["Children"]:
            if self._data["Children"][x].GetLevelSavings() / EXEC_TIME < 0.001:
                continue
            childReturns.append(self._data["Children"][x].BuildView(newPrior))
            childReturns[-1].append(x)

        myView = []
        for x in prior:
            myView.append(x)
        myView.append(header)

        childReturns.sort(key=lambda x: x[0],reverse=True)
        for x in childReturns:
            x[1].SetLinkID(self._data["Children"][x[2]].GetLinkID())
            myView.append(x[1])

        FOLD_ID.AddElement(self._data["FoldID"], myView)
        return [self.GetLevelSavings(), self.BuildHeader(len(prior) + 1)]


class BuildStackTree:
    def __init__(self, stacks):
        global cudaCalls
        global FOLD_ID
        FOLD_ID._globalID = 50000000
        self._data = {"Children" : {}, "FoldID" : FOLD_ID.GetID(), "Stacks" : {}}
        for x in stacks:
            x.TruncateToCudaCall(cudaCalls)
            self._data["Stacks"][x.GetGlobalId()] = x


    def BuildTree(self):
        for x in self._data["Stacks"]:
            if x == 1:
                continue
            stack = self._data["Stacks"][x]
            stackItem = stack.GetStackPos(stack.GetStackLen() - 1)
            comp = stackItem.GetComparitor()
            if comp not in self._data["Children"]:
                self._data["Children"][comp] = Stacks(stackItem, self._data["Stacks"])
            self._data["Children"][comp].AddEntry(stack, stack.GetStackLen() - 2)

    def ExportToJson(self):
        global FOLD_ID
        childReturns = []
        for x in self._data["Children"]:
            if self._data["Children"][x].GetLevelSavings() / EXEC_TIME < 0.001:
                continue
            childReturns.append(self._data["Children"][x].BuildView([]))
            childReturns[-1].append(x)
        childReturns.sort(key=lambda x: x[0],reverse=True)

        outJson = {}
        outJson["initial"] = []

        for x in childReturns:
            x[1].SetLinkID(self._data["Children"][x[2]].GetLinkID())
            x[1]._data["timeData"] = x[0]
            outJson["initial"].append(x[1]._data)

        outJson["Elements"] = FOLD_ID.SerializeElements()["Elements"]
        j2 = json.dumps(outJson, indent=4)
        f = open("single_point.json", "wb")
        f.write(j2)
        f.close()

if __name__ == "__main__":
    f = open("combined_stacks.json", "rb")
    data = json.load(f)
    myStacks = []
    for z in data:
        myStacks.append(JSStack(data=z))

    stree = BuildStackTree(myStacks)
    stree.BuildTree()
    stree.ExportToJson()

