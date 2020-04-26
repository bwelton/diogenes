add_library(AutocorrectLibrary SHARED "GotchaWrappers.cpp" "Autocorrect.cpp" "FastStackTree.cpp" "CPUMemorLocation.cpp" "SimpleGPUMallocMap.cpp")

INCLUDE_DIRECTORIES(AutocorrectLibrary ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(AutocorrectLibrary ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(AutocorrectLibrary ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(AutocorrectLibrary ${CMAKE_SOURCE_DIR}/src/common_lib)
INCLUDE_DIRECTORIES(MemGraph ${CMAKE_SOURCE_DIR}/src/dyninst_common_lib)
INCLUDE_DIRECTORIES(AutocorrectLibrary ${CMAKE_SOURCE_DIR}/src/Autocorrect/mutatee)
add_dependencies(AutocorrectLibrary DiogenesCommon)
target_link_libraries(AutocorrectLibrary DiogenesCommon)
add_dependencies(AutocorrectLibrary DiogenesDyninstCommon)
target_link_libraries(AutocorrectLibrary DiogenesDyninstCommon)
target_link_libraries(AutocorrectLibrary "-L${GOTCHA_INSTALL_DIR}/lib -lgotcha")
target_link_libraries(AutocorrectLibrary "-lpthread -ldl")

install(TARGETS AutocorrectLibrary
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)