#pragma once
#include <iostream>
#include <fstream>    
#include <string>
#include <map>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <dlfcn.h>
#include "DriverWrapperBase.h"
#include "Parameters.h"
#include "PluginCommon.h"
//#include "StackTraceGen.h"
#ifndef INTERPOSITION_BUILD
#include "DriverAPIHeader.h"
#include "cuda.h"
#else
#include "InterpositionHeader.h"
#endif

#include <boost/thread/recursive_mutex.hpp>

typedef std::function<PluginReturn(std::vector<std::string> &)> InitFunc;
typedef std::function<PluginReturn(std::shared_ptr<Parameters>)> PrecallFunc;
typedef std::function<PluginReturn(std::shared_ptr<Parameters>)> PostcallFunc;

class DriverWrapperFactory : public DriverWrapperBase{
private:
	boost::recursive_mutex _driverMtx;
	//StackTraceGen * _stack;
	size_t _globalID;
	bool firstExec;
	std::vector<std::tuple<InitFunc, PrecallFunc, PostcallFunc> > _plugins;
public:
	DriverWrapperFactory();
	int PerformAction(std::shared_ptr<Parameters> params);
	~DriverWrapperFactory();
	void LoadLibraries(std::vector<std::string> libs);
	std::vector<std::string> GetLibraryNames(const char * file);
	void PrintStack();
};

#define BUILD_FACTORY \
	if (DriverFactory.get() == NULL) { \
		fprintf(stderr, "%s\n", "Setting up our global data structure"); \
		DriverFactory.reset(new DriverWrapperFactory()); \
	} 

#define FACTORY_PTR DriverFactory.get()

