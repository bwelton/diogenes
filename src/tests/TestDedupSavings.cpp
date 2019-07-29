#define BOOST_TEST_MODULE dedupsavings_test
#define BOOST_TEST_MAIN
#include "TestDedupSavings.h"

BOOST_AUTO_TEST_SUITE( test_dedupsavings )

BOOST_AUTO_TEST_CASE(TestReadTimeline) {
	std::vector<uint64_t> ids;
	std::vector<uint32_t> hashes; 
	std::vector<size_t> sizes; 
	std::vector<uint64_t> duplicates;
	std::vector<std::string> types; 

	// Generate the fake timeline
	std::string out_timeline = CreateFakeTimeline(10000, ids, hashes, sizes, types, duplicates);
	
	// Write the timeline to a file
	std::ofstream ofs ("ReadTimelineTest.txt", std::ofstream::out);
	ofs << out_timeline;
	ofs.close();

	// Have dedup savings read it back to us. 
	CalculateDedupSavings x("ReadTimelineTest.txt","BLANK","BLANK");
	std::vector<TimelineRec> records;
	x.ReadTimeline(records);
	uint64_t id, dupid;
	size_t size;
	for (int i = 0; i < records.size(); i++) {
		std::tie(id, size, dupid) = records[i];
		BOOST_CHECK_EQUAL(id, ids[i]);
		BOOST_CHECK_EQUAL(dupid, duplicates[i]);
		BOOST_CHECK_EQUAL(size, sizes[i]);
	}
}


BOOST_AUTO_TEST_CASE(TestReadCorrelationSimple) {
	std::vector<uint64_t> ids;
	std::vector<uint32_t> hashes; 
	std::vector<size_t> sizes; 
	std::vector<uint64_t> duplicates;
	std::vector<std::string> types; 

	// Generate the fake timeline
	std::string out_timeline = CreateFakeTimeline(10000, ids, hashes, sizes, types, duplicates);

	std::vector<uint64_t> streams;
	std::vector<uint64_t> procids;
	std::vector<uint64_t> threads;
	std::string ret = CreateFakeCorrelation(1,1,1,ids, sizes, streams, procids, threads);
	std::ofstream ofs ("ReadCorrelationTest.txt", std::ofstream::out);
	ofs << ret;
	ofs.close();
	
	CalculateDedupSavings x("BLANK","ReadCorrelationTest.txt","BLANK");
	std::vector<CorrelationRec> records;
	x.ReadCorrelation(records);

	uint64_t id, stream, procid, threadid;
	for (int i = 0;  i < records.size(); i++) {
		std::tie(id, stream, procid, threadid) = records[i];
		BOOST_CHECK_EQUAL(id, ids[i]);
		BOOST_CHECK_EQUAL(stream, streams[i]);
		BOOST_CHECK_EQUAL(threadid, threads[i]);		
		BOOST_CHECK_EQUAL(procid, procids[i]);
	}
}


BOOST_AUTO_TEST_CASE(TestReadCorrelationFourProcs) {
	std::vector<uint64_t> ids;
	std::vector<uint32_t> hashes; 
	std::vector<size_t> sizes; 
	std::vector<uint64_t> duplicates;
	std::vector<std::string> types; 

	// Generate the fake timeline
	std::string out_timeline = CreateFakeTimeline(10000, ids, hashes, sizes, types, duplicates);

	std::vector<uint64_t> streams;
	std::vector<uint64_t> procids;
	std::vector<uint64_t> threads;
	std::string ret = CreateFakeCorrelation(4,1,1,ids, sizes, streams, procids, threads);
	std::ofstream ofs ("ReadCorrelationTest.txt", std::ofstream::out);
	ofs << ret;
	ofs.close();
	
	CalculateDedupSavings x("BLANK","ReadCorrelationTest.txt","BLANK");
	std::vector<CorrelationRec> records;
	x.ReadCorrelation(records);

	uint64_t id, stream, procid, threadid;
	for (int i = 0;  i < records.size(); i++) {
		std::tie(id, stream, procid, threadid) = records[i];
		BOOST_CHECK_EQUAL(id, ids[i]);
		BOOST_CHECK_EQUAL(stream, streams[i]);
		BOOST_CHECK_EQUAL(threadid, threads[i]);		
		BOOST_CHECK_EQUAL(procid, procids[i]);
	}
}

