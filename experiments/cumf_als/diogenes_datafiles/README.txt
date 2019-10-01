Diogenes results can be viewed without installation (or rerunning Diogenes) using these files.

To view Diogenes Results:

cd diogenes/experiments/cumf_als/diogenes_datafiles
python ../../../src/analysis/TemplateFolder.py 

Requirements:
	Analysis scripts are built for python 2.7. 
	Requires PyInquirer to be installed (https://github.com/CITGuru/PyInquirer) and can be autoinstalled via pip (pip install PyInquirer).

Small Note: TemplateFolder.py will display results using a stacked ordering where the three types of issues (template, single point, sequence) are seperated and ordered based on time. After some experimentation between the SC paper unified ordering and this ordering, I recommend using this mode because it is slightly clearer as to what types of problems exist. 