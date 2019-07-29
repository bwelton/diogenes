#pragma once
#include <map>
#include "TransferBase.h"
class InstrumentBase {
public: 
	virtual int PerformAction(TransferPtr t) = 0;
	virtual int PostTransfer(TransferPtr t) = 0;
	virtual ~InstrumentBase() = 0;
};
