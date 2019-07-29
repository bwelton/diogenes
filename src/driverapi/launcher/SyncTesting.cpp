#include "SyncTesting.h"
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#define  TIMER_FUNCTION_CALL(A,B) std::chrono::high_resolution_clock::time_point starttime = std::chrono::high_resolution_clock::now();\
	A;\
	std::chrono::high_resolution_clock::time_point endtime = std::chrono::high_resolution_clock::now();\
	std::chrono::duration<double> diff = endtime - starttime;\
	std::cerr << B << " : " << diff.count() << std::endl;


SyncTesting::SyncTesting(boost::program_options::variables_map vm) :
	_vm(vm) {
	if (vm.count("prog") > 0){
		std::vector<std::string> progName = _vm["prog"].as<std::vector<std::string> >();
		_programName = std::string(basename((char *)progName[0].c_str()));
	}	
}

void SyncTesting::ReprocessModel() {
	// Add a bogus execution time for now so that other things do not break
	_model.AddExecutionTime(0.0);

	// Get the stacks and line info.
	_model.ReadStackFiles();
	//_model.ExtractLineInfo();

	// Get the timing list
	std::vector<StackPoint> timingList;
	_model.GetTimingList(timingList);

	// Read the syncronization times.
	_model.CaptureSyncTime();
}

void SyncTesting::DecodeStack(std::string infile, std::string outfile) {
	_model.TranslateStackfile(infile, outfile);
}

std::shared_ptr<DyninstProcess> SyncTesting::LaunchApplication(bool debug) {
	std::shared_ptr<DyninstProcess> ret(new DyninstProcess(_vm, debug));
	assert(ret->LaunchProcess() != NULL);

	// Load libcuda.so into the address space of the process
	ret->LoadLibrary(std::string("libcuda.so.1"));
	return ret;
}

void SyncTesting::CaptureDriverCalls() {
	system("exec rm -rf ./stackOut.*");
	std::shared_ptr<DyninstProcess> proc = LaunchApplication(false);
	std::vector<std::string> pluginNames = {"libDriverAPITrace"};
	CreatePluginFile(pluginNames);	
	std::cerr << "Running " << _programName << " with driver api trace to obtain total execution time" << std::endl;
	std::cerr << "Saving application output to file : " << _programName << ".apitrace.log" << std::endl;

	APICaptureInstrimentation inst(proc);
	inst.InsertInstrimentation();
	TIMER_FUNCTION_CALL(proc->RunUntilCompleation(), "[SEGMENTTIME] Driver API Time is (sec) = ")
	// {
	// 	double time;
	// 	TimeApplications base(_vm);
	// 	std::vector<std::string> pluginNames = {"libDriverAPITrace"};
	// 	CreatePluginFile(pluginNames);
	// 	std::cerr << "Running " << _programName << " with driver api trace to obtain total execution time" << std::endl;
	// 	std::cerr << "Saving application output to file : " << _programName << ".apitrace.log" << std::endl;
	// 	base.RedirectOutToFile(_programName + std::string(".apitrace.log"));
	// 	time = base.RunWithDriverAPITrace();
	// 	base.ReturnToTerminal();
	// 	std::cerr << "Application executed with runtime of - " << time << "s" << std::endl;
	// }
	// Write out the stack values that were saved to CS_StackFile.txt
	_model.WriteStackFile(std::string("CS_StackFile.txt"), std::string("CS_HumanStacks.txt"));
}

