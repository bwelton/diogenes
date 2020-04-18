add_library(DiogenesDyninstCommon SHARED "DynHelper.cpp" "DyninstProcess.cpp" "DyninstOneTime.cpp" "DyninstEntryExit.cpp")

configure_file (
    "${PROJECT_SOURCE_DIR}/src/dyninst_common_lib/CommonDefines.h.in"
    "${PROJECT_SOURCE_DIR}/src/dyninst_common_lib/CommonDefines.h"
)


INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CMAKE_SOURCE_DIR}/src/common_lib)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CMAKE_SOURCE_DIR}/src/SyncDetector/common)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${LIBCSA_INSTALL_DIR}/include)
add_dependencies(DiogenesDyninstCommon DiogenesCommon)

target_link_libraries(DiogenesDyninstCommon "-L${LIBCSA_INSTALL_DIR}/lib -lcuda_sync_analyzer")
target_link_libraries(DiogenesDyninstCommon DiogenesCommon)
target_link_libraries(DiogenesDyninstCommon "-lpthread -ldl")
target_link_libraries(DiogenesDyninstCommon ${Boost_LIBRARIES})
target_link_libraries(DiogenesDyninstCommon ${DYNINST_LIBRARIES})


install(TARGETS DiogenesDyninstCommon
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)