add_library(TransferTimeline SHARED "TransferTimeline.cpp")
add_compile_options(-O3 -g)
target_link_libraries(TransferTimeline ${Boost_LIBRARIES})
target_link_libraries(TransferTimeline ${DYNINST_LIBRARIES})
target_link_libraries(TransferTimeline "-lpthread")
target_link_libraries(TransferTimeline DriverAPIWrapper)
INCLUDE_DIRECTORIES(TransferTimeline ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(TransferTimeline ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(TransferTimeline ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi")
target_link_libraries(TransferTimeline "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart -L${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/lib64 -lcupti")
install(TARGETS TransferTimeline
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/plugins
        ARCHIVE DESTINATION lib/static)
