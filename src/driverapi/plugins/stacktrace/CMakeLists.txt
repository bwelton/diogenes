add_library(STraceWrapSync SHARED "STraceWrapSync.cpp")
add_compile_options(-O3 -g)
target_link_libraries(STraceWrapSync ${Boost_LIBRARIES})
target_link_libraries(STraceWrapSync ${DYNINST_LIBRARIES})
target_link_libraries(STraceWrapSync "-lpthread")
target_link_libraries(STraceWrapSync DriverAPIWrapper)
INCLUDE_DIRECTORIES(STraceWrapSync ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(STraceWrapSync ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(STraceWrapSync ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/deduplicate")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi/launcher")
target_link_libraries(STraceWrapSync "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart -L${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/lib64 -lcupti -lboost_timer")
install(TARGETS STraceWrapSync
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/plugins
        ARCHIVE DESTINATION lib/static)


add_library(Stacktrace SHARED "Stacktrace.cpp")
add_compile_options(-O3 -g)
target_link_libraries(Stacktrace ${Boost_LIBRARIES})
target_link_libraries(Stacktrace ${DYNINST_LIBRARIES})
target_link_libraries(Stacktrace "-lpthread")
target_link_libraries(Stacktrace STraceWrapSync)
target_link_libraries(Stacktrace DriverAPIWrapper)
target_link_libraries(Stacktrace PluginCommon)
INCLUDE_DIRECTORIES(Stacktrace ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(Stacktrace ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(Stacktrace ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/deduplicate")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi/launcher")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi/plugins/common")
target_link_libraries(Stacktrace "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart -L${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/lib64 -lcupti -lboost_timer")
install(TARGETS Stacktrace
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/plugins
        ARCHIVE DESTINATION lib/static)
