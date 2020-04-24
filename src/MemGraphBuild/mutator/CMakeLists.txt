add_executable(MemGraph "Main.cpp")
INCLUDE_DIRECTORIES(MemGraph ${CMAKE_SOURCE_DIR}/src/dyninst_common_lib)
INCLUDE_DIRECTORIES(MemGraph ${CMAKE_SOURCE_DIR}/src/dyninst_common_lib)
INCLUDE_DIRECTORIES(MemGraph ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CMAKE_SOURCE_DIR}/src/common_lib/extern/procparser)
INCLUDE_DIRECTORIES(MemGraph ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(MemGraph ${CMAKE_SOURCE_DIR}/src/common_lib)

add_dependencies(MemGraph DiogenesCommon)
target_link_libraries(MemGraph DiogenesCommon)
add_dependencies(MemGraph DiogenesDyninstCommon)
target_link_libraries(MemGraph DiogenesDyninstCommon)
target_link_libraries(MemGraph "-L${GOTCHA_INSTALL_DIR}/lib -lgotcha")
target_link_libraries(MemGraph "-lpthread -ldl")

install(TARGETS MemGraph
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)