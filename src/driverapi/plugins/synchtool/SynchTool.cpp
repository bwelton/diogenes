#include "SynchTool.h"
volatile int SYNCTOOL_exited = 0;
volatile int SYNCTOOL_inSpecialCase = 0;
volatile uint64_t SYNCTOOL_LOADSTORE_COUNT =0;
std::shared_ptr<SynchTool> Worker;
thread_local LoadStoreDriverPtr _LoadStoreDriver;
thread_local CheckAccessesPtr _dataAccessManager;
FILE * _temporaryFiles;
volatile bool SYNCTOOL_GLOBAL_SYNC_TYPE = false;

bool enteredMe = false;
volatile int justChecking = 8;
volatile bool SYNCTOOL_DONOTCHECK = false;
volatile bool SYNCTOOL_INCUDACALL = false;
extern "C" {

// Warpper Functions;
	void DIOGENES_exitDestroyer_syncTool() {
		SYNCTOOL_exited = 1;
		//DIOGENES_GetGlobalLock();
	}

void INIT_SYNC_COMMON() {
	if(SYNCTOOL_exited == 1)
		return;
	if (_dataAccessManager.get() != NULL)
		return;
	atexit(DIOGENES_exitDestroyer_syncTool);
	// gotcha_set_priority("cuda/specialcases", 1);
	// int result = gotcha_wrap(gotfuncs, 1, "cuda/specialcases");
	// assert(result == GOTCHA_SUCCESS);
	_dataAccessManager.reset(new CheckAccesses());
	_LoadStoreDriver.reset(new LoadStoreDriver(_dataAccessManager, SYNCTOOL_GLOBAL_SYNC_TYPE));
	//_temporaryFiles = fopen("TemporaryOutput.txt","w");
}

void diogenes_memrange_check(uint64_t addr, uint64_t size, uint64_t id) {
	if (SYNCTOOL_exited == 1)
		return;
	INIT_SYNC_COMMON();
	_LoadStoreDriver->EnterInstrimentation();
	_LoadStoreDriver->RecordAccessRange(id, addr, size);
	_LoadStoreDriver->ExitingInstrimentation();
}

void diogenes_strchr_wrapper(char * destination, uint64_t id) {
	if(SYNCTOOL_exited == 1 || SYNCTOOL_INCUDACALL == true)
		return;
	diogenes_memrange_check((uint64_t)destination, strlen(destination), id);
}

void diogenes_strcpy_wrapper(char * destination, char * source, uint64_t id) {
	if(SYNCTOOL_exited == 1 || SYNCTOOL_INCUDACALL == true)
		return;

	diogenes_memrange_check((uint64_t)source, strlen(source), id);
	diogenes_memrange_check((uint64_t)destination, strlen(destination), id);
}
void diogenes_strcat_wrapper(char * destination, char * source, uint64_t id) {
	if(SYNCTOOL_exited == 1 || SYNCTOOL_INCUDACALL == true)
		return;

	diogenes_memrange_check((uint64_t)source, strlen(source), id);
	diogenes_memrange_check((uint64_t)destination, strlen(destination) + strlen(source), id);
}

void diogenes_memcpy_wrapper(void * dest, void * source, uint64_t size, uint64_t id) {
	if(SYNCTOOL_exited == 1 || SYNCTOOL_INCUDACALL == true)
		return;

	//fprintf(stderr, "In Memcpy Check - Source=%p, Dest=%p, size=%llu\n",source, dest, size);
	diogenes_memrange_check((uint64_t)source, size, id);
	diogenes_memrange_check((uint64_t)dest, size, id);
}

void diogenes_memset_wrapper(void * dest, int value, uint64_t size, uint64_t id) {
	if(SYNCTOOL_exited == 1 || SYNCTOOL_INCUDACALL == true)
		return;	
	diogenes_memrange_check((uint64_t)dest, size, id);
}

void diogenes_bzero_wrapper(void * dest, uint64_t size, uint64_t id) {
	if(SYNCTOOL_exited == 1 || SYNCTOOL_INCUDACALL == true)
		return;	
	diogenes_memrange_check((uint64_t)dest, size, id);	
}

void diogenes_strncpy_wrapper(char * dest, char * source, uint64_t size, uint64_t id) {
	if(SYNCTOOL_exited == 1 || SYNCTOOL_INCUDACALL == true)
		return;	
	diogenes_memrange_check((uint64_t)dest, size, id);		
	diogenes_memrange_check((uint64_t)source, size, id);
}


static gotcha_wrappee_handle_t memcpyWrapper_handle;
static void * memcpyWrapper(void * dest, void * src, size_t count);
struct gotcha_binding_t SYNCTOOL_funcBinders[] = { {"memcpy",(void *)memcpyWrapper,&memcpyWrapper_handle}};

	void * memcpyWrapper(void * dest, void * src, size_t count) {
		INIT_SYNC_COMMON();
		SYNCTOOL_inSpecialCase = 1;
	}

	void RECORD_FUNCTION_ENTRY(uint64_t id) {
		if(SYNCTOOL_exited == 1)
			return;

		// if (justChecking == 8)
		// 	justChecking = 1;
		// else 
		// 	justChecking = 1002321;
		INIT_SYNC_COMMON();
		_LoadStoreDriver->EnterInstrimentation();
		// //assert(1 == 0);
		// // fprintf(_temporaryFiles,"[SynchTool] Captured function entry - %llu\n", id);
		// // fflush(_temporaryFiles);
		//std::cerr << "[SynchTool] Captured function entry - " << id << std::endl;
		_LoadStoreDriver->PushStack(id);
		_LoadStoreDriver->ExitingInstrimentation();
	}
	void RECORD_FUNCTION_EXIT(uint64_t id) {
		if(SYNCTOOL_exited == 1)
			return;
		// if (justChecking == 8)
		// 	justChecking = 1;
		// else 
		// 	justChecking = 1002321;
		INIT_SYNC_COMMON();
		// //assert(1==0);
		// // fprintf(_temporaryFiles,"[SynchTool] Captured function exit - %llu\n", id);
		// // fflush(_temporaryFiles);
		//std::cerr << "[SynchTool] Captured function exit - " << id << std::endl;
		_LoadStoreDriver->EnterInstrimentation();
		_LoadStoreDriver->PopStack(id);
		_LoadStoreDriver->ExitingInstrimentation();
	}
//extern void DYNINST_disableCondInst();
//extern void DYNINST_enableCondInst();
	void SYNC_CAPTURE_SYNC_CALL() {
		if(SYNCTOOL_exited == 1)
			return;
		// if (justChecking == 8)
		// 	justChecking = 1;
		// else 
		// 	justChecking = 1002321;
	    INIT_SYNC_COMMON();
		//DYNINST_disableCondInst();
		_LoadStoreDriver->EnterInstrimentation();
		std::cerr << "[SynchTool] Captured Synchronization call" << std::endl;
		_LoadStoreDriver->SyncCalled();
		_LoadStoreDriver->ExitingInstrimentation();
	}

	void SYNC_RECORD_MEM_ACCESS(uint64_t addr, uint64_t id) {
		
		if (SYNCTOOL_exited == 1)
			return;
		SYNCTOOL_LOADSTORE_COUNT++;
		//fprintf(stderr, "Inside of stack %llu\n",id);
		INIT_SYNC_COMMON();
		_LoadStoreDriver->EnterInstrimentation();
		//fprintf(stderr, "Inside of stack %llu\n",id);
		if(SYNCTOOL_exited == 1 || SYNCTOOL_inSpecialCase == 1 || SYNCTOOL_INCUDACALL == true ){
			_LoadStoreDriver->ExitingInstrimentation();
			return;
		}
		// SYNCTOOL_DONOTCHECK = true;
		//fprintf(stderr, "Inside of address %llu\n",id);
		
		//std::cerr << "Inside of address " << std::hex << addr<< std::endl;
		// if (justChecking == 8)
		// 	justChecking = 1;
		// else 
		// 	justChecking = 1002321;
		//INIT_SYNC_COMMON();
		//assert(1==0);
//		fprintf(_temporaryFiles,"[SynchTool] Captured memory access - %llu, %llu\n", addr, id);
//		fflush(_temporaryFiles);
		//std::cerr << "[SynchTool] Captured memory access at " << id << " with mem location " << std::hex << addr << std::dec << std::endl;
		_LoadStoreDriver->RecordAccess(id, addr);
		_LoadStoreDriver->ExitingInstrimentation();
		// SYNCTOOL_DONOTCHECK = false;
	}


	void SYNC_SET_GLOBAL_TO_TRUE() {
		SYNCTOOL_GLOBAL_SYNC_TYPE = true;
	}

	void SYNC_CAPTURE_SYNC_ENDTIME() {
		if(SYNCTOOL_exited == 1 || SYNCTOOL_inSpecialCase == 1)
			return;

		INIT_SYNC_COMMON();	
		_LoadStoreDriver->SyncCalled();	
		std::chrono::high_resolution_clock::time_point endSyncTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> dtime = endSyncTime.time_since_epoch();

		_LoadStoreDriver->WriteStackTime(0,dtime.count());
	}

	void SYNC_CAPTURE_TIMEACCESS(uint64_t addr, uint64_t id) {
		if(SYNCTOOL_exited == 1 || SYNCTOOL_inSpecialCase == 1)
			return;

		INIT_SYNC_COMMON();		
		std::chrono::high_resolution_clock::time_point endSyncTime = std::chrono::high_resolution_clock::now();	
		std::chrono::duration<double> dtime = endSyncTime.time_since_epoch();
		_LoadStoreDriver->RecordAccessWithTime(id, addr, dtime.count());

	}
}

