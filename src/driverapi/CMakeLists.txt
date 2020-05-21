add_compile_options(-O3 -g)
link_libraries("-L${BOOST_LIBRARYDIR}")
add_library(StubLib SHARED "StubLib.cpp")
target_link_libraries(StubLib ${Boost_LIBRARIES})
target_link_libraries(StubLib ${DYNINST_LIBRARIES})
target_link_libraries(StubLib "-lpthread")
INCLUDE_DIRECTORIES(StubLib ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(StubLib ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(StubLib ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(StubLib ${CMAKE_SOURCE_DIR}/src/driverapi)
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
target_link_libraries(StubLib "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart -L${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/lib64 -lcupti")
set_property(TARGET StubLib PROPERTY POSITION_INDEPENDENT_CODE ON)
add_dependencies(StubLib dyninst)
install(TARGETS StubLib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

add_library(DriverAPIWrapper SHARED "Parameters.cpp" "DriverWrapperBase.cpp" "DriverWrapperFactory.cpp" "DriverAPIWrappers.cpp" "MemoryTransfer.cpp")
target_link_libraries(DriverAPIWrapper ${Boost_LIBRARIES})
target_link_libraries(DriverAPIWrapper ${DYNINST_LIBRARIES})
target_link_libraries(DriverAPIWrapper "-lpthread -ldl")
target_link_libraries(DriverAPIWrapper StubLib)
target_link_libraries(DriverAPIWrapper "-L${GOTTCHA_LIBRARIES} -lgotcha" )
INCLUDE_DIRECTORIES(DriverAPIWrapper ${GOTTCHA_INCLUDE})
INCLUDE_DIRECTORIES(DriverAPIWrapper ${CUDA_TOOLKIT_ROOT_DIR}/include)
INCLUDE_DIRECTORIES(DriverAPIWrapper ${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include)
INCLUDE_DIRECTORIES(DriverAPIWrapper ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(DriverAPIWrapper ${CMAKE_SOURCE_DIR}/src/driverapi)
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
set_property(TARGET DriverAPIWrapper PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(DriverAPIWrapper "-L${CUDA_TOOLKIT_ROOT_DIR}/lib64 -lcudart -L${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/lib64 -lcupti")
add_dependencies(DriverAPIWrapper dyninst)
install(TARGETS DriverAPIWrapper
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

configure_file (
    "${PROJECT_SOURCE_DIR}/src/driverapi/defs/DriverAPIWrapper.def.in"
	"${CMAKE_INSTALL_PREFIX}/defs/DriverAPIWrapper.def"
)

# add_library(InterpositionDriverAPI SHARED "Parameters.cpp" "InterpositionWrappers.cpp" "DriverWrapperFactory.cpp" "DriverWrapperBase.cpp" "StackTraceGen.cpp")
# add_compile_options(-O0 -g)
# target_compile_definitions(InterpositionDriverAPI PRIVATE INTERPOSITION_BUILD=1)
# target_link_libraries(InterpositionDriverAPI ${Boost_LIBRARIES})
# target_link_libraries(InterpositionDriverAPI ${DYNINST_LIBRARIES})
# target_link_libraries(InterpositionDriverAPI "-lpthread")
# INCLUDE_DIRECTORIES(InterpositionDriverAPI ${Boost_INCLUDE_DIRS})
# INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
# add_dependencies(InterpositionDriverAPI dyninst)
# install(TARGETS InterpositionDriverAPI
#         RUNTIME DESTINATION bin
#         LIBRARY DESTINATION lib
#         ARCHIVE DESTINATION lib/static)


# add_library(dlsymwrap SHARED "dlsym_wrapper.cpp")
# target_link_libraries(dlsymwrap ${Boost_LIBRARIES})
# target_link_libraries(dlsymwrap ${DYNINST_LIBRARIES})
# target_link_libraries(dlsymwrap "-lpthread")
# INCLUDE_DIRECTORIES(dlsymwrap ${Boost_INCLUDE_DIRS})
# SET_TARGET_PROPERTIES(dlsymwrap PROPERTIES COMPILE_FLAGS "-Wl,-wrap,dlsym -Wl,-wrap,dlopen -ldl")
# INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/common")
# install(TARGETS dlsymwrap
#         RUNTIME DESTINATION bin
#         LIBRARY DESTINATION lib
#         ARCHIVE DESTINATION lib/static)

add_subdirectory(plugins)
add_subdirectory(launcher)