import json
import os
from Driver2 import JSStack, JSStackEntry
import sys
DEPTH = int(sys.argv[1])

class Aggregator:
	def __init__(self, stackEntry, cudaCall):
		self._stackEntry = stackEntry
		self._stacks = []
		self._cudaCall = cudaCall

	def AddStack(self, stack):
		self._stacks.append(stack)

	def GetUsageInfo(self):
		ret = {"CudaCall": self._cudaCall, "StackEntry" : self._stackEntry, "CallCount" : 0, "UnnecessaryCalls" : 0, "TotalTime" : 0.0, "TimeSaved" : 0.0, "Removed" : 0.0}
		syncIssue = False
		transIssue = False
		for x in self._stacks:
			ret["CallCount"] += x.GetCount()
			ret["UnnecessaryCalls"] += x.GetUnnecessaryCount()
			ret["TotalTime"] += x.GetTotalTime()
			ret["TimeSaved"] += x.GetEstimatedSavings2()
			ret["Removed"] += x.GetEstimatedSavings()
			if x.HasSynchronizationIssues():
				syncIssue = True
			if x.HasTransferIssues():
				transIssue = True
		if ret["Removed"] > ret["TotalTime"]:
			ret["Removed"] = ret["TotalTime"]
		if ret["Removed"] < ret["TimeSaved"]:
			ret["Removed"] = ret["TimeSaved"]

		if syncIssue and transIssue:
			ret["TypeInfo"] = "Sync+Trans"
		elif syncIssue:
			ret["TypeInfo"] = "Sync"
		else:
			ret["TypeInfo"] = "Trans"
			
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
			return [tmp[x].GetFunctionName(),x-1]
	return None

def GetCustomID(stack, startPos):
	global DEPTH
	tmp = stack.GetStacks()	
	count = 0
	ident = ""
	for x in range(startPos, 0, -1):
		ident += GetRawFunctionName(tmp[x].to_dict()["funcname"]) + ":"
		count += 1
		if count >= DEPTH:
			break
	return ident[:-1]


def GetRawFunctionName(funcName):
	pos = funcName.find("(")
	pos2 = funcName.find("<")
	if pos == -1:
		return funcName
	if pos2 < pos:
		return funcName.split("<")[0]
	return funcName


def GetLineInfo(stack, startPos):
	global DEPTH
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



indiPoints = {}

for x in stacks:
	#print x
	userCall = GetFirstUserCall(x)
	#print userCall
	if userCall == None:
		continue
	h = GetCustomID(x, userCall[1])
	if h not in indiPoints:
		indiPoints[h] = Aggregator(GetLineInfo(x, userCall[1]), userCall[0])
	indiPoints[h].AddStack(x)



print "%-30.30s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-10.10s" % ("Stack Trace", 
	"Cuda Call", "Call Count", "Unnecessary Calls", "Total Time", "Time Recoverable", "If Removed")
print ""

tmpStore = []
for x in indiPoints:
	usage = indiPoints[x].GetUsageInfo()
	#stackInfo = usage["StackEntry"].to_dict()
	idList = ""
	for y in indiPoints[x]._stacks:
		idList += str(y.GetGlobalId()) + " "
	idList = ""
	printList = []
	for y in usage["StackEntry"]:
		# print y
		tmp = ("%-60.60s" % y)
		if len(printList) > 0:
			printList.append((" " * len(printList)*2) + tmp[:-len(printList)*2])
		else:
			printList.append(y)
	# print printList
	tmpStore.append([usage["TimeSaved"],"%-45.45s | %-21.21s | %-13.13s | %-13.13s | %-10.10s | %-11.11s | %-10.10s |" % (printList[0],usage["CudaCall"],str(usage["CallCount"]),str(usage["UnnecessaryCalls"]), str(usage["TotalTime"]), str(usage["TimeSaved"]), str(usage["Removed"]))])
	for x in range(1, len(printList)):
		tmpStore[-1][-1] += "\n" + "%-45.45s | %-21.21s | %-13.13s | %-13.13s | %-10.10s | %-11.11s | %-10.10s |" %(printList[x],"","","","","","")

tmpStore.sort(key=lambda x: x[0])

for x in tmpStore:
	print x[1]
	print "-" * (45+21+13+13+10+10+10+10+10)
print "%-45.45s | %-21.21s | %-13.13s | %-13.13s | %-10.10s | %-11.11s | %-10.10s" % ("Stack Trace", 
	"Cuda Call", "Call Count", "Unnecessary", "Total Time", "Est Benefit", "If Removed")