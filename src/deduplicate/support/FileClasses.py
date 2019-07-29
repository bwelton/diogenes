import os
from Iterators import PacketCorrectionIterator, TimelineIterator, TimingInfoIterator
from pprint import pprint
from collections import deque

class BaseRecord:
	def __init__(self, *args):
		self._publicAttrs = [x for x in self._parseParameters]
		self._checked = False
		if len(args) < len(self._parseParameters):
			print "Argument length differs from parameters!"
			print len(args)
			print len(self._parseParameters)
			print args
			exit(-1)
		for x in range(0, len(self._parseParameters)):
			setattr(self, self._parseParameters[x][0], self._parseParameters[x][1](args[x]))

	def SetChecked(self):
		self._checked = True

	def PrintName(self):
		print self.GetAttr("cname")

	def GetString(self):
		s = ""
		for x in self._publicAttrs:
			value = self.GetAttr(x[0])
			s += str(value) + ","
		return s[:-1]

	def SetAttr(self, name, value):
		self._publicAttrs.append([name, type(value)])
		setattr(self, name, value)

	def GetAttr(self, attr):
		return getattr(self, attr, None)

	def IsDuplicate(self):
		if self.GetAttr("dupid") != None:
			if self.GetAttr("dupid") != 0:
				return True
		return False

	def GetGPUTime(self):
		if self.GetAttr("type") != None:
			if self.GetAttr("type") == "CPY":
				return max(0, self.GetAttr("etime") - self.GetAttr("stime"))
		return 0.0

	def GetCPUTime(self):
		if self.GetAttr("type") != None:
			if self.GetAttr("type") == "RR" or self.GetAttr("type") == "DR":
				return max(0, self.GetAttr("etime") - self.GetAttr("stime"))
		return 0.0

	def IsSynchronization(self):
		name = self.GetAttr("cname")
		t = str(self.GetAttr("type"))
		if name == None or t == None:
			return False
		if "DR" != t and "RR" != t:
			return False
		if "Async" not in name:
			return True
		else:
			return False

	def __str__(self):
		pprint(vars(self))
		return ""
class TimingInfoCall(BaseRecord):
	_parseParameters =  [("type", str), ("cname", str),
						 ("corrid", int), ("stime", int), 
						 ("etime",int), ("procid",int), 
						 ("threadid",int)]
	def __init__(self, *args):
		BaseRecord.__init__(self,*args)
		self._parseParameters = []

class TimingInfoCPY(BaseRecord):
	_parseParameters =  [("type", str), ("cname", str),
						 ("corrid", int), ("stime", int), 
						 ("etime",int), ("size",int), 
						 ("runcorr",int), ("ctx", int), 
						 ("dev",int), ("stream", int)]
	def __init__(self, *args):
		BaseRecord.__init__(self,*args)
		self._parseParameters = []

class DedupTimeline(BaseRecord):
	_parseParameters =  [("timelineid", int), ("hash", int),
						 ("size", int), ("ttype", str), 
						 ("dupid",int)]
	def __init__(self, *args):
		BaseRecord.__init__(self,*args)
		self._parseParameters = []

class PacketCorrection(BaseRecord):
	_parseParameters = [("timelineid", int), ("size", int), ("stream", int),
						("procid", int), ("threadid", int)]
	def __init__(self, *args):
		BaseRecord.__init__(self,*args)	
		self._parseParameters = []


	def CheckCorrelation(self, elements):
		size = -1
		procid = -1
		threadid = -1
		stream = -1
		for x in elements:
			if x._checked == True:
				return False
			if x.GetAttr("size") != None:
				size = x.GetAttr("size")
			if x.GetAttr("procid") != None:
				procid = x.GetAttr("procid")
			if x.GetAttr("threadid") != None:
				threadid = x.GetAttr("threadid")
			if x.GetAttr("stream") != None:
				stream = x.GetAttr("stream")
		## TODO: Readadd stream and threadid checking when we figure out why they differ
		# between CUPTI and us
		if self.size == size and self.procid == procid:
			return True
		return False

				


