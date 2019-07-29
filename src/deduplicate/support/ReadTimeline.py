import os

class TimelineRecord:
	def __init__(self, id, hash, size, type, duplicate):
		self._id = int(id)
		self._hash = int(hash)
		self._size = int(size)
		self._type = type
		if int(duplicate) != 0:
			self._duplicate = True
		else:
			self._duplicate = False

	def GetId(self):
		return self._id

	def GetHash(self):
		return self._hash

	def GetSize(self):
		return self._size	

	def GetType(self):
		return self._type

	def IsDuplicate(self):
		return self._duplicate


class TimelineRecords:
	def __init__(self):
		self._recs = {}

	def AddRecord(self, line):
		self._recs[int(line.split(",")[0])] = TimelineRecord(*(line.split(",")))

	def GetSizeById(self, id):
		if int(id) not in  self._recs:
			return None
		return self._recs[id].GetSize()

	def GetTypeById(self, id):
		if int(id) not in self._recs:
			return None
		return self._recs[id].GetType()

	def IsDuplicateId(self, id):
		if int(id) not in self._recs:
			return None
		return self._recs[id].IsDuplicate()

class ReadTimeline:
	def __init__(self, file):
		f = open(file, "r")
		self._data = f.readlines()
		f.close()

	def Process(self):
		self._records = TimelineRecords()
		for x in self._data:
			if "\n" in x:
				x = x[:-1]
			self._records.AddRecord(x)
		return self._records
