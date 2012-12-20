# check for location of odbc_config
find_program(UNIX_ODBC_CONFIG odbc_config 
	$ENV{ODBC_PATH}/bin 
	/usr/bin 
	/usr/local/bin 
	PATHS)

if(${UNIX_ODBC_CONFIG} MATCHES "UNIX_ODBC_CONFIG-NOTFOUND")
	message(STATUS "Couldn't find unixODBC")
else(${UNIX_ODBC_CONFIG} MATCHES "UNIX_ODBC_CONFIG-NOTFOUND")
	message(STATUS "Found unixODBC: odbc_config in ${UNIX_ODBC_CONFIG}")
	exec_program(${UNIX_ODBC_CONFIG} ARGS "--include-prefix" OUTPUT_VARIABLE ODBC_INCLUDE_DIR)
	include_directories(${ODBC_INCLUDE_DIR})
	exec_program(${UNIX_ODBC_CONFIG} ARGS "--libs" OUTPUT_VARIABLE ODBC_LINK_FLAGS)
	add_definitions(-DPOCO_UNIXODBC)
endif(${UNIX_ODBC_CONFIG} MATCHES "UNIX_ODBC_CONFIG-NOTFOUND")

find_program(IODBC_CONFIG iodbc-config 
	$ENV{ODBC_PATH}/bin 
	/usr/bin 
	/usr/local/bin 
	PATHS)

if(${IODBC_CONFIG} MATCHES "IODBC_CONFIG-NOTFOUND")
	message(STATUS "Couldn't find iODBC")
else(${IODBC_CONFIG} MATCHES "IODBC_CONFIG-NOTFOUND")
	message(STATUS "Found iODBC: iodbc-config in ${IODBC_CONFIG}")
	exec_program(${IODBC_CONFIG} ARGS "--cflags" OUTPUT_VARIABLE ODBC_CFLAGS)
	add_definitions( ${ODBC_CFLAGS} )
	exec_program(${IODBC_CONFIG} ARGS "--libs" OUTPUT_VARIABLE ODBC_LINK_FLAGS)
	add_definitions(-DPOCO_IODBC)
endif(${IODBC_CONFIG} MATCHES "IODBC_CONFIG-NOTFOUND")

message(STATUS "UNIX ODBC: ${UNIX_ODBC_CONFIG}")
message(STATUS "IODBC: ${IODBC_CONFIG}")
