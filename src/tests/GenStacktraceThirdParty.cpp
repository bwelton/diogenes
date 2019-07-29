#include <stdio.h>
#include "BPatch.h"
#include "BPatch_addressSpace.h"
#include "BPatch_process.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_point.h"
#include "BPatch_function.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <cassert>
#include <deque>
#include <sys/time.h>
#include <cstdlib>
#include <sstream>
#include <tuple>
#include <utility> 
#include <stdarg.h>
#include <map>
#include <set> 
#include <iomanip>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <mutex>

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
#include "dynC.h"
#include "set"
using namespace Dyninst;
using namespace ParseAPI;
using namespace PatchAPI;
using namespace SymtabAPI;


std::vector<std::string> funcNames;
std::map<uint64_t, std::string> namePoints; 
std::map<std::string, uint64_t> stackCounts;
std::set<std::string> _interestingStacks;

BPatch_addressSpace * addrs;
BPatch bpatch;
bool loaded;

BPatch_addressSpace * LaunchProcess(const char * name, const char * argv[]) {
	BPatch_addressSpace * handle = NULL;
	handle = bpatch.processCreate(name, argv);
	assert(handle != NULL);
	return handle;
}

void InsertBreakpoints(BPatch_module * mod){
	// if (loaded == true)
	// 	return;
	BPatch_breakPointExpr bp;
	//BPatch_Vector<BPatch_function *> * allFuncs = mod->getProcedures(true);
	BPatch_Vector<BPatch_point *> points;
	for(auto i : funcNames) {
		std::cerr << "Trying to insert " << i << std::endl;
		BPatch_Vector<BPatch_function *> funcs;
		BPatch_Vector<BPatch_function *> * tmp = mod->findFunction(i.c_str(), funcs, true, false, false, false);
		if (tmp == NULL)
			continue;
		if (tmp->size() ==0)
			continue;
		BPatch_Vector<BPatch_point *> * entry_points;
		entry_points = (*tmp)[0]->findPoint(BPatch_entry);
		for (auto x : *entry_points){
			uint64_t p = (uint64_t)x->getAddress();
			namePoints[p] = i;
			//std::cerr << std::hex << p << std::dec << std::endl;
		}
		points.insert(points.end(), entry_points->begin(), entry_points->end());
		std::cerr << "Inserting instrimentation into " << i << std::endl;
	}

	if(!addrs->insertSnippet(bp, points)) {
		fprintf(stderr, "%s\n", "InsertFailed");
		exit(-1);
	}
}


void ForkCallback(BPatch_thread * parent, BPatch_thread * child) {
	std::cerr << "We have forked" << std::endl;

}

void ExecCallback(BPatch_thread * exec) {
	std::cerr << "We have execed something" << std::endl;
}

void LibLoadedCallBack(BPatch_thread * thread, BPatch_object * obj, bool l) {

	// if (loaded == true)
	// 	return;
	BPatch_process * myProc = thread->getProcess();
	std::cerr << "My process id is " << myProc->getPid() << std::endl;
	if (l == false)
		return;
	std::cerr << "in loaded library callback" << std::endl;
	BPatch_process* appProc = dynamic_cast<BPatch_process*>(addrs);
	bpatch.registerPreForkCallback((BPatchForkCallback)&ForkCallback);
	BPatch_image *image = addrs->getImage();
	BPatch_Vector<BPatch_module*> mods;
	image->getModules(mods);
	// obj->modules(mods);
	for (auto mod : mods){
		char * name = (char *) malloc(500 * sizeof(char));
		name = mod->getName(name, 500);
		std::string tmp = std::string(name);
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
		if (tmp.find(std::string("libcuda.so")) != std::string::npos) {
			// We have found libcuda, trigger instrimentation
			std::cerr << "We are inserting into " << name << std::endl;
			InsertBreakpoints(mod);
			//assert(appProc->isStopped() == true);
			// InsertBreakpoints(mod);
			loaded = true;
			break;
		}
	}


}

std::vector<std::string> GetFunctionNames(const char * file) {
	std::cerr << "Loading function file : " << file << std::endl;
	std::vector<std::string> ret;
	std::ifstream t;
	std::string line;
	t.open(file,std::ifstream::in);
	while (std::getline(t, line)) {
		ret.push_back(line);
	}
	return ret;
}

