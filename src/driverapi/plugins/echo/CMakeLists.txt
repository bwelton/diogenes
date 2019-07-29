add_library(Echo SHARED "echo.cpp")
add_compile_options(-O3 -g)
target_link_libraries(Echo ${Boost_LIBRARIES})
target_link_libraries(Echo ${DYNINST_LIBRARIES})
target_link_libraries(Echo "-lpthread")
target_link_libraries(Echo DriverAPIWrapper)
INCLUDE_DIRECTORIES(Echo ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(Echo ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(Echo ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi")
target_link_libraries(Echo "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart -L${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/lib64 -lcupti")
install(TARGETS Echo
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/plugins
        ARCHIVE DESTINATION lib/static)
