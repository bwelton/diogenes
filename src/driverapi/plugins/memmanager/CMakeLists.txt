add_compile_options(-O3 -g -mno-vsx)
add_compile_options(-mno-vsx)

add_library(DiogenesMemManager SHARED "MemManager.cpp")
target_link_libraries(DiogenesMemManager ${Boost_LIBRARIES})
target_link_libraries(DiogenesMemManager ${DYNINST_LIBRARIES})
target_link_libraries(DiogenesMemManager "-lpthread")
target_link_libraries(DiogenesMemManager DriverAPIWrapper)
target_link_libraries(DiogenesMemManager "-L${GOTTCHA_LIBRARIES} -lgotcha" )
INCLUDE_DIRECTORIES(DiogenesMemManager ${GOTTCHA_INCLUDE})
INCLUDE_DIRECTORIES(DiogenesMemManager ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(DiogenesMemManager ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(DiogenesMemManager ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/deduplicate")
target_link_libraries(DiogenesMemManager "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart -L${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/lib64 -lcupti -lcuda -lboost_timer")
install(TARGETS DiogenesMemManager
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/plugins
        ARCHIVE DESTINATION lib/static)