void SyncTesting::CaptureDuplicateTransfers() {
	system("exec rm -rf ./stackOut.*");
	std::shared_ptr<DyninstProcess> proc = LaunchApplication(false);
	std::vector<std::string> pluginNames = {"libDataTransfer"};
	CreatePluginFile(pluginNames);	
	std::cerr << "Running " << _programName << " to detect duplicate transfers" << std::endl;
	std::cerr << "Saving application output to file : " << _programName << ".datatransfer.log" << std::endl;

	APICaptureInstrimentation inst(proc);
	inst.InsertInstrimentation();
	TIMER_FUNCTION_CALL(proc->RunUntilCompleation(), "[SEGMENTTIME] Capture duplicate transfer time (sec) = ")

	// {
	// 	double time;
	// 	TimeApplications base(_vm);
	// 	std::vector<std::string> pluginNames = {"libDataTransfer"};
	// 	CreatePluginFile(pluginNames);
	// 	std::cerr << "Running " << _programName << " with data transfer trace to identify duplicate transfers" << std::endl;
	// 	std::cerr << "Saving application output to file : " << _programName << ".datatransfer.log" << std::endl;
	// 	base.RedirectOutToFile(_programName + std::string(".datatransfer.log"));
	// 	time = base.RunWithDriverAPITrace();
	// 	base.ReturnToTerminal();
	// 	std::cerr << "Application executed with runtime of - " << time << "s" << std::endl;
	// }
	// Write out the stack values that were saved to CS_StackFile.txt
	_model.WriteStackFile(std::string("DT_stacks.bin"), std::string("DT_humanstacks.txt"));	
}
void SyncTesting::TimeTransfers() {
	system("exec rm -rf ./stackOut.*");
	std::shared_ptr<DyninstProcess> proc;
	{
	TIMER_FUNCTION_CALL(proc = LaunchApplication(false), "[PARSE_TIME]")
	}
	std::vector<std::string> pluginNames = {"libDataSyncTimer"};
	CreatePluginFile(pluginNames);	
	std::cerr << "Running " << _programName << " to time transfers" << std::endl;
	std::cerr << "Saving application output to file : " << _programName << ".timetransfer.log" << std::endl;

	APICaptureInstrimentation inst(proc);
	inst.InsertInstrimentation();
	{
	//proc->DetachForDebug();
	TIMER_FUNCTION_CALL(proc->RunUntilCompleation(), "[SEGMENTTIME] Timing of transfers = ")
	}
	// {
	// 	double time;
	// 	TimeApplications base(_vm);
	// 	std::vector<std::string> pluginNames = {"libDataSyncTimer"};
	// 	CreatePluginFile(pluginNames);
	// 	std::cerr << "Running " << _programName << " with data transfer trace identify duplicate transfers" << std::endl;
	// 	std::cerr << "Saving application output to file : " << _programName << ".timetransfer.log" << std::endl;
	// 	base.RedirectOutToFile(_programName + std::string(".timetransfer.log"));
	// 	time = base.RunWithDriverAPITrace();
	// 	base.ReturnToTerminal();
	// 	std::cerr << "Application executed with runtime of - " << time << "s" << std::endl;
	// }
	// Write out the stack values that were saved to CS_StackFile.txt
	_model.WriteStackFile(std::string("DSTIME_stacks.bin"), std::string("DSTIME_humanstacks.txt"));	
}

void SyncTesting::RunWithSyncStacktracing(StackRecMap & recs) {
	system("exec rm -rf ./stackOut.*");
	std::shared_ptr<DyninstProcess> proc = LaunchApplication(false);
	proc->RunCudaInit();
	StacktraceSynchronizations sync(proc);
	sync.InsertStacktracing();
	TIMER_FUNCTION_CALL(proc->RunUntilCompleation(), "[SEGMENTTIME] Identification of Synchronizations Time = ")

	sync.ReadResults(recs);

}

std::set<std::string> GetAllFilesInDirectory(std::string inDir) {
	std::set<std::string> ret;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(inDir.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			std::cout << "FILENAME! = " << ent->d_name << std::endl;
			ret.insert(std::string(ent->d_name));
		}
	}
	closedir(dir);
	return ret;
}

bool DoesDirectoryExist(std::string inDir) {
	DIR* dir = opendir(inDir.c_str());
	if (dir){
		closedir(dir);
		return true;
	}
	return false;
}

