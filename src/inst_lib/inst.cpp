#include "inst.h"

InstStorage * OpenBinary(char * app_binary) {
	InstStorage * ret = new InstStorage[1];
	ret->bpatch.setInstrStackFrames(true);
	BPatch_binaryEdit *app = ret->bpatch.openBinary(app_binary, true);
	ret->unnamedCount = 1;
	if (!app)
		fprintf(stderr, "COULD NOT OPEN BINARY: %s\n", app_binary);
	ret->app = app;
	ret->binaryName = std::string(strdup(app_binary));
	return ret;
}

int WrapAllFunctions(InstStorage * storage, char * binary_function, char * wrapper_function, 
				 char * wrapper_library) {
	assert(storage != NULL);
	if (storage->wrapAllFunctions.find(binary_function) != storage->wrapAllFunctions.end()) {
		fprintf(stderr, "%s: %s\n", "Replacment function already exists", binary_function);
		return -1;		
	}
	storage->wrapAllFunctions[strdup(binary_function)] = std::make_tuple(strdup(wrapper_function), strdup(wrapper_library));
	return 1;
}

int ReplaceFunction(InstStorage * storage, char * binary_function, char * replacement_function, 
					char * replacement_library) {

	assert(storage != NULL);
	if (storage->replaceFuncs.find(binary_function) != storage->replaceFuncs.end()){
		fprintf(stderr, "%s: %s\n", "Replacment function already exists", binary_function);
		return -1;
	}
	storage->replaceFuncs[strdup(binary_function)] = std::make_pair(strdup(replacement_function), strdup(replacement_library));
	return 1;
}

int WrapFunction(InstStorage * storage, char * binary_function, char * wrapper_function, 
				 char * wrapper_library, char * wrapper_hookName) {
	assert(storage != NULL);
	if (storage->wrapFunctions.find(binary_function) != storage->wrapFunctions.end()){
		fprintf(stderr, "%s: %s\n", "Replacment function already exists", binary_function);
		return -1;
	}
	storage->wrapFunctions[strdup(binary_function)] = std::make_tuple(strdup(wrapper_function), strdup(wrapper_library), strdup(wrapper_hookName));
	return 1;
}

int InsertAtFunctionEntry(InstStorage * storage, char * binary_function, char * wrapper_function, 
				 char * wrapper_library) {
	assert(storage != NULL);
	if (storage->InsertAtEntry.find(binary_function) != storage->InsertAtEntry.end()){
		fprintf(stderr, "%s: %s\n", "Replacment function already exists", binary_function);
		return -1;
	}
	storage->InsertAtEntry[strdup(binary_function)] = std::make_tuple(strdup(wrapper_function), strdup(wrapper_library));
}


int InsertAtFunctionEntryOffset(InstStorage * storage, uint64_t offset, char * wrapper_function, 
				 char * wrapper_library, char * preferredName) {
	assert(storage != NULL);

	// Create the sumbol to write to the file 
	if (storage->SymbolsToWrite.find(offset) != storage->SymbolsToWrite.end()){
		fprintf(stderr, "%s: %llu\n", "Symbol(s) already exists at point", offset);
		return -1;
	}

	storage->SymbolsToWrite[offset] = std::string(strdup(preferredName));
	InsertAtFunctionEntry(storage, preferredName, wrapper_function, wrapper_library);
	return 0;
}

int AddSymbolAtOffset(InstStorage * storage, uint64_t offset, char * preferredName) {
	assert(storage != NULL);

	// Create the sumbol to write to the file 
	if (storage->SymbolsToWrite.find(offset) != storage->SymbolsToWrite.end()){
		fprintf(stderr, "%s: %llu\n", "Symbol(s) already exists at point", offset);
		return -1;
	}

	storage->SymbolsToWrite[offset] = std::string(strdup(preferredName));
	//InsertAtFunctionEntry(storage, preferredName, wrapper_function, wrapper_library);
	return 0;
}



PyObject * FindAllSymbolsWithPrefix(InstStorage * storage, char * prefix) {
	BPatch_Vector<BPatch_module *> modules = *(storage->app->getImage()->getModules());
	std::string search = std::string(prefix);
	std::transform(search.begin(), search.end(), search.begin(), ::tolower);
	PyObject* ret = PyList_New(0);
	for(BPatch_module * mod : modules){
		Dyninst::SymtabAPI::Module *symtab =  Dyninst::SymtabAPI::convert(mod);
		std::vector<Symbol *> all_symbols;
		symtab->getAllSymbols(all_symbols);
		for(Symbol * sym : all_symbols) {
			std::string tmp = sym->getPrettyName();
			std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
			if (tmp.find(search) != std::string::npos) {
				PyList_Append(ret, PyString_FromString(sym->getPrettyName().c_str()));
			}
		} 	
	}
	return ret;
}


