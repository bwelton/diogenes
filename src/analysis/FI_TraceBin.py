import sys
import struct

class FI_TraceBin:
	def __init__(self, filename):
		self._filename = filename
		f = open(self._filename,"rb")
		self._data = f.read()
		f.close()

	def DecodeFile(self):
		## File Format
		# uint64_t pointID, uint64_t stackID
		self._entries = []
		self._entriesMap = {}
		length = int(int(len(self._data) / 8) / 3)
		for x in range(0, length):
			tmp = struct.unpack_from("QQd", self._data, offset=(x*8*3))
			self._entries.append(tmp)
			if tmp[1] not in self._entriesMap:
				self._entriesMap[tmp[1]] = []
			self._entriesMap[tmp[1]].append([tmp[0],tmp[2]])
		# for x in self._entries:
		# 	print x		
	
	def __str__(self):
		ret = "LS_Trace file  (FI Trace Key ID, FI Stack Trace ID)\n"
		for x in self._entries:
			ret += str(x[0]) + "," + str(x[1]) + "\n"
		return ret

