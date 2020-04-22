#ifndef DIOGENES_READSTACKSFROMMUTATEE
#define DIOGENES_READSTACKSFROMMUTATEE 1
#include <iostream>
#include <map>
#include <vector>
#include "ReadStackKeys.h"
#include "BinaryAddress.h"
#include "HashMap.h"
#include "FileIO.h"

std::map<uint64_t, std::vector<DiogenesCommon::BinaryAddress>> ReadStacksFromMutatee(char * infile);

#endif