int GetNextDirNumber(std::string inDir) {
	auto dirs = GetAllFilesInDirectory(inDir);
	std::set<int> tmp;
	tmp.insert(0);
	for (auto n : dirs){
		try {
		int tmpN = std::stoi(n);
		//if(sscanf(n.c_str(), "%d", &tmpN) == 0)
		tmp.insert(tmpN);
		} catch (...) {
			std::cout << "Could Not decode int from filename = " << n << std::endl;
		}
	}
	return (*(tmp.rbegin())) + 1;
}

std::set<std::string> GetFileNamesToMove(std::string fileList) {
	std::ifstream t(fileList.c_str());
	std::string line;
	std::set<std::string> ret;
	while (std::getline(t, line)) {
		ret.insert(line);
	}	
	t.close();
	return ret;
}

void SyncTesting::CopyOldFiles() {
	if (DoesDirectoryExist("./DiogenesData") == false)
		mkdir("./DiogenesData", S_IRWXU);

	std::string nextFolder = std::string("./DiogenesData/") + std::to_string(GetNextDirNumber(std::string("./DiogenesData")));
	std::set<std::string> files = GetAllFilesInDirectory(std::string("."));
	std::set<std::string> filesToMove = GetFileNamesToMove(std::string(DIOGENES_OUTPUT_FILELIST));
	bool madeFolder =false;
	for (auto i : files) {
		if (filesToMove.find(i) != filesToMove.end()) {
			if (madeFolder == false){
				mkdir(nextFolder.c_str(), S_IRWXU);
				madeFolder = true;
			}
		    std::ifstream  src(i, std::ios::binary);
		    std::ofstream  dst(nextFolder + std::string("/") + i,   std::ios::binary);
		    dst << src.rdbuf();	
		    src.close();
		    dst.close();
		    remove(i.c_str());
		}
	}
}


void SyncTesting::RunWithoutInstrimentation() {
	system("exec rm -rf ./stackOut.*");
	std::shared_ptr<DyninstProcess> proc = LaunchApplication(false);
	// std::vector<std::string> pluginNames = {"libDataSyncTimer"};
	// CreatePluginFile(pluginNames);	
	std::cerr << "Running " << _programName << " without instrimentation" << std::endl;
	std::cerr << "Saving application output to file : " << _programName << ".orig.log" << std::endl;
	// APICaptureInstrimentation inst(proc);
	// inst.InsertInstrimentation();
	proc->RunUntilCompleation();	
}

void SyncTesting::TimeSynchronizations(StackRecMap & recs) {
	system("exec rm -rf ./stackOut.*");
	std::shared_ptr<DyninstProcess> proc = LaunchApplication(false);
	proc->RunCudaInit();
	TimerInstrimentation t(proc);
	t.InsertTimers(recs);
	//proc->DetachForDebug();
	TIMER_FUNCTION_CALL(proc->RunUntilCompleation(), "[SEGMENTTIME] Timing of synchronizations = ")

	t.PostProcessing(recs);
}

void SyncTesting::FixProblems(StackRecMap & recs) {
	system("exec rm -rf ./stackOut.*");
	std::shared_ptr<DyninstProcess> proc = LaunchApplication(false);
	proc->RunCudaInit();
	FixCudaFree t(proc);
	t.InsertAnalysis(recs);
//	proc->DetachForDebug();
	TIMER_FUNCTION_CALL(proc->RunUntilCompleation(), "[SEGMENTTIME] Timing of synchronizations = ")

	//t.PostProcessing(recs);
}

CallTransPtr SyncTesting::MemRecorderLaunch(StackRecMap & recs) {
	system("exec rm -rf ./stackOut.*");
	std::shared_ptr<DyninstProcess> proc = LaunchApplication(false);
	proc->RunCudaInit();
	MemRecorder t(proc);
	t.InsertAnalysis(recs);
//	proc->DetachForDebug();
	TIMER_FUNCTION_CALL(proc->RunUntilCompleation(), "[SEGMENTTIME] Timing of synchronizations = ")
	CallTransPtr ret = t.PostProcessing();
	return ret;
	//t.PostProcessing(recs);
}

