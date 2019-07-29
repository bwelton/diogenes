from FileClasses import ReadDatFiles
from ProcessSplit import SplitByProcess
from GenCorrelation import GenCorrelation
from CalculateTS import CalculateTS

import sys

if __name__ == "__main__":
	x = ReadDatFiles("dedup_timeline.txt", "timing_info.txt", "timing_packet_corr.txt")
	iters = x.GetIterators()
	procs = SplitByProcess(*iters)
	procs = procs.GetProcesses()
	print procs
	for x in procs:
		t = GenCorrelation(x.GetTimelines()).CorrelateIDs()
		CalculateTS(t).CalculateTimeSavings(x.GetTimelines())