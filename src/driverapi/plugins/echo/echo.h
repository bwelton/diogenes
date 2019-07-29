#pragma ONCE
#include "Parameters.h"
#include "PluginCommon.h"

extern "C" {
	void init(std::vector<std::string> & cmd_list);
	PluginReturn Precall(std::shared_ptr<Parameters> params);
	PluginReturn Postcall(std::shared_ptr<Parameters> params);
}

class Echo {
public:
	Echo(std::vector<std::string> & cmd_list);
	~Echo();
	PluginReturn Precall(std::shared_ptr<Parameters> params);
	PluginReturn Postcall(std::shared_ptr<Parameters> params);
private:
	std::map<int,std::string> _cmdToName;
	size_t callcount;
};

#define PLUG_BUILD_FACTORY(param) \
	if (Worker.get() == NULL) { \
		Worker.reset(new Echo(param)); \
	} 

#define PLUG_FACTORY_PTR Worker.get()
