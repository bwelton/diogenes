#pragma once
#include <vector>
#include <memory>
#include "CheckAccesses.h"
#include "OutputWriter.h"
#include <chrono>
// No Synchronization = Disabled
// Synchronization and No Use = Enabled
// Synchronization and use = Enabled
// Within Instrimetnation =  disabled


struct InstrimentationControl {
	enum INST_STATE
	{
		DISABLED = 0,
		SYNC_NOUSE,
		SYNC_USEFOUND,
	};
	INST_STATE _state;
	InstrimentationControl();

	void EnteringInst();

	void SyncCalled();

	void FoundUse();

	void ExitingInst();
	
};

class LoadStoreDriver {
public:
	LoadStoreDriver(CheckAccessesPtr access, bool timefu = false);
	void EnterInstrimentation();
	void ExitingInstrimentation();
	void RecordAccess(uint64_t id, uint64_t addr);
	void PushStack(uint64_t stack);
	void PopStack(uint64_t stack);
	bool ShouldSignalSync(bool inSync);
	void SyncCalled();
	void SignalSync();
	void RecordAccessRange(uint64_t id, uint64_t addr, uint64_t count);
	void WriteStackTime(uint64_t id, double tv);
	void RecordAccessWithTime(uint64_t id, uint64_t addr, double timetv);
private:
	std::vector<uint64_t> _storedStack;
	std::vector<uint64_t> _stackAtSync;
	uint64_t _checkCount;
	uint64_t _totalCheck;
	std::chrono::time_point<std::chrono::high_resolution_clock> _start;
	OutputWriterPtr _writer;
	CheckAccessesPtr _access;
	bool _syncTriggered;
	bool _found;
	bool _firstWrite;
	bool _firstSync;
	uint64_t _syncStackDepth;
	bool _timefu;

	InstrimentationControl _instControler;
};

typedef std::shared_ptr<LoadStoreDriver> LoadStoreDriverPtr;