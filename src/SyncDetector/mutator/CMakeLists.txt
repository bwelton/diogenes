add_executable(SyncDetector "Main.cpp")
INCLUDE_DIRECTORIES(SyncDetector ${CMAKE_SOURCE_DIR}/src/dyninst_common_lib)
INCLUDE_DIRECTORIES(SyncDetector ${CMAKE_SOURCE_DIR}/src/dyninst_common_lib)
INCLUDE_DIRECTORIES(SyncDetector ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CMAKE_SOURCE_DIR}/src/common_lib/extern/procparser)
INCLUDE_DIRECTORIES(SyncDetector ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(SyncDetector ${CMAKE_SOURCE_DIR}/src/common_lib)

add_dependencies(SyncDetector DiogenesCommon)
target_link_libraries(SyncDetector DiogenesCommon)
add_dependencies(SyncDetector DiogenesDyninstCommon)
target_link_libraries(SyncDetector DiogenesDyninstCommon)
target_link_libraries(SyncDetector "-L${GOTCHA_INSTALL_DIR}/lib -lgotcha")
target_link_libraries(SyncDetector "-lpthread -ldl")

install(TARGETS SyncDetector
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)