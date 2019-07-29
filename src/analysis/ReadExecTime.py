import os

def RemoveNewline(line):
    return line.replace("\n","")
def ReadExecTime():
	try:
		f = open("ttime.txt","rb")
		data = RemoveNewline(f.read())
		f.close()
		return float(data)
	except:
		return 0.0
	return 0.0


	