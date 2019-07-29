import json
import os
from Driver2 import JSStack, JSStackEntry


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


indiPoints = {}

for x in stacks:
	#print x
	userCall = GetFirstUserCall(x)
	#print userCall
	if userCall == None:
		continue
	h = x.GetGlobalId()
	if h not in indiPoints:
		indiPoints[h] = Aggregator(userCall[1], userCall[0])
	indiPoints[h].AddStack(x)



print "%-60.60s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-4.4s" % ("File Name", 
	"Cuda Call", "Call Count", "Unnecessary Calls", "Total Time", "Time Saved", "ID")
print ""

tmpStore = []
for x in indiPoints:
	usage = indiPoints[x].GetUsageInfo()
	stackInfo = usage["StackEntry"].to_dict()
	idList = ""
	for y in indiPoints[x]._stacks:
		idList += str(y.GetGlobalId()) + " "
	tmp = stackInfo["filename"].split("/")
	if len(tmp) > 0:
		stackInfo["filename"] = tmp[-1]
	tmpStore.append([usage["TimeSaved"],"%-60.60s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-4.4s" % (stackInfo["funcname"],
		usage["CudaCall"],str(usage["CallCount"]),str(usage["UnnecessaryCalls"]), str(usage["TotalTime"]), str(usage["TimeSaved"]), idList)])


tmpStore.sort(key=lambda x: x[0])

for x in tmpStore:
	print x[1]

print "%-60.60s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-4.4s" % ("File Name", 
	"Cuda Call", "Call Count", "Unnecessary Calls", "Total Time", "Time Saved", "ID")