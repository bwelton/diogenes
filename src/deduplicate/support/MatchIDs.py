import os
from ReadCUPTIEvents import ReadCUPTIEvents
from ReadTimingEvents import ReadTimingPacket

class CorrelationClass:
	def __init__(self):
		self._cupti = {}
		self._timing = {}

	def AddCorrelation(self, cupti, timing):
		self._cupti[cupti] = timing
		self._timing[timing] = cupti

	def GetCorrelationCupti(self, cupti):
		if cupti not in self._cupti:
			return None
		return self._cupti[cupti]

	def GetCorrelationTiming(self, timing):
		if timing not in self._timing:
			return None
		return self._timing[timing]

class MatchIDs:
	def __init__(self, cuptiEvents, timingEvents):
		self._cuptiEvents = cuptiEvents
		self._timingEvents = timingEvents

	def Correlate(self):
		cuptiProcs = self._cuptiEvents.keys()
		timingProcs = self._timingEvents.keys()
		correlation = {}
		for x in timingProcs:
			found = False
			tEvent = self._timingEvents[x].GetNextEvent()
			for y in cuptiProcs:
				if self._timingEvents[x].CountTransfers() != self._cuptiEvents[y].CountTransfers():
					continue
				cEvent = self._cuptiEvents[y].GetNextTransferEvent()
				if cEvent.Compare(tEvent[1], tEvent[2]):
					print y + " correlates to " + x
					correlation[y] = x
					found = True
					cuptiProcs.remove(y)
					self._cuptiEvents[y].ResetPosition()
					break	
				self._cuptiEvents[y].ResetPosition()			
			self._timingEvents[x].ResetPosition()
		print correlation
		self._correlation = correlation
		self._correlationIndex = self.BuildCorrelation()

	def GetCUPTIRecord(self, timingId, transId):
		foundId = None
		for x in self._correlation.keys():
			if self._correlation[x] == timingId:
				foundId = x
				break
		if foundId == None:
			print "Could not find timing -> CUPTI correlation for : " + timingId
			return None
		corrId = self._correlationIndex.GetCorrelationTiming(transId)
		ret = self._cuptiEvents[foundId].GetEventCorrId(corrId)
		self._prevEvent = ret[1]
		self._foundId = foundId
		return ret[0]

	def GetNextEvent(self):
		self._prevEvent += 1
		return self._cuptiEvents[self._foundId].GetEvent(self._prevEvent)


	def BuildCorrelation(self):
		ret = CorrelationClass()
		for index in self._correlation:
			procCupti = self._cuptiEvents[index]
			procTiming = self._timingEvents[self._correlation[index]]
			procCupti.ResetPosition()
			procTiming.ResetPosition()
			while(1):
				cEvent = procCupti.GetNextTransferEvent()
				tEvent = procTiming.GetNextEvent()
				if (cEvent == None or tEvent == None):
					break
				if cEvent.Compare(tEvent[1], tEvent[2]) == False:
					print "Error correlating events"
					print str(cEvent)
					print str(tEvent)
					return
				ret.AddCorrelation(cEvent.GetCorrId(), tEvent[0])
			procCupti.ResetPosition()
			procTiming.ResetPosition()
		print ret


	def Process(self):
		self.Correlate()