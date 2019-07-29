#include "TimeCall.h"
std::shared_ptr<TimeCall> Worker;
thread_local int TIMECALL_exited;
TimeCall::TimeCall(std::vector<std::string> & cmd_list) {
	TIMECALL_exited = 0;
	std::ifstream infile("FuncsToTime.txt");
	std::string line;
	while (std::getline(infile, line)) {
		for (int i = 0; i < cmd_list.size(); i++) {
			if (cmd_list[i] == line){
				_callsToMonitor.insert((CallID)i);
				std::cerr << "[TimeCall] Timing - " << cmd_list[i] << std::endl;
				break;
			}
		}
	}
}

TimeCall::~TimeCall() {
	TIMECALL_exited = 1;
	if(_timers.size() > 0) {
		FILE * out = fopen("call_timers.txt","w");
		for(auto i : _timers)
			fprintf(out, "%s,%llu,%f\n",std::get<0>(i), std::get<1>(i), std::get<2>(i));
		fclose(out);
		_timers.clear();
	}
}

PluginReturn TimeCall::Precall(std::shared_ptr<Parameters> params) {
	if (_callsToMonitor.find(params.get()->GetID()) != _callsToMonitor.end()) {
		auto start = std::chrono::high_resolution_clock::now();
		params.get()->Call();
		auto stop = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = stop-start;
		_timers.push_back(std::make_tuple(params.get()->GetName(), params.get()->GetInstID(), diff.count()));
		return PERFORMED_ACTION;
	}
	return NO_ACTION;
}

PluginReturn TimeCall::Postcall(std::shared_ptr<Parameters> params) {
	return NO_ACTION;
}
struct StoreDelta {
	std::chrono::high_resolution_clock::time_point _start;
	uint64_t _prevId;
	FILE * __outFile;
	StoreDelta() {
		_prevId = 0;
		_start = std::chrono::high_resolution_clock::time_point();
		__outFile = fopen("TF_delta.bin","w");
	};
	~StoreDelta() {
		fclose(__outFile);
	};
	void Write(uint64_t id, double time) {
		fwrite(&id, 1, sizeof(uint64_t), __outFile);
		fwrite(&time, 1, sizeof(double), __outFile);
	};
	void AddEndingTime(uint64_t id, std::chrono::high_resolution_clock::time_point & timePoint) {
		_prevId = id;
		_start = timePoint;
	};

	void NextSyncStart(std::chrono::high_resolution_clock::time_point & timePoint) {
		if (_prevId > 0) {
			std::chrono::duration<double> diff = timePoint - _start;
			Write(_prevId, diff.count());
		}
	};
};

struct TrackSyncTime {
	FILE * _outFile;
	std::chrono::high_resolution_clock::time_point _start;
	double _data[2];
	TrackSyncTime() {
		assert(sizeof(uint64_t) == sizeof(double));
		_outFile = fopen("TF_synctime.bin","wb");
	};

	~TrackSyncTime() {
		fclose(_outFile);
	};

	inline void Start() {
		_start = std::chrono::high_resolution_clock::now();
	};

	inline void End() {
		std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - _start;
		((double *)_data)[1] = diff.count();
	};

	inline void Write(uint64_t id) {
		((uint64_t*)_data)[0] = id;
		fwrite(&_data, sizeof(double), 2, _outFile);
		_data[0] = 0.0;
		_data[1] = 0.0;
	};


};


thread_local std::shared_ptr<TFReaderWriter> TIMECALL_outFile;
thread_local std::shared_ptr<StackKeyWriter> TIMECALL_keyFile;
thread_local std::shared_ptr<TrackSyncTime>  TIMECALL_trackSyncTime;

thread_local std::shared_ptr<StoreDelta> TIMECALL_deltafile;
thread_local TF_Record TIMECALL_tfRecord;
thread_local std::vector<std::pair<uint64_t, std::chrono::high_resolution_clock::time_point> > TIMECALL_TimingPairs; 
thread_local std::vector<uint64_t> TIMECALL_TimingCount; 
thread_local int alreadyStarted = 0;



//std::shared_ptr<LogInfo> _timingLog;

