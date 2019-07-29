from ctypes import *

lib = cdll.LoadLibrary("libinst_lib.so")
openBinary = lib.OpenBinary
replaceFunction =  lib.ReplaceFunction
wrapFunction = lib.WrapFunction
performRewrite = lib.PerformRewrite
openBinary.restype = c_void_p
getmnames = lib.GetModuleNames
getmnames.restype = py_object
getModuleSymbols = lib.GetBinarySymbolsForModule
getModuleSymbols.restype = py_object
findSymbolPrefix = lib.FindAllSymbolsWithPrefix
findSymbolPrefix.restype = py_object
insertBeforeCall = lib.WrapAllFunctions
addSymAtOffset = lib.AddSymbolAtOffset
insertAtEntry = lib.InsertAtFunctionEntry
writeSymbols = lib.InsertSymbols

class Instrimenter(object):
    def __init__(self):
        self._storage = None

    def OpenBinary(self, appName):
        self._storage = openBinary(c_char_p(appName))
        if self._storage == None:
            print "Could not open the application binary: " + appName
            return -1
        return 0

    def ReplaceFunction(self, functionToReplace, replacementFunction, libraryWithReplacement):
        print functionToReplace
        print replacementFunction
        print libraryWithReplacement
        ret = replaceFunction(self._storage, c_char_p(functionToReplace),
                                c_char_p(replacementFunction), c_char_p(libraryWithReplacement))
        if (ret < 0):
            print "We could not setup the replacement for function " + functionToReplace
        return ret

    def InsertAtFunctionEntry(self, funName, funToCall, wrapLibrary):
        ret = insertAtEntry(self._storage, c_char_p(funName), c_char_p(funToCall), c_char_p(wrapLibrary))
        if (ret < 0):
            print "We could not insert a point at entry of " + str(funName)

    def WrapFunction(self, functionToWrap, wrapperFunction, libraryWithWrapper, wrapperHookName):
        ret = wrapFunction(self._storage, c_char_p(functionToWrap), c_char_p(wrapperFunction),
                            c_char_p(libraryWithWrapper), c_char_p(wrapperHookName))
        if (ret < 0):
            print "We could not setup the wrapper for function " + functionToWrap

    def AddSymbolAtOffset(self, offset, symbolname):
        ret = addSymAtOffset(self._storage, c_uint64(offset), c_char_p(symbolname))
        if (ret < 0):
            print "Could not add symbol at " + str(offset)

    def PerformRewrite(self, outputName):
        ret = performRewrite(self._storage, c_char_p(outputName))
        if (ret < 0):
            print "Binary rewrite has failed"

    def GetModules(self):
        ret = getmnames(self._storage)
        return ret

    def GetModuleSymbols(self, modname):
        ret = getModuleSymbols(self._storage, c_char_p(modname))
        return ret

    def WriteSymbolsToFile(self, outputName):
        ret = writeSymbols(self._storage, c_char_p(outputName))
        if (ret < 0):
            print "Symbol rewrite has failed"    
                
    def FindSymbolPrefix(self, prefix):
        ret = findSymbolPrefix(self._storage, c_char_p(prefix))
        return ret

    def InsertBeforeCall(self, functionToReplace, wrapperFunction, libraryWithWrapper):
        print functionToReplace
        print wrapperFunction
        print libraryWithWrapper
        ret = insertBeforeCall(self._storage, c_char_p(functionToReplace), c_char_p(wrapperFunction), c_char_p(libraryWithWrapper))
        if ret < 0:
            print "Insert has failed " + str(functionName)
            return -1
        return 0