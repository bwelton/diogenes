import os
import subprocess
import os.path
class StackEntry:
    def __init__(self, libname = "", offset = 0):
        self._libname = libname
        self._offset = offset
        self._funcname = ""
        self._lineNum = ""
        self._fileName = ""

        ## remove the version number from the .so
        if self._libname.find(".so") != -1:
            if os.path.isfile(self._libname[0:self._libname.find(".so")+3]) and "libc.so.6" not in self._libname:
                self._libname = self._libname[0:self._libname.find(".so")+3]

    def __eq__(self, other):
        #st = "Comparing " + str(self) + " to " + str(other) + " - "
        if self._libname.split("/")[-1] == other._libname.split("/")[-1]:
            if int(self._offset) == int(other._offset):
                #st += " MATCH"
                #print st
                return True
        if (str(self) == str(other)):
            #st += " MATCH"
            #print st
            return True
        #st += " NOT EQUAL"
        #print st
        return False

    def __str__(self):
        return str(self._libname) + "@" + str(self._offset) 

    def PrintNormalHuman(self):
        if self._fileName != "" and "/" in self._fileName:
            self._fileName = self._fileName.split("/")[-1] 
        tmpWriteLine = self._lineNum
        if tmpWriteLine != "":
            tmpWriteLine = tmpWriteLine.replace("\n","")
            tmpWriteLine = str(int(tmpWriteLine,16))
        return (str(self._funcname) + " IN (" + str(self._fileName) + ":" + str(tmpWriteLine) + ")" + str(self._libname) + "@" + str(self._offset)).replace("\n","")
    def GetFilename(self):
        return self._libname.split("/")[-1]

