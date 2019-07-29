#include "TimeApplications.h"

TimeApplications::TimeApplications(boost::program_options::variables_map vm) :
	_vm(vm) {
		terminal_stdout = dup(fileno(stdout));
		terminal_stderr = dup(fileno(stderr));

}

TimeApplications::~TimeApplications() {
	close(terminal_stderr);
	close(terminal_stdout);	
}

void TimeApplications::RedirectOutToFile(std::string filename) {
	remove(filename.c_str());
	freopen(filename.c_str(),"w",stdout);
	dup2(fileno(stdout), fileno(stderr));
}

void TimeApplications::ReturnToTerminal() {
	dup2(terminal_stdout, fileno(stdout));
	dup2(terminal_stderr, fileno(stderr));
}


double TimeApplications::RunWithDriverAPITrace() {
	LogInfo log(std::string(""), std::string(""), false);
	ProcessController proc(_vm, &log);
	//proc.DontFinalize();
	proc.LaunchProcess();
	proc.InsertAPICapture();
	//proc.InsertStacktracing();
	assert(proc.ContinueExecution() == true);
	auto start = std::chrono::high_resolution_clock::now();
	while (!proc.IsTerminated()){
		proc.Run();
		assert(proc.ContinueExecution() == true);
	}
	auto stop = std::chrono::high_resolution_clock::now();	
	std::chrono::duration<double> diff = stop-start;
	std::cerr << "[TIMEAPP] Application runtime with stacktracing - " << diff.count() << std::endl;
	return diff.count();
}


double TimeApplications::RunWithStackTracing() {
	LogInfo log(std::string(""), std::string(""), false);
	ProcessController proc(_vm, &log);
	proc.DontFinalize();
	proc.LaunchProcess();
	proc.InsertStacktracing();
	assert(proc.ContinueExecution() == true);
	auto start = std::chrono::high_resolution_clock::now();
	while (!proc.IsTerminated()){
		proc.Run();
		assert(proc.ContinueExecution() == true);
	}
	auto stop = std::chrono::high_resolution_clock::now();	
	std::chrono::duration<double> diff = stop-start;
	std::cerr << "[TIMEAPP] Application runtime with stacktracing - " << diff.count() << std::endl;
	return diff.count();
}



double TimeApplications::InternalRun() {
	LogInfo log(std::string(""), std::string(""), false);
	ProcessController proc(_vm, &log);
	proc.LaunchProcess();
	assert(proc.ContinueExecution() == true);
	auto start = std::chrono::high_resolution_clock::now();
	while (!proc.IsTerminated()){
		proc.Run();
		assert(proc.ContinueExecution() == true);
	}
	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = stop-start;
	std::cerr << "[TIMEAPP] Application runtime without instrimentation - " << diff.count() << std::endl;
	return diff.count();	
}


double TimeApplications::InternalRunWithTiming(std::vector<StackPoint> points) {
	LogInfo log(std::string(""), std::string(""), false);
	ProcessController proc(_vm, &log);
	proc.LaunchProcess();
	proc.InsertInstrimentation(std::string(""));
	proc.InsertTimers(points);
	assert(proc.ContinueExecution() == true);
	auto start = std::chrono::high_resolution_clock::now();
	while (!proc.IsTerminated()){
		proc.Run();
		assert(proc.ContinueExecution() == true);
	}
	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = stop-start;
	std::cerr << "[TIMEAPP] Application runtime without instrimentation - " << diff.count() << std::endl;
	return diff.count();	
}
double TimeApplications::Run() {
	// Run this twice to get rid of dyninst overhead....
	return InternalRun();
}

double TimeApplications::RunWithInstrimentation(std::string wrapperDef, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras) {
	LogInfo log(std::string("InstRun.txt"), std::string("[InstRun]"), true);
	ProcessController proc(_vm, &log);
	proc.LaunchProcess();
	for (auto i : extras)
		proc.InsertWrapperDef(std::get<0>(i), std::get<1>(i), std::get<2>(i), std::get<3>(i), std::get<4>(i));
	proc.InsertInstrimentation(wrapperDef);
	proc.ContinueExecution();
	auto start = std::chrono::high_resolution_clock::now();
	while (!proc.IsTerminated()){
		proc.Run();
	}
	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = stop-start;
	std::cerr << "[TIMEAPP] Application runtime with instrimentation - " << diff.count() << std::endl;
	return diff.count();	
}

