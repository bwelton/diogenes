import os
import sys
import struct
from ReadStackFiles import StackReader, StackEntry

class BinaryFileStorage:
    def __init__(self):
        self._fileIdToName = {}

    def AddName(self, ident, filename):
        self._fileIdToName[ident] = filename

    def GetFilename(self, ident):
        return self._fileIdToName[ident]

class InstPoint:
    def __init__(self, ident, fileid, offset):
        self._ident = ident
        self._fileid = fileid
        self._offset = offset

    def GetId(self):
        return self._ident

    def GetFileId(self):
        return self._fileid

    def GetOffset(self):
        return self._offset

class SearchForBin:
    def __init__(self, binStore, instPoints):
        self._binStore = binStore
        self._instPoints = instPoints

    def GetStackPoint(self, ident):
        filename = self._binStore.GetFilename(self._instPoints[ident].GetFileId())
        return [filename, self._instPoints[ident].GetOffset()]

class ReadBinLoc:
    def ParseHeader(self, line):
        tmp = line.split(",")
        self._fileIdToName.AddName(int(tmp[0]), tmp[1])

    def ParseInstPoint(self, line):
        tmp = line.split(",")
        return InstPoint(int(tmp[0]), int(tmp[1]), int(tmp[2]))

    def __init__(self):
        self._fileIdToName = BinaryFileStorage()
        self._instPoints = {}
        lines = [line.rstrip('\n') for line in open("LS_BinLoc.txt","rb")]
        endOfHeader = False
        for x in lines:
            if endOfHeader == False:
                if "EndOfHeader" in x:
                    endOfHeader = True
                else:
                    self.ParseHeader(x)
            else:
                ret = self.ParseInstPoint(x)
                self._instPoints[ret.GetId()] = ret

    def GetSearchClass(self):
        return SearchForBin(self._fileIdToName, self._instPoints)


class BinaryStack:
    def __init__(self, stackID):
        self._stackID = stackID
        self._stack =[]

    def AddEntry(self, entry):
        self._stack.append(entry)

    def ReturnStack(self, searchClassInst):
        ret = []
        for z in self._stack:
            tmp = searchClassInst.GetStackPoint(z)
            ret.append(StackEntry(tmp[0],int(tmp[1])))
        return ret

class ReadLSTraceBinary:
    def __init__(self):
        self._stacks = []
        f = open("LS_stackkey.bin","rb")
        self._data = f.read()
        f.close()
        self._pos = 0
        while self._pos + 8 < len(self._data):
            tmp = list(struct.unpack_from("QQ", self._data, offset=self._pos))
            bstack = BinaryStack(int(tmp[1]))
            tmp[0] = tmp[0] - 8
            self._pos += 16
            print self._pos
            print len(self._data)
            print tmp[0]
            while tmp[0] > 0:
                entry = list(struct.unpack_from("Q", self._data, offset=self._pos))
                tmp[0] = tmp[0] - 8
                self._pos += 8
                bstack.AddEntry(int(entry[0]))
            self._stacks.append(bstack)

    def GetStacks(self):
        return self._stacks

trace = ReadLSTraceBinary()
binLoc = ReadBinLoc()

test = StackReader("LS_stackkey.txt")
decodedStacks = test.GetAllStacks()
print "What was returned by Diogenes C++: "
for x in decodedStacks:
    print "Stack Start:"
    for y in decodedStacks[x].GetStack():
        print str(y)

print "Direct Decoding: "
seachC = binLoc.GetSearchClass()
for x in trace.GetStacks():
    print "Start Stack:"
    z = x.ReturnStack(seachC)
    for y in z:
        print str(y)


