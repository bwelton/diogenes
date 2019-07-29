import os
from ReadCUPTIEvents import ReadCUPTIEvents
from ReadTimingEvents import ReadTimingPacket

class CalculateSavings:
	def __init__(self, cupti, timing, correlation, timeline):
		self._cupti = cupti
		self._timing = timing
		self._correlation = correlation
		self._timeline = timeline

	def Process(self):
		cuptiProcs = self._cuptiEvents.keys()
		timingProcs = self._timingEvents.keys()
		for x in timingProcs:
			tEvent = self._timingEvents[x].GetNextEvent()

			while tEvent != None:
				if self._timeline.IsDuplicateId(tEvent[0]) == False:
					print "Transfer " + str(tEvent[0]) + " is not a duplicate, skipping"
					continue
				cEvent = self._correlation.GetCUPTIRecord(x, tEvent[0])
				

