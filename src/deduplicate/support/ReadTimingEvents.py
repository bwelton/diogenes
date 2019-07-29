import os

class TimingProcess:
	def __init__(self, ident):
		self._ident = ident
		self._events = []
		self._pos = 0
	def AddEvent(self, ident, stream, size):
		self._events.append([ident, size, stream])

	def GetNextEvent(self):
		if self._pos >= len(self._events):
			return None
		ret = self._events[self._pos]
		self._pos += 1
		return ret

	def CountTransfers(self):
		print self._events[-1]
		print len(self._events)
		return len(self._events)

	def ResetPosition(self):
		self._pos = 0

class ReadTimingPacket:
	def __init__(self, filename):
		f = open(filename, "r")
		if f == None:
			print "Could not open CUPTI events file"
			return
		self._data = f.readlines()
		f.close()

	def Process(self):
		timingProcs = {}
		for x in self._data:
			tmp = x.split(",")
			key = tmp[3] + "&" + tmp[4]
			if key not in timingProcs:
				timingProcs[key] = TimingProcess(key)
			timingProcs[key].AddEvent(int(tmp[0]), int(tmp[1]), int(tmp[2]))
		self._timingProcs = timingProcs
		return timingProcs

