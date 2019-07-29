file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/scripts)
configure_file (
  "${PROJECT_SOURCE_DIR}/src/scripts/inst_app.sh.in"
  "${PROJECT_BINARY_DIR}/scripts/inst_app.sh"
  @ONLY
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/src/scripts/SetEnv.sh.in"
  "${PROJECT_BINARY_DIR}/scripts/SetEnv.sh"
  @ONLY
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/src/scripts/BuildInstrimentation.sh.in"
  "${PROJECT_BINARY_DIR}/scripts/BuildInstrimentation.sh"
  @ONLY
  )

install(FILES ${PROJECT_BINARY_DIR}/scripts/inst_app.sh 
		DESTINATION bin)

install(FILES ${PROJECT_BINARY_DIR}/scripts/BuildInstrimentation.sh 
		DESTINATION bin)

