#include "DynHelper.h"
#include "FindCudaSync.h"

// Helper functions
std::vector<BPatch_object *> DynHelper_GetObjects(BPatch_addressSpace * aspace) {
	BPatch_image * img = aspace->getImage();
	std::vector<BPatch_object *> objects;
	img->getObjects(objects);
	return objects;
}

std::vector<std::pair<BPatch_function *, uint64_t>> DynHelper_GetFunctions(BPatch_object * obj) {
	std::vector<BPatch_module *> modules;
	obj->modules(modules);
	std::vector<std::pair<BPatch_function *, uint64_t>> ret;
	for (auto i : modules) {
		std::vector<BPatch_function*>* funcs =  i->getProcedures();
		uint64_t baseAddr = (uint64_t)i->getBaseAddr();
		for(auto x : *funcs) {
			ret.push_back(std::make_pair(x,baseAddr));
		}
		delete funcs;
	}
	return ret;
}

bool DynHelper_CheckIfFileExists (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

uint64_t DynHelper_GetSynchronizationOffset(std::string & ret) {
	return CSA_FindSyncAddress(ret);
}

BPatch_function * DynHelper_GetFuncByName(BPatch_addressSpace * aspace, std::string libname, std::string funcName) {
	std::vector<BPatch_object *> binObjs = DynHelper_GetObjects(aspace);
	for (auto i : binObjs) {
		if (i->pathName().find(libname) != std::string::npos){
			std::vector<std::pair<BPatch_function *, uint64_t>> n = DynHelper_GetFunctions(i);
			for (auto x : n) {
				if(x.first->getName().find(funcName) != std::string::npos)
					return x.first;
			}
		}
	}
	return NULL;
}

BPatch_function * DynHelper_GetFuncAtAddress(BPatch_addressSpace * aspace, std::string libname, uint64_t offset) {
	std::vector<BPatch_object *> binObjs = DynHelper_GetObjects(aspace);
	for (auto i : binObjs) {
		if (i->pathName().find(libname) != std::string::npos){
			std::vector<std::pair<BPatch_function *, uint64_t>> n = DynHelper_GetFunctions(i);
			for (auto x : n) {
				uint64_t binAddr = (((uint64_t)x.first->getBaseAddr()) - x.second);
				if (binAddr == offset)
					return x.first;
			}
		}
	}
	return NULL;
}

std::string DynHelper_GetInstallDirectory() {
	return std::string(DIOGENES_INSTALL_DIRECTORY);
}