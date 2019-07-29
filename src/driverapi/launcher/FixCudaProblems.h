#pragma once
#include "DyninstProcess.h"
#include "Constants.h"
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <iostream>
#include <memory>
#include <algorithm>
#include <sys/types.h>
#include <unistd.h>
#include "APICaptureInstrimentation.h"

#include "DynOpsClass.h"
#include "Common.h"

// Dyninst includes
#include "CodeObject.h"
#include "CFG.h"
#include "PatchObject.h"
#include "PatchMgr.h"
#include "Point.h"
#include "BPatch_object.h"
#include "BPatch_snippet.h"
#include "BPatch.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_image.h"
#include "BPatch_function.h"
#include "BPatch_Vector.h"
#include "BPatch_point.h"
#include "BPatch_addressSpace.h"
#include "BPatch_statement.h"
#include "BPatch_basicBlock.h"
#include "InstructionCategories.h"
#include "dynC.h"
#include "set"
#include "LogInfo.h"
#include "Constants.h"
#include "StackPoint.h"
#include "StackStorage.h"
#include "ReadStackKeys.h"
#include "TFReaderWriter.h"
#include "InstrimentationTracker.h"
#include "BinaryLocationIDMap.h"

#include "DyninstFunctionWraps.h"
#include "DyninstFunction.h"
#include "DyninstCallsite.h"
#include "DebugInstrimentationTemp.h"
#include "CallTransformation.h"
class FixCudaProblems {
public:
	FixCudaProblems(std::shared_ptr<DyninstProcess> proc);
	void InsertAnalysis(StackRecMap & recs, CallTransPtr callTrans);
	void PostProcessing();
private:
	std::shared_ptr<DyninstProcess> _proc;
	//std::vector<std::shared_ptr<DyninstFunction> > _dyninstFunctions;
	std::map<uint64_t, std::shared_ptr<DyninstFunction>> _dyninstFunctions;
	std::shared_ptr<BinaryLocationIDMap> _bmap;
};