BOOST_AUTO_TEST_CASE(TestReadCorrelationFourProcsFourStreams) {
	std::vector<uint64_t> ids;
	std::vector<uint32_t> hashes; 
	std::vector<size_t> sizes; 
	std::vector<uint64_t> duplicates;
	std::vector<std::string> types; 

	// Generate the fake timeline
	std::string out_timeline = CreateFakeTimeline(10000, ids, hashes, sizes, types, duplicates);

	std::vector<uint64_t> streams;
	std::vector<uint64_t> procids;
	std::vector<uint64_t> threads;
	std::string ret = CreateFakeCorrelation(4,1,4,ids, sizes, streams, procids, threads);
	std::ofstream ofs ("ReadCorrelationTest.txt", std::ofstream::out);
	ofs << ret;
	ofs.close();
	
	CalculateDedupSavings x("BLANK","ReadCorrelationTest.txt","BLANK");
	std::vector<CorrelationRec> records;
	x.ReadCorrelation(records);

	uint64_t id, stream, procid, threadid;
	for (int i = 0;  i < records.size(); i++) {
		std::tie(id, stream, procid, threadid) = records[i];
		BOOST_CHECK_EQUAL(id, ids[i]);
		BOOST_CHECK_EQUAL(stream, streams[i]);
		BOOST_CHECK_EQUAL(threadid, threads[i]);		
		BOOST_CHECK_EQUAL(procid, procids[i]);
	}
}


BOOST_AUTO_TEST_CASE(TestReadCorrelationFourProcsFourThreadsFourStreams) {
	std::vector<uint64_t> ids;
	std::vector<uint32_t> hashes; 
	std::vector<size_t> sizes; 
	std::vector<uint64_t> duplicates;
	std::vector<std::string> types; 

	// Generate the fake timeline
	std::string out_timeline = CreateFakeTimeline(10000, ids, hashes, sizes, types, duplicates);

	std::vector<uint64_t> streams;
	std::vector<uint64_t> procids;
	std::vector<uint64_t> threads;
	std::string ret = CreateFakeCorrelation(4,4,4,ids, sizes, streams, procids, threads);
	std::ofstream ofs ("ReadCorrelationTest.txt", std::ofstream::out);
	ofs << ret;
	ofs.close();
	
	CalculateDedupSavings x("BLANK","ReadCorrelationTest.txt","BLANK");
	std::vector<CorrelationRec> records;
	x.ReadCorrelation(records);

	uint64_t id, stream, procid, threadid;
	for (int i = 0;  i < records.size(); i++) {
		std::tie(id, stream, procid, threadid) = records[i];
		BOOST_CHECK_EQUAL(id, ids[i]);
		BOOST_CHECK_EQUAL(stream, streams[i]);
		BOOST_CHECK_EQUAL(threadid, threads[i]);		
		BOOST_CHECK_EQUAL(procid, procids[i]);
	}
}


BOOST_AUTO_TEST_CASE(TestReadTimingInfo) {
	std::vector<TimingRec> recs;
	std::vector<std::string> cnames;
	std::string ret = CreateFakeCUPTIRand(10000, recs, cnames);
	std::ofstream ofs ("ReadCUPTITest.txt", std::ofstream::out);
	ofs << ret;
	ofs.close();

	CalculateDedupSavings x("BLANK","BLANK","ReadCUPTITest.txt");
	std::vector<TimingRec> records;
	double finalTime;
	x.ReadTiming(records, finalTime);
	BOOST_CHECK_EQUAL(records.size(), recs.size());

	for(int i = 0; i < records.size(); i++) {
		int nameid = x.NameToId(cnames[i]);
		BOOST_CHECK(nameid != -1);
		std::get<2>(recs[i]) = nameid;
		if (recs[i] != records[i]) {
			std::cerr << "Record not equal "  << i << std::endl;
			std::cerr << PrintTimelineRec(recs[i]) << std::endl;
			std::cerr << PrintTimelineRec(records[i]) << std::endl;
			BOOST_FAIL("TestReadTimingInfo");
		}
	}
}


