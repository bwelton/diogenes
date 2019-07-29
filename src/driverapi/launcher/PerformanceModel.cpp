#include "PerformanceModel.h"

//#define DEBUG_MODEL 1
PerformanceModel::PerformanceModel() : _fastestExecTime(std::numeric_limits<double>::max()), _totalSyncs(0) {

}

void PerformanceModel::AddExecutionTime(double secs) {
	if (secs < _fastestExecTime) {
		std::cerr << "[PerfModel] Updating lowest execution time - " << _fastestExecTime << "=>" << secs << std::endl;
		_fastestExecTime = secs;
		std::ofstream tmp("PM_totaltime.txt", std::ofstream::out);
		tmp << secs << std::endl;
		tmp.close();
	}
}

void PerformanceModel::AddFirstUses(std::map<uint64_t, StackPoint> uses) {
	_firstUses = uses;
}

bool PerformanceModel::FindElement(uint64_t genId, uint64_t & startPos, std::vector<TimingData> & array) {
	for (uint64_t i = startPos; i < array.size(); i++){
		if(array[i].genId == genId){
			startPos = i;
			return true;
		}
	}
	return false;
}

std::map<uint64_t,uint64_t> PerformanceModel::MatchStackTraces(std::map<uint64_t, StackRecord> & a, std::map<uint64_t, StackRecord> & b) {
	std::map<uint64_t,uint64_t> ret;
	for (auto outer : a) {
		for (auto inner : b) {
			if (outer.second.IsEqual(inner.second)){
				ret[outer.first] = inner.first;
				break;
			}
		}
	}
	return ret;
}