void StoppedThreadCheck(BPatch_Vector<BPatch_thread *> & threads) {
	for(auto i : threads){
		std::vector<std::string> libraries;
		std::stringstream ss;
		BPatch_Vector<BPatch_frame> frames;
		i->getCallStack(frames);
		for (auto frame : frames) {
			BPatch_function * func = frame.findFunction();
			BPatch_point * point = frame.getPoint();

			if (func == NULL && point == NULL){
				ss << "<Unknown Frame>" << std::endl;
				continue;
			}
			if (func == NULL && point != NULL){
				uint64_t p = (uint64_t)point->getAddress();
				if (namePoints.find(p) != namePoints.end()){
					ss << namePoints[p] <<  " (libcuda.so.1)" << std::endl;
					libraries.push_back(std::string("libcuda.so.1"));
				}
				else
					ss << "<Unknown Frame w/point>" << std::endl;
				continue;
			}
			std::string name = func->getName();
			assert(func->getModule() != NULL);
			const char * libname = func->getModule()->libraryName();
			std::string outlibname;
			if (libname == NULL)
				outlibname = std::string("Unknown");
			else{
				outlibname = std::string(libname);
				//outlibname = std::string(strdup(libname));
				libraries.push_back(outlibname);
			}
			// libraries.push_back(std::string(strdup(libname)));
			name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
			ss << name << " (" << outlibname << ")" << std::endl;
		}
		std::string s = ss.str();
		// if (s.find("HIDDEN") == std::string::npos)
		// 	continue;
		if (stackCounts.find(s) == stackCounts.end())
			stackCounts[s] = 0;
		stackCounts[s] = stackCounts[s] + 1;
	}
}

int main(const int argc, const char * argv[]){
	loaded = false;
	funcNames = GetFunctionNames(argv[1]);
	bpatch.registerPostForkCallback((BPatchForkCallback)&ForkCallback);
	addrs = LaunchProcess(argv[3], &(argv[3]));
	bpatch.registerDynLibraryCallback((BPatchDynLibraryCallback)&LibLoadedCallBack);
	bpatch.registerExecCallback((BPatchExecCallback)&ExecCallback);
	BPatch_process* appProc = dynamic_cast<BPatch_process*>(addrs);
	// BPatch_binaryEdit* appBin = dynamic_cast<BPatch_binaryEdit*>(addrs);
	BPatch_image *image = addrs->getImage();
	BPatch_Vector<BPatch_module*> mods;
	image->getModules(mods);
	for (auto mod : mods){
		char * name = (char *) malloc(500 * sizeof(char));
		name = mod->getName(name, 500);
		std::string tmp = std::string(name);
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
		if (tmp.find(std::string("libcuda.so")) != std::string::npos) {
			// We have found libcuda, trigger instrimentation
			std::cerr << "We are inserting into " << name << std::endl;
			InsertBreakpoints(mod);
			loaded = true;
			break;
		}
	}
	if (!appProc->continueExecution()) {
	    fprintf(stderr, "continueExecution failed\n");
	}
	while (!appProc->isTerminated()) {
		bpatch.waitForStatusChange();
		if (loaded == false)
			sleep(1);
		//std::cerr << "Status Changed...." << std::endl;
		// We have stopped
		if(appProc->isStopped() == true) {
			BPatch_Vector<BPatch_thread *> threads;
			appProc->getThreads(threads);
			StoppedThreadCheck(threads);
		}
		appProc->continueExecution();
	}
	std::cerr << "loaded: " << loaded << std::endl;
	uint64_t totalCount = 0;
	std::ofstream outfile;
	outfile.open(argv[2], std::ios::binary | std::ios::out);
	// Print the stack traces:
	for(auto i : stackCounts) {
		std::cout << "Unique Stack with Count: " << i.second << std::endl;
		std::cout << "===========================================" << std::endl;
		std::cout << i.first << std::endl;
		std::cout << "===========================================" << std::endl << std::endl << std::endl;
		if (i.first.size() == 0){
			std::cerr << "STACK WITHOUT ANY FRAMES PRESENT" << std::endl;
			continue;
		}
		std::string tmpstr = i.first;
		std::replace(tmpstr.begin(), tmpstr.end(),'\n','$');

		tmpstr.erase(tmpstr.size()-1, 1);
		outfile << i.second << "$" << tmpstr << std::endl;
	}
	outfile.close();
}
