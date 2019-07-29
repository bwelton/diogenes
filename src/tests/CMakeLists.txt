if ("${BUILD_HOST}" MATCHES "(titan-ext)[1-7]")
	set(PROJ_ID "$ENV{PROJ_ID}")
	set(LAUNCHER "aprun -n 1 -N 1")
endif("${BUILD_HOST}" MATCHES "(titan-ext)[1-7]")

if (NOT DEFINED ENV{PROJ_ID})
	set(PROJ_ID "0")
endif (NOT DEFINED ENV{PROJ_ID})

if (NOT DEFINED LAUNCHER)
	set(LAUNCHER "")
endif (NOT DEFINED LAUNCHER)

if (NOT DEFINED SPACK_ROOT)
	MESSAGE(INFO "No spack directory set, cannot create titan test launch file")
	set(SPACK_MOD_DIR "")
else()
	message(INFO "Spack root set to ${SPACK_ROOT}share/spack/modules/cray-CNL-interlagos")
	set(SPACK_MOD_DIR "${SPACK_ROOT}share/spack/modules/cray-CNL-interlagos")
endif (NOT DEFINED SPACK_ROOT)

add_executable(basic_checker_test "basic_checker_test.cpp")

target_link_libraries(basic_checker_test ${Boost_LIBRARIES})
target_link_libraries(basic_checker_test "-lpthread")
target_link_libraries(basic_checker_test "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES(basic_checker_test ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(basic_checker_test ${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

install(TARGETS basic_checker_test
        RUNTIME DESTINATION tests)

add_executable(timeline_checker "timeline_checker.cpp")
target_link_libraries(timeline_checker ${Boost_LIBRARIES})
target_link_libraries(timeline_checker "-lpthread")
target_link_libraries(timeline_checker "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES(timeline_checker ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(timeline_checker ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(timeline_checker "${CMAKE_SOURCE_DIR}/src/common")
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

install(TARGETS timeline_checker
        RUNTIME DESTINATION tests)

add_executable(desthash_check "DestHashCheck.cpp")
target_link_libraries(desthash_check ${Boost_LIBRARIES})
target_link_libraries(desthash_check "-lpthread")
target_link_libraries(desthash_check "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES(desthash_check ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(desthash_check ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(desthash_check "${CMAKE_SOURCE_DIR}/src/common")
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

install(TARGETS desthash_check
        RUNTIME DESTINATION tests)

add_executable(cupti_test "cupti_test.cpp")
target_link_libraries(cupti_test ${Boost_LIBRARIES})
target_link_libraries(cupti_test "-lpthread")
target_link_libraries(cupti_test "-L/opt/cray/nvidia/default/lib64 -lcuda")
target_link_libraries(cupti_test "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES(cupti_test ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(cupti_test ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(cupti_test "${CMAKE_SOURCE_DIR}/src/common")
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

install(TARGETS cupti_test
        RUNTIME DESTINATION tests)

add_executable(PerfEstimateCheck "PerfEstimateCheck.cpp")
target_link_libraries(PerfEstimateCheck ${Boost_LIBRARIES})
target_link_libraries(PerfEstimateCheck "-lpthread")
target_link_libraries(PerfEstimateCheck "-L/opt/cray/nvidia/default/lib64 -lcuda")
target_link_libraries(PerfEstimateCheck "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES(PerfEstimateCheck ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(PerfEstimateCheck ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(PerfEstimateCheck "${CMAKE_SOURCE_DIR}/src/common")
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

install(TARGETS PerfEstimateCheck
        RUNTIME DESTINATION tests)

add_executable(DyninstSanity "DyninstSanity.cpp")
target_link_libraries(DyninstSanity ${Boost_LIBRARIES})
target_link_libraries(DyninstSanity "-lpthread")
target_link_libraries(DyninstSanity ${DYNINST_LIBRARIES})
INCLUDE_DIRECTORIES(DyninstSanity ${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
install(TARGETS DyninstSanity
        RUNTIME DESTINATION tests)


add_executable(DumpFunctionNames "DumpFunctionNames.cpp")
target_link_libraries(DumpFunctionNames ${Boost_LIBRARIES})
target_link_libraries(DumpFunctionNames "-lpthread")
target_link_libraries(DumpFunctionNames "-L/opt/cray/nvidia/default/lib64 -lcuda")
target_link_libraries(DumpFunctionNames "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES(DumpFunctionNames ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(DumpFunctionNames ${Boost_INCLUDE_DIRS})
target_link_libraries(DumpFunctionNames ${DYNINST_LIBRARIES})
INCLUDE_DIRECTORIES(DumpFunctionNames "${CMAKE_SOURCE_DIR}/src/common")
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

add_executable(TestInstrimentation "TestInstrimentation.cpp")
target_link_libraries(TestInstrimentation ${Boost_LIBRARIES})
target_link_libraries(TestInstrimentation "-lpthread")
target_link_libraries(TestInstrimentation "-L/opt/cray/nvidia/default/lib64 -lcuda")
target_link_libraries(TestInstrimentation "-lboost_filesystem -lboost_system -L${CUDA_TOOLKIT_ROOT_DIR}/lib64")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES(TestInstrimentation ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(TestInstrimentation ${Boost_INCLUDE_DIRS})
target_link_libraries(TestInstrimentation ${DYNINST_LIBRARIES})
INCLUDE_DIRECTORIES(TestInstrimentation "${CMAKE_SOURCE_DIR}/src/common")
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

install(TARGETS DumpFunctionNames
        RUNTIME DESTINATION tests)

install(TARGETS TestInstrimentation
        RUNTIME DESTINATION tests)
# add_executable(HostToDev "artifacts/HostToDevice.cpp")
# target_link_libraries(HostToDev "-lpthread")
# target_link_libraries(HostToDev "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart")
# INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
# INCLUDE_DIRECTORIES(HostToDev ${CUDA_TOOLKIT_ROOT_DIR}/include)
# INCLUDE_DIRECTORIES(HostToDev ${Boost_INCLUDE_DIRS})
# INCLUDE_DIRECTORIES(HostToDev "${CMAKE_SOURCE_DIR}/src/common")
# ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

# install(TARGETS HostToDev
#         RUNTIME DESTINATION tests)


add_executable(launch_check "launch_check.cpp")
add_dependencies(launch_check dedup)
add_dependencies(launch_check DriverAPIWrapper)
target_link_libraries(launch_check dedup)
#target_link_libraries(launch_check DriverAPIWrapper)
target_link_libraries(launch_check ${Boost_LIBRARIES})
target_link_libraries(launch_check "-lpthread")
INCLUDE_DIRECTORIES(launch_check ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
target_link_libraries(launch_check "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart")
target_link_libraries(launch_check "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart")
INCLUDE_DIRECTORIES(launch_check ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(launch_check ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(launch_check "${CMAKE_SOURCE_DIR}/src/deduplicate")
INCLUDE_DIRECTORIES(launch_check "${CMAKE_SOURCE_DIR}/src/driverapi")
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

install(TARGETS launch_check
        RUNTIME DESTINATION tests)

add_executable(TestDedupSavings "TestDedupSavings.cpp" "${CMAKE_SOURCE_DIR}/src/deduplicate/CalculateDedupSavings.cpp")
target_link_libraries(TestDedupSavings ${Boost_LIBRARIES})
target_link_libraries(TestDedupSavings "-lpthread")
INCLUDE_DIRECTORIES(TestDedupSavings ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(TestDedupSavings "${CMAKE_SOURCE_DIR}/src/deduplicate")
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
install(TARGETS TestDedupSavings
        RUNTIME DESTINATION tests)

add_executable(GetExportTableTest "GetExportTableTest.cpp")
target_link_libraries(GetExportTableTest ${Boost_LIBRARIES})
target_link_libraries(GetExportTableTest "-L/usr/lib/x86_64-linux-gnu/libcuda.so -lcuda")
target_link_libraries(GetExportTableTest "-lpthread")
INCLUDE_DIRECTORIES(GetExportTableTest ${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
install(TARGETS GetExportTableTest
        RUNTIME DESTINATION tests)

add_executable(FindPtrFunctions "FindPtrFunctions.cpp")
target_link_libraries(FindPtrFunctions ${Boost_LIBRARIES})
target_link_libraries(FindPtrFunctions "-lpthread")
target_link_libraries(FindPtrFunctions ${DYNINST_LIBRARIES})
INCLUDE_DIRECTORIES(FindPtrFunctions ${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
install(TARGETS FindPtrFunctions
        RUNTIME DESTINATION tests)

add_executable(GenStacktraceThirdParty "GenStacktraceThirdParty.cpp")
target_link_libraries(GenStacktraceThirdParty ${Boost_LIBRARIES})
target_link_libraries(GenStacktraceThirdParty "-lpthread")
target_link_libraries(GenStacktraceThirdParty ${DYNINST_LIBRARIES})
INCLUDE_DIRECTORIES(GenStacktraceThirdParty ${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
install(TARGETS GenStacktraceThirdParty
        RUNTIME DESTINATION tests)

add_executable(SmallModify "SmallModify.cpp")
target_link_libraries(SmallModify ${Boost_LIBRARIES})
target_link_libraries(SmallModify "-lpthread")
target_link_libraries(SmallModify ${DYNINST_LIBRARIES})
INCLUDE_DIRECTORIES(SmallModify ${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
install(TARGETS SmallModify
        RUNTIME DESTINATION tests)

add_executable(GetCalledFuncs "GetCalledFuncs.cpp")
target_link_libraries(GetCalledFuncs ${Boost_LIBRARIES})
target_link_libraries(GetCalledFuncs "-lpthread")
target_link_libraries(GetCalledFuncs ${DYNINST_LIBRARIES})
INCLUDE_DIRECTORIES(GetCalledFuncs ${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
install(TARGETS GetCalledFuncs
        RUNTIME DESTINATION tests)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/scripts)
configure_file (
  "${PROJECT_SOURCE_DIR}/src/tests/LoadSpackModules.py.in"
  "${PROJECT_BINARY_DIR}/scripts/LoadSpackModules.py"
  @ONLY
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/src/tests/RunTests.sh.in"
  "${PROJECT_BINARY_DIR}/scripts/RunTests.sh"
  @ONLY
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/src/tests/TitanTestScript.ksh.in"
  "${PROJECT_BINARY_DIR}/scripts/TitanTestScript.ksh"
  @ONLY
  )

install(FILES ${PROJECT_BINARY_DIR}/scripts/TitanTestScript.ksh ${PROJECT_BINARY_DIR}/scripts/RunTests.sh  ${PROJECT_BINARY_DIR}/scripts/LoadSpackModules.py
		DESTINATION tests)

install(FILES "dedup_test_configure.conf"
        DESTINATION tests)