void PerformanceModel::CaptureSyncTime() {

	std::cerr << "[PerformanceModel] In capture sync time" << std::endl;
	ReadTimingStacks(std::string("TF_timekey.bin"),std::string("TF_trace.bin"));

	// Because for timing we instrument the entries into libcuda, 
	// we do not get the correct function names at those locations. We must
	// find out what those functions should be and replace the stack positions
	// with the correct names. 
	for (auto i : _stackToDynId) {
		uint64_t stackIDTiming = i.first;
		uint64_t genericID = i.second;
		if (stackIDTiming == 0 || genericID == 0){
			std::cerr << "[PerformanceModel] Skipping a zero ID - " << stackIDTiming << " - " << genericID << std::endl; 
			continue;
		}
		
		std::string tmp = _callMapper.GeneralToName(genericID);
		std::cerr << "[PerformanceModel] Changing timing stackid " << stackIDTiming << " to have libcuda entry at " << tmp << std::endl;
		if (_timingStackRecords.find(stackIDTiming) == _timingStackRecords.end()){
			std::cerr << "[PerformanceModel] Could not find timing stack record with id " << stackIDTiming << " continuing " << std::endl;
			continue;
		}

		uint64_t cudaPos = _timingStackRecords[stackIDTiming].GetFirstLibDynRTPosition();
		std::cerr << "[PerformanceModel] For stack record " << stackIDTiming << " first cuda position is " << cudaPos << std::endl;
		if (cudaPos != 0){
			uint64_t tmpStackID = _callMapper.GeneralToStackID(genericID);
			if (tmpStackID == 0){
				std::cerr << "[PerformanceModel] Cannot insert new cuda call into timing trace " << std::endl;
				continue;
			}
			_timingStackRecords[stackIDTiming].ChangePointAtPosition(_stackRecords[tmpStackID].GetFirstCudaCall(), cudaPos);
			//_timingStackRecords[stackIDTiming].PrintStack();
		}
	}

	StackKeyWriter outKey(fopen("TF_timekey.txt","wb"));
	for (auto i : _timingStackRecords) {
		//i.second.PrintStack();
		std::vector<StackPoint> tmp_map = i.second.GetStackpoints();
		outKey.InsertStack(i.first, tmp_map);
	}



	// std::cerr << "[PerformanceModel] Timing stacks after correction" << std::endl;
	// for (auto i : _timingStackRecords) 
	// 	i.second.PrintStack();	
	
	// // Find Identical Stacks. 
	// std::map<uint64_t,uint64_t> s = MatchStackTraces(_timingStackRecords, _stackRecords);

	// // Check for stacks that were not found matching...
	// std::set<uint64_t> timeStackIDs;
	// std::set<uint64_t> stackRecordIDs;

	// for (auto i : _timingStackRecords)
	// 	timeStackIDs.insert(i.first);
	// for (auto i : _stackRecords) 
	// 	stackRecordIDs.insert(i.first);

	// for (auto i : s) {
	// 	std::cerr << "[PerformanceModel] Timing Stack of " << i.first << " matches with stack record " << i.second << std::endl;
	// 	timeStackIDs.erase(i.first);
	// 	stackRecordIDs.erase(i.first);
	// }
	
	// for (auto i : timeStackIDs)
	// 	std::cerr << "[PerformanceModel] Error: Could not find timing stack that matched - " << i << std::endl;

	// for (auto i : stackRecordIDs) 
	// 	std::cerr << "[PerformanceModel] Error: Could not find stack record that matched - " << i << std::endl;

	// if (timeStackIDs.size() == stackRecordIDs.size())
	// 	std::cerr << "[PerformanceModel] Backtraces match between timing and stack record ids" << std::endl;

	// std::cerr << "[PerformanceModel] Checking if stacks line up " << std::endl;

	// std::map<uint64_t, std::vector<uint64_t> > timingBins;
	// assert(_timingData.size() > 1);

	// for(int64_t i = _timingData.size() - 1; i >= 0; i = i - 1) {
	// 	if (timingBins.find(_timingData[i].stackId) ==timingBins.end())
	// 		timingBins[_timingData[i].stackId] = std::vector<uint64_t>();
	// 	timingBins[_timingData[i].stackId].push_back(i);
	// }

	// std::map<uint64_t,uint64_t> stackToTiming = MatchStackTraces(_stackRecords, _timingStackRecords);
	// uint64_t notFound = 0;
	// for (uint64_t i = 0; i < _orderingInfo.size(); i++) {
	// 	if (stackToTiming.find(i) != stackToTiming.end()){
	// 		if (timingBins.find(stackToTiming[i]) != timingBins.end()){
	// 			if (timingBins[stackToTiming[i]].size() != 0){
	// 				_timingData[timingBins[stackToTiming[i]].back()].count--;
	// 				if (_timingData[timingBins[stackToTiming[i]].back()].count == 0)
	// 					timingBins[stackToTiming[i]].pop_back();
	// 				continue;
	// 			}
	// 		}
	// 	}
	// 	notFound++;
	// }

	//std::cerr << "[PerformanceModel] Number of elements in ordering info not found - " << notFound << std::endl;

	//_callMapper.GeneralToName()
	// FILE * inFile = fopen("callDelay.out","rb");
	// assert(inFile != NULL);

	// fseek(inFile, 0, SEEK_END);
	// uint64_t elementCount = ftell(inFile);
	// fseek(inFile, 0, SEEK_SET);

	// elementCount = elementCount / (sizeof(uint64_t) + sizeof(double) + sizeof(uint64_t));
	// _timingData = std::vector<TimingData>(elementCount);

	// uint64_t id;
	// double time; 
	// uint64_t count;
	// uint64_t total = 0;
	// for (int i = 0; i < elementCount; i++) {
	// 	fread(&id, 1, sizeof(uint64_t), inFile);
	// 	fread(&time, 1, sizeof(double), inFile);
	// 	fread(&count, 1, sizeof(uint64_t), inFile); 
	// 	_timingData[i].genId = id;
	// 	_timingData[i].time = time;
	// 	_timingData[i].count = count;
	// 	//std::cerr << id << "," << time << "," << count << std::endl;
	// 	total += count;
	// }
	// std::cerr << "[PerformanceModel] Total Synchronization Count: " << total << " Expecting: " << _capturedSyncs << std::endl;
	// std::cerr << "[PerformanceModel] Checking for errors" << std::endl;

	// // std::cerr << "[PerformanceModel] Decoded Ordering Info: "  << std::endl;
	// // for (auto i = 0; i < _orderingInfo.size(); i++) {
	// // 	std::cerr << "[PerformanceModel]\t Pos: " << i << " GenID: " << _orderingInfo[i].genId << " StackID: " << _orderingInfo[i].stackId <<  " Call Count: " << _orderingInfo[i].count << " CallName: " << _callMapper.GeneralToName(_orderingInfo[i].genId) << std::endl;
	// // }

	// // std::cerr << "[PerformanceModel] Decoded Timing Info: "  << std::endl;
	// // for (auto i = 0; i < _timingData.size(); i++) {
	// // 	std::cerr << "[PerformanceModel]\t Pos: " << i << " GenID: " << _timingData[i].genId << " StackID: " << _timingData[i].stackId <<  " Call Count: " << _timingData[i].count <<" CallName: " << _callMapper.GeneralToName(_timingData[i].genId) << std::endl;		
	// // }

	// std::map<uint64_t, std::vector<uint64_t> > orderInfo;
	// std::map<uint64_t, std::vector<uint64_t> > timingInfo;
	// for (uint64_t z = 0; z < _timingData.size(); z++){
	// 	auto i = _timingData[z];
	// 	if (timingInfo.find(i.genId) == timingInfo.end()) {
	// 		timingInfo[i.genId] = std::vector<uint64_t>();
	// 	}
	// 	timingInfo[i.genId].push_back(z);
	// }

	// for (uint64_t z = 0; z < _orderingInfo.size(); z++){
	// 	auto i = _orderingInfo[z];
	// 	if (orderInfo.find(i.genId) == orderInfo.end()) {
	// 		orderInfo[i.genId] = std::vector<uint64_t>();
	// 	}
	// 	orderInfo[i.genId].push_back(z);
	// }

	// for (auto i : timingInfo) {
	// 	if (orderInfo.find(i.first) == orderInfo.end()) {
	// 		std::cerr << "[PerformanceModel] Ordering Info is missing element " << i.first << std::endl;
	// 	} else {
	// 		if (i.second.size() != orderInfo[i.first].size()) {
	// 			std::cerr << "[PerformanceModel] Timing model at GenID " << i.first << " does not match OrderInfo size" << std::endl;
	// 			std::cerr << "[PerformanceModel]\t Timing Info Size: " << i.second.size() << " Order Info Size: " << orderInfo[i.first].size() << std::endl;
	// 		}
	// 		uint64_t pOff = 0;
	// 		bool correct = true;
	// 		for (auto n : i.second) {
	// 			if (n != orderInfo[i.first][pOff]){
	// 				std::cerr << "[PerformanceModel] Timing and Order Info differ at array offset " << pOff << " element position " << n <<  " for genid " << i.first << std::endl;
	// 				correct = false;
	// 				break;
	// 			}
	// 			pOff++;
	// 			if (orderInfo[i.first].size() == pOff)
	// 				break;
	// 		}
	// 		// Secondary check to see if the arrays are shifted by a common offset
	// 		if (correct == false) {
	// 			std::set<uint64_t> findOffsets;
	// 			pOff = 0;
	// 			for (auto n : i.second) {
	// 				findOffsets.insert((n > orderInfo[i.first][pOff]) ? (n - orderInfo[i.first][pOff]): (orderInfo[i.first][pOff] - n));
	// 				pOff++;
	// 				if (orderInfo[i.first].size() == pOff)
	// 					break;					
	// 			}
	// 			if (findOffsets.size() == 1)
	// 				std::cerr << "[PerformanceModel] GenID " << i.first << " has consistent offset pattern" << std::endl;
	// 			else
	// 				std::cerr << "[PerformanceModel] GenID " << i.first << " has inconsistent offset pattern" << std::endl;
	// 		}
	// 	}
	// }


	// uint64_t correctCount = 0;
	// uint64_t errorCount = 0;
	// uint64_t timingStart = 0;
	// uint64_t stackStart = 0;
	// bool finished = false;
	// do {
	// 	// If the current positions for each match, check sizes.
	// 	// If the sizes match, advance both pointers.
	// 	if (_timingData[timingStart].genId == _orderingInfo[stackStart].genId || _orderingInfo[stackStart].genId == 0 || _timingData[timingStart].genId == 0 ) {
	// 		correctCount++;
	// 		// If equal, advance both counters
	// 		if(_timingData[timingStart].count == _orderingInfo[stackStart].count) {
	// 			timingStart++;
	// 			stackStart++;
	// 		// If not equal, advance only stack start.
	// 		} else if (_timingData[timingStart].count > _orderingInfo[stackStart].count) {
	// 			_timingData[timingStart].count -= _orderingInfo[stackStart].count;
	// 			stackStart++;
	// 		} else if (_timingData[timingStart].count < _orderingInfo[stackStart].count) {
	// 			_orderingInfo[stackStart].count -= _timingData[timingStart].count;
	// 			timingStart++;
	// 		}
	// 	} else {
	// 		errorCount++;
	// 		std::cerr << "[PerformanceModel] Check Error - Missmatch between " << stackStart << " and " << timingStart << std::endl;
	// 		std::cerr << "[PerformanceModel] Timing ID: " << _timingData[timingStart].genId << " StackID: " << _orderingInfo[stackStart].genId << std::endl;
	// 		// We have a missmatch, in this case find the nearest element in either list and adjust the stack postion to that
	// 		// location.
	// 		uint64_t tmpStack = stackStart;
	// 		uint64_t tmptiming = timingStart;
	// 		if (FindElement(_timingData[timingStart].genId, tmpStack, _orderingInfo)){
	// 			if(FindElement(_orderingInfo[stackStart].genId, tmptiming, _timingData)){
	// 				if (tmptiming - timingStart == tmpStack - stackStart){
	// 					//stackStart = tmpStack;
	// 					timingStart = tmptiming;
	// 				} else if (tmptiming - timingStart > tmpStack - stackStart){
	// 					stackStart = tmpStack;
	// 				} else {
	// 					timingStart = tmptiming;
	// 				}
	// 			} else {
	// 				stackStart = tmpStack;
	// 				//break;
	// 			}
	// 		} else {
	// 			break;
	// 		}
	// 	}
	// 	if (stackStart >= _orderingInfo.size() || timingStart >= _timingData.size())
	// 		break;
	// } while(finished != true);

	// std::cerr << "[Performance Model] Errors/Correct Stack Identifications - " << errorCount << " / " << correctCount << std::endl;

	// if (total == _orderingInfo.size()) {
	// 	// We have an exact match, we can relate 1 to 1 observed with stacks.
	// 	uint64_t curPos = 0;
	// 	uint64_t remaining = _timingData[0].count;
	// 	uint64_t curId = _timingData[0].genId;
	// 	for (uint64_t i = 0; i < _orderingInfo.size(); i++) {
	// 		if (remaining == 0){
	// 			curPos++;
	// 			if (curPos == _timingData.size())
	// 				break;
	// 			remaining = _timingData[curPos].count;
	// 			curId = _timingData[curPos].genId;
	// 		}

	// 		uint64_t orderGen = _callMapper.StackIDToGeneral(_orderingInfo[i]);
	// 		if (curId == orderGen && remaining > 0) {
	// 			remaining = remaining - 1;
	// 			_timingData[curPos].stackId = _orderingInfo[i];
	// 		} else if (curId == 0) {
	// 			_timingData[curPos].genId = orderGen;
	// 			_timingData[curPos].stackId = _orderingInfo[i];
	// 			remaining = _timingData[curPos].count - 1;
	// 			curId = orderGen;
	// 		} else if (orderGen == 0) {
	// 			remaining = remaining - 1;
	// 		} else if (orderGen != curId) {
	// 			std::cout << "[PerformanceModel] Missmatch between timing and stack data at position - Timing Data: " << curPos << " and Order Data: " <<  i << std::endl;
	// 			std::cout << "[PerformanceModel] Ordering Info - ID: " << _orderingInfo[i] << " GenID: " << orderGen << std::endl;
	// 			std::cout << "[PerformanceModel] Timing Info - ID: " << _timingData[curPos].genId << " GenID: " << _timingData[curPos].count << std::endl;
	// 			remaining = remaining - 1;
	// 		}
	// 	}
	// } else {
	// 	std::cout << "[PerformanceModel] Error - Timing and Stack models differ, this will cause issues with output. Cannot provide correlation between stacktraces and time saved" << std::endl;
	// }

	// Set the real id's for the timing data calls

	// // Manually check the first position
	// uint64_t curPos = 0;
	// for (uint64_t i = 0; i < _orderingInfo.size(); i++) {

	// }
	// uint64_t curPos = 0;
	// for (uint64_t i = 0; i < _timingData.size(); i++) {
	// 	uint64_t prevPos = curPos;
	// 	uint64_t genId = _timingData[i].genId;
	// 	uint64_t remaining = _timingData[i].count;
	// 	int currentId = -1;

	// 	for (uint64_t z = curPos; z < _orderingInfo.size(); z++) {
	// 		if (currentId == -1) {
	// 			currentId = _orderingInfo[z];
	// 			if (_callMapper.StackIDToGeneral(_orderingInfo[z]) != genId){
	// 				break;
	// 			}
	// 		}
	// 		if (currentId != _orderingInfo[z])
	// 			break;
	// 		remaining = remaining - 1;
	// 		curPos++;
	// 		if (remaining == 0)
	// 			break;
	// 	}
	// 	if (remaining != _timingData[i].count && currentId != -1) {
	// 		_timingData[i].stackId = currentId;
	// 	}  
	// 	if (remaining != 0) {
	// 		std::cout << "[PerformanceModel] ERROR - Could not identify all synchronizations at position " << z << " with count " << _timingData[i].count << " have " << remaining << " unsatisfied " << std::endl;
	// 	}

	// }

	// std::map<std::string, uint64_t> callNameToId;
	// for (auto i : _stackRecords) {
	// 	if (i.first != 0) {
	// 		callNameToId[i.second.GetFirstCudaCall().funcName] = i.first;
	// 	}
	// }
	// callNameToId[std::string("unknown")] = 0;

 	// std::ifstream ifs ("callDelay.out", std::ifstream::in);
 	// std::string line;
 	// uint64_t pos = 0;
 	// uint64_t nMapId = 0;

  //  	while (std::getline(ifs, line)) {
  // 		std::vector<std::string> params;
  // 		std::stringstream ss(line);
  // 		while(ss.good()){
  // 			std::string substr;
  // 			getline( ss, substr, ',' );
  // 			params.push_back(substr);
  // 		}
  // 		assert(params.size() == 3);
  // 		if (params[0][0] == ' ')
  // 			params[0].erase(0,1);

  // 		if(callNameToId.find(params[0]) == callNameToId.end()){
  // 			std::cout << "[PerformanceModel] ERROR Could not identify call " << params[0] << " at position " << pos << std::endl;
  // 		} else {
  // 			_stackRecords[callNameToId[params[0]]].AddTimingData(pos,uint64_t(std::stoi(params[2])), ::atof(params[1].c_str()));
  // 		}
  // 		pos += uint64_t(std::stoi(params[2]));
  // 	}
//   	// Temporary fix for failure to capture first synchronization.
//   	_callPoints[0].syncCount = _callPoints[0].syncCount;
// #ifdef DEBUG_MODEL
//   	std::cerr << "Call info read from callDelay file" << std::endl;
//   	for (auto i : _callPoints)
//   		std::cerr << i.libCudaCallname << "," << i.time << "," << i.syncCount << std::endl;
// #endif

//   	// Check against the total number of syncs we have captured and their stack traces
//   	uint64_t orderPos = 0;
//   	for (auto & i : _callPoints) {
//   		int count = i.syncCount;
//   		i.startPos = orderPos;
//   		for (int n = orderPos; n < _stackOrder.size(); n++) {
//   			bool found = false;
//   			// Search the line info of this stack ordering for the call 
//   			for (auto m : _lineInfo[_stackOrder[n]]) {
//   				if (m.first == i.libCudaCallname) {
//   					found = true;
//   					break;
//   				}
//   			}
  			
//   			// If we cant find the callname in the stacks, our stack ordering DOES NOT match the timing data
//   			// this run is useless.
//   			if (found == false){
//   				std::cerr << i.libCudaCallname << " NOT FOUND IN EXISTING LINE INFO FROM PREVIOUS RUNS! " << std::endl;
//   				std::cerr << "The following stack will be skipped\n" << std::endl;
//   				for (auto m : _lineInfo[_stackOrder[n]]) {
//   					std::cerr << m.first << std::endl;
//   				}
//   				continue;
//   				//assert(found == true);
//   			}

//   			count -= 1;
//  #ifdef DEBUG_MODEL
//   			std::cerr << "Found " << i.libCudaCallname << " with " << count << " calls remaining" << std::endl;
//  #endif

//   			if (count == 0) {
//   				orderPos = n+1;
//   				break;
//   			}
//   		}
//   		if (count > 0) {
//   			std::cerr << "Stacks do not align between timing and synchronization run, exiting now!" << std::endl;
//   			assert(count == 0);
//   		}
//   	}
//   	std::cerr << "Timing data matches previous run, continuing profiling" << std::endl;
}

