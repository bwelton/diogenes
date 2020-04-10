add_executable(TestHashMap "TestHashMap.c")

INCLUDE_DIRECTORIES(TestHashMap ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(TestHashMap ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(TestHashMap ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(TestHashMap ${CMAKE_SOURCE_DIR}/src/common_lib)
add_dependencies(TestHashMap DiogenesCommon)
target_link_libraries(TestHashMap DiogenesCommon)
target_link_libraries(TestHashMap "-L${GOTCHA_INSTALL_DIR}/lib -lgotcha")
target_link_libraries(TestHashMap "-lpthread -ldl")

add_executable(TestSkipList "TestSkipList.c")

INCLUDE_DIRECTORIES(TestSkipList ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(TestSkipList ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(TestSkipList ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(TestSkipList ${CMAKE_SOURCE_DIR}/src/common_lib)
add_dependencies(TestSkipList DiogenesCommon)
target_link_libraries(TestSkipList DiogenesCommon)
target_link_libraries(TestSkipList "-L${GOTCHA_INSTALL_DIR}/lib -lgotcha")
target_link_libraries(TestSkipList "-lpthread -ldl")


add_executable(TestStackTrie "TestStackTrie.c")

INCLUDE_DIRECTORIES(TestStackTrie ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(TestStackTrie ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(TestStackTrie ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(TestStackTrie ${CMAKE_SOURCE_DIR}/src/common_lib)
add_dependencies(TestStackTrie DiogenesCommon)
target_link_libraries(TestStackTrie DiogenesCommon)
target_link_libraries(TestStackTrie "-L${GOTCHA_INSTALL_DIR}/lib -lgotcha")
target_link_libraries(TestStackTrie "-lpthread -ldl")
