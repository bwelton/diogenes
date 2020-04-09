add_library(DiogenesCommon SHARED "PageLocker.c" "SkipList.c" "StackTrie.c" "HashMap.c" "Stackwalk.c")

target_link_libraries(DiogenesCommon "-lpthread -ldl")