void PerformanceModel::AddStack(std::vector<StackPoint> stack) {
	// std::stringstream ss;
	// _totalSyncs+=1;
	// // for (auto z : stack) {
	// // 	ss << z.framePtr << std::endl;
	// // }
	// uint64_t hash = std::hash<std::string>()(ss.str());
	// //std::cerr << hash << std::endl;
	// _stackOrder.push_back(hash);
	// if (_stackCount.find(hash) == _stackCount.end())
	// 	_stackCount[hash] = 0;
	// _stackCount[hash] += 1;
	// if (_stackPoints.find(hash) == _stackPoints.end())
	// 	_stackPoints[hash] = stack;	
}

 std::map<uint64_t, StackRecord> PerformanceModel::GetTimingStackRecords() {
 	return _stackRecords;
 }

void PerformanceModel::TranslateStackfile(std::string fileName, std::string outFilename) {
	FILE * keyFile = fopen(fileName.c_str(), "rb");
	assert(keyFile != NULL);

	StackKeyReader reader(keyFile);
	std::map<uint64_t, std::vector<StackPoint> > ret = reader.ReadStacks();

	std::map<uint64_t, StackRecord> translateRecords; 
	for (auto & i : ret)
		translateRecords[i.first] = StackRecord(i.first, i.second);	

	ExtractLineInfo(translateRecords);
	std::ofstream outFile;
	outFile.open(outFilename.c_str(), std::ofstream::out);
	for (auto i : translateRecords) {
		outFile << i.first; 
		i.second.PrintEncodedStack(outFile);
		outFile << std::endl;
	}
	outFile.close();
}

