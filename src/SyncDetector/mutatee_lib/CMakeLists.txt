add_library(SyncDetectorMutatee SHARED "GotchaWrappers.c" "SignalHandler.c" "SyncDetector.c" "CPPLocker.cpp")

INCLUDE_DIRECTORIES(SyncDetectorMutatee ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(SyncDetectorMutatee ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(SyncDetectorMutatee ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(SyncDetectorMutatee ${CMAKE_SOURCE_DIR}/src/common_lib)
INCLUDE_DIRECTORIES(SyncDetectorMutatee ${CMAKE_SOURCE_DIR}/src/SyncDetector/common)
add_dependencies(SyncDetectorMutatee DiogenesCommon)
target_link_libraries(SyncDetectorMutatee DiogenesCommon)
target_link_libraries(SyncDetectorMutatee "-L${GOTCHA_INSTALL_DIR}/lib -lgotcha")
target_link_libraries(SyncDetectorMutatee "-lpthread -ldl")

install(TARGETS SyncDetectorMutatee
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)