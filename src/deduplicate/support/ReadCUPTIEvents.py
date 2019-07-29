import os
from sets import Set
import sys

class CUPTIEvent:
	def __init__(self, line):
		self._split =  line.split(",")
		if "\n" in self._split[-1]:
			self._split[-1] = self._split[-1][:-1]
		if "TET" in line:
			return
		self._recType = self._split[0]
		self._callName = self._split[1]
		self._coorId = self._split[2]
		self._startTime = int(self._split[3])
		self._endTime = int(self._split[4])
		self.InitTypeValues()
		if "RR" in self._split[0] or "DR" in self._split[0]:
			self.ParseRRandDR()
		elif "CPY" in self._split[0]:
			self.ParseCPY()

	def __str__(self):
		return str(self._recType) + "," + str(self._callName) + "," + (str(self._bytes) if self._bytes != None else "0") + "\n"

	def GetTime(self):
		return self._endTime - self._startTime

	def GetCorrId(self):
		return self._coorId

	def GetProcId(self):
		return self._processId

	def GetThreadId(self):
		return self._threadId

	def ReturnType(self):
		return self._recType

	def InitTypeValues(self):
		self._processId = None
		self._threadId = None
		self._bytes = None
		self._runtimeID = None
		self._contextId = None
		self._deviceId = None
		self._streamId = None

	def CompareStreamAndSize(self, streamId, size):
		# print self._streamId 
		# print self._bytes
		if self._streamId == None or self._bytes == None:
			return False
		## Fix this later....
		# print int(size)
		if int(self._bytes) == int(size):
			return True
		return False

	def ParseRRandDR(self):
		self._processId = str(self._split[5])
		self._threadId = str(self._split[6])

	def ParseCPY(self):
		self._bytes = int(self._split[5])
		self._runtimeID = int(self._split[6])
		self._contextId = int(self._split[7])
		self._deviceId = int(self._split[8])
		self._streamId = int(self._split[9])

class TransferEvent:
	def __init__(self, correlationId):
		self._coorId = correlationId
		self._events = []
		self._threadId = None
		self._procId = None

	def IsTransfer(self):
		for x in self._events:
			if x.ReturnType() == "CPY":
				return True
		return False


	def GetCorrId(self):
		return self._coorId

	def AddEvent(self, event):
		if event.GetCorrId() != self._coorId:
			print "ERROR ATTEMPTED TO ADD A TRANSFER WITH IMPROPER CORRELATION ID"
			return
		if event.GetProcId() != None:
			if self._procId != None and event.GetProcId() != self._procId:
				print "ERROR - Multiple process id's for the same transfer event, should not happen"
				return
			self._procId = event.GetProcId()
		if event.GetThreadId() != None:
			if self._threadId != None and event.GetThreadId() != self._threadId:
				print "ERROR - Multiple thread id's for the same transfer event, should not happen"
				return
			self._threadId = event.GetThreadId()		

		self._events.append(event)

	def GetProcAndThreadId(self):
		if self._procId == None or self._threadId == None:
			print "Error getting thread/proc id for transfer"
			return "ERROR"
		return str(self._procId) + "&" + str(self._threadId)

	def Compare(self, streamId, size):
		for x in self._events:
			if x.CompareStreamAndSize(streamId, size) == True:
				return True
		return False


	def __str__(self):
		ret =  "TransferEvent: " + str(self._coorId) + " " + str(self._threadId) + " " + str(self._procId)
		for x in self._events:
			ret += str(x) 
		return ret

class CPUProcess:
	def __init__(self, procId, threadId):
		self._procId = procId
		self._threadId = threadId
		self._transferEvents = []
		self._transferLoc = {}
		self._transferPos = 0
		self._transferCount = -1

	def AddEvent(self, event):
		self._transferEvents.append(event)
		self._transferLoc[event.GetCorrId()] = len(self._transferEvents) - 1
	def GetNextTransferEvent(self):
		if self._transferPos > len(self._transferEvents):
			return None
		ret = -1
		for x in range(self._transferPos, len(self._transferEvents)):
			if self._transferEvents[x].IsTransfer() == True:
				ret = x
				break
		if ret == -1:
			self._transferPos = len(self._transferEvents)
			return None
		self._transferPos = ret + 1
		# print str(self._transferEvents[ret])
		return self._transferEvents[ret]

	def CountTransfers(self):
		if self._transferCount >= 0:
			return self._transferCount
		count = 0
		prevTrans = self._transferPos
		self.ResetPosition()
		while (self.GetNextTransferEvent() != None):
			count += 1

		print "Process Count" + str(count)
		self._transferCount = count
		self._transferPos =  prevTrans

		return count

	def GetEvent(self, id):
		if len(self._transferEvents) <= id:
			return None
		return self._transferEvents[id]


	def GetEventCorrId(self, id):
		if id not in self._transferLoc:
			return None
		return [self._transferEvents[self._transferLoc[id]],self._transferLoc[id]]

	def ResetPosition(self):
		self._transferPos = 0

	def __str__(self):
		ret = "CPU Process: " + str(self._procId) + " " + str(self._threadId) + "\n"
		for x in self._transferEvents:
			ret += str(x) + "\n"
		return ret


class ReadCUPTIEvents:
	def __init__(self, filename):
		f = open(filename, "r")
		if f == None:
			print "Could not open CUPTI events file"
			return
		self._data = f.readlines()
		self._processes = {}
		f.close()

	def Process(self):
		events = []
		TET = []
		for x in self._data:
			if "CPY," in x or "DR," in x or "RR," in x:
				events.append(CUPTIEvent(x))
			elif "TET" in x:
				TET.append(float(x.split(",")[1]))
		del self._data

		## Merge the correlation ids into single transfer events
		transfers = {}
		for x in events:
			if x.GetCorrId() not in transfers:
				transfers[x.GetCorrId()] = TransferEvent(x.GetCorrId())
			transfers[x.GetCorrId()].AddEvent(x)

		ids = transfers.keys()
		ids.sort(key=int)
		del transfers[ids[0]]
		ids = ids[1:]
		procNames = {}
		for x in ids:
			procNames[transfers[x].GetProcAndThreadId()] = 1

		processes = {}
		for x in procNames.keys():
			processes[x] = CPUProcess(x.split("&")[0], x.split("&")[1])

		for x in ids:
			processes[transfers[x].GetProcAndThreadId()].AddEvent(transfers[x])
		
		self._processes = processes
		return processes

	def __str__(self):	
		ret = "We have data for " + str(len(self._processes))
		for x in self._processes.keys():
			ret += str(self._processes[x]) + "\n"
		return ret
