add_library(MemGraphBuildMutatee SHARED "GotchaWrappers.c" "SignalHandler.c" "SyncDetector.c")

INCLUDE_DIRECTORIES(MemGraphBuildMutatee ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(MemGraphBuildMutatee ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(MemGraphBuildMutatee ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(MemGraphBuildMutatee ${CMAKE_SOURCE_DIR}/src/common_lib)
INCLUDE_DIRECTORIES(MemGraphBuildMutatee ${CMAKE_SOURCE_DIR}/src/SyncDetector/common)
add_dependencies(MemGraphBuildMutatee DiogenesCommon)
target_link_libraries(MemGraphBuildMutatee DiogenesCommon)
target_link_libraries(MemGraphBuildMutatee "-L${GOTCHA_INSTALL_DIR}/lib -lgotcha")
target_link_libraries(MemGraphBuildMutatee "-lpthread -ldl")

install(TARGETS MemGraphBuildMutatee
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)