#pragma once
#include <iostream>
#include <fstream>    
#include <string>
#include <map>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <memory>

// Dyninst includes
#include "DriverWrapperBase.h"
#include "Parameters.h"
#include "framestepper.h"
#include "walker.h"
#include "procstate.h"
#include "swk_errors.h"
#include "steppergroup.h"
#include "frame.h"
#include "sw_c.h"
#include "Symtab.h"
#include "BPatch.h"
#include "BPatch_process.h"
using namespace Dyninst;
using namespace Dyninst::Stackwalker;
using namespace SymtabAPI;

class StackTraceGen : public DriverWrapperBase {
public: 
	std::string GenStackTrace();
	int PerformAction(DriverAPICall t, std::shared_ptr<ParameterBase> params);
	StackTraceGen();
	~StackTraceGen();

};