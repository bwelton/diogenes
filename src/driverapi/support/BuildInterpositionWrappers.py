import os
import sys 
from string import Template

f = open("cudaFunctionProtos.txt", "r")
protos = f.readlines()
f.close()

f = open("InterpositionTemplate.txt","r")
funcTemplate = f.read()
f.close()

f = open("InterpositionHeader.txt", "r")
headerTemplate =  f.read()
f.close()


## Build Header for Wrappers
alreadyWritten = {}
header = '#pragma once\n#include <dlfcn.h>\n#include "Parameters.h"\n#include "DriverWrapperBase.h"\nextern "C"{\n#define CUDAAPI\n\n'
backFill = ""
wrapperTemp = '#include "'+ "InterpositionHeader.h" +'"\n#include <tuple>\n#include "DriverWrapperBase.h"\n#include "DriverWrapperFactory.h"\nstd::shared_ptr<DriverWrapperFactory> DriverFactory;\nextern "C" {'
typeDef = {}
count = 0 
for x in protos:
	variables = {"RETURN_TYPE" : None, "CALL_NAME" : None, "PARAMETERS_NAMES" : None,
				"PARAMETERS_FULL" : None, "PARAMETERS_NAMES_VOID_CAST" : None}

	tmp = x.split(",")
	variables["RETURN_TYPE"] = tmp[0];
	if "CUresult" in tmp[0]:
		typeDef["CUresult"] = "int"		
	else:
		continue
	variables["CALL_NAME"] = tmp[1]
	if tmp[1] in alreadyWritten:
		continue
	alreadyWritten[tmp[1]] = 1
	variables["PARAMETERS_FULL"] = ""
	variables["PARAMETERS_NAMES"] = ""
	variables["PARAMETER_TYPES"] = "int,"
	variables["PARAMETERS_NAMES_VOID_CAST"] = ""
	variables["PARAMETERS_TYPES"] = ""

	## Parameters
	tmp[-1] = tmp[-1][:-1]
	for y in tmp[2:]:
		pnames = y.split("$")
		variables["PARAMETER_TYPES"] += pnames[0] + ","
		variables["PARAMETERS_TYPES"] += pnames[0] + ","
		variables["PARAMETERS_FULL"] += pnames[0] + " " + pnames[1] + ", "
		variables["PARAMETERS_NAMES"] += pnames[1] + ","
		variables["PARAMETERS_NAMES_VOID_CAST"] += "(void *)&" + pnames[1] + ","
		if "CU" not in pnames[0]:
			continue
		if "CUresult" not in pnames[0]:
			nt = pnames[0].split(" ")
			for z in nt: 
				if "CU" in z:
					typeDef[z] = "void *"

	variables["PARAMETERS_TYPES"] = variables["PARAMETERS_TYPES"][:-1]
	variables["PARAMETERS_FULL"] = variables["PARAMETERS_FULL"][:-2]	
	variables["PARAMETERS_NAMES"] = variables["PARAMETERS_NAMES"][:-1]
	variables["PARAMETER_TYPES"] = variables["PARAMETER_TYPES"][:-1]
	variables["PARAMETERS_NAMES_VOID_CAST"] = variables["PARAMETERS_NAMES_VOID_CAST"][:-1]
	if len(variables["PARAMETERS_NAMES"]) == 0:
		variables["PARAMETERS_NAMES"] = ""
		variables["PARAMETER_TYPES"] = "int"
		variables["PARAMETERS_NAMES_VOID_CAST"] = ""
		variables["PARAMETERS_TYPES"] = "void"
	else:
		variables["PARAMETERS_NAMES"] = "," + variables["PARAMETERS_NAMES"]
	variables["CALL_ID"] = str(count)
	count+= 1
	wrapperTemp += Template(funcTemplate).substitute(variables)
	backFill += Template(headerTemplate).substitute(variables) + "\n"
header += "\n"
for x in typeDef.keys():
	header += "#define " + x + " " + typeDef[x] + "\n"
header += backFill
header += "\n};\n"
wrapperTemp += "\n};"


f = open(sys.argv[2], "w")
f.write(header)
f.close()

f = open(sys.argv[1],"w")
f.write(wrapperTemp)
f.close()
