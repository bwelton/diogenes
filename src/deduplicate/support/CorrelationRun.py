from FileClasses import ReadDatFiles, CorrelateTimlineWithTiming
import sys

if __name__ == "__main__":
	x = None
	if len(sys.argv) > 2:
		x = ReadDatFiles(sys.argv[1], sys.argv[2], sys.argv[3])
	else:
		x = ReadDatFiles("dedup_timeline.txt", "timing_info.txt", "timing_packet_corr.txt")
	tmp = CorrelateTimlineWithTiming(x.GetLists())
	tmp.Correlate()
	ret = tmp.CalculateTS()
	if len(sys.argv) > 4:
		f = open(sys.argv[4],"w")
		f.write(str(ret[0]) + "," + str(ret[1]))
		f.close()
		