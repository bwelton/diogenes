link_libraries("-L${BOOST_LIBRARYDIR}")
configure_file (
    "${PROJECT_SOURCE_DIR}/src/driverapi/launcher/main.cpp.in"
	"${PROJECT_SOURCE_DIR}/src/driverapi/launcher/main.cpp"
)
configure_file (
    "${PROJECT_SOURCE_DIR}/src/driverapi/launcher/SyncTesting.h.in"
	"${PROJECT_SOURCE_DIR}/src/driverapi/launcher/SyncTesting.h"
)
configure_file (
    "${PROJECT_SOURCE_DIR}/src/driverapi/launcher/Constants.h.in"
	"${PROJECT_SOURCE_DIR}/src/driverapi/launcher/Constants.h"
)
add_executable(LaunchCUInstriment "main.cpp" "SyncTesting.cpp" "LoadStoreInst.cpp" "PerformanceModel.cpp" "SymbolLookup.cpp" "StacktraceInst.cpp" "StackStorage.cpp" "DynOpsClass.cpp" "BinaryLocationIDMap.cpp" "InstrimentationTracker.cpp" "ReadLoadStoreFiles.cpp" "TimeFirstUse.cpp" "Common.cpp" "LocateCudaSynchronization.cpp" "DyninstProcess.cpp" "APICaptureInstrimentation.cpp" "StacktraceSynchronizations.cpp" "ReadStackKeys.cpp" "TimerInstrimentation.cpp" "LoadStoreInstrimentation.cpp" "DyninstFunction.cpp" "TimeUse.cpp" "DyninstFunctionWraps.cpp" "DyninstCallsite.cpp" "DebugInstrimentationTemp.cpp" "FixCudaFree.cpp" "MemRecorder.cpp" "CallTransformation.cpp" "FixCudaProblems.cpp" "BPatchBinary.cpp" "LaunchIdentifySync.cpp" "SerializeTypes.cpp")
target_link_libraries(LaunchCUInstriment ${DYNINST_LIBRARIES})
target_link_libraries(LaunchCUInstriment ${Boost_LIBRARIES})
target_link_libraries(LaunchCUInstriment "-lpthread")
target_link_libraries(LaunchCUInstriment "-lssl -lcrypto -L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lboost_program_options -lboost_filesystem -lboost_system")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi")
INCLUDE_DIRECTORIES(LaunchCUInstriment ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(LaunchCUInstriment ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(LaunchCUInstriment "${CMAKE_SOURCE_DIR}/src/common")

configure_file (
    "${PROJECT_SOURCE_DIR}/src/driverapi/launcher/LoadStoreFunctions.skip"
	"${CMAKE_INSTALL_PREFIX}/lib/LoadStoreFunctions.skip"
)
configure_file (
    "${PROJECT_SOURCE_DIR}/src/driverapi/launcher/OutputFiles.txt"
    "${CMAKE_INSTALL_PREFIX}/lib/OutputFiles.txt"
)
configure_file (
    "${PROJECT_SOURCE_DIR}/src/driverapi/launcher/LoadStoreModules.skip"
	"${CMAKE_INSTALL_PREFIX}/lib/LoadStoreModules.skip"
)
configure_file (
    "${PROJECT_SOURCE_DIR}/src/driverapi/launcher/CallTracingFunctions.skip"
	"${CMAKE_INSTALL_PREFIX}/lib/CallTracingFunctions.skip"
)
configure_file (
    "${PROJECT_SOURCE_DIR}/src/driverapi/launcher/CallTracingModules.skip"
	"${CMAKE_INSTALL_PREFIX}/lib/CallTracingModules.skip"
)
configure_file (
    "${PROJECT_SOURCE_DIR}/src/driverapi/launcher/SyncDriverVerisons.txt"
    "${CMAKE_INSTALL_PREFIX}/lib/SyncDriverVerisons.txt"
)
install(TARGETS LaunchCUInstriment
        RUNTIME DESTINATION bin)
