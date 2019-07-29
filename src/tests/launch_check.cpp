#define BOOST_TEST_MODULE launch_check
#define BOOST_TEST_MAIN
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <boost/test/unit_test.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "CudaWrapper.h"
#include "DriverAPIHeader.h"
// Tests to ensure that the rewritten binary is actually functional
// This is needed because core files wont be generated if there is a segfault
// in the wrapper.
BOOST_AUTO_TEST_SUITE( launch_checker )
BOOST_AUTO_TEST_CASE(checkinit) {
	CheckInit();
	//CheckInit_DriverAPI();
}


BOOST_AUTO_TEST_SUITE_END()