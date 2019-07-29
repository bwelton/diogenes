class ProcessIterator:
	def __init__(self, procId, threadId):
		self._procId = procId
		self._threadId = threadId

	def AddTimelines(self, timeline, timingInfo, packetCorr):
		self._timeline =  timeline
		self._timingInfo =  timingInfo
		self._packetCorr = packetCorr

	def GetTimelines(self):
		return [self._timeline, self._timingInfo, self._packetCorr]	

class SplitByProcess:
	def __init__(self, timeline, timingInfo, packetCorr):
		self._timeline =  timeline
		self._timingInfo =  timingInfo
		self._packetCorr = packetCorr

	def GetProcesses(self):
		processes = self._timingInfo.GetProcThreadIds()
		print processes
		if len(processes.keys()) == 1:
			## Special case when there is one process/thread doing cuda work
			ret = ProcessIterator(processes.keys()[0].split(",")[0], processes.keys()[0].split(",")[1])
			ret.AddTimelines(self._timeline, self._timingInfo, self._packetCorr)
			return [ret]

		ret = []
		for x in processes.keys():
			procId, threadId = x.split(",")
			timingIds = self._timingInfo.CreateSubsetTiming(self._timingInfo.GetIdsByProcess(int(procId), int(threadId)))
			packetCorr = self._packetCorr.CreateSubsetPacket(self._packetCorr.GetIdsByProcess(int(procId), int(threadId)))
			timeline = self._timeline.CreateSubsetTimeing(self._timeline.GetIdsByCorrection(packetCorr))
			tmp = ProcessIterator(int(procId), int(threadId))
			tmp.AddTimelines(timeline, timingIds, packetCorr)
			ret.append(tmp)
		return ret

