import os
import sys
import struct

f = open(sys.argv[1], "rb")
d = f.read()
f.close()

length = int(len(d) / 8)
seperator = int(sys.argv[2])
hex_output = ""
int_output = ""
for x in range(0, length):
	tmp = struct.unpack_from("Q", d, offset=(x*8))
	hex_output += str(format(tmp[0], "016x")) + " "
	int_output += str(format(tmp[0], "032d")) + " "
	if (x % seperator == 0):
		hex_output += "\n"
		int_output += "\n"

print "Hex output:"
print hex_output

print "\nInteger Output"
print int_output


