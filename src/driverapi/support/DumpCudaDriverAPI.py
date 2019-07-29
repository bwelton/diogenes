import os
import CppHeaderParser
cppHeader = CppHeaderParser.CppHeader("cuda.h")
f = open("cudaFunctionProtos.txt", "w")
funcs = []
for x in cppHeader.functions:
	parameters = []
	name = x["name"]
	for y in x["parameters"]:
		parameters.append((y["type"], y["name"]))
	funcs.append([name, parameters, x["rtnType"]])

for x in funcs:
	params = ""
	for y in x[1]:
		params += y[0] +"$" + y[1] + ","
	params = params[:-1]
	f.write(x[2] + "," + x[0] + "," + params + "\n")

f.close()