void SyncTesting::FixKnownProblems(StackRecMap & recs, CallTransPtr & trans) {
	system("exec rm -rf ./stackOut.*");
	std::shared_ptr<DyninstProcess> proc = LaunchApplication(false);
	proc->RunCudaInit();
	FixCudaProblems t(proc);
	t.InsertAnalysis(recs, trans);
//	proc->DetachForDebug();
	TIMER_FUNCTION_CALL(proc->RunUntilCompleation(), "[SEGMENTTIME] Timing of synchronizations = ")
	t.PostProcessing();
	//return ret;
	//t.PostProcessing(recs);
}

void SyncTesting::RunLoadStoreAnalysis(StackRecMap & recs, std::vector<StackPoint> & uses) {
	system("exec rm -rf ./stackOut.*");
	std::shared_ptr<DyninstProcess> proc = LaunchApplication(false);
	proc->RunCudaInit();
	std::vector<std::string> pluginNames = {"libSynchTool"};
	CreatePluginFile(pluginNames);		
	LoadStoreInstrimentation ls(proc);
	ls.InsertAnalysis(recs);
//	proc->DetachForDebug();
	TIMER_FUNCTION_CALL(proc->RunUntilCompleation(), "[SEGMENTTIME] Load Store Analysis Time = ")
	ls.PostProcessing(recs, uses);	
}

void SyncTesting::RunTimeUse(StackRecMap & recs, std::vector<StackPoint> & uses) {
	system("exec rm -rf ./stackOut.*");
	if (uses.size() == 0)
		return;
	std::shared_ptr<DyninstProcess> proc = LaunchApplication(false);
	proc->RunCudaInit();
	std::vector<std::string> pluginNames = {"libSynchTool"};
	CreatePluginFile(pluginNames);		
	TimeUse tu(proc);
	tu.InsertAnalysis(recs, uses);
	TIMER_FUNCTION_CALL(proc->RunUntilCompleation(), "[SEGMENTTIME] Sync-Uset time = ")
	tu.PostProcessing(recs, uses);	
}