void PerformanceModel::ReadTimingStacks(std::string keyFile, std::string timelineFile) {
	std::cerr << "[PerformanceModel] Reading Timing Information" << std::endl;
	std::cerr << "[PerformanceModel] Reading timing stack file: " << timelineFile << std::endl;
	std::cerr << "[PerformanceModel] Reading timing key file: " << keyFile << std::endl;

	FILE * kf = fopen(keyFile.c_str(), "rb");
	assert(kf != NULL);

	StackKeyReader reader(kf);
	std::map<uint64_t, std::vector<StackPoint> > ret = reader.ReadStacks();

	for (auto & i : ret)
		_timingStackRecords[i.first] = StackRecord(i.first, i.second);

	ExtractLineInfo(_timingStackRecords);

	for (auto i : _timingStackRecords) 
		std::cerr << "Adding " << i.second.GetFirstCudaCall().funcName << " with hash id " << i.first << std::endl;

	FILE * inFile = fopen(timelineFile.c_str(), "rb");
	assert(inFile != NULL);
	fseek(inFile, 0, SEEK_END);
	uint64_t elementCount = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);
	fclose(inFile);

	elementCount = elementCount / (sizeof(uint64_t) + sizeof(double) + sizeof(uint64_t) + sizeof(uint64_t));
	_timingData = std::vector<TF_SyncRecord>(elementCount);

	TFReaderWriter rw;
	TF_Record rec;
	uint64_t total = 0;
	uint64_t pos = 0;
	assert(rw.Open(timelineFile.c_str(), TF_READ) == true);
	while (rw.Read(rec)) {
		if (rec.type == TF_SYNCRECORD) {
			_timingData[pos] = rec.s;
			_stackToDynId[rec.s.stackId] = rec.s.dynId;
			total += rec.s.count;
			pos++;
		}
	}

	// for (int i = 0; i < elementCount; i++) {
	// 	fread(&dynId, 1, sizeof(uint64_t), inFile);
	// 	fread(&stackId, 1, sizeof(uint64_t), inFile);
	// 	fread(&count, 1, sizeof(uint64_t), inFile); 
	// 	fread(&time, 1, sizeof(double), inFile);

	// 	_timingData[i].dynId = dynId;
	// 	_timingData[i].stackId = stackId;
	// 	_timingData[i].time = time;
	// 	_timingData[i].count = count;
	// 	_stackToDynId[stackId] = dynId;
	// 	total += count;
	// }
	// std::cerr << "[PerformanceModel] Captured " << total << " synchronizations from timing " << std::endl;
}

