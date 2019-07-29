import json
import os
from TF_trace import TF_Trace
from Driver2 import JSStack, JSStackEntry, BuildMap


cudaFunctions = os.path.join(os.path.dirname(os.path.realpath(__file__)),"cudaFunctions.txt")
f = open(cudaFunctions, "r")
data = f.readlines()
f.close()

def RemoveNewline(line):
    return line.replace("\n","")

cudaCalls = {}
for x in data:
	cudaCalls[RemoveNewline(x)] = None

def GetFirstUserCall2(stack):
	global cudaCalls
	tmp = stack.GetStacks()
	for x in range(0, len(tmp)): 
		if tmp[x].GetFunctionName() in cudaCalls:
			print tmp[x].GetFunctionName()
			return [tmp[x-1].GetFunctionName(),tmp[x],tmp[x-1]]
	return None






class NeighborStack:
	def __init__(self, stack):
		self._stack = stack
		self._data = {"Count": stack.GetCount(), "Remaining": 0 if stack.GetSyncUses() > 0 else stack.GetCount(), "UseAverage" :0.0,
					  "SyncUses" : stack.GetSyncUses(), "DeltaAvg" : 0.0}
		if  stack.GetCount() > 0:
			self._data["UseAverage"] = float(stack.GetTotalSyncTime() / stack.GetCount())
		if stack.GetDelta()[0] > 0:
			self._data["DeltaAvg"] = float(stack.GetDelta()[1] / stack.GetDelta()[0])

	def GetUse(self):
		ret = {}
		if self._data["Remaining"] > 0:
			self._data["Remaining"] = self._data["Remaining"] - 1
			ret["UseAverage"] = self._data["UseAverage"]
			ret["DeltaAvg"] = self._data["DeltaAvg"]
			return ret
		return None


class StackEntry:
	def __init__(self, stack):
		## Equation: sum(timeSaveds) < sum(deltaSaves) = sum(timeSaves)
		self._stack = stack
		self._totalTime = 0.0
		self._count = 0
		self._deltaTimes = 0.0
		self._deltaCount = 0
		self._allStacks = {}
		self._neighbors = {}

	def GetTimeSavable(self):
		print self._deltaTimes
		print self._totalTime
		if (self._deltaTimes > self._totalTime):
			return self._totalTime
		return self._deltaTimes



	def GetPrintout(self):
		transIssues = 0
		syncIssues = 0
		issuesMatrix = {}
		callsSeen = {}
		for x in self._neighbors:
			issue = {"Sync": False, "Trans" : False}
			if len(self._neighbors[x]._stack._transferCollisions) > 0 or self._neighbors[x]._stack._overwrites > 0:
				issue["Trans"] = True
				transIssues += 1
			if self._neighbors[x]._stack._syncUses < self._neighbors[x]._stack._count:
				syncIssues += 1
				issue["Sync"] = True
			issuesMatrix[x] = issue

			call = GetFirstUserCall2(self._neighbors[x]._stack)
			if call == None:
				continue
			# print call
			if call[0] not in callsSeen:
				callsSeen[call[0]] = []
			callsSeen[call[0]].append([x,call])
		callCount = 0
		ret = "=" * 80
		ret += "\n= Time Recoverable: {0:2.3f}s ({1:2.2f}% of execution time) ".format(self.GetTimeSavable(), (self.GetTimeSavable() / 38.943) * 100.0) + " " * 25 + " ="
		ret += "\n= Sync Issues: {0:3d}    Transfer Issues: {1:3d}".format(syncIssues, transIssues) + " " * 37 + "="
		ret += "\n=" + "-" * 78 + "="
		ret += "\n= {0:34s} | {1:4s} | {2:10s} | {3:11s} | {4:5s} ".format("Function Call", "Line", "Cuda Call", "Issue Type", "Count") + "="
		for x in callsSeen:
			ret += "\n= {0:34.34s} | {1:4.4s} | {2:10.10s} | {3:11.11s} | {4:5.5s} ".format(x, "---", "----------", "-----------", "----") + "="
			for y in callsSeen[x]:
				callCount += 1
				issueId = ""
				if issuesMatrix[y[0]]["Sync"] == True:
					issueId = "Sync"
					if issuesMatrix[y[0]]["Trans"] == True:
						issueId += "+Trans"
				elif issuesMatrix[y[0]]["Trans"] == True:
					issueId = "Trans"
				ret += "\n= {0:34.34s} | {1:4d} | {2:10.10s} | {3:11.11s} | {4:5d} ".format("^^^", y[1][2].to_dict()["linenum"], y[1][1].GetFunctionName(), issueId, self._neighbors[y[0]]._stack.GetCount()) + "="
		ret += "\n=" + "-" * 78 + "="

		saveTime = 0.0
		if self.GetTimeSavable() > 0 and callCount > 0:
			saveTime = float(self.GetTimeSavable()) / float(callCount)
		return [saveTime, ret]
	def BuildRelationships(self, idMap, idList, tf_records):
		myId = self._stack.GetID("tf_id")
		if myId == 0 or myId == 1:
			return
		for x in idMap:
			self._allStacks[x] = NeighborStack(idList[x])

		inChain = False
		for x in tf_records:
			entryId = int(x[2])
			if entryId == 0 or entryId not in self._allStacks:
				continue
			
			if entryId != myId and inChain == False:
				continue
			if entryId == myId:
				inChain = True

			useData = self._allStacks[entryId].GetUse()

			if useData == None:
				inChain = False
				continue
			if  entryId not in self._neighbors:
				self._neighbors[entryId] = self._allStacks[entryId]
			self._totalTime += useData["UseAverage"]
			self._deltaTimes += useData["DeltaAvg"]
			self._count += 1
			self._deltaCount += 1