class ReadDatFiles:
	def __init__(self, timeline, timingInfo, timingCorrection):
		self.ReadTimeline(timeline)
		self.ReadTimingInfo(timingInfo)
		self.ReadTimingCorrection(timingCorrection)

	def ReadTimeline(self, timeline):
		f = open(timeline,"r")
		data = f.readlines()
		f.close()
		self._timeline = []
		for x in data:
			tmp = x.replace('\n',"")
			tmp = tmp.split(",")
			self._timeline.append(DedupTimeline(*tmp))

	def ReadTimingInfo(self, timingInfo):
		f = open(timingInfo,"r")
		data = f.readlines()
		f.close()
		self._timingInfo = []
		for x in data:
			tmp = x.replace('\n',"")
			tmp = tmp.split(",")
			if "RR" in tmp[0] or "DR" in tmp[0]:
				self._timingInfo.append(TimingInfoCall(*tmp))
			elif "CPY" in tmp[0]:
				self._timingInfo.append(TimingInfoCPY(*tmp))
			elif "TET" in tmp[0]:
				self._totalTime = float(tmp[1])
			else:
				print "Could not id record"
				print x
				exit(-1)
		self._timingInfo = self._timingInfo[2:]
		

	def ReadTimingCorrection(self, timingCorrection):
		f = open(timingCorrection,"r")
		data = f.readlines()
		f.close()
		self._timingCorr = []
		for x in data:
			tmp = x.replace('\n',"")
			tmp = tmp.split(",")
			self._timingCorr.append(PacketCorrection(*tmp))		

	def GetLists(self):
		return [self._timeline, self._timingInfo, self._timingCorr]
	def GetIterators(self):
		return None
		# return [TimelineIterator(self._timeline), TimingInfoIterator(self._timingInfo), PacketCorrectionIterator(self._timingCorr)]


class CorrelateTimlineWithTiming:
	def __init__(self, lists):
		self._timeline = lists[0]
		self._timingInfo = lists[1]
		self._timingCorr = lists[2]

	def Correlate(self):
		tlineD = {}
		timingD = {}
		corr = {}

		for x in self._timeline:
			tlineD[x.GetAttr("timelineid")] = x

		for x in self._timingInfo:
			key = x.GetAttr("corrid")
			if key not in timingD:
				timingD[key] = []
			timingD[key].append(x)

		for x in self._timingCorr:
			corr[x.GetAttr("timelineid")] = x
		tmp = tlineD.keys()
		tmp.sort()
		timelineOrder = deque(tmp)
		tmp = timingD.keys()
		tmp.sort()
		timingOrder = deque(tmp)
		finalOrdering = timingOrder
		
		print "Starting correlation - "
		while len(timelineOrder) > 0:
			element = timelineOrder.popleft()
			corrEle = corr[element]
			found = -1
			for x in timingOrder:
				if corrEle.CheckCorrelation(timingD[x]):
					found = x
					break

			if found != -1:
				print found
				timingD[found][0].SetChecked()
				for x in timingD[found]:
					x.SetAttr("timelineid", element)
					x.SetAttr("dupid", tlineD[element].GetAttr("dupid"))
			else: 
				print "Could not correlate record - " + str(element)
		f = open("testing_correlation.txt", "w")
		for x in finalOrdering:
			for y in timingD[x]:
				f.write(y.GetString() +"\n")
		f.close()

	def CalculateTS(self):
		m = 0.0
		exact = 0.0
		for x in self._timingInfo:
			if x.IsDuplicate():
				m += x.GetCPUTime()

		timingD = {}
		for x in self._timingInfo:
			key = x.GetAttr("corrid")
			if key not in timingD:
				timingD[key] = []
			timingD[key].append(x)

		## Add support for multi-threads
		timingOrder = timingD.keys()
		timingOrder.sort()
		ctrans = 0.0
		gtrans = 0.0
		count = 0
		for x in timingOrder:
			if timingD[x][0].IsDuplicate() == False:
				count+=1
				synchronized = False
				for y in timingD[x]:
					y.PrintName()
					if y.IsSynchronization() and synchronized == False:
						print "Here"
						exact += ctrans + y.GetCPUTime() - max(0, y.GetCPUTime() - gtrans)
						ctrans = 0.0
						gtrans = 0.0
			else:
				syncAtEnd = False
				for y in timingD[x]:
					if y.IsSynchronization():
						syncAtEnd = True
						csyncTime = y.GetCPUTime()
						ctrans += y.GetCPUTime() 
						gtrans += y.GetGPUTime()
					else:
						ctrans += y.GetCPUTime() 
						gtrans += y.GetGPUTime()
				if syncAtEnd:
					exact += ctrans + csyncTime - max(0, csyncTime - gtrans)
					ctrans = 0.0
					gtrans = 0.0
		print count

		print "Maximum Time Savings (NanoSeconds): " + str(exact)
		print "Minimum Timing Savings (NanoSeconds): " + str(m)
		return (exact,m)




