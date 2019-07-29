import json
import os
from DisplayFormatter import TextRow, DisplayFormatter
from sets import Set
from TF_trace import TF_Trace
from Driver2 import JSStack, JSStackEntry, BuildMap
from ReadExecTime import ReadExecTime
EXEC_TIME = ReadExecTime()
cudaFunctions = os.path.join(os.path.dirname(os.path.realpath(__file__)),"cudaFunctions.txt")
f = open(cudaFunctions, "r")
data = f.readlines()
f.close()

localDisplay =  DisplayFormatter()
localDisplay._globalID = 10000000


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
		self._perNeighborTimes = {}
		self._ordering = []

	def GetTimeSavable(self):
		print self._deltaTimes
		print self._totalTime
		if (self._deltaTimes > self._totalTime):
			return self._totalTime
		return self._deltaTimes

	def GetSubsequenceSavings(self, start, end):
		startPos = self._ordering.index(start)
		endPos = self._ordering.index(end)
		dtimes = 0.0
		ttimes = 0.0
		for x in range(startPos, endPos+1):
			ttimes += self._perNeighborTimes[self._ordering[x]][0]
			dtimes += self._perNeighborTimes[self._ordering[x]][1]
		if dtimes > ttimes:
			return ttimes
		return dtimes

	def BuildSequenceTable(self):
		retTable = []
		for x in range(0, len(self._ordering)):
			retTable.append([0.0 for z in self._ordering])
			for y in range(x, len(self._ordering)):
				retTable[x][y] = self.GetSubsequenceSavings(self._ordering[x], self._ordering[y])


		for x in range(0, len(self._ordering)):
			ret = ""
			for y in range(0, len(self._ordering)):
				ret += " {0:4.2}".format(retTable[x][y])
			print ret
		return retTable


	def BuildSpecificView(self, startPos, endPos, seqtable, callsSeen):
		global localDisplay
		myID = localDisplay.GetID()
		start = self._ordering.index(startPos)
		end = self._ordering.index(endPos)
		textRow = ["Time Recoverable In Subsequence: {0:2.3f}s ({1:2.2f}% of execution time) ".format(seqtable[start][end], (seqtable[start][end] / EXEC_TIME) * 100.0) + " " * 25]
		for x in range(0, len(self._ordering)):
			if self._ordering[x] not in callsSeen:
				continue
			if x >= start and x <=end:
				textRow.append("{0:2d}. {1:s} in {2:s} at line {3:d}".format(x, callsSeen[self._ordering[x]][1].GetFunctionName(), callsSeen[self._ordering[x]][2].to_dict()["filename"],callsSeen[self._ordering[x]][2].to_dict()["linenum"]))

		localDisplay.AddElement(myID, [TextRow(textRow,0)])
		return myID

	def BuildStartSequences(self, startPos,seqtable, callsSeen):
		global localDisplay
		myID = localDisplay.GetID()
		textRow = [TextRow(["Time Recoverable: {0:2.3f}s ({1:2.2f}% of execution time) ".format(self.GetTimeSavable(), (self.GetTimeSavable() / EXEC_TIME) * 100.0) + " " * 25],0)]
		textRow.append(TextRow(["Select ending of subsequence to get refined estimate"],0))
		textRow.append(TextRow([""],0))
		
		start = self._ordering.index(startPos)
		for x in range(0, len(self._ordering)):
			if self._ordering[x] not in callsSeen:
				continue
			if x < start:
				textRow.append(TextRow(["{0:2d}. {1:s} in {2:s} at line {3:d}".format(x+1, callsSeen[self._ordering[x]][1].GetFunctionName(), callsSeen[self._ordering[x]][2].to_dict()["filename"],callsSeen[self._ordering[x]][2].to_dict()["linenum"])],0))
			elif x == start:
				# print callsSeen[self._ordering[x]]
				# print callsSeen[self._ordering[x]][2].to_dict()["filename"]
				# print callsSeen[self._ordering[x]][1].GetFunctionName()

				item = ["[Start] {0:2d}. {1:s} in {2:s} at line {3:d}".format(x+1, 
					callsSeen[self._ordering[x]][1].GetFunctionName(), 
					callsSeen[self._ordering[x]][2].to_dict()["filename"], 
					callsSeen[self._ordering[x]][2].to_dict()["linenum"])]
				textRow.append(TextRow(item,0))
			else:
				# print callsSeen[self._ordering[x]]
				# print self._ordering[x]
				# print callsSeen[self._ordering[x]][1].GetFunctionName()
				# print callsSeen[self._ordering[x]][2].to_dict()["filename"]
				# print callsSeen[self._ordering[x]][2].to_dict()["linenum"]
				otherId =  self.BuildSpecificView(startPos, self._ordering[x], seqtable, callsSeen)
				textRow.append(TextRow(["{0:2d}. {1:s} in {2:s} at line {3:d}".format(x+1, 
					callsSeen[self._ordering[x]][1].GetFunctionName(), 
					callsSeen[self._ordering[x]][2].to_dict()["filename"],
					callsSeen[self._ordering[x]][2].to_dict()["linenum"])],
					0,otherId))
		localDisplay.AddElement(myID, textRow)
		return myID

	def CreateDisplayOutput(self):
		global localDisplay
		global EXEC_TIME

		idMaper = {}
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
			print "My X is " + str(x)
			if call == None:
				continue
			# print call
			if x not in callsSeen:
				callsSeen[x] = []
			callsSeen[x] = call

		myID = localDisplay.GetID()
		textRow = "Time Recoverable: {0:2.3f}s ({1:2.2f}% of execution time) ".format(self.GetTimeSavable(), (self.GetTimeSavable() / EXEC_TIME) * 100.0) + " " * 25
		headerRows = [TextRow([textRow],0)]
		textRow = "Number of Sync Issues: {0:3d}    Number of Transfer Issues: {1:3d}".format(syncIssues, transIssues) + " " * 37
		headerRows.append(TextRow([textRow],0))
		textRow = ""
		headerRows.append(TextRow([textRow],0))
		textRow = "Select start/ending subsequence to get refined estimate"
		headerRows.append(TextRow([textRow],0))

		print "Building Sequence Table for - " + str(self.GetTimeSavable())
		sequenceTable = self.BuildSequenceTable()
		print self._perNeighborTimes
		# headerRows = []
		curCount = 1
		print callsSeen
		for x in self._ordering:
			if x not in callsSeen:
				continue
			startID = self.BuildStartSequences(x, sequenceTable, callsSeen)
			rowString = TextRow(["{0:2d}. {1:s} in {2:s} at line {3:d}".format(curCount, callsSeen[x][1].GetFunctionName(), callsSeen[x][2].to_dict()["filename"],callsSeen[x][2].to_dict()["linenum"])], 0, startID)
			headerRows.append(rowString)
			curCount+= 1


		localDisplay.AddElement(myID, headerRows)
		fileName = callsSeen[self._ordering[0]][2]._fileName.split("/")[-1]
		return [self.GetTimeSavable(),myID,TextRow(["{0:2.3f}s ({1:2.2f})% Sequence starting at call {2:s} in {3:s} at line {4:d}".format(self.GetTimeSavable(), (self.GetTimeSavable() / EXEC_TIME) * 100.0,
			callsSeen[self._ordering[0]][0][:30], fileName, callsSeen[self._ordering[0]][2]._lineNum)],0,myID)]


	def GetPrintout(self):
		global EXEC_TIME
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
		ret += "\n= Time Recoverable: {0:2.3f}s ({1:2.2f}% of execution time) ".format(self.GetTimeSavable(), (self.GetTimeSavable() / EXEC_TIME) * 100.0) + " " * 25 + " ="
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
			saveTime = float(self.GetTimeSavable())
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
				self._perNeighborTimes[entryId] = [0.0,0.0]
				self._ordering.append(entryId)

			self._totalTime += useData["UseAverage"]
			self._perNeighborTimes[entryId][0] += useData["UseAverage"]
			self._deltaTimes += useData["DeltaAvg"]
			self._perNeighborTimes[entryId][1] += useData["DeltaAvg"]
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
print len(stacks)
for x in stacks:
	stackEntries.append(StackEntry(x))
	print "Building Build Relationships"
	stackEntries[-1].BuildRelationships(idMap,stacks,tf_trace._records)
