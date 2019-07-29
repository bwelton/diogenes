import json
import os
from Driver2 import JSStack, JSStackEntry

class Aggregator:
	def __init__(self):
		self._count = 0
		self._time = 0.0
		self._unnecessary = 0
		self._timeRecoverable = 0.0

	def AddCall(self, count, time):
		self._count += count
		self._time += time

	def Unnecessary(self, count, time):
		self._unnecessary += count
		self._timeRecoverable += time

	def PrintSummary(self, name):
		return [self._timeRecoverable,"%-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s" % (name, str(self._count), str(self._time), str(self._unnecessary), str(self._timeRecoverable))]


## View 1: Top Level View
#  cudaCall callCount total time %unnecessary estimated time savings
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

print len(stacks)

for x in stacks:
	callId = None
	stack = x.GetStacks()
	for y in stack:
		if y.GetFunctionName() in cudaCalls:
			callId = y.GetFunctionName()
			break
	if callId == None:
		continue
	if cudaCalls[callId] == None:
		cudaCalls[callId] = Aggregator()
	cudaCalls[callId].AddCall(x.GetCount(), x.GetTotalTime())
	cudaCalls[callId].Unnecessary(x.GetUnnecessaryCount(), x.GetEstimatedSavings())

outList = []

for x in cudaCalls:
	if cudaCalls[x] != None:
		outList.append(cudaCalls[x].PrintSummary(x))

outList.sort(key=lambda x: x[0])


print "%-20.20s | %-20.20s | %-20.20s | %-20.20s | %-20.20s" % ("Function", "Call Count", "Time (s)", "Unnecessary Calls", "Time Recoverable")
for x in outList:
	print x[1]


