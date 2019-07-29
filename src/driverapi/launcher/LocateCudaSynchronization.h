#pragma once
#include <string>
#include <map>
#include <iomanip>
#include <sstream> 
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cassert>
#include <boost/filesystem.hpp>
#include <openssl/md5.h>
#include <iostream>
#include <fstream>
#include <sys/mman.h>
#include <fcntl.h>    
#include <unistd.h>   
#include "Constants.h"
#include "Common.h"

class LocateCudaSynchronization {
public:
	uint64_t FindLibcudaOffset();
	std::map<std::string, uint64_t> ReadDriverList();
	std::string GetMD5Sum(boost::filesystem::path file);
	boost::filesystem::path FindLibCuda();
};
