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
			return [tmp[x].GetFunctionName(),x-1]
	return None

def GetCustomID(stack, startPos):
	global DEPTH
	tmp = stack.GetStacks()	
	count = 0
	ident = ""
	for x in range(startPos, 0, -1):
		ident += tmp[x].GetComparitor() + ":"
		count += 1
		if count >= DEPTH:
			break
	return ident[:-1]

def GetLineInfo(stack, startPos):
	global DEPTH
	ret = []
	tmp = stack.GetStacks()	
	count = 0
	ident = ""
	for x in range(startPos, 0, -1):
		ret.append(tmp[x].to_dict()["funcname"])
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



print "%-60.60s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s" % ("Stack Trace", 
	"Cuda Call", "Call Count", "Unnecessary Calls", "Total Time", "Time Recoverable")
print ""

tmpStore = []
for x in indiPoints:
	usage = indiPoints[x].GetUsageInfo()
	#stackInfo = usage["StackEntry"].to_dict()
	idList = ""
	for y in indiPoints[x]._stacks:
		idList += str(y.GetGlobalId()) + " "
	#idList = ""
	printList = []
	for y in usage["StackEntry"]:
		# print y
		tmp = ("%-60.60s" % y)
		if len(printList) > 0:
			printList.append((" " * len(printList)*2) + tmp[:-len(printList)*2])
		else:
			printList.append(y)
	# print printList
	tmpStore.append([usage["TimeSaved"],"%-60.60s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %s" % (printList[0],usage["CudaCall"],str(usage["CallCount"]),str(usage["UnnecessaryCalls"]), str(usage["TotalTime"]), str(usage["TimeSaved"]), idList)])
	for x in range(1, len(printList)):
		tmpStore[-1][-1] += "\n" + "%-60.60s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %s" %(printList[x],"","","","","","")

tmpStore.sort(key=lambda x: x[0])

for x in tmpStore:
	print x[1]
	print "-" * (60+20+20+20+20+37)
print "%-60.60s | %-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s" % ("Stack Trace", 
	"Cuda Call", "Call Count", "Unnecessary Calls", "Total Time", "Time Recoverable")