BOOST_AUTO_TEST_CASE(TestCombineTimelineCorrelation) {
	std::vector<uint64_t> ids;
	std::vector<uint32_t> hashes; 
	std::vector<size_t> sizes; 
	std::vector<uint64_t> duplicates;
	std::vector<std::string> types; 

	// Generate the fake timeline
	std::string out_timeline = CreateFakeTimeline(10000, ids, hashes, sizes, types, duplicates);
	{
		std::ofstream ofs ("ReadTimelineTest.txt", std::ofstream::out);
		ofs << out_timeline;
		ofs.close();
	}
	std::vector<uint64_t> streams;
	std::vector<uint64_t> procids;
	std::vector<uint64_t> threads;
	std::string ret = CreateFakeCorrelation(1,1,1,ids, sizes, streams, procids, threads);
	{
		std::ofstream ofs ("ReadCorrelationTest.txt", std::ofstream::out);
		ofs << ret;
		ofs.close();
	}
	// Have dedup savings read it back to us. 
	CalculateDedupSavings x("ReadTimelineTest.txt","ReadCorrelationTest.txt","BLANK");
	std::vector<TimelineRec> tlineRecords;
	x.ReadTimeline(tlineRecords);
	std::vector<CorrelationRec> records;
	x.ReadCorrelation(records);
	std::vector<CombinedRecord> output;
	x.CombineTimelineCorrelation(tlineRecords, records, output);
	BOOST_CHECK_EQUAL(output.size(), tlineRecords.size());
	BOOST_CHECK_EQUAL(output.size(), records.size());
	for(int i = 0; i < output.size(); i++) {
		CombinedRecord tmp = std::make_tuple(std::get<0>(tlineRecords[i]), std::get<1>(tlineRecords[i]), std::get<2>(tlineRecords[i]),
								std::get<1>(records[i]), std::get<2>(records[i]), std::get<3>(records[i]));
		if (tmp != output[i]){
			std::cerr << "Record not equal "  << i << std::endl;
			std::cerr << "Expected: " << PrintCombinedRecord(tmp) << std::endl;
			std::cerr << "Got: " << PrintCombinedRecord(output[i]) << std::endl;	
			BOOST_FAIL("TestCombineTimelineCorrelation");
		}
	}
}

BOOST_AUTO_TEST_CASE(TestCombineTimelineCorrelationFourProcsFourStreams) {
	std::vector<uint64_t> ids;
	std::vector<uint32_t> hashes; 
	std::vector<size_t> sizes; 
	std::vector<uint64_t> duplicates;
	std::vector<std::string> types; 

	// Generate the fake timeline
	std::string out_timeline = CreateFakeTimeline(10000, ids, hashes, sizes, types, duplicates);
	{
		std::ofstream ofs ("ReadTimelineTest.txt", std::ofstream::out);
		ofs << out_timeline;
		ofs.close();
	}
	std::vector<uint64_t> streams;
	std::vector<uint64_t> procids;
	std::vector<uint64_t> threads;
	std::string ret = CreateFakeCorrelation(4,4,1,ids, sizes, streams, procids, threads);
	{
		std::ofstream ofs ("ReadCorrelationTest.txt", std::ofstream::out);
		ofs << ret;
		ofs.close();
	}
	// Have dedup savings read it back to us. 
	CalculateDedupSavings x("ReadTimelineTest.txt","ReadCorrelationTest.txt","BLANK");
	std::vector<TimelineRec> tlineRecords;
	x.ReadTimeline(tlineRecords);
	std::vector<CorrelationRec> records;
	x.ReadCorrelation(records);
	std::vector<CombinedRecord> output;
	x.CombineTimelineCorrelation(tlineRecords, records, output);
	BOOST_CHECK_EQUAL(output.size(), tlineRecords.size());
	BOOST_CHECK_EQUAL(output.size(), records.size());
	for(int i = 0; i < output.size(); i++) {
		CombinedRecord tmp = std::make_tuple(std::get<0>(tlineRecords[i]), std::get<1>(tlineRecords[i]), std::get<2>(tlineRecords[i]),
								std::get<1>(records[i]), std::get<2>(records[i]), std::get<3>(records[i]));
		if (tmp != output[i]){
			std::cerr << "Record not equal "  << i << std::endl;
			std::cerr << "Expected: " << PrintCombinedRecord(tmp) << std::endl;
			std::cerr << "Got: " << PrintCombinedRecord(output[i]) << std::endl;
			BOOST_FAIL("TestCombineTimelineCorrelationFourProcsFourStreams");
		}
	}
}

