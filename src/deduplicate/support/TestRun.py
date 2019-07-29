import sys
from ReadCUPTIEvents import ReadCUPTIEvents
from ReadTimingEvents import ReadTimingPacket
from MatchIDs import MatchIDs
from ReadTimeline import ReadTimeline
ReadEvents = ReadCUPTIEvents(sys.argv[1])
ReadTimingLog = ReadTimingPacket(sys.argv[2])
match = MatchIDs(ReadEvents.Process(),ReadTimingLog.Process())
match.Process()

timeline = ReadTimeline(sys.argv[3])
timlineEvents = timeline.Process()