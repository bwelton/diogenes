#include "DriverAPITrace.h"
std::shared_ptr<DriverAPITracer> __DriverTracer;
int exited = 0;
CallInstance::CallInstance(){}
CallInstance::CallInstance(uint64_t callid, char * name) {
	_name = std::string(name);
	_callId = callid;
	maxTime = -1;
	minTime = 9999999.9999;
	totalTime = 0;
	_count = 0;
}

CallInstance::~CallInstance() { }

void CallInstance::InsertInstance(uint64_t stackID, double time) {
	if (time > maxTime)
		maxTime = time;
	if (time < minTime)
		minTime = time;
	totalTime += time;
	if (_stackIDs.find(stackID) == _stackIDs.end())
		_stackIDs.insert(stackID);
	_count += 1;
}

std::string CallInstance::Print() {
	std::stringstream ss;
	ss << "\t" << std::setfill(' ') << std::setw(30) << _name << "\t" << std::setfill(' ') << std::setw(6) << _count << "\t"
	   <<  std::setfill(' ') << std::setw(10) << totalTime << "\t" <<  std::setfill(' ') << std::setw(10) << maxTime << "\t"
	   << std::setfill(' ') << std::setw(10) << minTime << "\t";
	for (auto i : _stackIDs) {
		ss << i << ",";
	}
	return ss.str();
}

APITracker::APITracker() {}
APITracker::~APITracker() {}
void APITracker::InsertNewEntry(uint64_t callid, uint64_t stackID, char * name, double time) {
	if (_calls.find(callid) == _calls.end())
		_calls.insert(std::make_pair(callid, CallInstance(callid, name)));
	_calls[callid].InsertInstance(stackID, time);

}
std::string APITracker::Print() {
	std::stringstream ss;
	ss << "\t" << std::setfill(' ') << std::setw(30) << "Call Name" << "\t" << std::setfill(' ') << std::setw(6) << "Count" << "\t"
	   <<  std::setfill(' ') << std::setw(10) << "Total Time" << "\t" <<  std::setfill(' ') << std::setw(10) << "Max Time" << "\t"
	   << std::setfill(' ') << std::setw(10) << "Min Time" << "\t Stack IDS" << std::endl;
	for (auto i : _calls)
		ss << i.second.Print() << std::endl;
	return ss.str();
}

DriverAPITracer::DriverAPITracer() {
	std::cerr << "[DriverAPITracer] Tracer has been enabled\n";

	_outFile.reset(new OutputFile(std::string("CS_Summary.txt")));
	_keyFile.reset(new StackKeyWriter(fopen("CS_StackFile.txt","w")));
	_tracer.reset(new APITracker());
}
DriverAPITracer::~DriverAPITracer() {
	exited = 1;
	std::string tmp = _tracer->Print();
	fprintf(_outFile->outFile, "%s\n",tmp.c_str());
	fflush(_outFile->outFile);
}
PluginReturn DriverAPITracer::Precall(std::shared_ptr<Parameters> params){
	// if (params->GetID() == ID_cuMemAllocManaged)
	// 	usleep(100);
	_points[params] = std::chrono::high_resolution_clock::now();
	return NO_ACTION;
}

PluginReturn DriverAPITracer::Postcall(std::shared_ptr<Parameters> params) {
	auto stop = std::chrono::high_resolution_clock::now();
	if (_points.find(params) == _points.end()){
		std::cerr << "[DriverAPITracer] WARNING -  Could not find matching pair for call " << params->GetName() << std::endl;
		return NO_ACTION;
	}
	std::chrono::duration<double> diff = stop -  _points[params];
	std::vector<StackPoint> points;
	bool ret = GET_FP_STACKWALK(points);
	uint64_t id = 0;
	if (ret != false){
		id = _keyFile->InsertStack(points);
		_tracer->InsertNewEntry(params->GetID(), id, (char *) params->GetName(), diff.count());
	}
	_points.erase(params);
	return NO_ACTION;
}

extern "C"{

void init(std::vector<std::string> & cmd_list) {
	exited = 0;
	PLUG_BUILD_FACTORY
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