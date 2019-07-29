# cuda_deduplicator
Tool for detecting and correcting duplicate transfers in CUDA applications. This is done by wrapping the CUDA driver API function calls to intercept the data being transfered to/from device. This project supports the following features:

**Detection of Duplicate Transfers**

Collects statistical information about the presence of duplicate data transfers within an application. 

- Counting of the number of data transfers containing duplicate data to/from the GPU
- Size of the duplicate transfers
- Amount of time spent transferring duplicate data (accuracy of this number is a work in progress)
- Supported transfer calls: cuMemcpyHtoDAsync_v2,cuMemcpyAtoD_v2,cuMemcpyAtoH_v2,cuMemcpyAtoHAsync_v2,cuMemcpyDtoA_v2,cuMemcpyDtoH_v2,cuMemcpyDtoHAsync_v2, and cuMemcpyHtoD_v2.

**Automatic Data Deduplication**

Functions by creating a temporary cache in GPU memory for data transfers. When a duplicate transfer is detected to the GPU device (detected via hashing), a device to device transfer from this cache will replace the host to device transfer.

- Supported transfer calls: cuMemcpyHtoDAsync_v2, cuMemcpyAtoD_v2, and cuMemcpyHtoD_v2

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
- -DENABLE_STRACE=1 *Enables stacktrace support. when a duplicate is detected a stacktrace showing the location of the duplicate transfer will be printed*

**Usage**

For gathering statistical information on duplicate transfers (detection but no correction) run the following command from the root install directory (CMAKE_INSTALL_PREFIX directory):

1. bash ./bin/inst_app.sh */path/to/LIBCUDA.so*/libcuda.so.1 ./defs/DetectDuplicates.def.in */path/to/save/rewritten/libcuda*/libcuda.so.1 (WARNING: Do not attempt to overwrite the original libcuda.so.1)
2. export LD_LIBRARY_PATH=*/path/to/save/rewritten/libcuda*:*/CMAKE/INSTALL/PREFIX/PATH*/lib:$LD_LIBRARY_PATH
3. Run your application, statistical information printed to STDERR at end of execution.

If you are interested in automatic correction of duplicate transfers, perform the following:

1. bash ./bin/inst_app.sh */path/to/LIBCUDA.so*/libcuda.so.1 ./defs/Deduplicate.def */path/to/save/rewritten/libcuda*/libcuda.so.1 (WARNING: Do not attempt to overwrite the original libcuda.so.1)
2. export LD_LIBRARY_PATH=*/path/to/save/rewritten/libcuda*:*/CMAKE/INSTALL/PREFIX/PATH*/lib:$LD_LIBRARY_PATH
3. Run your application, statistical information (number of duplicates caught and corrected) printed to STDERR at end of execution.

**Known Issues/Works In Progress**

- ORNL Titan has issues with certain versions of dyninst, right now it is recommended not setting DYNINST_ROOT such that we can build a compatible version (git rev a8252fd). It is likely that dyninst issues impact other cray platforms, it is highly recommended you try building with git rev a8252fd of Dyninst if you run into issues.
- Deduplicate correction is highly experimental and may not show the performance benefits you may expect. 
- Automated correction of duplicates may require Cuda MPS support to be enabled to function correctly (specifically in cases where multiple different threads on the same node are using CUDA in different CUDA contexts). 

**Disclaimer**

This software should be considered research grade right now, use at your own risk. 





