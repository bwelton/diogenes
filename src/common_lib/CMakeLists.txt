add_library(DiogenesCommon SHARED "CaptureProcMap.c" "PageLocker.c" "SkipList.c" "StackTrie.c" "HashMap.c" "Stackwalk.c" "CVector.c" "ReadStackKeys.c")
INCLUDE_DIRECTORIES(DiogenesCommon ${LIBUNWIND_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(DiogenesCommon ${CMAKE_SOURCE_DIR}/src/dyninst_common_lib)
INCLUDE_DIRECTORIES(DiogenesCommon ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(DiogenesCommon ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(DiogenesCommon ${CMAKE_SOURCE_DIR}/src/common_lib)

target_link_libraries(DiogenesCommon "-lpthread -ldl")
target_link_libraries(DiogenesCommon "-L${LIBUNWIND_INSTALL_DIR}/lib -lunwind")
target_link_libraries(DiogenesCommon "-L${GOTCHA_INSTALL_DIR}/lib -lgotcha")
target_link_libraries(DiogenesCommon DyninstFPStackwalk)
add_dependencies(DiogenesCommon DyninstFPStackwalk)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/common_lib/examples)

install(TARGETS DiogenesCommon
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)