##
# Verify the integreity of the result based on a prior run
# Used to test that changes have not broke functionality without parsing insane log files
# Checks the following files in the following ways:
#
# TF_timekey.bin -> md5sum (contains stacktraces)
# TF_timekey.txt -> md5sum (contains stacktraces)
# TF_trace.bin -> Custom (checks element count, stack ids, discards time which may vary by run)
# DSTIME_stacks.bin -> md5sum (contains stacktraces)
# DCPUTIME_trace.bin -> custom (discards time)
# DSTIME_trace.bin -> custom (discards time)
# DTOTIME_trace.bin -> custom (discatds time)
# DT_stacks.bin -> md5sum (contains stacktraces)
# DT_collisions.txt -> md5sum
# CS_StackFile.txt -> md5sum
# CS_HumanStacks.txt -> md5sum
# LS_stackkey.txt -> md5sum
# LS_tracekey.txt -> md5sum
# LS_stackkey.bin -> md5sum
# LS_trace.bin -> md5sum
import sys
import md5
import os

checkWithMD5 = ["TF_timekey.bin", "TF_timekey.txt","DSTIME_stacks.bin", "DT_stacks.bin","DT_collisions.txt","CS_StackFile.txt","CS_HumanStacks.txt", "LS_stackkey.txt", "LS_tracekey.txt","LS_stackkey.bin","LS_trace.bin"]

newRunFolder = sys.argv[1]
baselineFolder = sys.argv[2]

def ReturnHash(file):
	f = open(file,"rb")
	m = md5.new()
	m.update(f.read())
	f.close()
	return m.digest()

for x in checkWithMD5:
	newHash = ReturnHash(os.path.join(newRunFolder, x))
	oldHash = ReturnHash(os.path.join(baselineFolder, x))
	if oldHash == newHash:
		print x + " ---- OK"
	else:
		print x + " ---- HASH MISMATCH!"