void PerformanceModel::WriteStackFile(std::string stackFile, std::string fileName) {

	FILE * keyFile = fopen(stackFile.c_str(), "rb");
	assert(keyFile != NULL);

	StackKeyReader reader(keyFile);
	std::map<uint64_t, std::vector<StackPoint> > ret = reader.ReadStacks();
	std::map<uint64_t, StackRecord> tmpStorage;
	for (auto & i : ret)
		tmpStorage[i.first] = StackRecord(i.first, i.second);

	ExtractLineInfo(tmpStorage);
	std::stringstream ss;

	for (auto i : tmpStorage) {
		i.second.PrintStack(ss);
		ss << std::endl;
	}
	FILE * out = fopen(fileName.c_str(), "w");
	fprintf(out, "%s\n",ss.str().c_str());
	fclose(out);
}

void PerformanceModel::ReadStackFile(std::string key, std::string timeline) {
	std::cerr << "Reading stack file: " << timeline << std::endl;
	std::cerr << "Reading key file: " << key << std::endl;

	FILE * inFile = fopen(timeline.c_str(), "rb");
	assert(inFile != NULL);

	FILE * keyFile = fopen(key.c_str(), "rb");
	assert(keyFile != NULL);

	StackKeyReader reader(keyFile);
	std::map<uint64_t, std::vector<StackPoint> > ret = reader.ReadStacks();

	for (auto & i : ret)
		_stackRecords[i.first] = StackRecord(i.first, i.second);

	ExtractLineInfo(_stackRecords);

	for (auto i : _stackRecords) {
		std::cerr << "Adding " << i.second.GetFirstCudaCall().funcName << " with hash id " << i.first << std::endl;
		_callMapper.InsertStackID(i.second.GetFirstCudaCall().funcName, i.first);
	}


	fseek(inFile, 0, SEEK_END);
	uint64_t elementCount = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);
	elementCount = elementCount / 8;
	// Typically have ~7 calls to low level synchronization
	_orderingInfo.reserve(elementCount / 7);

	std::vector<StackPoint> e;
	// Insert an empty element at 0 for unidentified synchronizations.
	_stackRecords[0] = StackRecord(0, e);
	uint64_t hash = 0;
	uint64_t pos = 0;
	uint64_t syncCount = 0;
	bool start = true;
	bool found = false;
	while (fread(&hash, 1, sizeof(uint64_t), inFile) > 0){
		syncCount++;
		found = false;
		//std::cerr << "My Hash: " << hash << std::endl;
		if (start != true) {
			if (_orderingInfo.back().stackId == hash){
				_orderingInfo.back().count++;
				found = true;
			}
		}
		if (start == true || found == false){
			TF_SyncRecord tmp;
			tmp.stackId = hash;
			tmp.dynId = _callMapper.StackIDToGeneral(hash);
			tmp.count = 1;
			_orderingInfo.push_back(tmp);
			start = false;
		}
		if (feof(inFile))
			break;
	}
	_capturedSyncs = syncCount;
	fclose(inFile);
}

