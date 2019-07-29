import shutil
import sys
import os
outputFiles = ["TF_trace.bin","DCPUTIME_trace.bin","DSTIME_trace.bin","DTOTIME_trace.bin","TF_timekey.bin", "TF_timekey.txt","DSTIME_stacks.bin", "DT_stacks.bin","DT_collisions.txt","CS_StackFile.txt","CS_HumanStacks.txt", "LS_stackkey.txt", "LS_tracekey.txt","LS_stackkey.bin","LS_trace.bin"]
logFiles = ["FI_log.txt","IT_log.txt","LS_log.txt","synch_log.out"]
appSpecific = [".apitrace.log",".datatransfer.log",".firstUse.log",".stacktrace.log",".synchronization.log",".time.log",".timetransfer.log"]

inDirectory = sys.argv[1]
outDirectory = sys.argv[2]

for x in outputFiles:
	shutil.move(os.path.join(inDirectory, x), os.path.join(outDirectory,x))

for x in logFiles:
	shutil.move(os.path.join(inDirectory, x), os.path.join(outDirectory,x))

tmp = os.listdir(inDirectory)

for x in tmp:
	for y in appSpecific:
		if y in x:
			shutil.move(os.path.join(inDirectory, x), os.path.join(outDirectory,x))