double TimeApplications::RunWithLSInstrimentation(std::string wrapperDef, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras,
				std::vector<StackPoint> & points, std::map<uint64_t, StackRecord> & syncStacks) {
	LogInfo log(std::string("InstRun.txt"), std::string("[InstRun]"), true);
	ProcessController proc(_vm, &log);
	proc.DontFinalize();
	proc.LaunchProcess();
	for (auto i : extras)
		proc.InsertWrapperDef(std::get<0>(i), std::get<1>(i), std::get<2>(i), std::get<3>(i), std::get<4>(i));
	proc.InsertInstrimentation(std::string(""));
	//proc.InsertInstrimentation(wrapperDef);
	std::vector<uint64_t> skips;
	uint64_t total_functions = 0;
	proc.InsertLoadStores(skips, total_functions, points, syncStacks);
	proc.ContinueExecution();
	auto start = std::chrono::high_resolution_clock::now();
	while (!proc.IsTerminated()){
		proc.Run();
		if (proc.IsStopped()){
			proc.ContinueExecution();
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = stop-start;
	std::cerr << "[TIMEAPP] Application runtime with instrimentation - " << diff.count() << std::endl;

	_firstUses = proc.GetFirstUse();
	return diff.count();	
}
double TimeApplications::GetFirstUses(std::string wrapperDef, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras,
				std::vector<StackPoint> & points, std::map<uint64_t, StackRecord> & syncStacks) {
	LogInfo log(std::string("FI_TimeLog.txt"), std::string("[TimeLog]"), true);
	ProcessController proc(_vm, &log);
	proc.DontFinalize();
	proc.LaunchProcess();
	for (auto i : extras)
		proc.InsertWrapperDef(std::get<0>(i), std::get<1>(i), std::get<2>(i), std::get<3>(i), std::get<4>(i));
	proc.InsertInstrimentation(std::string(""));
	//proc.InsertInstrimentation(wrapperDef);
	std::vector<uint64_t> skips;
	uint64_t total_functions = 0;
	proc.InsertFirstUse(skips, total_functions, points, syncStacks);
	proc.ContinueExecution();
	auto start = std::chrono::high_resolution_clock::now();
	while (!proc.IsTerminated()){
		proc.Run();
		if (proc.IsStopped()){
			proc.ContinueExecution();
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = stop-start;
	std::cerr << "[TIMEAPP] Application runtime with instrimentation - " << diff.count() << std::endl;

	//_firstUses = proc.GetFirstUse();
	return diff.count();	
}


double TimeApplications::RunWithBreakpoints(std::string wrapperDef, 
											std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras,
											std::vector<std::string> breakFunctions,
											std::vector<std::string> libLoads,
											std::function<void(ProcessController *)> cb) {
	LogInfo log(std::string("BreakpointRun.txt"), std::string("[BPRun]"), true);
	ProcessController proc(_vm, &log);
	proc.LaunchProcess();
	for (auto i : extras)
		proc.InsertWrapperDef(std::get<0>(i), std::get<1>(i), std::get<2>(i), std::get<3>(i), std::get<4>(i));
	proc.InsertInstrimentation(wrapperDef);
	for (auto i : libLoads) 
		proc.LoadWrapperLibrary(i);

	//proc.InsertBreakpoints(breakFunctions);
	proc.ContinueExecution();
	auto start = std::chrono::high_resolution_clock::now();
	while (!proc.IsTerminated()){
		proc.Run();
		if (proc.IsStopped()){
			std::cerr << "[TIMEAPP] Breakpoint has been hit" << std::endl;
			cb(&proc);
			proc.ContinueExecution();
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = stop-start;
	std::cerr << "[TIMEAPP] Application runtime with instrimentation - " << diff.count() << std::endl;
	return diff.count();	
}


void TimeApplications::IdentifyDyninstBugs(std::string wrapperDef, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras, std::vector<std::string> libLoads) {
	//std::vector<std::string> testerFunctions = {"__GI_strlen"};
	std::vector<std::string> testerFunctions = {"__random"};
	LogInfo outLog(std::string("NonworkingCalls.txt"), std::string("[NonWorking]"), true);
	for (auto n : testerFunctions) {
		remove("run_done.txt");
		LogInfo log(std::string("LoadStoreRun.txt"), std::string("[LSRUN]"), true);
		ProcessController proc(_vm, &log);
		proc.LaunchProcess();
		// proc.InsertLoadStores();
		for (auto i : libLoads) 
			proc.LoadWrapperLibrary(i);
		proc.InsertLoadStoreSingle(std::string("__random_r"));
		proc.InsertLoadStoreSingle(std::string(n));

		// for (auto i : extras)
		// 	proc.InsertWrapperDef(std::get<0>(i), std::get<1>(i), std::get<2>(i), std::get<3>(i), std::get<4>(i));
		// proc.InsertInstrimentation(wrapperDef);
		
		std::vector<std::string> bpoints;
		bpoints.push_back(std::string("SYNCH_SIGNAL_DYNINST"));
		bpoints.push_back(std::string("SYNC_RECORD_MEM_ACCESS"));
		bpoints.push_back(std::string("SYNC_RECORD_FUNCTION_ENTRY"));

		//proc.InsertBreakpoints(bpoints);
		proc.ContinueExecution();
		bool inserted = true;
		auto start = std::chrono::high_resolution_clock::now();
		while (!proc.IsTerminated()){
			proc.Run();

			if (proc.IsStopped()) {
				// std::cerr << "Hit breakpoint" << std::endl;
				// std::map<uint64_t, std::vector<StackPoint> > stackmap = proc.GetThreadStacks();
				// for (auto i : stackmap) {
				// 	std::cerr << "Stack length: " << i.second.size() << std::endl;
				// }
				proc.ContinueExecution();
			}

		}
		auto stop = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = stop-start;
		std::cerr << "[TIMEAPP] Application runtime with instrimentation - " << diff.count() << std::endl;
		bool badCall = false;
	    if (FILE *file = fopen("run_done.txt", "r")) {
	        fclose(file);
	    } else {
	        badCall = true;
	    }   
	    if (badCall == true){
	    	std::stringstream ss;
	    	ss << n << std::endl;
	    	outLog.Write(ss.str());
	    	outLog.Flush();
	    	std::cerr << n << " has failed" << std::endl;
	    }
	}
	// 	std::vector<std::string> testerFunctions = {"__random_r"};
	// LogInfo outLog(std::string("NonworkingCalls.txt"), std::string("[NonWorking]"), true);
	// for (auto n : testerFunctions) {
	// 	remove("run_done.txt");
	// 	LogInfo log(std::string("LoadStoreRun.txt"), std::string("[LSRUN]"), true);
	// 	ProcessController proc(_vm, &log);
	// 	proc.GenerateDebugBinary("/lib/x86_64-linux-gnu/libc.so.6");
	// 	// proc.InsertLoadStores();
	// 	for (auto i : libLoads) 
	// 		proc.LoadWrapperLibrary(i);
	// 	proc.InsertLoadStoreSingle(std::string("__random_r"));
	// 	//proc.InsertLoadStoreSingle(std::string(n));

	// 	// for (auto i : extras)
	// 	// 	proc.InsertWrapperDef(std::get<0>(i), std::get<1>(i), std::get<2>(i), std::get<3>(i), std::get<4>(i));
	// 	// proc.InsertInstrimentation(wrapperDef);
	// 	proc.WriteOutput(std::string("written_libc.so.6"));
	// 	std::vector<std::string> bpoints;

	// 	// bpoints.push_back(std::string("SYNCH_SIGNAL_DYNINST"));
	// 	// bpoints.push_back(std::string("SYNC_RECORD_MEM_ACCESS"));
	// 	// bpoints.push_back(std::string("SYNC_RECORD_FUNCTION_ENTRY"));

	// 	// //proc.InsertBreakpoints(bpoints);
	// 	// proc.ContinueExecution();
	// 	// bool inserted = true;
	// 	// auto start = std::chrono::high_resolution_clock::now();
	// 	// while (!proc.IsTerminated()){
	// 	// 	proc.Run();

	// 	// 	if (proc.IsStopped()) {
	// 	// 		// std::cerr << "Hit breakpoint" << std::endl;
	// 	// 		// std::map<uint64_t, std::vector<StackPoint> > stackmap = proc.GetThreadStacks();
	// 	// 		// for (auto i : stackmap) {
	// 	// 		// 	std::cerr << "Stack length: " << i.second.size() << std::endl;
	// 	// 		// }
	// 	// 		proc.ContinueExecution();
	// 	// 	}

	// 	// }
	// 	// auto stop = std::chrono::high_resolution_clock::now();
	// 	// std::chrono::duration<double> diff = stop-start;
	// 	// std::cerr << "[TIMEAPP] Application runtime with instrimentation - " << diff.count() << std::endl;
	// 	// bool badCall = false;
	//  //    if (FILE *file = fopen("run_done.txt", "r")) {
	//  //        fclose(file);
	//  //    } else {
	//  //        badCall = true;
	//  //    }   
	//  //    if (badCall == true){
	//  //    	std::stringstream ss;
	//  //    	ss << n << std::endl;
	//  //    	outLog.Write(ss.str());
	//  //    	outLog.Flush();
	//  //    	std::cerr << n << " has failed" << std::endl;
	//  //    }
	// }

}

double TimeApplications::RunWithLoadStore(std::string wrapperDef, std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string> > extras, std::vector<std::string> libLoads) {
	// IdentifyDyninstBugs(wrapperDef, extras, libLoads);
	// return 0.0;
	// uint64_t total_functions = 0;
	{
		LogInfo log(std::string("LoadStoreRun.txt"), std::string("[LSRUN]"), true);
		ProcessController proc(_vm, &log);

		proc.LaunchProcess();
		//proc.InsertLoadStores();
		for (auto i : libLoads) 
			proc.LoadWrapperLibrary(i);
		std::vector<uint64_t> skips;
		//proc.InsertLoadStores(skips, total_functions);
		// for (auto i : extras)
		// 	proc.InsertWrapperDef(std::get<0>(i), std::get<1>(i), std::get<2>(i), std::get<3>(i), std::get<4>(i));
		// proc.InsertInstrimentation(wrapperDef);
		
		std::vector<std::string> bpoints;
		bpoints.push_back(std::string("SYNCH_SIGNAL_DYNINST"));
		bpoints.push_back(std::string("SYNC_RECORD_MEM_ACCESS"));
		bpoints.push_back(std::string("SYNC_RECORD_FUNCTION_ENTRY"));

		//proc.InsertBreakpoints(bpoints);
		proc.ContinueExecution();
		bool inserted = true;
		auto start = std::chrono::high_resolution_clock::now();
		while (!proc.IsTerminated()){
			proc.Run();

			if (proc.IsStopped()) {
				// std::cerr << "Hit breakpoint" << std::endl;
				// std::map<uint64_t, std::vector<StackPoint> > stackmap = proc.GetThreadStacks();
				// for (auto i : stackmap) {
				// 	std::cerr << "Stack length: " << i.second.size() << std::endl;
				// }
				proc.ContinueExecution();
			}

		}
		auto stop = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = stop-start;
		std::cerr << "[TIMEAPP] Application runtime with instrimentation - " << diff.count() << std::endl;
	}

	// // Experimental remove later.
	// LogInfo outLog(std::string("IdsOfNonWorkers.txt"), std::string("[NonWorking]"), true);
	// uint64_t curPos = 260;
	// std::vector<uint64_t> funskips;
	// while (curPos < total_functions) {
	// 	remove("run_done.txt");
	// 	uint64_t tmpMe = curPos + 1;
	// 	LogInfo log(std::string("LoadStoreRun.txt"), std::string("[LSRUN]"), true);
	// 	ProcessController proc(_vm, &log);

	// 	proc.LaunchProcess();
	// 	// proc.InsertLoadStores();
	// 	for (auto i : libLoads) 
	// 		proc.LoadWrapperLibrary(i);
	// 	proc.InsertLoadStores(funskips, tmpMe);
	// 	// for (auto i : extras)
	// 	// 	proc.InsertWrapperDef(std::get<0>(i), std::get<1>(i), std::get<2>(i), std::get<3>(i), std::get<4>(i));
	// 	// proc.InsertInstrimentation(wrapperDef);
		
	// 	std::vector<std::string> bpoints;
	// 	bpoints.push_back(std::string("SYNCH_SIGNAL_DYNINST"));
	// 	bpoints.push_back(std::string("SYNC_RECORD_MEM_ACCESS"));
	// 	bpoints.push_back(std::string("SYNC_RECORD_FUNCTION_ENTRY"));

	// 	//proc.InsertBreakpoints(bpoints);
	// 	proc.ContinueExecution();
	// 	bool inserted = true;
	// 	auto start = std::chrono::high_resolution_clock::now();
	// 	while (!proc.IsTerminated()){
	// 		proc.Run();

	// 		if (proc.IsStopped()) {
	// 			// std::cerr << "Hit breakpoint" << std::endl;
	// 			// std::map<uint64_t, std::vector<StackPoint> > stackmap = proc.GetThreadStacks();
	// 			// for (auto i : stackmap) {
	// 			// 	std::cerr << "Stack length: " << i.second.size() << std::endl;
	// 			// }
	// 			proc.ContinueExecution();
	// 		}

	// 	}
	// 	auto stop = std::chrono::high_resolution_clock::now();
	// 	std::chrono::duration<double> diff = stop-start;
	// 	std::cerr << "[TIMEAPP] Application runtime with instrimentation - " << diff.count() << std::endl;		
	// 	bool badCall = false;
	//     if (FILE *file = fopen("run_done.txt", "r")) {
	//         fclose(file);
	//     } else {
	//         badCall = true;
	//     }   
	//     if (badCall == true){
	//     	std::stringstream ss;
	//     	ss << curPos << std::endl;
	//     	outLog.Write(ss.str());
	//     	outLog.Flush();
	//     	funskips.push_back(curPos);
	//     	std::cerr << curPos << " has failed" << std::endl;
	//     }
	//     curPos++;
	// }
	return 0.0;
}