void PerformanceModel::ReadStackFiles() {
	// Read all files in the directory looking for stackOut files. 
	std::vector<std::string> files;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (".")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if (strncmp(ent->d_name, "stackOut.", 9) == 0)
				files.push_back(std::string(ent->d_name));
		}
		closedir(dir);
	}
	assert(files.size() > 0);

	if (files.size() != 2) {
		std::cerr << "We have more than one stack output directory, multithreaded synchronization support not yet availible" << std::endl;
		assert(files.size() == 2);
	}
	if (files[0].find("key") != std::string::npos){
		ReadStackFile(files[0], files[1]);
	} else {
		ReadStackFile(files[1], files[0]);
	}

}


void PerformanceModel::FinalProcessing() {
	// std::stringstream sortByUse;
	// std::stringstream sortByTime;
	// std::vector<int> unncessarySyncs;

	// uint64_t unnecssaryCount = 0;
	// double unnecssaryTime = 0.0;
	// for (int x = 0; x < _callPoints.size(); x++) {
	// 	if (_firstUses.find(x) == _firstUses.end()) {
	// 		unnecssaryCount += 1;
	// 		unnecssaryTime += _callPoints[x].time;
	// 		unncessarySyncs.push_back(x);
	// 	}
	// }

	// std::cerr << "Unnecssary Synchronization Count: " << unnecssaryCount << "/" << _callPoints.size() << " " << float(unnecssaryCount) / float(_callPoints.size()) * 100 << "% unncessary" << std::endl;
	// std::cerr << "Potential time savings: " << unnecssaryTime << "/" << _fastestExecTime << " " << unnecssaryTime / _fastestExecTime * 100 << "% of exectuion time wasted" << std::endl;

	// // Sort by count
	// std::map<uint64_t, uint64_t> unncessaryStack;
	// for (auto x : unncessarySyncs) {
	// 	uint64_t myStack = _stackOrder[_callPoints[x].startPos];
	// 	if (unncessaryStack.find(myStack) == unncessaryStack.end())
	// 		unncessaryStack[myStack] = 0;
	// 	unncessaryStack[myStack] += 1;
	// }

	// sortByUse << "Table Performane Model Count: " << std::endl;
	// sortByUse << "StackID\tStackCount" << std::endl;

	// // Likely should be iterating over the key, should fix this since we shouldn't be iterating backward.
	// for (auto iter : unncessaryStack) {
	// 	sortByUse << iter.first << "\t\t" << iter.second << std::endl; 
	// }
	// for (auto iter : unncessaryStack) {
	// 	sortByUse << "Stack with unncessary synchronization - " << iter.first << std::endl;
	// 	for(auto i : _lineInfo[iter.first]) {
	// 		sortByUse <<  i.first << "," << i.second.filename << "," << i.second.lineNum << std::endl;
	// 	}
	// }

	// // Sort by time...
	// std::map<uint64_t, double> unncessaryStackTime;
	// for (auto x : unncessarySyncs) {
	// 	uint64_t myStack = _stackOrder[_callPoints[x].startPos];
	// 	if (unncessaryStack.find(myStack) == unncessaryStack.end())
	// 		unncessaryStackTime[myStack] = 0.0;
	// 	unncessaryStackTime[myStack] += _callPoints[x].time;
	// }

	// sortByTime << "Table Performance Model Time: " << std::endl;
	// sortByTime << "StackID\tTime Wasted" << std::endl;
	// for(auto x : unncessaryStackTime) {
	// 	sortByTime << x.first << "\t\t" << x.second << std::endl;
	// }
	// for (auto x : unncessaryStackTime) {
	// 	sortByTime << "Stack with unncessary synchronization: " << x.first << std::endl;
	// 	for(auto i : _lineInfo[x.first]) {	
	// 		sortByTime <<  i.first << "," << i.second.filename << "," << i.second.lineNum << std::endl;
	// 	}
	// }

	// std::ofstream pfile;
	// pfile.open("UnncessarySyncsCount.txt");
	// pfile << sortByUse.str() << std::endl;
	// pfile.close();
	// pfile.open("UnncessarySyncsTime.txt");
	// pfile << sortByTime.str() << std::endl;
	// pfile.close();
	// std::cerr << "Performance model is complete" << std::endl;
}

