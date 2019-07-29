##
#  Setup Run Directory for Diogenes 
#  Looks for Environment Variable: DIOGENES_LAUNCH_SCRIPTS for directory of launch scripts
##

import os 
from shutil import copyfile


def BackupFile(filename):
	if os.path.isfile(filename):
		copyfile(filename, filename+".BACKUP")


def SetupLoadStoreModules():
	print "Setting up LoadStoreModules.inst file"
	inp = raw_input("What modules do you want to instriment (CSV List): ")
	tmp = inp.split(",")
	BackupFile("LoadStoreModules.inst")
	f = open("LoadStoreModules.inst", "w")
	for x in tmp:
		f.write(x + "\n")

	f.close()

def SetupSettingINIFile():
	print "Setup Settings.ini..."
	BackupFile("settings.ini")
	f = open("settings.ini","w")
	f.write("synch = true\n")
	tmp = raw_input("Enter CMD Line used to launch program: ")
	f.write("prog = " + tmp +"\n")
	f.close()


def CopyLaunchScripts():
	print "Copying launch scripts..."
	if "DIOGENES_LAUNCH_SCRIPTS" not in os.environ:
		print "Error, please set DIOGENES_LAUNCH_SCRIPTS to where the launch scripts for this machine are located"
		exit(1)
	copyfile(os.path.join(os.environ["DIOGENES_LAUNCH_SCRIPTS"],"launchInteractive.sh"), "launchInteractive.sh")
	copyfile(os.path.join(os.environ["DIOGENES_LAUNCH_SCRIPTS"],"setupEnv.sh"), "setupEnv.sh")
	copyfile(os.path.join(os.environ["DIOGENES_LAUNCH_SCRIPTS"],"run.sh"), "run.sh")
	os.system("mkdir logs")


CopyLaunchScripts()
SetupSettingINIFile()
SetupLoadStoreModules()


