# Diogenes - Redesign

This is a redesign of Diogenes to substantially clean up the code base, expand compatibility to x86 architectures, substantially improve stability and performance, and to allow future researchers of the Paradyn group (or other groups) to have something that is easier to work with to expand functionality. 

Currently supported features from the original version of Diogenes are all of the features demonstrated in our autocorrect paper at ICS 2020 (automatic remedy identification and autocorrection of synchronization problems in CUDA programs) EXCEPT for the automatic correction of memory transfer problems (most of the framework is in place to support this and this feature can be added easily by modifying the autocorrect mutatee, my time at Wisconsin finished before I was able to complete this). 

A substantial difference between the autocorrect paper and this implementation is the usage of page protection instead of load store analysis to determine the location of first use of data after a synchronization. We have found that  page protection substantially reduces the overhead of running Diogenes (we have seen as low as 2-3x overhead from by using this new method over the 40x of the original Diogenes implementation). We should note that testing with this technique has been limited and overhead may be higher when tested on a wider range of applications. 

**Building and Installation**

Five dependencies are required for install of this redesign of Diogenes. Boost, LLNL's Gotcha, UW-Madison's Dyninst, libunwind, and the cuda sync analyzer (https://github.com/dyninst/tools/tree/master/cuda_sync_analyzer/src). Installation can be performed using standard cmake commands:

cd build

cmake ..  -DCMAKE_INSTALL_PREFIX= -DBOOST_ROOT= -DGOTCHA_INSTALL_DIR= -DDYNINST_ROOT= -DLIBUNWIND_INSTALL_DIR= -DLIBCSA_INSTALL_DIR=

make install

**Usage**
In the install directory, three executables will be created:

1. MemGraphBuild which builds the memory graph needed for autocorrection. 
2. SyncDetector that detects the necessity of a synchronization. 
3. Autocorrect which applies corrections to the binary at execution time. 

These should be run in order and each executable takes the application you want to run (and its parameters) as its execution parameters (i.e. MemGraphBuild <application name> <application parameters>).

**NOTE**

If you are interested in running Diogenes on your machine, contact me at welton [at] cs.wisc.edu. Its highly likely you will run into issues 

**Disclaimer**

This software should be considered research grade right now, use at your own risk. 
