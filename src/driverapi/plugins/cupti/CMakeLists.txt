add_library(CUPTIEventHandler SHARED "CUPTIEventHandler.cpp")
add_compile_options(-O3 -g)
target_link_libraries(CUPTIEventHandler ${Boost_LIBRARIES})
target_link_libraries(CUPTIEventHandler ${DYNINST_LIBRARIES})
target_link_libraries(CUPTIEventHandler "-lpthread")
target_link_libraries(CUPTIEventHandler DriverAPIWrapper)
INCLUDE_DIRECTORIES(CUPTIEventHandler ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(CUPTIEventHandler ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(CUPTIEventHandler ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/deduplicate")
target_link_libraries(CUPTIEventHandler "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart -L${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/lib64 -lcupti -lboost_timer")
install(TARGETS CUPTIEventHandler
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/plugins
        ARCHIVE DESTINATION lib/static)
