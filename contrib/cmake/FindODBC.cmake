if(WITH_UNIXODBC)
	# check for location of odbc_config
	find_program(ODBC_CONFIG odbc_config $ENV{ODBC_PATH}/bin PATHS)
		
	if(NOT ODBC_CONFIG)
		message(FATAL_ERROR "Couldn't find unixODBC")
	endif(NOT ODBC_CONFIG)

	message(STATUS "unixODBC: Found odbc_config in ${ODBC_CONFIG}")

	exec_program(${ODBC_CONFIG} ARGS "--include-prefix" OUTPUT_VARIABLE ODBC_INCLUDE_DIR)
	set (CMAKE_FLAGS "${CMAKE_FLAGS} -I${ODBC_INCLUDE_DIR}")

	exec_program(${ODBC_CONFIG} ARGS "--libs" OUTPUT_VARIABLE ODBC_LINK_FLAGS)

else(WITH_UNIXODBC)

	find_program(ODBC_CONFIG iodbc-config $ENV{ODBC_PATH}/bin PATHS)
		
	if(NOT ODBC_CONFIG)
		message(FATAL_ERROR "Couldn't find iODBC")
	endif(NOT ODBC_CONFIG)

	message(STATUS "iODBC: Found iodbc-config in ${ODBC_CONFIG}")

	exec_program(${ODBC_CONFIG} ARGS "--cflags" OUTPUT_VARIABLE ODBC_CFLAGS)
	set(CMAKE_FLAGS "${CMAKE_FLAGS} ${ODBC_CFLAGS}")

	exec_program(${ODBC_CONFIG} ARGS "--libs" OUTPUT_VARIABLE ODBC_LINK_FLAGS)
		
endif(WITH_UNIXODBC)