BOOST_AUTO_TEST_CASE(TestGenerateCUDAProcesses) {
	std::vector<TimingRec> recs;
	std::vector<std::string> cnames;
	std::vector<CUDAProcess> procs;
	std::map<uint64_t, CUPTIRecord> c_records;
	std::map<uint64_t, CUPTIRecord> rc_records;
	std::vector<TimingRec> records;
	double finalTime;
	uint64_t corrid, start_time, end_time, procid, threadid, size, stream;
	uint64_t cputime, gputime;
	uint32_t type_key, cname_key;
	int runcorr, ctx, dev;

	std::string ret = CreateFakeCUPTIRand(10000, recs, cnames);
	std::ofstream ofs ("ReadCUPTITest.txt", std::ofstream::out);
	ofs << ret;
	ofs.close();

	CalculateDedupSavings x("BLANK","BLANK","ReadCUPTITest.txt");

	x.ReadTiming(records, finalTime);
	BOOST_CHECK_EQUAL(records.size(), recs.size());	

	for(auto i : records) {
		std::tie(corrid, type_key, cname_key, start_time, end_time, procid, threadid, size, runcorr, ctx, dev, stream) = i;
		if (c_records.find(corrid) == c_records.end())
			c_records[corrid] = std::make_tuple(0,0,0,0,0,0,0,0,0,0,0,0);
		if (type_key == 1 || type_key == 2){
			std::get<0>(c_records[corrid]) = corrid;
			std::get<1>(c_records[corrid]) = cname_key;
			std::get<4>(c_records[corrid]) = std::get<4>(c_records[corrid]) +  std::max(int(end_time) - int(start_time), 0);
			std::get<5>(c_records[corrid]) = procid;
			std::get<6>(c_records[corrid]) = threadid;
		} else if (type_key == 3){
			std::get<0>(c_records[corrid]) = corrid;
			std::get<2>(c_records[corrid]) = cname_key;
			std::get<3>(c_records[corrid]) = std::get<3>(c_records[corrid]) +  std::max(int(end_time) - int(start_time), 0);
			std::get<7>(c_records[corrid]) = size;
			std::get<8>(c_records[corrid]) = runcorr;
			std::get<9>(c_records[corrid]) = ctx;
			std::get<10>(c_records[corrid]) = dev;
			std::get<11>(c_records[corrid]) = stream;
		}
	}

	x.GenerateCUDAProcesses(records, procs, rc_records);
	BOOST_CHECK_EQUAL(rc_records.size(), c_records.size());
	for(auto i : c_records) {
		bool found = false;
		for (auto t : rc_records) {
			if (i.first == t.first && i.second == t.second) {
				found = true;
				break;
			}
		}

		BOOST_CHECK_EQUAL(found, true);
		if (found != true) {
			std::cerr << "Could not find entry: " << i.first << " " << PrintCUPTIRecord(i.second) << std::endl;
			std::cerr << "Closest Matches: " << std::endl;
			for(auto t : rc_records) {
				if (i.second == t.second || i.first ==  t.first) {
					std::cerr << "REC: " << t.first << " " << PrintCUPTIRecord(t.second) << std::endl;
				} 
			}
		}
	}

	// for (auto p : procs) {
	// 	std::cerr << "CUPTI Process Element "  << p.procid << " " << p.threadid << std::endl;
	// 	for(auto i : p.records)
	// 		std::cerr << PrintCUPTIRecord(i) << std::endl;
	// }

	for (auto i : c_records) {
		bool found = false;
		uint64_t record_proc = std::get<5>(i.second);
		uint64_t record_thread = std::get<6>(i.second);
		std::vector<CUDAProcess> matchingProcs;
		for (auto p : procs) {
			if (record_proc == p.procid && record_thread == p.threadid) {
				matchingProcs.push_back(p);
			}
		}
		std::cerr << record_proc << " " << record_thread << " " << PrintCUPTIRecord(i.second) << std::endl;
		BOOST_CHECK_EQUAL(matchingProcs.size(), 1);
		if (matchingProcs.size() > 1) {
			for (auto p : matchingProcs)
				std::cerr << "Proc Matches - " << p.procid << " " << p.threadid << std::endl;
		}
		for (auto p : matchingProcs) {
			for(auto z : p.records) {
				if (z == i.second) 
					found = true;
			}
		}
		BOOST_CHECK_EQUAL(found, true);
		if (found == false) {
			for (auto p : matchingProcs) {
				for(auto z : p.records) {
						std::cerr << PrintCUPTIRecord(z) << std::endl;
						std::cerr << PrintCUPTIRecord(i.second) << std::endl;
				}
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(TestNormalizeProcessIDs) {
	std::vector<uint64_t> ids;
	std::vector<uint32_t> hashes; 
	std::vector<size_t> sizes; 
	std::vector<uint64_t> duplicates;
	std::vector<std::string> types; 

	// Generate the fake timeline
	std::string out_timeline = CreateFakeTimeline(10000, ids, hashes, sizes, types, duplicates);
	{
		std::ofstream ofs ("ReadTimelineTest.txt", std::ofstream::out);
		ofs << out_timeline;
		ofs.close();
	}
	std::vector<uint64_t> streams;
	std::vector<uint64_t> procids;
	std::vector<uint64_t> threads;
	std::string ret = CreateFakeCorrelation(4,4,1,ids, sizes, streams, procids, threads);
	{
		std::ofstream ofs ("ReadCorrelationTest.txt", std::ofstream::out);
		ofs << ret;
		ofs.close();
	}
	std::vector<TimingRec> crecords;
	std::vector<std::string> cnames;
	std::string cuptiRecords = CreateFakeCUPTI(10000, crecords, ids, procids, threads, sizes, cnames);
	{
		std::ofstream ofs ("ReadCUPTITest.txt", std::ofstream::out);
		ofs << cuptiRecords;
		ofs.close();
	}	

	CalculateDedupSavings x("ReadTimelineTest.txt","ReadCorrelationTest.txt","ReadCUPTITest.txt");
	std::vector<CUDAProcess> procs;
	std::vector<TimelineRec> tlineRecords;
	std::vector<CorrelationRec> records;
	std::vector<CombinedRecord> output;
	std::vector<TimingRec> timingRec;
	std::map<uint64_t, CUPTIRecord> rc_records;

	double finalTime = 0.0;

	x.ReadTimeline(tlineRecords);
	x.ReadCorrelation(records);
	x.CombineTimelineCorrelation(tlineRecords, records, output);
	x.ReadTiming(timingRec, finalTime);	
	x.GenerateCUDAProcesses(timingRec, procs, rc_records);

	std::map<uint64_t, CombinedRecord> recordMap;
	for (auto i : output) {
		recordMap[std::get<0>(i)] = i;
	}
	x.NormalizeProcessIDs(output, procs);
	for(auto i : procs) {
		BOOST_CHECK_EQUAL(i.matched, true);
		for (auto t : i.transferRecords) {
			if (recordMap.find(std::get<0>(t)) != recordMap.end())
				recordMap.erase(std::get<0>(t));
		}
	}
	BOOST_CHECK_EQUAL(recordMap.size(),0);
}

BOOST_AUTO_TEST_SUITE_END()