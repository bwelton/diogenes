#pragma once
#define BOOST_TEST_DYN_LINK
#include "CalculateDedupSavings.h"
#include <boost/test/unit_test.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <string>       
#include <iostream>     
#include <sstream>      
#include <time.h>

std::string PrintTimelineRec(TimingRec & rec){
	std::stringstream ss;	
	ss << std::get<0>(rec) << "," << std::get<1>(rec) << "," << std::get<2>(rec) << ","
	   << std::get<3>(rec) << "," << std::get<4>(rec) << "," << std::get<5>(rec) << ","
	   << std::get<6>(rec) << "," << std::get<7>(rec) << "," << std::get<8>(rec) << ","
	   << std::get<9>(rec) << "," << std::get<10>(rec) << "," << std::get<11>(rec);
	return ss.str();
}

std::string PrintCombinedRecord(CombinedRecord & rec) {
	std::stringstream ss;	
	ss << std::get<0>(rec) << "," << std::get<1>(rec) << "," << std::get<2>(rec) << ","
	   << std::get<3>(rec) << "," << std::get<4>(rec) << "," << std::get<5>(rec);
	return ss.str();
}

std::string PrintCUPTIRecord(CUPTIRecord & rec){
	std::stringstream ss;	
	ss << std::get<0>(rec) << "," << std::get<1>(rec) << "," << std::get<2>(rec) << ","
	   << std::get<3>(rec) << "," << std::get<4>(rec) << "," << std::get<5>(rec) << ","
	   << std::get<6>(rec) << "," << std::get<7>(rec) << "," << std::get<8>(rec) << ","
	   << std::get<9>(rec) << "," << std::get<10>(rec) << "," << std::get<11>(rec);
	return ss.str();
}
std::string CreateFakeTimeline(size_t elements, std::vector<uint64_t> & ids, 
							   std::vector<uint32_t> & hashes, std::vector<size_t> & sizes, 
							   std::vector<std::string> & types, std::vector<uint64_t> & duplicates) {
	std::stringstream ss;
	int duplicate;
	for (int i = 0; i < elements; i++) {
		ids.push_back(i+1);
		hashes.push_back(rand()%10000000);
		sizes.push_back(rand()%10000000);
		types.push_back(((rand() % 2 == 0) ? std::string("DeviceToHost") : std::string("HostToDevice")));
		if (rand() % 2 == 0 || i < 2){
			duplicates.push_back(0);
		} else {
			duplicates.push_back((rand() % (i - 1)) + 1);
		}
		ss << ids[i] << "," << hashes[i] << "," << sizes[i] << "," << types[i] << "," << duplicates[i] << std::endl;
	}
	return ss.str();
}

std::string CreateFakeCorrelation(size_t numProcs, size_t numThreads, size_t numStreams, std::vector<uint64_t> ids, 
		std::vector<size_t> sizes, std::vector<uint64_t> & streams, std::vector<uint64_t> & procid, std::vector<uint64_t> & threadid) {
	std::vector<uint64_t> procs;
	std::vector<uint64_t> threads;
	std::vector<uint64_t> nstreams;
	std::stringstream ss;
	for(int i = 0; i < numProcs; i++){
		procs.push_back((rand() % 20000) + 200);
	}
	for(int i = 0; i < numThreads; i++){
		threads.push_back((rand() % 50000) + 500000);
	}
	for(int i = 0; i < numStreams; i++){
		nstreams.push_back((rand() % 200)+1);
	}
	for (auto i : sizes){
		streams.push_back(nstreams[rand() % nstreams.size()]);
		threadid.push_back(threads[rand() % threads.size()]);
		procid.push_back(procs[rand() % procs.size()]);
	}
	for (int i = 0; i < ids.size(); i++) {
		ss << ids[i] << "," << sizes[i] << "," << streams[i] << "," << procid[i] << "," << threadid[i] << std::endl;
	}
	return ss.str();
}