void PerformanceModel::ExtractLineInfo(std::map<uint64_t, StackRecord> & rec) {
	SymbolMap symbolInfo;
	
	// Open all symtabs for functions found on the stack. 
	for (auto i : rec) {
		std::vector<std::string> lnames = i.second.GetLibNames();
		for (auto z : lnames){
			if (symbolInfo.find(z) == symbolInfo.end())
				symbolInfo[z] = std::shared_ptr<SymbolLookup>(new SymbolLookup(z));
		}	
	}

	for (auto & i : rec)
		i.second.GetStackSymbols(symbolInfo);

// 	// Decode line info for every stack
// 	for (auto & i : _stackPoints) {
// 		_lineInfo[i.first] = std::vector<std::pair<std::string, LineInfo> >();
// 		for(auto & z : i.second) {
// 			// Convert to line info and add  to _lineInfo
// 			if (symbolInfo.find(z.libname) == symbolInfo.end())
// 				continue;
// 			std::pair<std::string, LineInfo> tmp;
// 			symbolInfo[z.libname]->GetInfoAtLocation(z.libOffset, tmp);
// 			z.funcOffset = symbolInfo[z.libname]->GetFunctionOffset(z.libOffset);
// 			z.funcName = symbolInfo[z.libname]->GetFuncName(z.libOffset);
// 			//z.fname = z.funcName;
// 			assert(z.funcOffset != 0);
// 			_lineInfo[i.first].push_back(tmp);
// #ifdef DEBUG_MODEL
// 			// std::cerr << "Looking up " << z.libOffset << " in " << z.libname << std::endl;
// 			// std::cerr << "Returned Function Name: " << tmp.first << std::endl;
// #endif
// 		}
// 	}
// #ifdef DEBUG_MODEL
// 	for(auto i : _lineInfo) {
// 		std::cerr << "Stack with Synchronization" << std::endl;
// 		for (auto z : i.second){
// 			std::cerr << z.first << "," << z.second.filename << "," << z.second.lineNum << std::endl;
// 		}
// 	}
// #endif
	//std::cerr << "Synchronization Count: " << _totalSyncs << std::endl;
	//ProcessStacks();
}