void SyncTesting::Run() {
	// CopyOldFiles();
	// return;
	StackRecMap syncTiming; 
	double time;
	// {
	// 	TimeApplications base(_vm);
	// 	std::cerr << "Running " << _programName << " without instrimentation to obtain total execution time" << std::endl;
	// 	std::cerr << "Saving application output to file : " << _programName << ".base.out" << std::endl;
	// 	base.RedirectOutToFile(_programName + std::string(".base.out"));
	// 	time = base.Run();
	// 	base.ReturnToTerminal();
	// 	_model.AddExecutionTime(time);
	// 	std::cerr << "Application executed with runtime of - " << time << "s" << std::endl;
	// }
	//CaptureDriverCalls();

	//return;
	/*CopyOldFiles();
	RunWithSyncStacktracing(syncTiming);
	TimeTransfers();
	CaptureDuplicateTransfers();
    //RunWithoutInstrimentation();
	TimeSynchronizations(syncTiming);
	std::vector<StackPoint> uses;
	RunLoadStoreAnalysis(syncTiming, uses);
	RunTimeUse(syncTiming, uses);
	*/
	StackRecMap empty_map;
	CallTransPtr transRec = MemRecorderLaunch(empty_map);
	FixKnownProblems(empty_map, transRec);
	//RunTimeUse(sy)
	return;
	//RunWithCUPTI();
/*
	// Find out what user called functions actually contain a synchronization.
	// This also captures secret entries into libcuda with synchronizations and relates them back
	// to user level calls. 
	{
		// Remove all previous stack files from this directory.
		system("exec rm -rf ./stackOut.*");
		TimeApplications base(_vm);
		std::cerr << "Running " << _programName << " with stacktrace information enabled" << std::endl;
		std::cerr << "Saving application output to file : " << _programName << ".stacktrace.log" << std::endl;
		base.RedirectOutToFile(_programName + std::string(".stacktrace.log"));
		time = base.RunWithStackTracing();
		base.ReturnToTerminal();
		_model.AddExecutionTime(time);
		std::cerr << "Application executed with runtime of - " << time << "s" << std::endl;		
	}
	//return;
	 _model.ReadStackFiles();
	//_model.ExtractLineInfo();
	//InstrumentProgram();
	std::vector<StackPoint> timingList;
	_model.GetTimingList(timingList);
	std::cout << "Timing list of size - " << timingList.size() << std::endl;

	// Get timing for these functions
	{
		TimeApplications base(_vm);
		std::cerr << "Running " << _programName << " with timing information enabled" << std::endl;
		std::cerr << "Saving application output to file : " << _programName << ".time.log" << std::endl;
		base.RedirectOutToFile(_programName + std::string(".time.log"));		
		double mytime = base.InternalRunWithTiming(timingList);
		base.ReturnToTerminal();
		_model.AddExecutionTime(mytime);	
		std::cerr << "Application executed with runtime of - " << mytime << "s" << std::endl;	
	}


	// Check that our timing run matches the synchronizations identified.
	// If so, this program is very likely deterministic given the same inputs in relation to cuda calls.
	_model.CaptureSyncTime();
	//return;

	// Load Store Instrimentation.
	// What needs to be captured:
	// 		Function Entry for each function that Synchronizes* 
	//      Function Exit for each function that Synchronizes*
	//          Capture only target functions which we dont know.
	//      Synchronization Call
	//      Memory Allocations and Transfers
	//      Load Stores elsewhere
	// 
	{	
		std::vector<std::string> pluginNames = {"libSynchTool"};
		CreatePluginFile(pluginNames);
		std::string def(WRAPPER_DEF);
		//std::string def("");
		TimeApplications base(_vm);
		std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras;
		std::map<uint64_t, StackRecord> stackSyncs = _model.GetTimingStackRecords();
		std::cerr << "Saving application output to file : " << _programName << ".synchronization.log" << std::endl;
		base.RedirectOutToFile(_programName + std::string(".synchronization.log"));	
		//extras.push_back(std::make_tuple(std::string("wrap"), std::string(INTERNAL_SYNC), std::string("INTER_InternalSynchronization"), std::string(DRIVER_LIBRARY), std::string("ORIGINAL_InternalSynchronization")));
		double time = base.RunWithLSInstrimentation(def, extras, timingList, stackSyncs);
		base.ReturnToTerminal();
		std::cerr << "[Base] We are setting up syncs checking with " << stackSyncs.size() << std::endl;
		//_model.AddFirstUses(base._firstUses);
	}
	{	
		std::vector<std::string> pluginNames = {"libTimeFirstUse"};
		CreatePluginFile(pluginNames);
		std::string def(WRAPPER_DEF);
		//std::string def("");
		TimeApplications base(_vm);
		std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras;
		std::map<uint64_t, StackRecord> stackSyncs = _model.GetTimingStackRecords();
		std::cerr << "Saving application output to file : " << _programName << ".firstUse.log" << std::endl;
		base.RedirectOutToFile(_programName + std::string(".firstUse.log"));	
		//extras.push_back(std::make_tuple(std::string("wrap"), std::string(INTERNAL_SYNC), std::string("INTER_InternalSynchronization"), std::string(DRIVER_LIBRARY), std::string("ORIGINAL_InternalSynchronization")));
		double time = base.GetFirstUses(def, extras, timingList, stackSyncs);
		base.ReturnToTerminal();
		std::cerr << "[Base] We are setting up syncs checking with " << stackSyncs.size() << std::endl;
		//_model.AddFirstUses(base._firstUses);
	}
	*/
}