PyObject * GetModuleNames(InstStorage * storage) {
	BPatch_Vector<BPatch_module *> modules = *(storage->app->getImage()->getModules());
	PyObject* ret = PyList_New(0);
	char tmp[200];
	for(BPatch_module * mod : modules){
		mod->getFullName(tmp, 200);
		PyList_Append(ret, PyString_FromString((const char *)tmp));
	}

	return ret;
}
PyObject * GetBinarySymbolsForModule(InstStorage * storage, char * module_name) {
	PyObject* ret = PyList_New(0);
	BPatch_module * mod = storage->app->getImage()->findModule(module_name, true);
	Dyninst::SymtabAPI::Module *symtab =  Dyninst::SymtabAPI::convert(mod);
	std::vector<Symbol *> all_symbols;
	symtab->getAllSymbols(all_symbols);
	for(Symbol * sym : all_symbols) {
		PyList_Append(ret, PyString_FromString(sym->getPrettyName().c_str()));
	}
	return ret;
}


std::vector<BPatch_function *> findFuncByName(BPatch_image * appImage, const char * funcName)
{
  /* fundFunctions returns a list of all functions with the name 'funcName' in the binary */
  BPatch_Vector<BPatch_function * >funcs;
  if (NULL == appImage->findFunction(funcName, funcs) || !funcs.size() || NULL == funcs[0])
  {
      std::cerr << "Failed to find " << funcName <<" function in the instrumentation library" << endl;
      return std::vector<BPatch_function *>();
  }
  std::cerr << "Found " << funcName << " this many times " << funcs.size() << endl;
  if (funcs.size() > 1) {
    for(int i = 0; i < funcs.size(); i++ )
    {
        std::cerr << "    " << funcs[i]->getName() << std::endl;
    }
  }
  return funcs;
}

std::vector<BPatch_function *> findFuncByNameRegEx(BPatch_image * appImage, const char * funcName)
{
  /* fundFunctions returns a list of all functions with the name 'funcName' in the binary */
  BPatch_Vector<BPatch_function * >funcs;
  if (NULL == appImage->findFunction(funcName, funcs, true,false,false) || !funcs.size() || NULL == funcs[0])
  {
      std::cerr << "Failed to find " << funcName <<" function in the instrumentation library" << endl;
      return std::vector<BPatch_function *>();
  }
  std::cerr << "Found " << funcName << " this many times " << funcs.size() << endl;
  if (funcs.size() > 1) {
    for(int i = 0; i < funcs.size(); i++ )
    {
        std::cerr << "    " << funcs[i]->getName() << std::endl;
    }
  }
  return funcs;	

}

void InsertSymbols(InstStorage * storage, char * outputName) {
	// Close the existing open file....
	//delete storage->app;
	BPatch_image * appImage = storage->app->getImage();
	BPatch_Vector<BPatch_module *> mods;
	appImage->getModules(mods);

	// Make this non-specific to cuda
	BPatch_module * cudaMod = NULL;
	std::string search = std::string("libcuda.so");
	char * modname = (char*) malloc(500*sizeof(char));
	std::transform(search.begin(), search.end(), search.begin(), ::tolower);
	for (auto i : mods) {
		modname = i->getName(modname, 500);
		std::string mname = std::string(modname);
		std::transform(mname.begin(), mname.end(), mname.begin(), ::tolower);
		if (mname.find(search) != std::string::npos){
			cudaMod = i;
			fprintf(stderr, "%s %s\n", "Found Cuda Module", modname);
			break;
		}
	}
	free(modname);



	assert(cudaMod != NULL);
	Symtab *symtab =  Dyninst::SymtabAPI::convert(cudaMod->getObject());

	std::vector<Symbol *> symbols;
	symtab->getAllSymbols(symbols);
	std::map<uint64_t, Symbol *> symMap;
	for (auto i : symbols)
		symMap[uint64_t(i->getOffset())] = i;

	// Get all the symbols in this module

	for(auto i : storage->SymbolsToWrite){
		uint64_t size = 0;

		if (symMap.find(uint64_t(i.first)) != symMap.end()) {
			size = symMap[uint64_t(i.first)]->getSize();
		} else {
			for(auto p : symMap) {
				if(p.first < uint64_t(i.first))
					continue;
				size = p.first - i.first;
				break;
			}
		}


		if(symtab->createFunction(i.second, i.first, 0) == NULL){
			fprintf(stderr, "Could not write symbol: %s,%llu\n",i.second.c_str(),i.first);
		} else {
			storage->SymbolToAddr[i.second] = i.first;
			fprintf(stderr, "%s: %s %s %llu\n", "Wrote symbol to file", i.second.c_str(), "with size of", size);
		}
	}
	// std::string outfile = std::string(outputName) + std::string(".symboled");
	// storage->app = storage->bpatch.openBinary(outfile.c_str(), true);
	// if (storage->app == NULL){
	// 	fprintf(stderr, "%s\n", "COULD NOT REOPEN BINARY,EXITING NOW");
	// 	exit(-1);
	// }
}

