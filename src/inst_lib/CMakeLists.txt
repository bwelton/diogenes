add_library(inst_lib SHARED "inst.cpp")
add_dependencies(inst_lib dyninst)
add_dependencies(inst_lib PYTHON_2_7)
INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}")
INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIR}")
target_link_libraries(inst_lib ${DYNINST_LIBRARIES})
install(TARGETS inst_lib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)