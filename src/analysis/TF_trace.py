import sys
import struct

class TF_Trace:
	def __init__(self, filename):
		self._filename = filename
		f = open(self._filename,"rb")
		self._data = f.read()
		f.close()

		self._pos = 0

	def DecodeSyncRecord(self):
		tmp = struct.unpack_from("QQQd", self._data, offset=self._pos)
		self._pos += (8*4)
		return list(tmp)

	def DecodeTransferRecord(self):
		tmp = struct.unpack_from("QQd", self._data, offset=self._pos)
		self._pos += (8*3)
		return list(tmp)

	def ReadType(self):
		tmp = struct.unpack_from("i", self._data, offset=self._pos)
		self._pos += (4)
		return list(tmp)

	def GetNotInSet(self, ids):
		ret = []
		for x in self._records:
			if int(x[2]) not in ids:
				ret.append(x)
		return ret

	def DecodeFile(self):
		## Binary Layout
		## Type = 1 (Sync Record), 2 (Transfer Record)
		## If SyncRecord: dynId, stackId, count, time
		## if transfer record: dynId, stackId, time
		# all except type (int) and time (double) are uint64_t
		# Dynid is not needed here really (used to correct stack)
		self._records = []
		while (self._pos < len(self._data)):
			t = self.ReadType()
			if t[0] == 1:
				self._records.append([t[0]] + self.DecodeSyncRecord())
			elif t[0] == 2:
				self._records.append([t[0]] + self.DecodeTransferRecord())
		# for x in self._records:
		# 	print x

	def __str__(self):
		ret = "TF_Trace file decoded: \n"
		for x in self._records:
			for y in x:
				ret += str(y) + ","
			ret = ret[:-1] + "\n"
		return ret