SynchTool::SynchTool(std::vector<std::string> & cmd_list) {
	INIT_SYNC_COMMON();
	_cmd_list = cmd_list;
	SYNCTOOL_exited = 0;
	_sync_log.reset(new LogInfo(fopen("synch_log.out", "w")));
}

SynchTool::~SynchTool() {
	SYNCTOOL_exited = 1;
	_LoadStoreDriver.reset();
	_dataAccessManager.reset();
	std::cerr << "Exiting SyncTool Now!" << std::endl;
	std::cerr << "[SynchTool::~SynchTool] Load Store Count = " << SYNCTOOL_LOADSTORE_COUNT << std::endl;
}


void SynchTool::UnifiedAllocation(std::shared_ptr<Parameters> params) {
	// // PT_cuMemAllocManaged CUdeviceptr ** ,size_t* ,unsigned int*
	std::tuple<PT_cuMemAllocManaged> pvalues = GetParams<PT_cuMemAllocManaged>(params);
	MemoryRange tmp;
	tmp.begin = *((uint64_t *)(std::get<0>(pvalues)[0]));
	tmp.size = (uint64_t)(std::get<1>(pvalues)[0]);
	tmp.end = tmp.begin + tmp.size;
	tmp.transferID = params.get()->GetInstID();
	tmp.unifiedMemory = 1;
	tmp.stream = 0;
	std::stringstream ss;
	ss << "[SynchTool] Adding Unified Memory Location - " << params.get()->GetName() << " with the following info\n" 
	   << "\tCPU Starting Address = 0x" << std::hex << tmp.begin << std::dec 
	   << "\n\tAllocation Size = " << tmp.size 
	   << "\n\tStream = " << tmp.stream;
	std::cerr << ss.str() << std::endl; 
	_dataAccessManager->AddUnifiedMemRange(tmp);

	//_ranges[tmp.begin] = tmp;
}