extern "C"{

void INIT_TIMERS() {
	if (TIMECALL_outFile.get() == NULL) {
		TIMECALL_tfRecord.type = TF_SYNCRECORD;
		alreadyStarted = 0;
		std::cerr << "Starting timing log" << std::endl;
		TIMECALL_outFile.reset(new TFReaderWriter());
		TIMECALL_outFile->Open("TF_trace.bin", TF_WRITE);
	}
	if (TIMECALL_keyFile.get() == NULL) {
		std::cerr << "Starting TIMECALL_keyfile" << std::endl;
		TIMECALL_keyFile.reset(new StackKeyWriter(fopen("TF_timekey.bin","w")));
	}
	if (TIMECALL_deltafile.get() == NULL) {
		TIMECALL_deltafile.reset(new StoreDelta());
	}
	if (TIMECALL_trackSyncTime.get() == NULL) {
		TIMECALL_trackSyncTime.reset(new TrackSyncTime());
	}
	//std::cerr << "Done Init" << std::endl;
}

void TIMER_SIMPLE_COUNT_ADD_ONE() {
	//std::cerr << "Start TIMER_SIMPLE_COUNT_ADD_ONE" << std::endl;
	INIT_TIMERS();
	if (TIMECALL_TimingCount.size() > 0){
		TIMECALL_trackSyncTime->Start();
		TIMECALL_TimingCount[TIMECALL_TimingCount.size() - 1] += 1;
	}
	else {
		// Write out an unknown timing entry
		
		TIMECALL_outFile->Write(TIMECALL_tfRecord);

	// 	//TIMECALL_outFile->Write(0,0,0,0.0);
	// 	//TIMECALL_outFile->Write(0, 0.0, 1);
	 	std::cout << "Timing error, trying to add one to an unknown synchronization!" << std::endl;
	}
	//std::cerr << "End TIMER_SIMPLE_COUNT_ADD_ONE" << std::endl;
}

void TIMER_END_SYNCHRONIZATION_CALL() {
	INIT_TIMERS();
	if (TIMECALL_TimingCount.size() > 0){
		TIMECALL_trackSyncTime->End();
	}
}

void TIMER_SIMPLE_TIME_START(uint64_t id) {
	//std::cerr << "STARD TIMER_SIMPLE_TIME_START" << std::endl;
	INIT_TIMERS();
	TIMECALL_TimingCount.push_back(0);
	TIMECALL_TimingPairs.push_back(std::make_pair(id,std::chrono::high_resolution_clock::now()));
	//std::cerr << "END TIMER_SIMPLE_TIME_START" << std::endl;
}

void TIMER_SIMPLE_TIME_STOP(uint64_t id) {
	//std::cerr << "STARD TIMER_SIMPLE_TIME_START" << std::endl;
	INIT_TIMERS();
	std::chrono::high_resolution_clock::time_point endTimer = std::chrono::high_resolution_clock::now();

	int found = -1;
	for (int i = TIMECALL_TimingPairs.size() - 1; i >= 0; i = i - 1) {
		if (TIMECALL_TimingPairs[i].first == id){
			found = i;
			break;
		}
	}
	if (found != TIMECALL_TimingPairs.size() - 1)
		std::cerr << "We are not the last one" << std::endl;
	if (found == -1) {
		std::cerr << "Could not find starting time for call " << id << std::endl;
		assert(found != -1);
	}
	
	std::chrono::duration<double> diff = endTimer-TIMECALL_TimingPairs[found].second;

	if (TIMECALL_TimingCount[TIMECALL_TimingCount.size() - 1] > 0) {
		std::vector<StackPoint> points;
		bool ret = GET_FP_STACKWALK(points);
		TIMECALL_tfRecord.s.dynId = id;
		TIMECALL_tfRecord.s.count = TIMECALL_TimingCount[TIMECALL_TimingCount.size() - 1];
		TIMECALL_tfRecord.s.time = diff.count();
		if (ret == false) {
			std::cout << "unknown timing stack, discarding time" << std::endl;
			TIMECALL_tfRecord.s.stackId  = 0;
		} else {
			uint64_t pos = TIMECALL_keyFile->InsertStack(points);
			TIMECALL_tfRecord.s.stackId = pos;
		}
		TIMECALL_deltafile->NextSyncStart(TIMECALL_TimingPairs[found].second);
		TIMECALL_deltafile->AddEndingTime(TIMECALL_tfRecord.s.stackId,endTimer);
		TIMECALL_outFile->Write(TIMECALL_tfRecord);
		TIMECALL_trackSyncTime->Write(TIMECALL_tfRecord.s.stackId);
		TIMECALL_TimingCount.pop_back();
	}
	TIMECALL_TimingPairs.erase(TIMECALL_TimingPairs.begin() + found);
	//std::cerr << "END TIMER_SIMPLE_TIME_STOP" << std::endl;
}

void init(std::vector<std::string> & cmd_list) {
	PLUG_BUILD_FACTORY(cmd_list)
}

PluginReturn Precall(std::shared_ptr<Parameters> params){
	if (TIMECALL_exited == 1)
		return NO_ACTION;
	return PLUG_FACTORY_PTR->Precall(params);
}

PluginReturn Postcall(std::shared_ptr<Parameters> params) {
	if (TIMECALL_exited == 1)
		return NO_ACTION;
	return PLUG_FACTORY_PTR->Postcall(params);
}

}