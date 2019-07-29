from Iterators import BaseIterator
class CorrelatedRecord(BaseIterator):
	def __init__(self, l):
		BaseIterator.__init__(self, l)

	def StrOneLine(self):
		timeline_id = -1
		corr_id = -1
		
		if self.GetTimelineId() != None:
			timeline_id = self.GetTimelineId()

		if self.GetCorrId() != None:
			corr_id = self.GetCorrId()

		return ("Transfer Corrlation ID: %d Timeline ID: %d is %s" % (corr_id, timeline_id, ("a duplicate" if self.IsTransDuplicate() else "not a duplicate")))

	def GetTimelineId(self):
		for x in self._data:
			if x.GetAttr("timelineid") != None:
				return x.GetAttr("timelineid")
		return None	

	def printme(self):
		for x in self._data:
			print x

	def IsSynch(self):
		for x in self._data:
			if x.IsSynchronization() == True:
				print x
				return True
		return False

	def GetCPUTime(self):
		time = 0.0
		for x in self._data:
			time += x.GetCPUTime()
		return time

	def GetGPUTime(self):
		time = 0.0
		for x in self._data:
			time += x.GetGPUTime()
		return time
		
	def IsTransDuplicate(self):
		for x in self._data:
			if x.GetAttr("dupid") == None:
				continue
			else:
				if x.GetAttr("dupid") != 0:
					return True
		return False

	def GetCorrId(self):
		for x in self._data:
			if x.GetAttr("corrid") != None:
				return x.GetAttr("corrid")
		return None

class GenCorrelation:
	def __init__(self, process):
		self._process = process
		self._process[0].Reset()
		self._process[1].Reset()
		self._process[2].Reset()
		print type(self._process[0])
		print type(self._process[1])
		print type(self._process[2])

	def FindCorrInfo(self, time_entry):
		rec = self._process[2].FindID(time_entry.GetAttr("timelineid"))
		if rec == None:
			print "Could not find timeline id between Correlation and timeline"
			return None
		return rec

	def GetTimingInfo(self, coorRecord):
		corrId = -1
		ret = []
		while(1):
			timingInfo = self._process[1].GetCurrent()
			# print timingInfo 
			# print coorRecord
			if corrId == -1:
				if timingInfo.GetAttr("size") == coorRecord.GetAttr("size"):
					corrId = timingInfo.GetAttr("corrid")
					ret.append(timingInfo)
			elif corrId != timingInfo.GetAttr("corrid"):
				break
			elif corrId == timingInfo.GetAttr("corrid"):
				ret.append(timingInfo)
			if self._process[1].GetNext() == None:
				break
		if len(ret) == None:
			print "We could not correlate timing record to correlation record"
		return ret

	def CorrelateIDs(self):
		ret = []
		while(1):
			timelineRec = self._process[0].GetCurrent()
			coorRecord = self.FindCorrInfo(timelineRec)
			# print coorRecord
			timingInfo = self.GetTimingInfo(coorRecord)
			ret.append(CorrelatedRecord([timelineRec, coorRecord] + timingInfo))
			if self._process[0].GetNext() == None:
				break
		return ret

