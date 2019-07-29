#include "CalculateDedupSavings.h"

int main(int argc, char * argv[]) {
	if (argc < 4) {
		std::cerr << "Expected parameters: Timeline_File Correlation_file Timing_file" << std::endl;
		return -1;
	}
	CalculateDedupSavings x(argv[1], argv[2], argv[3]);
	std::vector<TimelineRec> timeline;
	std::vector<CorrelationRec> correction;
	std::vector<TimingRec> timing;
	double finalTime = 0.0;
	x.ReadTiming(timing, finalTime);
	x.ReadTimeline(timeline);
	x.ReadCorrelation(correction);
	std::vector<CombinedRecord> combo;
	x.CombineTimelineCorrelation(timeline, correction, combo);
	std::pair<uint64_t, uint64_t> est = x.GenerateEstimate(timing, combo);
	std::cout << est.first  << "," << est.second << "," << finalTime << std::endl;
}