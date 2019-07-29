#include <cassert>

extern "C" {
int SynchronizationWrapper(void * a, void * b, void * c) {
	assert(1 == 0);
};
}