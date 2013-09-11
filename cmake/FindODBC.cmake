# check for location of odbc_config
find_program(UNIX_ODBC_CONFIG odbc_config 
	$ENV{ODBC_PATH}/bin 
	/usr/bin 
	/usr/local/bin 
	PATHS)

# Check that we can find the ODBC headers in `odbc_config --include-prefix`
# location If the headers cannot be found, set UNIX_ODBC_CONFIG to
# UNIX_ODBC_CONFIG-NOTFOUND since the odbc_config tool is essentially useless
# if `odbc_config --include-prefix` doesn't actually point to ODBC headers
if(NOT ${UNIX_ODBC_CONFIG} MATCHES "UNIX_ODBC_CONFIG-NOTFOUND")
	execute_process(COMMAND ${UNIX_ODBC_CONFIG} --include-prefix
                        OUTPUT_VARIABLE UNIX_ODBC_INCLUDE_PREFIX
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
	if(NOT EXISTS ${UNIX_ODBC_INCLUDE_PREFIX}/sqlext.h)
        	set(UNIX_ODBC_CONFIG "UNIX_ODBC_CONFIG-NOTFOUND")
	endif(NOT EXISTS ${UNIX_ODBC_INCLUDE_PREFIX}/sqlext.h)
endif(NOT ${UNIX_ODBC_CONFIG} MATCHES "UNIX_ODBC_CONFIG-NOTFOUND")

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
