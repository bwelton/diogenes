#define BOOST_TEST_MODULE cupti_test
#define BOOST_TEST_MAIN
#include "BaseTesting.h"

BOOST_AUTO_TEST_SUITE( basic_copy_cupti )

// void BasicCopyTest(void) {
// 	RedirectOutputToFile();
// 	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
// 	DeviceMemory<double> dev(2048*30);
// 	dev.WriteAsync(tmp);
// 	dev.WriteAsync(tmp);
// 	dev.WriteAsync(tmp);
// 	dev.ReadAsync(tmp);
// 	dev.WriteSync();
// 	dev.ReadAsync(tmp);
// 	dev.WriteAsync(tmp);
// 	dev.ReadAsync(tmp);
// 	dev.WriteAsync(tmp);
// 	dev.ReadSync();
// 	// if (dev.CompareDevToCPU() == false) {
// 	// 	exit(-1);
// 	// }
// }

// BOOST_AUTO_TEST_CASE(basic_copy_test) {
// 	// SANITY CHECK: Checks to make sure that we can copy something to/from the GPU.
// 	// If this fails, something is wrong in the environment
// 	TestingFixture t;
// 	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(BasicCopyTest)) == 0);
// }

void SynchTiming(void) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	DeviceMemory<double> dev(1);
	dev.WriteAsync(tmp);
	tmp.get()->SynchronizeStream();
	tmp.get()->SynchronizeStream();
	tmp.get()->SynchronizeStream();
	tmp.get()->SynchronizeStream();
	tmp.get()->DeviceSynchronization();	
	tmp.get()->DeviceSynchronization();
	tmp.get()->DeviceSynchronization();
	tmp.get()->DeviceSynchronization();
	tmp.get()->CtxSynchronization();
	tmp.get()->CtxSynchronization();
	tmp.get()->CtxSynchronization();
	tmp.get()->CtxSynchronization();

	dev.WriteAsync(tmp);
	dev.WriteAsync(tmp);
	dev.ReadAsync(tmp);
	dev.ReadAsync(tmp);
	dev.ReadAsync(tmp);
	tmp.get()->DeviceSynchronization();
	// if (dev.CompareDevToCPU() == false) {
	// 	exit(-1);
	// }
}

BOOST_AUTO_TEST_CASE(synch_timing) {
	// SANITY CHECK: Checks to make sure that we can copy something to/from the GPU.
	// If this fails, something is wrong in the environment
	TestingFixture t;
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(SynchTiming)) == 0);
}



BOOST_AUTO_TEST_SUITE_END()