add_library(DiogenesCommon SHARED "CaptureProcMap.c" "PageLocker.c" "SkipList.c" "StackTrie.c" "HashMap.c" "Stackwalk.c" "CVector.c" "ReadStackKeys.c")
INCLUDE_DIRECTORIES(DiogenesCommon ${LIBUNWIND_INSTALL_DIR}/include)

target_link_libraries(DiogenesCommon "-lpthread -ldl")
target_link_libraries(DiogenesCommon "-L${LIBUNWIND_INSTALL_DIR}/lib -lunwind")
add_subdirectory(${CMAKE_SOURCE_DIR}/src/common_lib/examples)

install(TARGETS DiogenesCommon
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)