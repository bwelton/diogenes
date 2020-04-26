add_executable(AutoCorrect "Main.cpp")
INCLUDE_DIRECTORIES(AutoCorrect ${CMAKE_SOURCE_DIR}/src/dyninst_common_lib)
INCLUDE_DIRECTORIES(AutoCorrect ${CMAKE_SOURCE_DIR}/src/dyninst_common_lib)
INCLUDE_DIRECTORIES(AutoCorrect ${CMAKE_SOURCE_DIR}/src/Autocorrect/mutator)
INCLUDE_DIRECTORIES(AutoCorrect ${CMAKE_SOURCE_DIR}/src/Autocorrect/mutatee)
INCLUDE_DIRECTORIES(AutoCorrect ${CMAKE_SOURCE_DIR}/src/MemGraphBuild/mutatee)
INCLUDE_DIRECTORIES(AutoCorrect ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(DiogenesDyninstCommon ${CMAKE_SOURCE_DIR}/src/common_lib/extern/procparser)
INCLUDE_DIRECTORIES(AutoCorrect ${GOTCHA_INSTALL_DIR}/include)
INCLUDE_DIRECTORIES(AutoCorrect ${CMAKE_SOURCE_DIR}/src/common_lib)

add_dependencies(AutoCorrect DiogenesCommon)
target_link_libraries(AutoCorrect DiogenesCommon)
add_dependencies(AutoCorrect DiogenesDyninstCommon)
target_link_libraries(AutoCorrect DiogenesDyninstCommon)
target_link_libraries(AutoCorrect "-L${GOTCHA_INSTALL_DIR}/lib -lgotcha")
target_link_libraries(AutoCorrect "-lpthread -ldl")

install(TARGETS AutoCorrect
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include)