int PerformRewrite(InstStorage * storage, char * outputName) {
	fprintf(stderr, "%s %s\n", "Performing rewrite, saving to file", outputName);
	// Gather all libraries needed and all functions we are either going to
	// wrap or replace.

	// Insert any symbols we needm
	InsertSymbols(storage, outputName);
	std::set<char *> libnames;
	std::set<char *> functions;
	BPatch_binaryEdit * app = storage->app;

	assert(app != NULL);
	for(auto const & entry : storage->replaceFuncs) {
		functions.insert(entry.first);
		libnames.insert(entry.second.second);
	}

	for(auto const & entry : storage->wrapFunctions) {
		functions.insert(entry.first);
		libnames.insert(std::get<1>(entry.second));
	}

	for(auto const & entry : storage->InsertAtEntry) {
		functions.insert(entry.first);
		libnames.insert(std::get<1>(entry.second));
	}

	for(auto const & entry : storage->wrapAllFunctions) {
		libnames.insert(std::get<1>(entry.second));
	}



	for (const char * lname : libnames) {
		fprintf(stderr, "%s %s %s\n", "Inserting library", lname, "into binary");
		if(!app->loadLibrary(lname)) {
			fprintf(stderr, "%s: %s\n", "ERROR Loading Shared Object", lname);
			return -1;
		}
	}

	BPatch_image * appImage = app->getImage();
	for (char * fname : functions) {
		std::string tmpName = std::string(fname);
		std::vector<BPatch_function *> orig_funcs;
		if (storage->SymbolToAddr.find(tmpName) != storage->SymbolToAddr.end())
			orig_funcs.push_back(appImage->findFunction((unsigned long)storage->SymbolToAddr[tmpName]));
		else
		    orig_funcs = findFuncByName(appImage, fname);
		fprintf(stderr, "Replacing %lu occurances of %s\n", orig_funcs.size(), fname);
		for(BPatch_function * fun : orig_funcs) {
			if (storage->replaceFuncs.find(fname) != storage->replaceFuncs.end()) {
				// Wholesale replace this function with the one in the library.
				std::vector<BPatch_function *> rep_funcs = findFuncByName(appImage, storage->replaceFuncs[fname].first);
				if (rep_funcs.size() == 0) {
					fprintf(stderr, "%s: %s\n", "Could not find the function", fname);
					break;
				}
				if (rep_funcs.size() > 1) {
					fprintf(stderr, "%s: %s\n", "We found more than one of the following replacement function", fname);
					fprintf(stderr, "%s\n", "You may (and likely will) get undesired results....");
				}
				// Replace the function
				app->replaceFunction(*fun, *rep_funcs[0]);
			} else if(storage->wrapFunctions.find(fname) != storage->wrapFunctions.end()) {
				// Wrap function, pointing the original function call to wrapper_hookName
				std::vector<BPatch_function *> rep_funcs = findFuncByName(appImage, std::get<0>(storage->wrapFunctions[fname]));
				if (rep_funcs.size() == 0) {
					fprintf(stderr, "%s: %s\n", "Could not find the function", fname);
					break;
				}
				if (rep_funcs.size() > 1) {
					fprintf(stderr, "%s: %s\n", "We found more than one of the following wrapper function", fname);
					fprintf(stderr, "%s\n", "You may (and likely will) get undesired results....");
				}				

				// This needs to be made more efficient....
				// bool found = false;
				// std::vector<Symbol *> all_symbols;
				// Dyninst::SymtabAPI::Module *symtab =  Dyninst::SymtabAPI::convert(rep_funcs[0]->getModule());
				// symtab->getAllSymbols(all_symbols);
				// std::string wrapName = std::string(std::get<2>(storage->wrapFunctions[fname]));
				// for (Symbol * sym : all_symbols) {
				// 	if (sym->getPrettyName() == wrapName) {
				// 		app->wrapFunction(fun, rep_funcs[0], sym);
				// 		found = true;
				// 		break;
				// 	}
				// }
				// 
				bool found = false;
				std::vector<BPatch_object *> objects;
				appImage->getObjects(objects);
				std::string wrapName = std::string(std::get<2>(storage->wrapFunctions[fname]));
				for (auto mp : objects) {
					Dyninst::SymtabAPI::Symtab *symtab =  Dyninst::SymtabAPI::convert(mp);
					std::vector<Symbol *> all_symbols;	
					symtab->getAllSymbols(all_symbols);
					
					for (Symbol * sym : all_symbols) {
						if (sym->getPrettyName() == wrapName) {
							app->wrapFunction(fun, rep_funcs[0], sym);
							found = true;
							break;
						}
					}
					if (found == true)
						break;
				}
				
				// appImage
				// Dyninst::SymtabAPI::Module *symtab =  Dyninst::SymtabAPI::convert(rep_funcs[0]->getModule());
				// symtab->getAllSymbols(all_symbols);
				// std::string wrapName = std::string(std::get<2>(storage->wrapFunctions[fname]));
				// for (Symbol * sym : all_symbols) {
				// 	if (sym->getPrettyName() == wrapName) {
				// 		app->wrapFunction(fun, rep_funcs[0], sym);
				// 		found = true;
				// 		break;
				// 	}
				// }
				if (found == false) {
					fprintf(stderr, "%s: %s\n", "Could not find the wrapper function symbol for" , wrapName.c_str());
				}
			} else if (storage->InsertAtEntry.find(fname) != storage->InsertAtEntry.end()){
				// Insert a function at entry
				std::vector<BPatch_function *> rep_funcs = findFuncByName(appImage, std::get<0>(storage->InsertAtEntry[fname]));
				if (rep_funcs.size() == 0) {
					fprintf(stderr, "%s: %s\n", "Could not find the function", fname);
					break;
				}
				BPatch_funcCallExpr beforeExec(*(rep_funcs[0]),  std::vector<BPatch_snippet*>());
				BPatch_Vector<BPatch_point *> * entry_points;
				entry_points = fun->findPoint(BPatch_entry);		
				if (!app->insertSnippet(beforeExec, *entry_points)){
					fprintf(stderr, "%s %s %s %s\n", "Could not insert ", std::get<0>(storage->InsertAtEntry[fname]) , " before call ", fname);
				}					
			} 
		}
	}

	// Replace all functions by Prefix
	char * bin_func;
	char * wrap_func; 
	char * wrap_lib;
	for (auto wrap_all : storage->wrapAllFunctions) {
		std::tie(wrap_func, wrap_lib) = wrap_all.second;
		std::vector<BPatch_function *> rep_funcs = findFuncByName(appImage, wrap_func);
		if (rep_funcs.size() == 0) {
			fprintf(stderr, "%s: %s\n", "Could not find the function", wrap_func);
			break;
		}
		if (rep_funcs.size() > 1) {
			fprintf(stderr, "%s: %s\n", "We found more than one of the following wrapper function", wrap_func);
			fprintf(stderr, "%s\n", "You may (and likely will) get undesired results....");
		}			
		bin_func = (char *) wrap_all.first;
		BPatch_funcCallExpr beforeExec(*(rep_funcs[0]),  std::vector<BPatch_snippet*>());

		fprintf(stderr, "Trying to replace all with the following regex: %s\n", bin_func);
		std::vector<BPatch_function *> reps = findFuncByNameRegEx(appImage, (const char*)bin_func);
		for (BPatch_function * inst_f : reps) {
			BPatch_Vector<BPatch_point *> entry_points;
			inst_f->getEntryPoints(entry_points);
			if (!app->insertSnippet(beforeExec, entry_points)){
				fprintf(stderr, "%s %s %s %s\n", "Could not insert ", wrap_func, " before call ", bin_func);
			}
		}
	}

	fprintf(stderr, "Writing output binary to %s\n", outputName);
	if(!app->writeFile(outputName)) {
		fprintf(stderr, "Could not write output file %s\n", outputName);
		return -1;
	}
	fprintf(stderr, "Output binary written to %s\n", outputName);
	return 0;
}

//	int PerformRewrite(char * outputName);