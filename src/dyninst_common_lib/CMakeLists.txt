add_library(DiogenesDyninstCommon SHARED "DynHelper.cpp" "DyninstProcess.cpp")

INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CMAKE_SOURCE_DIR}/src/common_lib)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CMAKE_SOURCE_DIR}/src/SyncDetector/common)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${Boost_INCLUDE_DIRS})

add_dependencies(DiogenesDyninstCommon DiogenesCommon)

target_link_libraries(DiogenesDyninstCommon DiogenesCommon)
target_link_libraries(DiogenesDyninstCommon "-lpthread -ldl")
target_link_libraries(DiogenesDyninstCommon ${Boost_LIBRARIES})
target_link_libraries(DiogenesDyninstCommon ${DYNINST_LIBRARIES})