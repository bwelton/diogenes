import os
import sys
from PyInstriment import *

def PrintDescFileFormat():
    print "Use the following format for the description file:" 
    print "<Type>,<OriginalFunc>,<Replacement/WrapperFuncName>,<LibraryWithReplacement/Wrapper>,<OrignalCallname>\\n"
    print "Type = Wrap,Replace,InsertBeforeAll (All specifies if prefix matching will be used)"
    print "OriginalFunc = Name of the function to wrap/replace"
    print "Replacement/WrapperFuncname = Name of the replacement/wrapper function."
    print "LibraryWithReplacement/Wrapper = The full path to the libary (.so file)" 
    print "                                containing the replacement/wrapper function"
    print "OrignalCallname = A name to a function call in the wrapper which should be replaced to point to the"
    print "                 original function."


if len(sys.argv) < 4:
    print "Usage: python Instriment_Binary.py <binary> <replacement_description_file> <output name>"
    exit(-1)

inst = Instrimenter()
print "Opening binary: " + sys.argv[1]
if inst.OpenBinary(sys.argv[1]) != 0:
    print "Could not open binary " + sys.argv[1]
    exit()
    
f = open(sys.argv[2], "rb")
if f == None: 
    print "Could not open replacement description file: " + sys.argv[2]
    exit()
data = f.readlines()
f.close()

##
# The format of the descriptor is the following:
#   <Type>,<OriginalFunc>,<Replacement/WrapperFuncName>,<LibraryWithReplacement/Wrapper>,<OrignalCallname>
#   Type = Wrap or Replace
#   OriginalFunc = Name of the function to wrap/replace
#   Replacement/WrapperFuncname = Name of the replacement/wrapper function.
#   LibraryWithReplacement/Wrapper = The full path to the libary (.so file) 
#                                    containing the replacement/wrapper function
#   OrignalCallname = A name to a function call in the wrapper which should be replaced to point to the
#                     original function. 
##
symbolAdded = False
for x in data:
    tmp = x.replace("\n", "")
    tmp = tmp.split(",")

    if tmp[0].lower() == "wrap":
        if len(tmp) != 5:
            print "Incorrect number of arguements to wrap a function, expect 5 arguments"
            PrintDescFileFormat()
            print x
            exit(-1)
        print "Wrapping function: " + tmp[1] + " with " + tmp[2]
        inst.WrapFunction(tmp[1],tmp[2],tmp[3],tmp[4])
    elif tmp[0].lower() == "replace":
        if len(tmp) != 4:
            print "Incorrect number of arguements to replace a function, expect 4 arguments"
            PrintDescFileFormat()
            print x
            exit(-1)
        print "Replacing function: " + tmp[1] + " with " + tmp[2]
        inst.ReplaceFunction(tmp[1],tmp[2],tmp[3])
    elif tmp[0].lower() == "insertbeforeall":
        if len(tmp) != 4:
            print "Incorrect number of arguements to insert a function call, expect 4 arguments"
            PrintDescFileFormat()
            print x
            exit(-1)
        print "Inserting " + tmp[2] + " before function call " + tmp[1]
        print tmp
        inst.InsertBeforeCall(tmp[1],tmp[2],tmp[3])
    elif tmp[0].lower() == "insertsymbol":
        if len(tmp) != 3:
            print "Incorrect format for insert symbol, require exactly 3 params"
            exit(-1)
        inst.AddSymbolAtOffset(int(tmp[1],0), tmp[2])
        symbolAdded = True
    elif tmp[0].lower() == "insertatentry":
        if len(tmp) != 4:
            print "Incorrect format for insert at entry, require exactly 4 params"
            exit(-1)
        inst.InsertAtFunctionEntry(tmp[1],tmp[2],tmp[3])



inst.PerformRewrite(sys.argv[3])