void SyncTesting::ReadDefinition(std::string WrapperDef) {
	std::ifstream f;
	std::string line;
	f.open(WrapperDef.c_str(),std::ifstream::in);
	while (std::getline(f, line)) {
	    std::stringstream ss(line);
	    std::vector<std::string> tokens;
	    std::string item;
	    while (std::getline(ss, item, ',')) {
	        tokens.push_back(item);
	    }
	    if (tokens.size() != 5) {
	    	std::cerr << std::string("Token size is not 5 in wrapper def.... skipping this function\n");
	    	std::cerr << std::string("Line skipped: ")  << line << std::endl;
	    } else {
	    	std::transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(), ::tolower);
	    	_wrapperDefFunctions.push_back(std::make_tuple(tokens[0],tokens[1],tokens[2],tokens[3], tokens[4]));
	    }
	}
}

void SyncTesting::CreatePluginFile(std::vector<std::string> plugins) {
	std::vector<std::string> PluginList = PLUGIN_LIST;
	std::ofstream pfile;
	pfile.open("pluginlist.txt");
	for (auto i : plugins) {
		for (auto z : PluginList) {
			if (z.find(i) != std::string::npos){
				pfile << z << std::endl;
				break;
			}
		}
	}
	pfile.close();
}

void SyncTesting::ReadSynchronizationCalls() {
  std::ifstream ifs ("synchronous_calls.txt", std::ifstream::in);
  std::string line;
  while (std::getline(ifs, line)) {
  	_syncCalls.insert(line);
  }
  ifs.close();
}

void SyncTesting::GatherSynchronizationCalls() {
/*	std::vector<std::string> pluginNames = {"libSynchTool"};
	CreatePluginFile(pluginNames);
	std::string def(WRAPPER_DEF);
	TimeApplications base(_vm);
	std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras;
	extras.push_back(std::make_tuple(std::string("wrap"), std::string(INTERNAL_SYNC), std::string("INTER_InternalSynchronization"), std::string(DRIVER_LIBRARY), std::string("ORIGINAL_InternalSynchronization")));
	double time = base.RunWithInstrimentation(def, extras);
	ReadSynchronizationCalls();
*/
}

void SyncTesting::RunWithLoadStoreAnalysis() {
/*	std::vector<std::string> pluginNames = {"libSynchTool"};
	std::vector<std::string> pluginLoads;
	std::vector<std::string> PluginList = PLUGIN_LIST;
	for (auto i : PluginList) {
		if (i.find("libSynchTool") != std::string::npos){
			pluginLoads.push_back(i);
			break;
		}
	}	
	CreatePluginFile(pluginNames);
	std::string def("");
	TimeApplications base(_vm);
	std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras;
	extras.push_back(std::make_tuple(std::string("wrap"), std::string(INTERNAL_SYNC), std::string("INTER_InternalSynchronization"), std::string(DRIVER_LIBRARY), std::string("ORIGINAL_InternalSynchronization")));
	double time = base.RunWithLoadStore(def, extras, pluginLoads);
	//ReadSynchronizationCalls();
*/
}

// void SyncTesting::HandleSynchronizationBreakpoint(ProcessController & p) {

// }

//void SyncTesting::HandleBreakpoint(ProcessController * p) {
//	std::map<uint64_t, std::vector<StackPoint> > stacks = p->GetThreadStacks();
	// for (auto i : stacks) {
	// 	bool containsLibcuda = false;
	// 	for (auto z : i.second) 
	// 		if(z.libname.find("libSynchTool.so") != std::string::npos) {
	// 			containsLibcuda = true;
	// 			break;
	// 		}
	// 	if (containsLibcuda == true) {
	// 		_model.AddStack(i.second);
	// 		std::stringstream ss;
	// 		for (auto z : i.second) {
	// 			if(_stackKeys.find(z) == _stackKeys.end())
	// 				_stackKeys.insert(z);
	// 		}
	// 		if (_storedStacks.find(ss.str()) == _storedStacks.end())
	// 			_storedStacks[ss.str()] = 1;
	// 		else
	// 			_storedStacks[ss.str()] += 1;
	// 	}
	// }
