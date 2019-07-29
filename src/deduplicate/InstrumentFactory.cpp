#include "InstrumentFactory.h"
#include "cuda.h"
InstrumentFactory::InstrumentFactory() {
	// Factories enabled by default
	std::map<std::string, bool> Factories = {{std::string("EnableDestination"), true},
											 {std::string("EnableTimeline"), true},
											 {std::string("EnableChecker"), true},
											 {std::string("EnableCUPTITiming"), true},
											 {std::string("EnableStackTracing"), false}};

	std::ifstream f("dedup_settings.conf");
	if (f.good() == true) {
		for (std::string line; std::getline(f, line); ){
			std::vector<std::string> tmp;
			char * tmp_str = strdup(line.c_str());
			char * pch = strtok(tmp_str,"=");
			while (pch != NULL) {
				tmp.push_back(std::string(pch));
				pch = strtok(NULL,"=");
			}
			free(tmp_str);
			if (tmp.size() <= 1)
				break;
			if (Factories.find(tmp[0]) != Factories.end()) {
				boost::to_upper(tmp[1]);
				if (tmp[1] == std::string("TRUE") || tmp[1] == std::string("1")){
					std::cerr << tmp[0] << " is being enabled" << std::endl;
					Factories[tmp[0]] = true;
				}
				if (tmp[1] == std::string("FALSE") || tmp[1] == std::string("0")){
					std::cerr << tmp[0] << " is being disabled" << std::endl;
					Factories[tmp[0]] = false;
				}
			}
		}
	}
	for(auto i : Factories) {
		std::cerr << i.first << " is " << (i.second ? "enabled" : "disabled") << std::endl;
	}

	_globalID = 1;
	if (Factories[std::string("EnableCUPTITiming")] == true) 
		_workers.push_back(std::shared_ptr<InstrumentBase>(new CUPTIEventHandler(true, fopen("timing_info.txt","w"))));	
	if (Factories[std::string("EnableDestination")] == true) 
		_workers.push_back(std::shared_ptr<InstrumentBase>(new DestinationHash(true, fopen("dedup_desthash.txt","w"))));
	if (Factories[std::string("EnableChecker")] == true)
		_workers.push_back(std::shared_ptr<InstrumentBase>(new DuplicateTracker(true, fopen("dedup_out.txt","w"))));
	if (Factories[std::string("EnableTimeline")] == true)
		_workers.push_back(std::shared_ptr<InstrumentBase>(new TransferTimeline(true, fopen("dedup_timeline.txt","w"))));
	if (Factories[std::string("EnableStackTracing")] == true) 
		_workers.push_back(std::shared_ptr<InstrumentBase>(new StackTraceGen(true, fopen("stack_tracing.txt","w"))));
}

int InstrumentFactory::PerformAction(TransferPtr t) {
	bool bail = false;
	{
		boost::recursive_mutex::scoped_lock lock(_mtx);
		t.get()->SetID(_globalID);
		_globalID++;
	}

	for (auto i : _workers)
		i.get()->PerformAction(t);
	t.get()->PerformTransfer();
	return PostTransfer(t);
}

int InstrumentFactory::PostTransfer(TransferPtr t){
	for (auto i : _workers)
		i.get()->PostTransfer(t);	
	return 0;
}

InstrumentFactory::~InstrumentFactory() {

}