void SynchTool::FreeMemoryAllocation(std::shared_ptr<Parameters> params) {
	std::tuple<PT_cuMemFree> pvalues = GetParams<PT_cuMemFree>(params);
	MemoryRange tmp;
	tmp.begin = (uint64_t)(std::get<0>(pvalues)[0]);
	tmp.end = 0;
	tmp.transferID = params.get()->GetInstID();
	tmp.unifiedMemory = 1;
	tmp.stream = 0;
	_dataAccessManager->RemoveUnifiedMemoryRange(tmp);
	// std::stringstream ss;
	// ss << "[SynchTool] cudaMemoryFree at call - " << params.get()->GetName() << " with the following info\n" 
	//    << "\tCPU Starting Address = 0x" << std::hex << tmp.begin << std::dec;
	// std::cerr << ss.str() << std::endl; 
}

void SynchTool::GetLiveTransfer(std::shared_ptr<Parameters> params) {
	MemoryTransfer * mem = params.get()->GetMemtrans();
	uint64_t baseAddr = mem->GetCPUAddress();
	int stream = mem->GetStream();
	if (baseAddr == 0){
		// This is a DtoD call that we dont really care or handle. 
		return;
	}
	
	MemoryRange tmp;
	tmp.begin =  baseAddr;
	tmp.size =  mem->GetSize();
	tmp.end = baseAddr + tmp.size;
	tmp.transferID = params.get()->GetInstID();
	tmp.unifiedMemory = 0;
	tmp.stream = stream;
	_dataAccessManager->AddMemoryTransfer(tmp);

//#ifdef SYNCH_DEBUG
	/*std::stringstream ss;
	ss << "[SynchTool] Adding Memory Transfer - " << params.get()->GetName() << " with the following info\n" 
	   << "\tCPU Starting Address = 0x" << std::hex << tmp.begin << std::dec 
	   << "\n\tTransfer Size = " << tmp.size 
	   << "\n\tStream = " << tmp.stream;
	std::cerr << ss.str() << std::endl;*/
	//_sync_log.get()->Write(ss.str());
//#endif
}