std::string CreateFakeCUPTI(size_t count, std::vector<TimingRec> & recs, std::vector<uint64_t> ids, std::vector<uint64_t> & procid, std::vector<uint64_t> & threadid, std::vector<size_t> & sizes, std::vector<std::string> & cnames) {
	std::vector<std::string> types = {std::string("NONO"), std::string("RR"), std::string("DR"), std::string("CPY")};
	std::string randomChars = std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	std::stringstream ss;
	TimingRec rec;
	std::map<uint64_t, uint64_t> _procToGPUId;
	std::map<uint64_t, uint64_t> _threadToGPUId;
	uint64_t current = 6500;
	for(auto i : procid){
		_procToGPUId[i] =  current;
		current++;
	}
	for(auto i : threadid){
		_threadToGPUId[i] =  current;
		current++;
	}
	for (uint64_t i = 0; i < count; i++) {
		rec = std::make_tuple(i+1, 1, rand() % 25 + 1, rand() % 26000 + 1, rand() % 26000 + 4, 
			_procToGPUId[procid[i]], _threadToGPUId[threadid[i]], sizes[i], rand() % 25, rand() % 30, rand() % 20, rand() % 50);
		uint64_t type = std::get<1>(rec);
		std::string name;
		for(uint32_t q = 0; q < std::get<2>(rec); q++) {
			name = name + randomChars[rand() % (randomChars.size() - 1)];
		}
		cnames.push_back(name);
		if (type == 1 || type == 2) {
			ss << types[type] << "," << name << "," << std::get<0>(rec) << "," << std::get<3>(rec) << "," << std::get<4>(rec) << "," << std::get<5>(rec) << "," << std::get<6>(rec) << std::endl;
			std::get<2>(rec) = 0; 
			std::get<7>(rec) = 0; 
			std::get<8>(rec) = 0; 
			std::get<9>(rec) = 0; 
			std::get<10>(rec) = 0; 
			std::get<11>(rec) = 0;
		} else if (type == 3) {
			ss << types[type] << "," 
			   << name << "," 
			   << std::get<0>(rec) 
			   << ","
			    << std::get<3>(rec) << "," 
			    << std::get<4>(rec) << "," 
			    << std::get<7>(rec) << "," 
			    << std::get<8>(rec) << "," 
			    << std::get<9>(rec) << "," << std::get<10>(rec) 
			    << "," << std::get<11>(rec) 
			    << std::endl;
			std::get<2>(rec) = 0;
		    std::get<5>(rec) = 0;
		    std::get<6>(rec) = 0;
		} else {
			assert("WE SHOULD NOT BE HERE" == 0);
		}
		recs.push_back(rec);
	}	
	ss << "TET,931\n";
	return ss.str();
}

std::string CreateFakeCUPTIRand(size_t count, std::vector<TimingRec> & recs, std::vector<std::string> & cnames) {
	std::vector<std::string> types = {std::string("NONO"), std::string("RR"), std::string("DR"), std::string("CPY")};
	std::string randomChars = std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	std::stringstream ss;
	TimingRec rec;
	for (uint64_t i = 0; i < count; i++) {
		rec = std::make_tuple(i+1, rand() % 3+1, rand() % 25 + 1, rand() % 26000 + 1, rand() % 26000 + 4, 
			rand() % 20000 + 1, rand() % 2000000 + 1, rand() % 20000 + 1, rand() % 25, rand() % 30, rand() % 20, rand() % 50);
		uint64_t type = std::get<1>(rec);
		std::string name;
		for(uint32_t q = 0; q < std::get<2>(rec); q++) {
			name = name + randomChars[rand() % (randomChars.size() - 1)];
		}
		cnames.push_back(name);
		if (type == 1 || type == 2) {
			ss << types[type] << "," << name << "," << std::get<0>(rec) << "," << std::get<3>(rec) << "," << std::get<4>(rec) << "," << std::get<5>(rec) << "," << std::get<6>(rec) << std::endl;
			std::get<2>(rec) = 0; 
			std::get<7>(rec) = 0; 
			std::get<8>(rec) = 0; 
			std::get<9>(rec) = 0; 
			std::get<10>(rec) = 0; 
			std::get<11>(rec) = 0;
		} else if (type == 3) {
			ss << types[type] << "," 
			   << name << "," 
			   << std::get<0>(rec) 
			   << ","
			    << std::get<3>(rec) << "," 
			    << std::get<4>(rec) << "," 
			    << std::get<7>(rec) << "," 
			    << std::get<8>(rec) << "," 
			    << std::get<9>(rec) << "," << std::get<10>(rec) 
			    << "," << std::get<11>(rec) 
			    << std::endl;
			std::get<2>(rec) = 0;
		    std::get<5>(rec) = 0;
		    std::get<6>(rec) = 0;
		} else {
			assert("WE SHOULD NOT BE HERE" == 0);
		}
		recs.push_back(rec);
	}
	ss << "TET,931\n";

	return ss.str();
	// std::map<uint64_t, uint64_t> cpuToGPUStreamMap;
	// std::map<uint64_t, uint64_t> cpuToGPUProcMap;
	// std::map<uint64_t, uint64_t> cpuToGPUThreadMap;
	// 	    //         coorID,   id,        size,    stream,     proc,  thread
	// typedef std::tuple<uint64_t, uint64_t, size_t, uint64_t, uint64_t, uint64_t> CUPTITransfer;

	// std::vector<CUPTITransfer> cuTransfers;
	// uint64_t currentCorrId = rand() % 10000 + 1;
	// for (int i = 0; i < transIds.size(); i++) {
	// 	if (cpuTo)
	// }

	// Begin making records.....
	// Start by making 
	
}