class Stack:
    def __init__(self, i):
        self._stack = []
        self._ident = i
        self._gotName = False
        self._cuda_funcs = {}

    def GetID(self):
        return self._ident
    
    def TrimStacks(self):
        self.GetNameInfo()
        for x in range(0,len(self._stack)):
            if "INTER_" in self._stack[x]._funcname:
                self._stack = self._stack[0:x]
                break


    def TransGetFirstLibCuda(self):
        for x in self._stack:
            if "INTER_" in x._funcname:
                tmp = x._funcname

                return tmp.replace("INTER_", "")

    def TransGetFirstUserCall(self):
        count = 0
        for x in self._stack:
            if "cuda-9.2.148/nvidia" not in x._libname:
                count += 1
            else:
                break
        if count >= len(self._stack):
            return ""

        return self._stack[count]._funcname

    def HashStackDataTransfer(self):
        tmp = ""
        for i in self._stack:
            # break on the first invocation of Diogenes
            if "cudadedup-develop" in str(i):
                break
            tmp += str(i) + "\n"

        return int(hash(tmp))


    def AddEntry(self, libname, offset):
        if "libTimeCall" in libname:
            return
        self._stack.append(StackEntry(libname, offset))

    def GetStack(self):
        return self._stack

    def InNvidiaFPLibs(self):
        for x in self._stack:
            if "libcublas" in x._libname or "libcusparse" in x._libname:
                return True
        return False

    def FindFirstUserCall(self):
        startPos = 0
        self.GetNameInfo()
        for x in range(0,len(self._stack)):
            if "libcuda.so" in  self._stack[x]._libname or "libcudart.so" in  self._stack[x]._libname or "cuda-9.2.148/nvidia" in self._stack[x]._libname:
                startPos = x
                break
            else:
                if self._stack[x]._funcname in self._cuda_funcs:
                    startPos = x
                    break

        return self._stack[startPos-1]

    def FindFirstLibCuda(self):
        self.GetNameInfo()
        for x in self._stack:
            if "libcuda.so" in  x._libname:
                return x
            else:
                if x._funcname in self._cuda_funcs:
                    return x
        return StackEntry(libname="????", offset=0)

    def __str__(self):
        self.GetNameInfo()
        ret = "\tStack " + str(self._ident)  + "\n"
        for i in self._stack:
            ret += "\t\t" + str(i.PrintNormalHuman()) + "\n"
        return ret

    def InverseStack(self):
        tmp = []
        for x in range(len(self._stack) - 1, 0, -1):
            tmp.append(self._stack[x])
        self._stack = tmp

    def GetNameInfo(self):
        if self._gotName == True:
            return 
        f = open("tmp_encoded_stack.txt","w")
        count = 1
        for x in self._stack:
            f.write(str(count) + "$" + str(x) +"\n")
            count += 1
        f.close()
        try:
            os.remove("tmp_decoded_stack.txt")
        except:
            pass
        ## Run Diogenes to get stack info
        subprocess.call("/g/g17/welton2/repo/spack/opt/spack/linux-rhel7-ppc64le/gcc-4.9.3/cudadedup-develop-sfolqw2eykf4ubdm3umxxvnky2ul6k7r/bin/LaunchCUInstriment -d -i tmp_encoded_stack.txt -o tmp_decoded_stack.txt &> debug_symtab.txt", shell=True)
        f = open("tmp_decoded_stack.txt", "r")
        count = 0
        for x in f.readlines():
            tmp = x.split("$")
            count = tmp[0]
            tmp[1] = "".join(tmp[1:])
            if int(count,16) - 1 > len(self._stack):
                print "ERROR Could not find stack - " + x
            else:
                index = int(count,16) - 1
                a = tmp[1].split("@")
                if "<Unknown at Offset" in a[1]:
                    self._stack[index]._funcname = "Unknown"
                    continue
                if self._stack[index]._libname == a[0] and int(self._stack[index]._offset) == int(a[2],16):
                    lines = os.popen("/usr/bin/c++filt " + a[1]).read().splitlines()
                    if len(lines) == 0:
                        self._stack[index]._funcname = "Unknown"
                    else:
                        self._stack[index]._funcname = lines[0]
                    lineMapSplit = a[3].split("|")
                    if len(lineMapSplit) == 2:
                        if lineMapSplit[1] != "":
                            self._stack[index]._lineNum = lineMapSplit[1]
                        if lineMapSplit[0] != "":
                            self._stack[index]._fileName = lineMapSplit[0]

                    #print a[1]
                else:
                    print "NON MATCH"
                    print str(self._stack[index])
                    print x
                    if self._stack[index]._libname != a[0]:
                        print self._stack[index]._libname  + " does not match " + a[0]
        f.close()
        cudaFunctions = os.path.join(os.path.dirname(os.path.realpath(__file__)),"cudaFunctions.txt")
        f = open(cudaFunctions, "r")
        data = f.readlines()
        f.close()
        for x in data:
            #print x
            self._cuda_funcs[x[:-1]] = 1


        self._gotName = True


def RemoveNewline(line):
    return line.replace("\n","")

class StackReader:
    def __init__(self, filename):
        self._filename = filename
        try:
            f = open(filename, "rb")
            self._data = f.readlines()
            f.close()
        except:
            self._data = []

        self.__GetStacks()

    def __GetStacks(self):
        self._stacks = {}
        for x in self._data:
            tmp = RemoveNewline(x).split("$")
            self._stacks[int(tmp[0])] = Stack(int(tmp[0]))
            for y in tmp[1:]:
                ms = y.split("@")
                self._stacks[int(tmp[0])].AddEntry(ms[0], int(ms[1]))

    def __str__(self):
        ret = "Stack records read for file - " + str(self._filename) + "\n"
        for x in self._stacks:
            ret += str(self._stacks[x]) + "\n"
        return ret
    def GetStackAtID(self, i):
        if i not in self._stacks:
            return None
        return self._stacks[i]

    def GetAllStacks(self):
        return self._stacks

    def InverseStacks(self):
        for x in self._stacks:
            self._stacks[x].InverseStack()