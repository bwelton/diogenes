#define BOOST_TEST_MODULE timeline_checker_test
#define BOOST_TEST_MAIN
#include "BaseTesting.h"
#include "TimelineChecker.h"

BOOST_AUTO_TEST_SUITE( timeline_checker )

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
	TimelineMap tmp = ParseTimeline();

	for (int i = 1; i < 21; i++) {
		//typedef std::map<int, std::tuple<uint32_t, size_t, std::string, int> >  TimelineMap;
		if (tmp.find(i) == tmp.end()){
			fprintf(stderr, "%s %d\n", "Could not find", i );
			BOOST_FAIL("Could not find one of the outputs we expect...");
		}
		BOOST_CHECK_EQUAL(std::get<0>(tmp[i]), hashes[i-1]);
		BOOST_CHECK_EQUAL(std::get<1>(tmp[i]), size_t(sizes[i-1]) * sizeof(double));
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i]), std::string("HostToDevice"));
		BOOST_CHECK_EQUAL(std::get<3>(tmp[i]), 0);
	}
}


BOOST_AUTO_TEST_CASE(test_duplicate_sends) {
	TestingFixture t;
	std::vector<std::shared_ptr<double> > dataPtrs;
	std::vector<int> sizes;
	std::vector<uint32_t> hashes;
	int tmp_send = (int) (rand() % 4096) + 1024;
	std::shared_ptr<double> dt = std::shared_ptr<double>((double*)GenRandom(tmp_send * sizeof(double)), free);
	for (int i = 0; i < 20; i++) {
		sizes.push_back(tmp_send);
		dataPtrs.push_back(dt);
		hashes.push_back(GetHash<double>(dataPtrs[i].get(), tmp_send));
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(UniqueCopyTest, dataPtrs, sizes)) == 0);

	//s <<  globalId << "," << id << "," << size << "," << type << "," << origTransfer << std::endl;
	TimelineMap tmp = ParseTimeline();

	for (int i = 1; i < 21; i++) {
		//typedef std::map<int, std::tuple<uint32_t, size_t, std::string, int> >  TimelineMap;
		if (tmp.find(i) == tmp.end()){
			fprintf(stderr, "%s %d\n", "Could not find", i );
			BOOST_FAIL("Could not find one of the outputs we expect...");
		}
		BOOST_CHECK_EQUAL(std::get<0>(tmp[i]), hashes[i-1]);
		BOOST_CHECK_EQUAL(std::get<1>(tmp[i]), size_t(sizes[i-1]) * sizeof(double));
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i]), std::string("HostToDevice"));
		if (i - 1 == 0)
			BOOST_CHECK_EQUAL(std::get<3>(tmp[i]), 0);
		else
			BOOST_CHECK_EQUAL(std::get<3>(tmp[i]), 1);
	}
}

BOOST_AUTO_TEST_CASE(test_duplicate_sends_alternate) {
	TestingFixture t;
	std::vector<std::shared_ptr<double> > dataPtrs;
	std::vector<int> sizes;
	std::vector<uint32_t> hashes;
	for (int i = 0; i < 20; i++) {
		int tmp = (int) (rand() % 4096) + 1024;
		if (i != 0 && i % 2 == 0){
			sizes.push_back(sizes[i-1]);
			dataPtrs.push_back(dataPtrs[i-1]);
		}
		else{
			sizes.push_back(tmp);
			dataPtrs.push_back(std::shared_ptr<double>((double*)GenRandom(tmp * sizeof(double)), free));
		}
		hashes.push_back(GetHash<double>(dataPtrs[i].get(), sizes[i]));
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(UniqueCopyTest, dataPtrs, sizes)) == 0);

	//s <<  globalId << "," << id << "," << size << "," << type << "," << origTransfer << std::endl;
	TimelineMap tmp = ParseTimeline();

	for (int i = 1; i < 21; i++) {
		//typedef std::map<int, std::tuple<uint32_t, size_t, std::string, int> >  TimelineMap;
		if (tmp.find(i) == tmp.end()){
			fprintf(stderr, "%s %d\n", "Could not find", i );
			BOOST_FAIL("Could not find one of the outputs we expect...");
		}
		if (i - 1 != 0 && (i -1) % 2 == 0) {
			BOOST_CHECK_EQUAL(std::get<0>(tmp[i]), hashes[i-2]);
			BOOST_CHECK_EQUAL(std::get<1>(tmp[i]), size_t(sizes[i-2]) * sizeof(double));
			BOOST_CHECK_EQUAL(std::get<2>(tmp[i]), std::string("HostToDevice"));
			BOOST_CHECK_EQUAL(std::get<3>(tmp[i]), i-1);
		} else {
			BOOST_CHECK_EQUAL(std::get<0>(tmp[i]), hashes[i-1]);
			BOOST_CHECK_EQUAL(std::get<1>(tmp[i]), size_t(sizes[i-1]) * sizeof(double));
			BOOST_CHECK_EQUAL(std::get<2>(tmp[i]), std::string("HostToDevice"));
			BOOST_CHECK_EQUAL(std::get<3>(tmp[i]), 0);
		}
	}
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(UniqueCopyTest, dataPtrs, sizes)) == 0);
}

