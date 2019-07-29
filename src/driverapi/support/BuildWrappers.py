import os
import sys 
from string import Template

f = open("cudaFunctionProtos.txt", "r")
protos = f.readlines()
f.close()

f = open("WrapperTemplate.txt","r")
funcTemplate = f.read()
f.close()

f = open("HeaderTemplate.txt", "r")
headerTemplate =  f.read()
f.close()

f = open("DefinitionHeader.txt","r")
defTemplate = f.read()
f.close()

f = open("ExternTemplate.txt","r")
externTemplate = f.read()
f.close()

if (len(sys.argv) < 2):
	print "Please supply an output directory"
	sys.exit(0)

outputDirectory = os.path.abspath(sys.argv[1])
print "Output will be forwarded to directory - " + str(outputDirectory)

count = 0
## Build Header for Wrappers
alreadyWritten = {}
charVector = "static const std::vector<const char *> CallVector = {"
outStr = '#include "'+ "DriverAPIHeader.h" +'"\n#include <tuple>\n#include "DriverWrapperBase.h"\n#include "DriverWrapperFactory.h"\n#include "cuda.h"\nstd::shared_ptr<DriverWrapperFactory> DriverFactory;\nextern "C" {'
outStr+= """\n// MANUALLY ADDED.....
extern int ORIGINAL_InternalSynchronization( void * a, void * b, void * c);

int INTER_InternalSynchronization( void * a, void * b, void * c) {
	BUILD_FACTORY
	std::vector<void **> params = { (void **)&a,(void **)&b, (void**)&c };	
	std::shared_ptr<Parameters> paramsPtr(new Parameters(ID_InternalSynchronization, (void*) &ORIGINAL_InternalSynchronization, params));
	int ret = ( int ) FACTORY_PTR->PerformAction(paramsPtr);
	return ret;
}

void JustGenStackTrace() {
	BUILD_FACTORY
	FACTORY_PTR->PrintStack();
	return;
}


void CheckInit_DriverAPI() {
	
}\n"""

gottcha_struct = ""
for x in protos:
	variables = {"RETURN_TYPE" : None, "CALL_NAME" : None, "PARAMETERS_NAMES" : None,
				"PARAMETERS_FULL" : None}
	tmp = x.split(",")
	if "CUresult" in tmp[0]:
		variables["RETURN_TYPE"] = "int"
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
	variables["FROM_TEMP_PARAMS"] = "";
	tmp[-1] = tmp[-1][:-1]
	mCount = 0
	for y in tmp[2:]:
		pnames = y.split("$")
		variables["PARAMETER_TYPES"] += pnames[0] + ","
		variables["PARAMETERS_FULL"] += pnames[0] + " " + pnames[1] + ", "
		variables["PARAMETERS_NAMES"] += pnames[1] + ","
		variables["PARAMETERS_NAMES_VOID_CAST"] += "(void *)" + pnames[1] + ","
		variables["FROM_TEMP_PARAMS"] += "&tmpParams[" + str(mCount) + "],"
		mCount += 1
	variables["PARAMETERS_FULL"] = variables["PARAMETERS_FULL"][:-2]	
	variables["PARAMETERS_NAMES"] = variables["PARAMETERS_NAMES"][:-1]
	variables["PARAMETER_TYPES"] = variables["PARAMETER_TYPES"][:-1]
	variables["FROM_TEMP_PARAMS"] = variables["FROM_TEMP_PARAMS"][:-1]
	variables["PARAMETERS_NAMES_VOID_CAST"] = variables["PARAMETERS_NAMES_VOID_CAST"][:-1]
	if len(variables["PARAMETERS_NAMES"]) == 0:
		variables["PARAMETERS_NAMES"] = ""
		variables["PARAMETER_TYPES"] = "int"
		variables["PARAMETERS_NAMES_VOID_CAST"] = ""
	else:
		variables["PARAMETERS_NAMES"] = "," + variables["PARAMETERS_NAMES"]
	variables["CALL_ID"] = str(count)
	count+= 1
	charVector += '"' + tmp[1] + '",'
	gottcha_struct += '\n\t{"' + variables["CALL_NAME"] + '",INTER_' +  variables["CALL_NAME"]  +',&GOTTCHA_' + variables["CALL_NAME"]+'},'
	outStr += Template(funcTemplate).substitute(variables)

outStr += "\n}"
charVector = charVector[:-1] + "};\n"
outStr += charVector + "\n"
outStr += "#define NUM_GOTFUNCS " + str(count) + "\nstruct gotcha_binding_t gotfuncs[] = {"
outStr += gottcha_struct[:-1] + "\n};\n"


## Create The Header File
hdr = '#pragma once\n#include "PluginCommon.h"\n#include <vector>\n#include "Parameters.h"\n#include "cuda.h"\n#include "DriverWrapperBase.h"\n#include <vector>\nextern "C"{\n'
charVector = ""
alreadyWritten = {}
defFile = ""
count = 0
outDefines = ""
structFile = '#pragma once\n#include <iostream>\n#include <functional>\n#include <memory>\n#include <map>\n#include <vector>\n#include "Parameters.h"\n#include "DriverWrapperBase.h"\n'
structFile += 'enum PluginReturn {\n\tNO_ACTION = 0,\n\tNOTIFY_ON_CHANGE,	// Notify when the parameter has changed\n\tDO_NOT_PERFORM,		// Instruct driver to not perform the action\n\tPERFORMED_ACTION,	// This plugin performed the action, do not perform again\n\tMODIFIED_PARAMS,	// This plugin modified the parameters\n\tFAILED			// This plugin has failed\n};\n\n'
structFile += "enum CallID {\n\t"
externTemplates = "\n#ifndef DEFINED_TEMPLATES\n#define EXTERN_FLAG extern\n#else\n#define EXTERN_FLAG \n#endif\n"
defineBinders = 'extern "C" void DefineBinders() {\n'
defineBinders += '\tgotcha_set_priority("cuda/driverapibinders", 1);\n'
defineBinders += """\tresult = gotcha_wrap(gotfuncs, NUM_GOTFUNCS, "cuda/driverapibinders");
	assert(result == GOTCHA_SUCCESS);\n"""

