class CalculateTS:
	def __init__(self, corrIds):
		self._corrIds = corrIds

	def FindSynchronization(self, transfer, timeline):
		corrId = transfer.GetCorrId()
		self._timelines[1].JumpToCorrId(corrId+1)

		while(1):
			currentId = self._timelines[1].GetCurrentId()
			tmp = self._timelines[1].GetIdByKey(currentId)
			if (tmp == None):
				return None
			for x in tmp:
				if x[0].IsSynchronization() == True:
					return tmp
			if self._timelines[1].GetNextId() == None:
				break

		return None

	def CalculateSynchTransSavings(self, transfer):
		corrId = transfer.GetCorrId()
		GPUTime = transfer.GetGPUTime()
		CPUTime =  transfer.GetCPUTime()
		print "Synchronous transfer call "+ str(corrId) +  "," + str(GPUTime) + "," + str(CPUTime)
		return GPUTime
	def CalculateTimeSavings(self, timelines):
		ts = 0.0
		self._timelines = timelines
		self._timelines[1].ResetIDPos()
		for x in self._corrIds:
			if x.IsTransDuplicate() == False:
				continue
			print x.StrOneLine()


			# if x.IsSynch() == True:
			# 	ts += self.CalculateSynchTransSavings(x)
			# 	continue
			






