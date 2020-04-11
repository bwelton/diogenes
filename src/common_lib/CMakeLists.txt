add_library(DiogenesCommon SHARED "CaptureProcMap.c" "PageLocker.c" "SkipList.c" "StackTrie.c" "HashMap.c" "Stackwalk.c" "CVector.c" "ReadStackKeys.c")

target_link_libraries(DiogenesCommon "-lpthread -ldl")

add_subdirectory(${CMAKE_SOURCE_DIR}/src/common_lib/examples)