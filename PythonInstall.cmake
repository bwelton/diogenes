if (PYTHON_VERSION_STRING VERSION_GREATER 3.0 OR PYTHON_VERSION_STRING VERSION_LESS 2.7)
	message(INFO " Detected Python package is not 2.7, automatically installing python 2.7")
	message(INFO " If you have Python 2.7 installed, set -DPYTHON_INCLUDE_DIR, -DPYTHON_EXECUTABLE, and -DPYTHON_LIBRARY")
	file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/wrappers)
	configure_file (
	    "${PROJECT_SOURCE_DIR}/install_wrappers/python2.7/python_install.sh.in"
		"${PROJECT_BINARY_DIR}/wrappers/python_install.sh"
	)
	configure_file (
  		"${PROJECT_SOURCE_DIR}/install_wrappers/python2.7/python_configure.sh.in"
  		"${PROJECT_BINARY_DIR}/wrappers/python_configure.sh"
  	)
	ExternalProject_Add(
	  PYTHON_2_7
	  EXCLUDE_FROM_ALL 1
	  URL "https://www.python.org/ftp/python/2.7.9/Python-2.7.9.tgz"
	  URL_MD5 "5eebcaa0030dc4061156d3429657fb83"
	  SOURCE_DIR "${PROJECT_BINARY_DIR}/3rdparty/python"
	  CONFIGURE_COMMAND cp ${PROJECT_BINARY_DIR}/wrappers/python_configure.sh ${PROJECT_BINARY_DIR}/3rdparty/python/python_configure.sh && sh ${PROJECT_BINARY_DIR}/3rdparty/python/python_configure.sh --prefix=${CMAKE_INSTALL_PREFIX} --enable-shared
	  BUILD_COMMAND $(MAKE) -j 12
	  UPDATE_COMMAND ""  
	)	
	set(PYTHON_EXECUTABLE "${CMAKE_INSTALL_PREFIX}/bin/python")
	set(PYTHON_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
	set(PYTHON_LIBRARY "${CMAKE_INSTALL_PREFIX}/lib/libpython2.7.so")
else (PYTHON_VERSION_STRING VERSION_GREATER 3.0 OR PYTHON_VERSION_STRING VERSION_LESS 2.7)
	add_custom_target(PYTHON_2_7)
endif (PYTHON_VERSION_STRING VERSION_GREATER 3.0 OR PYTHON_VERSION_STRING VERSION_LESS 2.7)