class Aggregator:
	def __init__(self, stackEntry, cudaCall):
		self._stackEntry = stackEntry
		self._stacks = []
		self._cudaCall = cudaCall

	def AddStack(self, stack):
		self._stacks.append(stack)

	def GetUsageInfo(self):
		ret = {"CudaCall": self._cudaCall, "StackEntry" : self._stackEntry, "CallCount" : 0, "UnnecessaryCalls" : 0, "TotalTime" : 0.0, "TimeSaved" : 0.0}
		for x in self._stacks:
			ret["CallCount"] += x.GetCount()
			ret["UnnecessaryCalls"] += x.GetUnnecessaryCount()
			ret["TotalTime"] += x.GetTotalTime()
			ret["TimeSaved"] += x.GetEstimatedSavings()
		return ret

cudaFunctions = os.path.join(os.path.dirname(os.path.realpath(__file__)),"cudaFunctions.txt")
f = open(cudaFunctions, "r")
data = f.readlines()
f.close()

def RemoveNewline(line):
    return line.replace("\n","")

cudaCalls = {}
for x in data:
	cudaCalls[RemoveNewline(x)] = None


f = open("combined_stacks.json", "rb")
data = json.load(f)

stacks = []
for x in data:
	stacks.append(JSStack(data=x))


def GetFirstUserCall(stack):
	global cudaCalls
	tmp = stack.GetStacks()
	for x in range(0, len(tmp)):
		if tmp[x].GetFunctionName() in cudaCalls:
			return [tmp[x].GetFunctionName(),tmp[x-1]]
	return None


def GetRawFunctionName(funcName):
	pos = funcName.find("(")
	pos2 = funcName.find("<")
	if pos == -1:
		return funcName
	if pos2 < pos:
		return funcName.split("<")[0]
	return funcName

def GetLineInfo(stack, startPos, depth):
	ret = []
	tmp = stack.GetStacks()	
	count = 0
	ident = ""
	for x in range(startPos, 0, -1):
		ret.append(GetRawFunctionName(tmp[x].to_dict()["funcname"]))
		count += 1
		if count >= DEPTH:
			break
	# print ret
	return list(reversed(ret))

def ReturnIndentedStack(stack, depth):
	ret = GetLineInfo(stack, GetFirstUserCall(stack)[1], depth)
	printList = []
	for y in ret:
		# print y
		tmp = ("%-60.60s" % y)
		if len(printList) > 0:
			printList.append((" " * len(printList)*2) + tmp[:-len(printList)*2])
		else:
			printList.append(y)
	return printList
	

tf_trace = TF_Trace("TF_trace.bin")
tf_trace.DecodeFile()
indiPoints = {}
idMap = BuildMap(stacks, "tf_id")
stackEntries = []
for x in stacks:
	stackEntries.append(StackEntry(x))
	stackEntries[-1].BuildRelationships(idMap,stacks,tf_trace._records)
timeSavableEntries = []

for x in stackEntries:
	print "Time saveable in global id - " + str(x._stack.GetGlobalId()) + " equals " +  str(x.GetTimeSavable())
	neighborIDs = []
	timeSavableEntries.append(x.GetPrintout())
	for y in x._neighbors: 
		neighborIDs.append(x._neighbors[y]._stack.GetGlobalId())

	print "With the following neighbors:"
	print neighborIDs


	#print x
	# userCall = GetFirstUserCall(x)
	# #print userCall
	# if userCall == None:
	# 	continue
	# h = x.GetGlobalId()
	# if h not in indiPoints:
	# 	indiPoints[h] = Aggregator(userCall[1], userCall[0])
	# indiPoints[h].AddStack(x)

timeSavableEntries.sort(key=lambda x: x[0])

for x in timeSavableEntries:
	if x[0] == 0:
		continue
	print x[1]


# print "%-60.60s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-4.4s" % ("File Name", 
# 	"Cuda Call", "Call Count", "Unnecessary Calls", "Total Time", "Time Saved", "ID")
# print ""

# tmpStore = []
# for x in indiPoints:
# 	usage = indiPoints[x].GetUsageInfo()
# 	stackInfo = usage["StackEntry"].to_dict()
# 	idList = ""
# 	for y in indiPoints[x]._stacks:
# 		idList += str(y.GetGlobalId()) + " "
# 	tmp = stackInfo["filename"].split("/")
# 	if len(tmp) > 0:
# 		stackInfo["filename"] = tmp[-1]
# 	tmpStore.append([usage["TimeSaved"],"%-60.60s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-4.4s" % (stackInfo["funcname"],
# 		usage["CudaCall"],str(usage["CallCount"]),str(usage["UnnecessaryCalls"]), str(usage["TotalTime"]), str(usage["TimeSaved"]), idList)])


# tmpStore.sort(key=lambda x: x[0])

# for x in tmpStore:
# 	print x[1]

# print "%-60.60s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-4.4s" % ("File Name", 
# 	"Cuda Call", "Call Count", "Unnecessary Calls", "Total Time", "Time Saved", "ID")