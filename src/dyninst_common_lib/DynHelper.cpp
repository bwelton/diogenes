#include "DynHelper.h"

// Helper functions
std::vector<BPatch_object *> DynHelper_GetObjects(BPatch_addressSpace * aspace) {
	BPatch_image * img = aspace->getImage();
	std::vector<BPatch_object *> objects;
	img->getObjects(objects);
	return objects;
}

bool DynHelper_CheckIfFileExists (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}