##defineBinders += '\t void * handle = (void *)dlopen("libcuda.so.1", RTLD_LAZY);\n\tassert(handle != NULL);\n'
for x in protos:
	variables = {"RETURN_TYPE" : None, "CALL_NAME" : None, "PARAMETERS_NAMES" : None,
				"PARAMETERS_FULL" : None}
	tmp = x.split(",")
	if "CUresult" in tmp[0]:
		variables["RETURN_TYPE"] = "int"
	else:
		continue
	variables["CALL_NAME"] = tmp[1]
	if tmp[1] in alreadyWritten:
		continue

	alreadyWritten[tmp[1]] = 1
	variables["PARAMETERS_FULL"] = ""
	variables["PARAMETERS_NAMES"] = ""
	variables["PARAMETER_TYPES"] = "int,"
	variables["PARAMETER_TYPES_ORIG"] = ""
	variables["BINDER_PLACEHOLDERS"] = ""
	variables["PARAMETER_TYPES_ORIG_DEFINE"] = ""
	tmp[-1] = tmp[-1][:-1]	
	tr = 1
	for y in tmp[2:]:
		pnames = y.split("$")
		variables["PARAMETER_TYPES"] += pnames[0] + ","
		variables["PARAMETER_TYPES_ORIG"] += pnames[0] + ","
		variables["PARAMETER_TYPES_ORIG_DEFINE"] += pnames[0] + "* ,"
		variables["PARAMETERS_FULL"] += pnames[0] + " " + pnames[1] + ", "
		variables["PARAMETERS_NAMES"] += pnames[1] + ","
		variables["BINDER_PLACEHOLDERS"] += "std::placeholders::" + "_" + str(tr) +","
		tr += 1
	variables["BINDER_PLACEHOLDERS"] = variables["BINDER_PLACEHOLDERS"][:-1]
	variables["PARAMETERS_FULL"] = variables["PARAMETERS_FULL"][:-2]	
	variables["PARAMETERS_NAMES"] = variables["PARAMETERS_NAMES"][:-1]	
	variables["PARAMETER_TYPES"] = variables["PARAMETER_TYPES"][:-1]
	variables["PARAMETER_TYPES_ORIG"] = variables["PARAMETER_TYPES_ORIG"][:-1]
	variables["PARAMETER_TYPES_ORIG_DEFINE"] = variables["PARAMETER_TYPES_ORIG_DEFINE"][:-1]
	if len(variables["PARAMETERS_NAMES"]) == 0:
		variables["PARAMETERS_NAMES"] = ""
	else:
		variables["PARAMETERS_NAMES"] = "," + variables["PARAMETERS_NAMES"]
	outDefines += "#define " + "PT_" + str(tmp[1]) + " " +variables["PARAMETER_TYPES_ORIG_DEFINE"] + "\n" 
	variables["CALL_ID"] = str(count)
	if count == 0:
		structFile += "ID_" + tmp[1] + " = 0,\n\t"
	else:
		structFile += "ID_" + tmp[1] + ",\n\t"
	count += 1
	
	hdr += Template(headerTemplate).substitute(variables)
	defFile += Template(defTemplate).substitute(variables) + "\n"
	externTemplates += Template(externTemplate).substitute(variables) + "\n"
	defineBinders += "\tint (*TMP_PTR_"+variables["CALL_NAME"] +")(" + 	variables["PARAMETER_TYPES_ORIG"] +") = (int(*)(" + variables["PARAMETER_TYPES_ORIG"] + ")) gotcha_get_wrappee(GOTTCHA_" + variables["CALL_NAME"] +");\n"
	defineBinders += "\tPTR_ORIGINAL_"+variables["CALL_NAME"] + " = (void *) gotcha_get_wrappee(GOTTCHA_" + variables["CALL_NAME"] +");\n"
	defineBinders += "\tBound_" + variables["CALL_NAME"] + " = std::bind(TMP_PTR_" +variables["CALL_NAME"] + "," + variables["BINDER_PLACEHOLDERS"] + ");\n"
structFile = structFile[:-3] + "\n};\n" + externTemplates
hdr += "\n}\n" + charVector
outStr += "#define DEFINED_TEMPLATES 1" + externTemplates + "\n" + defineBinders + "\n}\n"

f = open(os.path.join(outputDirectory, "PluginCommon.h"),"w")
f.write(structFile)
f.close()

f = open(os.path.join(outputDirectory, "DriverAPIWrapper.def.in"), "w")
f.write(defFile)
f.close()

f = open(os.path.join(outputDirectory, "DriverAPIHeader.h"), "w")
f.write(hdr)
f.close()

f = open(os.path.join(outputDirectory, "DriverAPIWrappers.cpp"),"w")
f.write(outStr)
f.close()

print outDefines