PluginReturn SynchTool::Precall(std::shared_ptr<Parameters> params) {
	SYNCTOOL_INCUDACALL = true;
	_LoadStoreDriver->EnterInstrimentation();
	Parameters * p = params.get();
	// If the call is not a synchronization
	if (ID_InternalSynchronization != p->GetID()){
		MemoryTransfer * mem = p->GetMemtrans();
		if (mem->IsTransfer() == true){
			// Disable hash checking for performance.  
			mem->SetHashGeneration(false);
			mem->PreTransfer();
			GetLiveTransfer(params);
		} else if (p->GetID() == ID_cuMemAllocManaged){
			// // Cuda Malloc Mana
			// std::cerr << "[SynchTool::Precall] I am an idiot for not filling this in...." << std::endl;
			// UnifiedAllocation(params);
			// This is a unified memory address allocation, for now
			// we assume that these are always live
		} else if (p->GetID() == ID_cuMemFree){
			FreeMemoryAllocation(params);
		}		
	}
	_LoadStoreDriver->ExitingInstrimentation();
	return NO_ACTION;
}

PluginReturn SynchTool::Postcall(std::shared_ptr<Parameters> params) {
	_LoadStoreDriver->EnterInstrimentation();
	Parameters * p = params.get();
	// If the call is not a synchronization
	if (ID_InternalSynchronization != p->GetID()){
		if (p->GetID() == ID_cuMemAllocManaged){
			UnifiedAllocation(params);	
		}
	}
	_LoadStoreDriver->ExitingInstrimentation();
	SYNCTOOL_INCUDACALL = false;
	return NO_ACTION;
}

extern "C"{

void init(std::vector<std::string> & cmd_list) {
	PLUG_BUILD_FACTORY(cmd_list)
}

PluginReturn Precall(std::shared_ptr<Parameters> params){
	if (SYNCTOOL_exited == 1)
		return NO_ACTION;
	return PLUG_FACTORY_PTR->Precall(params);
}

PluginReturn Postcall(std::shared_ptr<Parameters> params) {
	if (SYNCTOOL_exited == 1)
		return NO_ACTION;
	return PLUG_FACTORY_PTR->Postcall(params);
}

}
