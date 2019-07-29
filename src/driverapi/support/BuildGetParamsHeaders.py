import os
outTypedefs = ""
outCallers = "switch(values.size()) {\n"
for i in range(0,16):
	outstr = "template<"
	for x in range(0, i):
		outstr+= "typename T"+ str(x) + ","
	if outstr[-1] == ',':
		outstr = outstr[:-1]
	outstr += ">\nstd::tuple<"
	for x in range(0, i):
		outstr+= "T"+ str(x) + ","
	if outstr[-1] == ',':
		outstr = outstr[:-1]	
	outstr += "> GetParams(std::shared_ptr<Parameters> P)\n{\n\treturn std::make_tuple("
	for x in range(0, i):
		outstr+= "(T"+ str(x) + ")(p.get()->values[" +str(x) +"]),"
	if outstr[-1] == ',':
		outstr = outstr[:-1]	
	outstr += ");\n}\n"	
	print outstr

	#typedef int (*TPVoid)(void *, void *, void *);
	if i == 0:
		outTypedefs += "typedef int (*TPVoid"+str(i)+ ")(void);\n"
	else:
		outTypedefs += "typedef int (*TPVoid"+str(i)+")("
		for x in range(0,i):
			outTypedefs+= "void *,"
		outTypedefs = outTypedefs[:-1] + ");\n"
	outCallers += "case " +str(i) +": " + "return ((TPVoid"+str(i) +")_func)(" 
	for x in range(0, i):
		outCallers += "*values[" + str(x) + "]" + ","
	if i > 0:
		outCallers = outCallers[:-1]
	outCallers = outCallers + ");\n"
print outTypedefs
print outCallers