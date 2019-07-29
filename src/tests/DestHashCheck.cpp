#define BOOST_TEST_MODULE dest_hash_check
#define BOOST_TEST_MAIN
#include "BaseTesting.h"
#include "DestHashCheck.h"

BOOST_AUTO_TEST_SUITE( dest_hash_checking )

void UniqueCopyTest(std::vector<std::shared_ptr<double> > data, std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	for (int i = 0; i < data.size(); i++) {
		DeviceMemory<double> dev(data[i], size_t(sizes[i]));
		dev.WriteSync();
	}
}

BOOST_AUTO_TEST_CASE(test_unique) {
	TestingFixture t;
	std::vector<std::shared_ptr<double> > dataPtrs;
	std::vector<int> sizes;
	std::vector<uint32_t> hashes;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		dataPtrs.push_back(std::shared_ptr<double>((double*)GenRandom(tmp * sizeof(double)), free));
		hashes.push_back(GetHash<double>(dataPtrs[i].get(), tmp));
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(UniqueCopyTest, dataPtrs, sizes)) == 0);

	//s <<  globalId << "," << id << "," << size << "," << type << "," << origTransfer << std::endl;
	DestHashMap tmp = ParseDestHash();
	//typedef std::map<int, std::tuple<std::string, uint32_t, uint32_t, size_t> >  DestHashMap;
	for (int i = 1; i < 21; i++) {
		//typedef std::map<int, std::tuple<uint32_t, size_t, std::string, int> >  TimelineMap;
		if (tmp.find(i) == tmp.end()){
			fprintf(stderr, "%s %d\n", "Could not find", i );
			BOOST_FAIL("Could not find one of the outputs we expect...");
		}
		BOOST_CHECK_EQUAL(std::get<0>(tmp[i]), std::string("diff"));
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i]), hashes[i-1]);
		BOOST_CHECK_EQUAL(std::get<3>(tmp[i]), size_t(sizes[i-1]) * sizeof(double));		
	}
}

void IdentDataCPU(std::vector<std::shared_ptr<double> > data, std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	for (int i = 0; i < data.size(); i++) {
		DeviceMemory<double> dev(data[i], size_t(sizes[i]));
		dev.WriteSync();
		dev.ReadSync();
	}
}

BOOST_AUTO_TEST_CASE(test_duplicate_cpu) {
	TestingFixture t;
	std::vector<std::shared_ptr<double> > dataPtrs;
	std::vector<int> sizes;
	std::vector<uint32_t> hashes;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		dataPtrs.push_back(std::shared_ptr<double>((double*)GenRandom(tmp * sizeof(double)), free));
		hashes.push_back(GetHash<double>(dataPtrs[i].get(), tmp));
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(IdentDataCPU, dataPtrs, sizes)) == 0);

	//s <<  globalId << "," << id << "," << size << "," << type << "," << origTransfer << std::endl;
	DestHashMap tmp = ParseDestHash();
	//typedef std::map<int, std::tuple<std::string, uint32_t, uint32_t, size_t> >  DestHashMap;
	for (int i = 1; i < 40; i += 2) {
		//typedef std::map<int, std::tuple<uint32_t, size_t, std::string, int> >  TimelineMap;
		if (tmp.find(i) == tmp.end()){
			fprintf(stderr, "%s %d\n", "Could not find", i );
			BOOST_FAIL("Could not find one of the outputs we expect...");
		}
		BOOST_CHECK_EQUAL(std::get<0>(tmp[i]), std::string("diff"));
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i]), hashes[i/2]);
		BOOST_CHECK_EQUAL(std::get<3>(tmp[i]), size_t(sizes[i/2]) * sizeof(double));		

		BOOST_CHECK_EQUAL(std::get<0>(tmp[i+1]), std::string("same"));
		BOOST_CHECK_EQUAL(std::get<1>(tmp[i+1]), hashes[i/2]);
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i+1]), hashes[i/2]);
		BOOST_CHECK_EQUAL(std::get<3>(tmp[i+1]), size_t(sizes[i/2]) * sizeof(double));		
	}
}

void IdentDataGPU(std::vector<std::shared_ptr<double> > data, std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	for (int i = 0; i < data.size(); i++) {
		DeviceMemory<double> dev(data[i], size_t(sizes[i]));
		dev.WriteSync();
		dev.WriteSync();
	}
}

BOOST_AUTO_TEST_CASE(test_duplicate_gpu) {
	TestingFixture t;
	std::vector<std::shared_ptr<double> > dataPtrs;
	std::vector<int> sizes;
	std::vector<uint32_t> hashes;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		sizes.push_back(tmp);
		dataPtrs.push_back(std::shared_ptr<double>((double*)GenRandom(tmp * sizeof(double)), free));
		hashes.push_back(GetHash<double>(dataPtrs[i].get(), tmp));
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(IdentDataGPU, dataPtrs, sizes)) == 0);

	//s <<  globalId << "," << id << "," << size << "," << type << "," << origTransfer << std::endl;
	DestHashMap tmp = ParseDestHash();
	//typedef std::map<int, std::tuple<std::string, uint32_t, uint32_t, size_t> >  DestHashMap;
	for (int i = 1; i < 40; i += 2) {
		//typedef std::map<int, std::tuple<uint32_t, size_t, std::string, int> >  TimelineMap;
		if (tmp.find(i) == tmp.end()){
			fprintf(stderr, "%s %d\n", "Could not find", i );
			BOOST_FAIL("Could not find one of the outputs we expect...");
		}
		BOOST_CHECK_EQUAL(std::get<0>(tmp[i]), std::string("diff"));
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i]), hashes[i/2]);
		BOOST_CHECK_EQUAL(std::get<3>(tmp[i]), size_t(sizes[i/2]) * sizeof(double));
		BOOST_CHECK_EQUAL(std::get<0>(tmp[i+1]), std::string("same"));
		BOOST_CHECK_EQUAL(std::get<1>(tmp[i+1]), hashes[i/2]);
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i+1]), hashes[i/2]);
		BOOST_CHECK_EQUAL(std::get<3>(tmp[i+1]), size_t(sizes[i/2]) * sizeof(double));		
	}
}

BOOST_AUTO_TEST_SUITE_END()