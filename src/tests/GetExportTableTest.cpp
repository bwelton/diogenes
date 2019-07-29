#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include "cuda.h"

typedef int (*SingleParam)(int *); 

int main(int argc, char * argv[]) {
	std::stringstream ss; 
	unsigned int value = 0;
	uint64_t tableId = 0;
	CUuuid pExportTableId; 
	std::ifstream t;
	std::ofstream outfile;
	outfile.open("function_pts.txt", std::ios::binary | std::ios::out);
	t.open(argv[1],std::ifstream::in);
	std::string line;
	cuInit(0);
	SingleParam getDeviceCount;
	while (std::getline(t, line)) {
		std::string backup = line;
		void * ppExportTable = NULL;
		int pos = 0;
		std::string token = line.substr(0, line.find(std::string(",")));
		while (token.size() > 0) {
			ss << token;
			ss >> std::hex >> value;
			pExportTableId.bytes[pos] = (char)uint8_t(value);
			
			if (line.find(std::string(",")) == std::string::npos)
				break;
			line = line.substr(line.find(std::string(",")) + 1, line.size());
			// line.erase(0, line.find(std::string(",")) + 1);
			token = line.substr(0, line.find(std::string(",")));
			ss.clear();
			pos++;
		}
		ss.clear();
		int ret = (int) cuGetExportTable((const void **)&ppExportTable, &pExportTableId);
		if (ret == CUDA_SUCCESS) {
			uint64_t count = 0;	
			if (ppExportTable == NULL)
				count = 0;
			else {
				volatile uint64_t * expTable = (uint64_t *)ppExportTable;
				count = expTable[0] / 8;
			}
			std::cout << tableId << " Valid Table: " << backup << " Ret = " << ret << " count: " << count << std::endl;
			if (count < 500){
				volatile uint64_t * expTable = (uint64_t *)ppExportTable;
				for(int i = 1; i < count; i++){
					if (expTable[i] != NULL)
						outfile << tableId << "," << std::hex << expTable[i] << std::dec << std::endl;
					else
						outfile << tableId << "," << std::hex << NULL << std::dec << std::endl;
					printf("\t\tFunction Ptr: %p, table pos: %d\n", expTable[i], i);
					if (i == 4 && tableId == 31)
						getDeviceCount = (SingleParam)expTable[i];
				}
			}
		}
		tableId++;
	}
	// int myret = 0;
	// int welp = getDeviceCount(&myret);
	outfile.close();
	fprintf(stderr, "%s %ld\n", "Wrote output file, attach dyninst now to pid:", getpid());
	while(1)
		sleep(10);
	// int mnret;
 //    CUdevice cuDevice = 0;
 //    cuDeviceGet(&cuDevice, 0);
 //    CUcontext cuContext;
 //    cuCtxCreate(&cuContext, 0, cuDevice);
	// CUdeviceptr ptr;
	// mnret = (int)cuMemAlloc(&ptr, sizeof(int)*1024);
	// if (CUDA_SUCCESS != mnret)
	// 	std::cerr << "Alloc Failed" << std::endl;
}