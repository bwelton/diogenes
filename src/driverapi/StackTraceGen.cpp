#include "StackTraceGen.h"

std::string StackTraceGen::GenStackTrace() {
	std::stringstream ret; 	
	std::vector<Frame> stackwalk;
	std::string s;
	void * sym; 
	Walker * walker = Walker::newWalker();
	walker->walkStack(stackwalk);
	Dyninst::Offset offset;
	for (int i = 0; i < stackwalk.size(); i++) {
		stackwalk[i].getName(s);
		ret << s << " - ";
		if(stackwalk[i].getLibOffset(s, offset, sym) == false){
		 	continue;
		}
		Symtab * curSym = static_cast<Symtab *>(sym);
		if (sym == NULL) {
			ret << "\n";
			continue;
		}
		std::vector<Statement *> lines;
		if((curSym)->getSourceLines(lines,offset) == false) {
			ret << "\n";
			continue;
		}
		for (int q = 0; q < lines.size(); q++) {
			ret << lines[q]->getFile() << ":" << std::to_string(lines[q]->getLine()) << " ";
		}
		ret << "\n";
	}
	ret << "\n";
	return ret.str();
}

int StackTraceGen::PerformAction(DriverAPICall t, std::shared_ptr<ParameterBase> params) {
	std::cout << GenStackTrace() << std::endl;
	return 0;
}

StackTraceGen::StackTraceGen() {

}

StackTraceGen::~StackTraceGen() {

}