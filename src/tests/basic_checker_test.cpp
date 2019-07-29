#define BOOST_TEST_MODULE basic_checker_test
#define BOOST_TEST_MAIN
#include "BaseTesting.h"


void BasicLaunchTest(void) {
	// A sanity check to make sure that we can launch a cuda program
	std::shared_ptr<CudaCtx> tmp;
	tmp.reset(new CudaCtx());
}

BOOST_AUTO_TEST_SUITE( basic_checker )

BOOST_AUTO_TEST_CASE(launch_test) {
	// SANITY CHECK: Checks to ensure that we libcuda was instrimented correctly.
	// If this fails, something is wrong in the environment
	TestingFixture t;
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(BasicLaunchTest)) == 0);

}

void BasicCopyTest(void) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	DeviceMemory<double> dev(2048);
	dev.WriteSync();
	if (dev.CompareDevToCPU() == false) {
		exit(-1);
	}
}

BOOST_AUTO_TEST_CASE(basic_copy_test) {
	// SANITY CHECK: Checks to make sure that we can copy something to/from the GPU.
	// If this fails, something is wrong in the environment
	TestingFixture t;
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(BasicCopyTest)) == 0);
}

void CheckWriteSyncSize(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	int count = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.WriteSync();
	}
}


BOOST_AUTO_TEST_CASE(check_write_size_sync) {
	// Checks to make sure the interceptor is getting the size correct
	// for each write transfer call.
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckWriteSyncSize, sizes)) == 0);
	std::pair<int, int> ret = ParseTotalTransferAndSize();
	std::cerr << "Returned first value: " << ret.first << " returned second value: " << ret.second << std::endl;
	std::cerr << "Expected first value: " << 20 << " expected second value: " << expected << std::endl;
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);
	exit(-1);
}

void CheckWriteASyncSize(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.WriteAsync(tmp);
	}
}


BOOST_AUTO_TEST_CASE(check_write_size_async) {
	// Checks to make sure the interceptor is getting the size correct
	// for each write transfer call.
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckWriteASyncSize, sizes)) == 0);
	std::pair<int, int> ret = ParseTotalTransferAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);
}

void CheckWriteCombinedSize(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		if (i % 2 == 0)
			dev.WriteAsync(tmp);
		else
			dev.WriteSync();
	}
}

BOOST_AUTO_TEST_CASE(check_write_size_combined) {
	// Checks to make sure the interceptor is getting the size correct
	// for each write transfer call.
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckWriteCombinedSize, sizes)) == 0);
	std::pair<int, int> ret = ParseTotalTransferAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);
}

void CheckReadSyncSize(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.ReadSync();
	}
}

BOOST_AUTO_TEST_CASE(check_read_size_sync) {
	// Checks to make sure the interceptor is getting the size correct
	// for each read transfer call.
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckReadSyncSize, sizes)) == 0);
	std::pair<int, int> ret = ParseTotalTransferAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);
}


void CheckReadASyncSize(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.ReadAsync(tmp);
	}
}

BOOST_AUTO_TEST_CASE(check_read_size_async) {
	// Checks to make sure the interceptor is getting the size correct
	// for each read transfer call.
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckReadASyncSize, sizes)) == 0);
	std::pair<int, int> ret = ParseTotalTransferAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);
}

void CheckReadCombinedSize(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		if (i % 2)
			dev.ReadAsync(tmp);
		else
			dev.ReadSync();
	}
}

BOOST_AUTO_TEST_CASE(check_read_size_combined) {
	// Checks to make sure the interceptor is getting the size correct
	// for each read transfer call.
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckReadCombinedSize, sizes)) == 0);
	std::pair<int, int> ret = ParseTotalTransferAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);
}

void CheckReadDuplicates(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.ReadSync();
		// Second read to get "duplicate" data.
		dev.ReadSync();
	}
}

BOOST_AUTO_TEST_CASE(check_read_duplicates) {
	// Check that read duplicates are being detected.
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckReadDuplicates, sizes)) == 0);
	std::pair<int, int> ret = ParseCollisionCountAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);	
}

void CheckReadAsyncDuplicates(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.ReadAsync(tmp);
		// Second read to get "duplicate" data.
		dev.ReadAsync(tmp);
	}
}

BOOST_AUTO_TEST_CASE(check_read_async_duplicates) {
	// Check that read duplicates are being detected.
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckReadAsyncDuplicates, sizes)) == 0);
	std::pair<int, int> ret = ParseCollisionCountAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);	
}

void CheckReadAsyncStreamDuplicates(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	std::shared_ptr<CudaCtx> tmp2(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.ReadAsync(tmp);
		// Second read to get "duplicate" data.
		dev.ReadAsync(tmp2);
	}
}

BOOST_AUTO_TEST_CASE(check_read_async_streams_duplicates) {
	// Check that read duplicates are being detected on different streams. 
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckReadAsyncStreamDuplicates, sizes)) == 0);
	std::pair<int, int> ret = ParseCollisionCountAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);	
}

void CheckWriteDuplicates(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.WriteSync();
		// Second write to get "duplicate" data.
		dev.WriteSync();
	}
}

BOOST_AUTO_TEST_CASE(check_write_duplicates) {
	// Check that read duplicates are being detected on different streams. 
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckWriteDuplicates, sizes)) == 0);
	std::pair<int, int> ret = ParseCollisionCountAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);	
}

void CheckWriteAsyncDuplicates(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.WriteAsync(tmp);
		// Second write to get "duplicate" data.
		dev.WriteAsync(tmp);
	}
}

BOOST_AUTO_TEST_CASE(check_write_async_duplicates) {
	// Check that read duplicates are being detected on different streams. 
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckWriteAsyncDuplicates, sizes)) == 0);
	std::pair<int, int> ret = ParseCollisionCountAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);	
}


void CheckWriteAsyncStreamDuplicates(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	std::shared_ptr<CudaCtx> tmp2(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.WriteAsync(tmp);
		// Second write to get "duplicate" data.
		dev.WriteAsync(tmp2);
	}
}

BOOST_AUTO_TEST_CASE(check_write_async_streams_duplicates) {
	// Check that read duplicates are being detected on different streams. 
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckWriteAsyncStreamDuplicates, sizes)) == 0);
	std::pair<int, int> ret = ParseCollisionCountAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);	
}

void CheckReadWriteDuplicates(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.WriteSync();
		// Second write to get "duplicate" data.
		dev.ReadSync();
	}
}

BOOST_AUTO_TEST_CASE(check_readwrite_duplicates) {
	// Check that read duplicates are being detected on different streams. 
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckReadWriteDuplicates, sizes)) == 0);
	std::pair<int, int> ret = ParseCollisionCountAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);	
}

void CheckReadWriteAsyncDuplicates(std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	uint64_t expected_size = 0;
	for (auto i : sizes) {
		DeviceMemory<double> dev(i);
		dev.WriteAsync(tmp);
		// Second write to get "duplicate" data.
		dev.ReadAsync(tmp);
	}
}

BOOST_AUTO_TEST_CASE(check_readwrite_async_duplicates) {
	// Check that read duplicates are being detected on different streams. 
	TestingFixture t;
	std::vector<int> sizes;
	int expected = 0;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		// Double is the testing datatype size
		expected += tmp * sizeof(double);
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(CheckReadWriteAsyncDuplicates, sizes)) == 0);
	std::pair<int, int> ret = ParseCollisionCountAndSize();
	BOOST_CHECK(ret.first == 20);
	BOOST_CHECK(ret.second == expected);	
}

BOOST_AUTO_TEST_SUITE_END()