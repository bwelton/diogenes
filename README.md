# Diogenes

Diogenes is a performance tool to identify unnecessary/misplaced synchronizations and memory tranfers. Diogenes provides an estimate of potential benefit of fixing the unnecessary/misplaced operation.

This project is very much a work in progress and is very much in a research state (lots of dead code, difficult to build, lots of bugs, no polish, etc).  

**Building and Installation**

Due to the number of dependencies that this project has (Dyninst, Boost, Cuda, Libdwarf, Libelf, and others), it is highly recommended you build this package with the spack package manager (https://github.com/LLNL/spack). A premade spack repo package for this project is availible in spack/. Adding ./spack to ~/.spack/repos.yaml will allow you to build the deduplicator with the following commands:

cd SPACK_DIR
./bin/spack install cudadedup 

**Required build parameters if building manually (unwise)**

The following parameters are required at CMake configure time if not set by environment variables.

- -DCUDA_TOOLKIT_ROOT_DIR=*Location of Cuda Toolkit* (Optional on Cray Machines)
- -DLIBCUDA_SO=*Absolute path to libcuda.so, this should be the path to libcuda that applications are linked against* (Optional on Cray Machines)
- -DLIBCUDART_SO=*Absolute path to libcudart.so* (Optional on Cray Machines)
- -DLIBELF_LIBFILE=*Absolute path to Libelf.so* (Optional on ORNL Cray Titan)
- -DLIBELF_INCLUDE=*Path to libelf include directory* (Optional on ORNL Cray Titan)
- -DLIBDWARF_LIBFILE=*Absolute path to libdwarf.so, CANNOT be the static version of libdwarf* (Optional on ORNL Cray Titan)
- -DLIBDWARF_INCLUDE=*Path to libdwarf include directory* (Optional on ORNL Cray Titan)

**Optional Build Parameters**

The following parameters are not required but can reduce compilation time if set (by skipping compilation of python/dyninst)

- -DDYNINST_ROOT=*Path to the installation directory of dyninst* (containing /lib and /include)
- -DPYTHON_ROOT=*Path to the installation directory of python 2.7*
- -DCMAKE_INSTALL_PREFIX=*Installation Directory*

**Usage**

To be filled in....

**Disclaimer**

This software should be considered research grade right now, use at your own risk. 





