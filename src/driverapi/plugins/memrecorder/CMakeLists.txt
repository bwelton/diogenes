add_compile_options(-O3 -g -mno-vsx)
add_compile_options(-mno-vsx)

add_library(DiogenesMemRecorder SHARED "MemRecorder.cpp")
target_link_libraries(DiogenesMemRecorder ${Boost_LIBRARIES})
target_link_libraries(DiogenesMemRecorder ${DYNINST_LIBRARIES})
target_link_libraries(DiogenesMemRecorder "-lpthread")
target_link_libraries(DiogenesMemRecorder DriverAPIWrapper)
target_link_libraries(DiogenesMemRecorder PluginCommon)
target_link_libraries(DiogenesMemRecorder "-L${GOTTCHA_LIBRARIES} -lgotcha" )
INCLUDE_DIRECTORIES(DiogenesMemRecorder ${GOTTCHA_INCLUDE})
INCLUDE_DIRECTORIES(DiogenesMemRecorder ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(DiogenesMemRecorder ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(DiogenesMemRecorder ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi/launcher")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/deduplicate")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/driverapi/plugins/common")
target_link_libraries(DiogenesMemRecorder "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart -L${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/lib64 -lcupti -lcuda -lboost_timer")
install(TARGETS DiogenesMemRecorder
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/plugins
        ARCHIVE DESTINATION lib/static)
