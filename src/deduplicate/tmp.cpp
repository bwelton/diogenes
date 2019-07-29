struct MemCpyActivity {
	
	const char * name;
	uint64_t correlationId;
	uint64_t startTime;
	uint64_t endTime;
	size_t bytes;
	uint64_t runCorrelation;
	uint64_t contextId;
	uint64_t deviceId;
	uint64_t streamId;

	MemCpyActivity(CUpti_ActivityMemcpy * cpy, const char * n, uint64_t startTimestamp) {
		name = n;
		correlationId = cpy->correlationId;
		startTime = cpy->start - startTimestamp;
		endTime = cpy->end - startTimestamp;
		bytes = cpy->bytes;
		runCorrelation = cpy->runtimeCorrelationId;
		contextId = cpy->contextId;
		deviceId = cpy->deviceId;
		streamId = cpy->streamId;
	}

	std::string ToString(){
		std::stringstream ss;
		ss << "CPY," << getMemcpyKindStringC((CUpti_ActivityMemcpyKind) cpy->copyKind) << "," 
		   << cpy->correlationId << "," << cpy->start - startTimestamp << ","  << cpy->end - startTimestamp << ","  
		   << cpy->bytes << "," << cpy->runtimeCorrelationId << "," << cpy->contextId << "," << cpy->deviceId 
		   << "," << cpy->streamId << std::endl;		
	}


				ss << "CPY," << getMemcpyKindStringC((CUpti_ActivityMemcpyKind) cpy->copyKind) << "," 
			   << cpy->correlationId << "," << cpy->start - startTimestamp << ","  << cpy->end - startTimestamp << ","  
			   << cpy->bytes << "," << cpy->runtimeCorrelationId << "," << cpy->contextId << "," << cpy->deviceId 
			   << "," << cpy->streamId << std::endl;




}