add_library(MemGraphMutateeLib SHARED "AllocationTracker.cpp" "GotchaWrappers.cpp" "MemGraph.cpp")

INCLUDE_DIRECTORIES(MemGraphMutateeLib ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(MemGraphMutateeLib ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(MemGraphMutateeLib ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(MemGraphMutateeLib ${CMAKE_SOURCE_DIR}/src/common_lib)
INCLUDE_DIRECTORIES(MemGraphMutateeLib ${CMAKE_SOURCE_DIR}/src/MemGraph/common)
add_dependencies(MemGraphMutateeLib DiogenesCommon)
target_link_libraries(MemGraphMutateeLib DiogenesCommon)
target_link_libraries(MemGraphMutateeLib "-L${GOTCHA_INSTALL_DIR}/lib -lgotcha")
target_link_libraries(MemGraphMutateeLib "-lpthread -ldl")

install(TARGETS MemGraphMutateeLib
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)