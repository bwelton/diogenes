add_compile_options(-O3 -g -mno-vsx)
add_compile_options(-mno-vsx)
add_library(TimeFirstUse SHARED "SynchTool.cpp" "CheckAccesses.cpp" "OutputWriter.cpp" "LoadStoreDriver.cpp")
target_link_libraries(TimeFirstUse ${Boost_LIBRARIES})
target_link_libraries(TimeFirstUse ${DYNINST_LIBRARIES})
target_link_libraries(TimeFirstUse "-lpthread")
target_link_libraries(TimeFirstUse DriverAPIWrapper)
INCLUDE_DIRECTORIES(TimeFirstUse ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(TimeFirstUse ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(TimeFirstUse ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/deduplicate")
target_link_libraries(TimeFirstUse "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart -L${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/lib64 -lcupti -lboost_timer")
install(TARGETS TimeFirstUse
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/plugins
        ARCHIVE DESTINATION lib/static)