void PerformanceModel::ProcessStacks() {
// 	std::vector<std::string> cudaLibs = {"libcudnn.so","libaccinj64.so","libcublas.so","libcudart.so","libcufft.so","libcufftw.so","libcuinj64.so","libcurand.so","libcusolver.so","libcusparse.so","libnppc.so","libnppial.so","libnppicc.so","libnppicom.so","libnppidei.so","libnppif.so","libnppig.so","libnppim.so","libnppist.so","libnppisu.so","libnppitc.so","libnpps.so","libnvblas.so","libnvgraph.so","libnvrtc-builtins.so","libnvrtc.so"};
// 	// Find first user CUDA call and the parent who made the call.
// 	for (auto i : _stackPoints) {
// 		std::string parentParentCall;
// 		std::string parentCall;
// 		std::string cudaCall;
// 		for (int z = i.second.size() - 1;  z > 0; z = z - 1){
// 			bool found = false;
// 			for (auto n : cudaLibs){
// 				if (i.second[z].libname.find(n) != std::string::npos){
// 					found = true;
// 					break;
// 				}
// 			}
// 			if (found != true) {
// 				if (_lineInfo[i.first][z].first.find("_ZN6cudart") != std::string::npos)
// 					found = true;
// 			}

// 			if (found == true) {
// 				cudaCall = _lineInfo[i.first][z].first;
// 				break;
// 			}
// 			else {
// 				parentParentCall = parentCall;
// 				parentCall = _lineInfo[i.first][z].first;
// 			}
// 		}
// 		_callPair[i.first] = std::make_tuple(parentParentCall,parentCall,cudaCall);
// 	}
// //#ifdef DEBUG_MODEL
// 	for (auto i : _callPair)
// 		std::cerr << "Synch at " << std::get<0>(i.second) << "," << std::get<1>(i.second) << "," << std::get<2>(i.second) << std::endl;
// //#endif

}

void PerformanceModel::GetTimingList(std::vector<StackPoint> & timingList) {
	std::set<uint64_t> alreadyPresent;
	for (auto i : _stackRecords) {
		StackPoint p = i.second.GetFirstCudaCall();
		if (p.empty == true){
			continue;
		}
		if (alreadyPresent.find(p.libOffset) == alreadyPresent.end()) {
			alreadyPresent.insert(p.libOffset);
			p.timerID = _callMapper.NameToGeneral(p.funcName);
			timingList.push_back(p);
		}
	}

	// std::set<uint64_t> alreadyPresent;
	// for (auto i : _stackPoints) {
	// 	StackPoint found;
	// 	for (int z = i.second.size() - 1;  z > 0; z = z - 1) {
	// 		if (i.second[z].libname.find("libcuda.so") != std::string::npos){
	// 			found = i.second[z];
	// 			found.funcName = _lineInfo[i.first][z].first;
	// 			break;
	// 		}
	// 	}
	// 	std::cerr << found.libname << std::endl;
	// 	assert(found.funcOffset != 0);
	// 	if (alreadyPresent.find(found.funcOffset) == alreadyPresent.end()){
	// 		timingList.push_back(found);
	// 		alreadyPresent.insert(found.funcOffset);
	// 	}
	// }
		
//#ifdef DEBUG_MODEL
	for (auto i : timingList)
		std::cerr << "Inserting timing info into - " << i.funcName << std::endl;
//#endif
}