timeSavableEntries = []



findLongest = []
for x in stackEntries:
	tmp = x.GetPrintout()
	if tmp[0] / EXEC_TIME < 0.001:
		continue
	
	findLongest.append([tmp[0],x, tmp, x.CreateDisplayOutput()])


findLongest.sort(key=lambda x: x[0], reverse=True)

discards = {}
for x in range(0, len(findLongest)):
	print x
	print findLongest[x]
	entries = Set(findLongest[x][1]._neighbors.keys())
	for y in range(x+1, len(findLongest)):
		other = Set(findLongest[y][1]._neighbors.keys())
		if entries.issuperset(other):
			discards[y] = 1
		elif entries.issubset(other):
			discards[y] = 1

print discards
outJson = {"initial":[]}
for x in range(0,len(findLongest)):
	if x in discards:
		continue
	print x
	outJson["initial"].append(findLongest[x][3][2]._data)
	outJson["initial"][-1]["timeData"] = float(findLongest[x][3][0])
	timeSavableEntries.append(findLongest[x][2])

outJson["Elements"] = localDisplay.SerializeElements()["Elements"]




# for x in stackEntries:
# 	print "Time saveable in global id - " + str(x._stack.GetGlobalId()) + " equals " +  str(x.GetTimeSavable())
# 	neighborIDs = []
# 	tmp = x.GetPrintout()
# 	if tmp[0] / EXEC_TIME < 0.001:
# 		continue
# 	timeSavableEntries.append(tmp)
# 	for y in x._neighbors: 
# 		neighborIDs.append(x._neighbors[y]._stack.GetGlobalId())

# 	print "With the following neighbors:"
# 	print neighborIDs


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

j2 = json.dumps(outJson, indent=4)
j = json.dumps(timeSavableEntries, indent=4)
f = open("sequential_pproc.json", "wb")
f.write(j)
f.close()
f = open("sequential.json", "wb")
f.write(j2)
f.close()

# for x in timeSavableEntries:
# 	if x[0] == 0:
# 		continue
# 	print x[1]


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