void SendRecv(std::vector<std::shared_ptr<double> > data, std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	for (int i = 0; i < data.size(); i++) {
		DeviceMemory<double> dev(data[i], size_t(sizes[i]));
		dev.WriteSync();
		dev.ReadSync();
	}
}


BOOST_AUTO_TEST_CASE(test_send_recv_duplicates) {
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
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(SendRecv, dataPtrs, sizes)) == 0);

	//s <<  globalId << "," << id << "," << size << "," << type << "," << origTransfer << std::endl;
	TimelineMap tmp = ParseTimeline();

	for (int i = 1; i < 40; i += 2) {
		//typedef std::map<int, std::tuple<uint32_t, size_t, std::string, int> >  TimelineMap;
		if (tmp.find(i) == tmp.end() || tmp.find(i+1) == tmp.end()){
			fprintf(stderr, "%s %d\n", "Could not find", i );
			BOOST_FAIL("Could not find one of the outputs we expect...");
		}
		BOOST_CHECK_EQUAL(std::get<0>(tmp[i]), hashes[i/2]);
		BOOST_CHECK_EQUAL(std::get<1>(tmp[i]), size_t(sizes[i/2]) * sizeof(double));
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i]), std::string("HostToDevice"));
		BOOST_CHECK_EQUAL(std::get<3>(tmp[i]), 0);
		BOOST_CHECK_EQUAL(std::get<0>(tmp[i+1]), hashes[i/2]);
		BOOST_CHECK_EQUAL(std::get<1>(tmp[i+1]), size_t(sizes[i/2]) * sizeof(double));
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i+1]), std::string("DeviceToHost"));
		BOOST_CHECK_EQUAL(std::get<3>(tmp[i+1]), i);			
	}
}


void SendRecvSend(std::vector<std::shared_ptr<double> > data, std::vector<int> sizes) {
	RedirectOutputToFile();
	std::shared_ptr<CudaCtx> tmp(new CudaCtx());
	for (int i = 0; i < data.size(); i++) {
		DeviceMemory<double> dev(data[i], size_t(sizes[i]));
		dev.WriteSync();
		dev.ReadSync();
		dev.WriteSync();
	}
}

BOOST_AUTO_TEST_CASE(test_send_recv_send_duplicates) {
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
	BOOST_REQUIRE(t.LaunchAndWait(boost::bind(SendRecvSend, dataPtrs, sizes)) == 0);

	//s <<  globalId << "," << id << "," << size << "," << type << "," << origTransfer << std::endl;
	TimelineMap tmp = ParseTimeline();

	for (int i = 1; i < 60; i += 3) {
		//typedef std::map<int, std::tuple<uint32_t, size_t, std::string, int> >  TimelineMap;
		if (tmp.find(i) == tmp.end()){
			fprintf(stderr, "%s %d\n", "Could not find", i );
			BOOST_FAIL("Could not find one of the outputs we expect...");
		}
		BOOST_CHECK_EQUAL(std::get<0>(tmp[i]), hashes[i/3]);
		BOOST_CHECK_EQUAL(std::get<1>(tmp[i]), size_t(sizes[i/3]) * sizeof(double));
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i]), std::string("HostToDevice"));
		BOOST_CHECK_EQUAL(std::get<3>(tmp[i]), 0);
		BOOST_CHECK_EQUAL(std::get<0>(tmp[i+1]), hashes[i/3]);
		BOOST_CHECK_EQUAL(std::get<1>(tmp[i+1]), size_t(sizes[i/3]) * sizeof(double));
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i+1]), std::string("DeviceToHost"));
		BOOST_CHECK_EQUAL(std::get<3>(tmp[i+1]), i);
		BOOST_CHECK_EQUAL(std::get<0>(tmp[i+2]), hashes[i/3]);
		BOOST_CHECK_EQUAL(std::get<1>(tmp[i+2]), size_t(sizes[i/3]) * sizeof(double));
		BOOST_CHECK_EQUAL(std::get<2>(tmp[i+2]), std::string("HostToDevice"));
		BOOST_CHECK_EQUAL(std::get<3>(tmp[i+2]), i);
	}
}

BOOST_AUTO_TEST_SUITE_END()