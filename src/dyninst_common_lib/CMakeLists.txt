add_library(DiogenesDyninstCommon SHARED "DynHelper.cpp" "DyninstProcess.cpp" "DyninstOneTime.cpp" "DyninstEntryExit.cpp" "ParseProcMap.cpp")

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
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CMAKE_SOURCE_DIR}/src/common_lib/extern/procparser)

add_dependencies(DiogenesDyninstCommon DiogenesCommon)
target_link_libraries(DiogenesDyninstCommon DiogenesCommon)
target_link_libraries(DiogenesDyninstCommon "-L${LIBCSA_INSTALL_DIR}/lib -lcuda_sync_analyzer")
target_link_libraries(DiogenesDyninstCommon "-lpthread -ldl")
target_link_libraries(DiogenesDyninstCommon ${Boost_LIBRARIES})
target_link_libraries(DiogenesDyninstCommon ${DYNINST_LIBRARIES})

install(TARGETS DiogenesDyninstCommon
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)


add_library(DyninstFPStackwalk SHARED "FPStackWalker.cpp")
INCLUDE_DIRECTORIES(DyninstFPStackwalk ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(DyninstFPStackwalk ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(DyninstFPStackwalk ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(DyninstFPStackwalk ${CMAKE_SOURCE_DIR}/src/common_lib)
INCLUDE_DIRECTORIES(DyninstFPStackwalk ${CMAKE_SOURCE_DIR}/src/SyncDetector/common)
INCLUDE_DIRECTORIES(DyninstFPStackwalk ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(DyninstFPStackwalk ${LIBCSA_INSTALL_DIR}/include)

target_link_libraries(DyninstFPStackwalk "-L${LIBCSA_INSTALL_DIR}/lib -lcuda_sync_analyzer")
target_link_libraries(DyninstFPStackwalk "-lpthread -ldl")
target_link_libraries(DyninstFPStackwalk ${Boost_LIBRARIES})
target_link_libraries(DyninstFPStackwalk ${DYNINST_LIBRARIES})


install(TARGETS DyninstFPStackwalk
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)