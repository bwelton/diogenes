import os
from sets import Set

class BaseIterator:
	def __init__(self, l):
		self._data = l
		self._curPos = 0

	def GetCorrID(self):
		return self._data[self._curPos].GetAttr("corrid")

	def GetTimelineId(self):
		return self._data[self._curPos].GetAttr("timelineid")

	def GetStartTime(self):
		return self._data[self._curPos].GetAttr("stime")

	def GetEndTime(self):
		return self._data[self._curPos].GetAttr("etime")

	def GetCallName(self):
		return self._data[self._curPos].GetAttr("cname")

	def GetSize(self):
		return self._data[self._curPos].GetAttr("size")

	def GetStream(self):
		return self._data[self._curPos].GetAttr("stream")

	def GetProcID(self):
		return self._data[self._curPos].GetAttr("procid")

	def GetThreadID(self):
		return self._data[self._curPos].GetAttr("threadid")

	def GetType(self):
		return self._data[self._curPos].GetAttr("type")

	def IsDuplicate(self):
		if self._data[self._curPos].GetAttr("dupid") == None:
			return -1
		else:
			return self._data[self._curPos].GetAttr("dupid")

	def GetNext(self):
		self._curPos += 1
		if self._curPos < len(self._data):
			return self._data[self._curPos]
		else:
			self.Reset()
			return None


	def GetCurrent(self):
		return self._data[self._curPos]

	def GetPosition(self):
		return self._curPos

	def Reset(self, pos = 0):
		self._curPos = pos

class PacketCorrectionIterator(BaseIterator):
	def __init__(self, l):
		BaseIterator.__init__(self,l)

	def DumpIds(self):
		ret = Set()
		self.Reset()
		ret.add(self.GetTimelineId())
		while self.GetNext() != None:
			ret.add(self.GetTimelineId())
		self.Reset()
		return ret


	def CheckCurrentProcThread(self, proc, thread):
		if self.GetProcID() == proc and self.GetThreadID() == thread:
			return True
		return False

	def GetIdsByProcess(self, procid, threadid):
		self.Reset()
		ids = [self.GetTimelineId()] if self.CheckCurrentProcThread(procid, threadid) == True else []
		while self.GetNext() != None:
			if self.CheckCurrentProcThread(procid, threadid) == True:
				ids.append(self.GetTimelineId())
		self.Reset()
		return ids

	def FindID(self, id):
		if self.GetTimelineId() == id:
			return self.GetCurrent()
		while self.GetNext() != None:
			if self.GetTimelineId() == id:
				return self.GetCurrent()
		return None

	def CreateSubsetPacket(self, ids):
		self.Reset()		
		idPos = 0
		retIds = [self.GetCurrent()] if self.GetTimelineId() == ids[idPos] else []
		while self.GetNext() != None:
			if self.GetTimelineId() == ids[idPos]:
				retIds.append(self.GetCurrent())
				idPos += 1
		if len(retIds) != len(ids):
			print "We should have equal id length, but we dont in timing correction"
			exit(-1)
		self.Reset()
		return PacketCorrection(retIds)

class TimelineIterator(BaseIterator):
	def __init__(self, l):
		BaseIterator.__init__(self, l)

	def GetCurrentTransfer(self):
		return self.GetCurrent()

	def GetNextTransfer(self):
		return self.GetNext()

	def DumpIds(self):
		ret = Set()
		self.Reset()
		ret.add(self.GetTimelineId())
		while self.GetNext() != None:
			ret.add(self.GetTimelineId())
		self.Reset()
		return ret

	def GetIdsByCorrection(self, corr):
		matchIds = corr.DumpIds() & self.DumpIds()
		tmp =  list(matchIds)
		tmp.sort()
		return tmp

	def CreateSubsetTimeing(self, ids):
		self.Reset()		
		idPos = 0
		retIds = [self.GetCurrent()] if self.GetTimelineId() == ids[idPos] else []
		while self.GetNext() != None:
			if self.GetTimelineId() == ids[idPos]:
				retIds.append(self.GetCurrent())
				idPos += 1
		if len(retIds) != len(ids):
			print "We should have equal id length, but we dont in timing correction"
			exit(-1)
		self.Reset()
		return TimelineIterator(retIds)

class TimingInfoIterator(BaseIterator):
	def __init__(self, l):
		BaseIterator.__init__(self, l)
		self.GroupById()
		self._curIdPos = 0

	def GroupById(self):
		self._ids = {}
		self._idKeys = []
		self._ids[self.GetCorrID()] = [self.GetCurrent()]
		while self.GetNext() != None:
			if self.GetCorrID() not in self._ids:
				self._ids[self.GetCorrID()] = []
				self._idKeys.append(self.GetCorrID())
			self._ids[self.GetCorrID()].append([self.GetCurrent(), self.GetPosition()])

	def GetCurrentIds(self):
		return self._ids[self._idKeys[self._curIdPos]]

	def GetIdByKey(self, id):
		return self._ids[id]

	def GetCurrentId(self):
		return self._idKeys[self._curIdPos]

	def JumpToCorrId(self, corrId):
		while 1:
			if self.GetCurrentId() >= corrId:
				return 
			if self.GetNextId() == None:
				break
		


	def GetNextId(self):
		self._curIdPos += 1
 		if self._curIdPos < len(self._idKeys):
 			return GetCurrentIds()
 		else:
 			self.ResetIDPos()
 			return None

	def CheckIDProcThread(self, curId, proc, thread):
		for x in curId:
			if x.GetProcID() == proc and x.GetThreadID() == thread:
				return True
		return False

	def CreateSubsetTiming(self, ids):
		l = []
		for x in ids:
			l += self.GetIdByKey(x)
		return TimingInfo(l)


	def GetIdsByProcess(self, procid, threadid):
		ret = [self.GetCurrentId()] if self.CheckIDProcThread(self.GetCurrentIds(), procid, threadid) == True else []
		while self.GetNextId() != None:
			if self.CheckIDProcThread(self.GetCurrentIds(), procid, threadid) == True:
				ret.append(self.GetCurrentId())

		return ret


	def ResetIDPos(self, pos = 0):
		self._curIdPos = pos

	def GetProcThreadIds(self):
		ret = {}
		pair = str(str(self.GetProcID()) if self.GetProcID() != None else "None") + "," + str(str(self.GetThreadID() if self.GetThreadID() != None else "None"))
		if "None" not in pair:
			ret[pair] = 1

		while self.GetNext() != None:
			pair = str(str(self.GetProcID()) if self.GetProcID() != None else "None") + "," + str(str(self.GetThreadID() if self.GetThreadID() != None else "None"))
			if "None" not in pair:
				ret[pair] = 1

		self.Reset()

		return ret


	def GetCurrentTransfer(self):
		return self.GetCurrent()

	def GetNextTransfer(self):
		return self.GetNext()


