# check for location of odbc_config
find_program(ODBC_CONFIG odbc_config 
	$ENV{ODBC_PATH}/bin 
	/usr/bin 
	/usr/local/bin 
	PATHS)

if(${ODBC_CONFIG} MATCHES "ODBC_CONFIG-NOTFOUND")
	message(STATUS "Couldn't find unixODBC")
else(${ODBC_CONFIG} MATCHES "ODBC_CONFIG-NOTFOUND")
	message(STATUS "Found unixODBC: odbc_config in ${ODBC_CONFIG}")
	exec_program(${ODBC_CONFIG} ARGS "--include-prefix" OUTPUT_VARIABLE ODBC_INCLUDE_DIR)
	include_directories(${ODBC_INCLUDE_DIR})
	exec_program(${ODBC_CONFIG} ARGS "--libs" OUTPUT_VARIABLE ODBC_LINK_FLAGS)
	add_definitions(-DPOCO_UNIXODBC)
endif(${ODBC_CONFIG} MATCHES "ODBC_CONFIG-NOTFOUND")

if(${ODBC_CONFIG} MATCHES "ODBC_CONFIG-NOTFOUND")
	find_program(ODBC_CONFIG iodbc-config 
		$ENV{ODBC_PATH}/bin 
		/usr/bin 
		/usr/local/bin 
		PATHS)
	if(${ODBC_CONFIG} MATCHES "ODBC_CONFIG-NOTFOUND")
		message(STATUS "Couldn't find iODBC")
	else(${ODBC_CONFIG} MATCHES "ODBC_CONFIG-NOTFOUND")
		message(STATUS "Found iODBC: iodbc-config in ${ODBC_CONFIG}")
		exec_program(${ODBC_CONFIG} ARGS "--cflags" OUTPUT_VARIABLE ODBC_CFLAGS)
		add_definitions( ${ODBC_CFLAGS} )
		exec_program(${ODBC_CONFIG} ARGS "--libs" OUTPUT_VARIABLE ODBC_LINK_FLAGS)
		add_definitions(-DPOCO_IODBC)
	endif(${ODBC_CONFIG} MATCHES "ODBC_CONFIG-NOTFOUND")
endif(${ODBC_CONFIG} MATCHES "ODBC_CONFIG-NOTFOUND")

if(${ODBC_CONFIG} MATCHES "ODBC_CONFIG-NOTFOUND")
	#try odbc32.lib on windows
endif(${ODBC_CONFIG} MATCHES "ODBC_CONFIG-NOTFOUND")
