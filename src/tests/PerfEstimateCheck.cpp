#define BOOST_TEST_MODULE peformance_estimation_test
#define BOOST_TEST_MAIN
#include "BaseTesting.h"
#include <unistd.h>
BOOST_AUTO_TEST_SUITE( peformance_eval )

void TestProgramWithDuplicateTransfers(int dupCount) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp;
	tmp.reset(new CudaCtx());
	DeviceMemory<double> dev(4096);
	sleep(3);
	// dupCount synchrounous duplicate transfers to the GPU
	for (int i = 0; i < dupCount + 1; i++) {
		dev.WriteSync();
	}
}

void WriteTimelineConfigure(void) {
	FILE * f = fopen("dedup_settings.conf","w");
	fprintf(f, "EnableDestination=false\nEnableTimeline=true\nEnableChecker=false\nEnableCUPTITiming=true");
	fclose(f);
}

void OnlyCUPTIConfigure(void) {
	FILE * f = fopen("dedup_settings.conf","w");
	fprintf(f, "EnableDestination=false\nEnableTimeline=false\nEnableChecker=false\nEnableCUPTITiming=true");
	fclose(f);
}

std::pair<double, double> ParseResults(void) {
	std::ifstream t("test_results.txt");
	double exact, worst;
	if (t.good()) {
		std::string input((std::istreambuf_iterator<char>(t)),
	    	              std::istreambuf_iterator<char>());
		sscanf(input.c_str(), "%lf,%lf", &exact, &worst);
		return std::make_pair(exact, worst);
	} else {
		std::cerr << "Could not open parse result output file" << std::endl;
	}
	return std::make_pair(-1,-1);
}

std::pair<double, double> ProcessResults(void) {
	remove("test_results.txt");
	pid_t child_pid = fork();
	if (child_pid == 0){
		// Child process, perform action then exit
		RedirectOutputToFile();
		//system( "python CorrelationRun.py first_timeline.txt timing_info.txt timing_packet_corr.txt test_results.txt");
		//std::cerr << "COULD NOT LAUNCH PYTHON!" << std::cerr;
		exit(-1);
	} else {
		pid_t tpid = 0;
		int status = 1;
		while (tpid != child_pid) {
			tpid = wait(&status);
			if(tpid != child_pid) {
				BOOST_FAIL("Could not wait on child process, fork may not be functional on this machine");
			}
		}
		return ParseResults();
	}
	return std::make_pair(-1,-1);
}

double GetTotalTime(void) {
	std::ifstream t("timing_info.txt");	
	double totalExec = -1;
	if (t.good()) {
		std::string input((std::istreambuf_iterator<char>(t)),
	    	              std::istreambuf_iterator<char>());
	    size_t startPos = input.find("TET,");
	    std::string intermediate = input.substr(startPos, input.size());
	 	sscanf(intermediate.c_str(), "TET,%lf\n", &totalExec);
	} else {
		std::cerr << "Could not open timing info output file" << std::endl;
	}
	return totalExec;
}

BOOST_AUTO_TEST_CASE(launch_test) {
	// SANITY CHECK: Checks to ensure that we libcuda was instrimented correctly.
	// If this fails, something is wrong in the environment

	// Enable timeline and CUPTI
	for(int i = 100; i < 2000; i = i + 100) {
		remove("dedup_timeline.txt");
		remove("first_timeline.txt");
		remove("timing_info.txt");
		WriteTimelineConfigure();
		TestingFixture t1;
		BOOST_REQUIRE(t1.LaunchAndWait(boost::bind(TestProgramWithDuplicateTransfers, i)) == 0);
		OnlyCUPTIConfigure();
		rename("dedup_timeline.txt", "first_timeline.txt");
		TestingFixture t2;
		BOOST_REQUIRE(t2.LaunchAndWait(boost::bind(TestProgramWithDuplicateTransfers, i)) == 0);
		std::pair<double, double> ret = ProcessResults();
		double tet = GetTotalTime();
		TestingFixture t3;
		remove("dedup_timeline.txt");
		remove("first_timeline.txt");
		remove("timing_info.txt");	
		BOOST_REQUIRE(t3.LaunchAndWait(boost::bind(TestProgramWithDuplicateTransfers, 0)) == 0);
		double tet_fixed = GetTotalTime();		
		std::cerr << "Iter=" << i << "," << "Exact=" << ret.first / 1000000000 << ",Min=" << ret.second / 1000000000 << ",TETDup=" << tet / 1000000000 << ",TET-Fixed=" << tet_fixed / 1000000000 << std::endl;
	}
}

BOOST_AUTO_TEST_SUITE_END()