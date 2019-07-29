add_library(PluginCommon SHARED "StackwalkingCommon.cpp")
add_compile_options(-O3 -g -mno-vsx)
target_link_libraries(PluginCommon ${Boost_LIBRARIES})
target_link_libraries(PluginCommon ${DYNINST_LIBRARIES})
target_link_libraries(PluginCommon "-lpthread")
target_link_libraries(PluginCommon DriverAPIWrapper)
INCLUDE_DIRECTORIES(PluginCommon ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(PluginCommon ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(PluginCommon ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/deduplicate")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi/launcher")
target_link_libraries(PluginCommon "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart -L${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/lib64 -lcupti -lboost_timer")
install(TARGETS PluginCommon
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/plugins
        ARCHIVE DESTINATION lib/static)