#include "echo.h"
#include "HelperFunctions.h"
std::shared_ptr<Echo> Worker;
int exited = 0;
Echo::Echo(std::vector<std::string> & cmd_list) {
	int count = 0;
	for (auto i : cmd_list) {
		_cmdToName[count] = i;
		count++;
	}
	callcount = 0;
}

Echo::~Echo() {
	exited = 1;
	std::cout << "[ECHO-END] Call Count: " << callcount << std::endl;
}

PluginReturn Echo::Precall(std::shared_ptr<Parameters> params) {
	std::cout << "[PRECALL] Call: " << params.get()->GetName() << " Param Count:" << params.get()->GetLen() << std::endl;
	if (params.get()->GetID() == ID_cuMemcpyHtoD_v2) {
		std::tuple<PT_cuMemcpyHtoD_v2> ret =  GetParams<PT_cuMemcpyHtoD_v2>(params);
		std::cout << "Copy Size: " << std::get<2>(ret)[0] << std::endl;
	}
	return NO_ACTION;
}

PluginReturn Echo::Postcall(std::shared_ptr<Parameters> params) {
	std::cout << "[POSTCALL] Call: " << _cmdToName[params.get()->GetID()] << " Param Count:" << params.get()->GetLen() << " Performed: " << params.get()->Called() << std::endl;
	if (params.get()->Called())
		callcount++;
	return NO_ACTION;
}

extern "C"{

void init(std::vector<std::string> & cmd_list) {
	PLUG_BUILD_FACTORY(cmd_list)
}

PluginReturn Precall(std::shared_ptr<Parameters> params){
	if (exited == 1)
		return NO_ACTION;
	return PLUG_FACTORY_PTR->Precall(params);
}

PluginReturn Postcall(std::shared_ptr<Parameters> params) {
	if (exited == 1)
		return NO_ACTION;
	return PLUG_FACTORY_PTR->Postcall(params);
}

}