//}

void SyncTesting::InstrumentProgram() {
	// This is going to be off by one for the first synchronization call.....
/*
	std::vector<std::string> pluginNames = {"libSynchTool"};
	std::vector<std::string> pluginLoads;
	std::vector<std::string> PluginList = PLUGIN_LIST;
	std::vector<std::string> breakpointNames = {"SYNCH_SIGNAL_DYNINST"};
	for (auto i : PluginList) {
		if (i.find("libSynchTool") != std::string::npos){
			pluginLoads.push_back(i);
			break;
		}
	}
	CreatePluginFile(pluginNames);
	std::string def("");
	TimeApplications base(_vm);
	std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras;
	extras.push_back(std::make_tuple(std::string("wrap"), std::string(INTERNAL_SYNC), std::string("INTER_InternalSynchronization"), std::string(DRIVER_LIBRARY), std::string("ORIGINAL_InternalSynchronization")));
	base.RedirectOutToFile(_programName + std::string(".breakpoint.out"));
	double time = base.RunWithBreakpoints(def, extras, breakpointNames,pluginLoads, std::bind(&SyncTesting::HandleBreakpoint, this, std::placeholders::_1));
	base.ReturnToTerminal();
	//ReadSynchronizationCalls();
*/
}

void SyncTesting::CreateFunctionTimers(std::vector<std::string> functions) {
	std::ofstream pfile;
	pfile.open("FuncsToTime.txt");
	for (auto i : functions) {
		pfile << i << std::endl;
	}
	pfile.close();	
}

void SyncTesting::RunWithCUPTI() {
/*	std::vector<std::string> cupti_plugin = {"libCUPTIEventHandler"};
	CreatePluginFile(cupti_plugin);
	TimeApplications base(_vm);
	std::string def(WRAPPER_DEF);
	std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras;
	base.RedirectOutToFile(_programName + std::string(".cupti.out"));
	double time = base.RunWithInstrimentation(def, extras);
	base.ReturnToTerminal();
*/
}

void SyncTesting::GatherSynchronizationDelay() {
/*	std::vector<std::string> cupti_plugin = {"libTimeCall"};
    std::vector<std::string> funcsToTime = {"InternalSynchronization"};
	std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras;
	ReadDefinition(std::string(WRAPPER_DEF));
	std::set<std::string> check;
	for (auto i : _wrapperDefFunctions) {
		std::string tmp = std::get<1>(i);
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
		for (auto z : _syncCalls) {
			std::string tmp2 = z;
			std::transform(tmp2.begin(), tmp2.end(), tmp2.begin(), ::tolower);
			tmp2 = tmp2 + std::string("_v2");
			if (tmp == tmp2) {
				extras.push_back(i);
				funcsToTime.push_back(std::get<1>(i));
				check.insert(z);
				break;
			}	
			tmp2 = z;
			std::transform(tmp2.begin(), tmp2.end(), tmp2.begin(), ::tolower);	
			if (tmp == tmp2) {
				extras.push_back(i);
				funcsToTime.push_back(std::get<1>(i));
				check.insert(z);
				break;
			}
		}
	}
	if (check.size() != _syncCalls.size())
		std::cerr << "POTENTIAL ERROR - Sync calls do not match timing calls!" << std::endl;
	CreatePluginFile(cupti_plugin);
	CreateFunctionTimers(funcsToTime);
	TimeApplications base(_vm);
	std::string def("");
	extras.push_back(std::make_tuple(std::string("wrap"), std::string(INTERNAL_SYNC), std::string("INTER_InternalSynchronization"), std::string(DRIVER_LIBRARY), std::string("ORIGINAL_InternalSynchronization")));
	double time = base.RunWithInstrimentation(def, extras);	*/
}
