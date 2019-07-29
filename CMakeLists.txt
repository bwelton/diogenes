## Check if we are using the cray compiler wrappers
#  If so we are on a cray machine
cmake_minimum_required (VERSION 3.1)
set(default_build_type "Debug")
set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
SET(CMAKE_CXX_STANDARD 11)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
project(cuda_deduplicator)
find_package(Boost COMPONENTS system thread unit_test_framework REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(PythonInterp REQUIRED)

if (NOT DEFINED SPACK_INSTALL)
	set(SPACK_INSTALL "no")
endif(NOT DEFINED SPACK_INSTALL)
cmake_host_system_information(RESULT BUILD_HOST QUERY HOSTNAME)
if ("${BUILD_HOST}" MATCHES "(titan-ext)[1-7]" AND NOT "${SPACK_INSTALL}"  MATCHES "yes")
	set(CRAY_MACHINE "Yes")
	set(CUDA_TOOLKIT_ROOT_DIR "$ENV{CRAY_CUDATOOLKIT_DIR}")
endif("${BUILD_HOST}" MATCHES "(titan-ext)[1-7]" AND NOT "${SPACK_INSTALL}" MATCHES "yes")
find_package(CUDA REQUIRED)


## Setup Python Paths
include(${CMAKE_SOURCE_DIR}/PythonInstall.cmake)
## Get dyninst library info
include(${CMAKE_SOURCE_DIR}/DyninstInstall.cmake)
## Setup Cuda
include(${CMAKE_SOURCE_DIR}/CudaLibDetect.cmake)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/inst_lib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/common)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/deduplicate)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/python_wrapper)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/scripts)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/tests)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/driverapi)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/analysis)