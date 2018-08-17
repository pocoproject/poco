SET(BINDIR32_ENV_NAME "ProgramFiles(x86)")
SET(BINDIR32 $ENV{${BINDIR32_ENV_NAME}})

find_path(MYSQL_INCLUDE_DIR mysql.h
		/usr/include/mysql
		/usr/local/include/mysql
		/opt/mysql/mysql/include
		/opt/mysql/mysql/include/mysql
		/usr/local/mysql/include
		/usr/local/mysql/include/mysql
		$ENV{MYSQL_INCLUDE_DIR}
		$ENV{MYSQL_DIR}/include
		$ENV{ProgramFiles}/MySQL/*/include
		${BINDIR32}/MySQL/include
		${BINDIR32}/MySQL/*/include
		$ENV{SystemDrive}/MySQL/*/include
		${MYSQL_INCLUDE_DIR}
		${MYSQL_DIR}/include)

if (NOT MYSQL_INCLUDE_DIR)
	find_path(MARIADB_INCLUDE_DIR mysql.h
			/usr/include/mariadb
			/usr/local/include/mariadb
			/opt/mariadb/mariadb/include
			/opt/mariadb/mariadb/include/mariadb
			/usr/local/mariadb/include
			/usr/local/mariadb/include/mariadb
			$ENV{MARIADB_INCLUDE_DIR}
			$ENV{MARIADB_DIR}/include
			${MARIADB_INCLUDE_DIR}
			${MARIADB_DIR}/include)
endif (NOT MYSQL_INCLUDE_DIR)

if (MSVC)
	if (CMAKE_BUILD_TYPE STREQUAL Debug)
		set(libsuffixDist debug)
		set(libsuffixBuild Debug)
	else (CMAKE_BUILD_TYPE STREQUAL Debug)
		set(libsuffixDist opt)
		set(libsuffixBuild Release)
		add_definitions(-DDBUG_OFF)
	endif (CMAKE_BUILD_TYPE STREQUAL Debug)
	find_library(MYSQL_LIB NAMES mysqlclient.lib
				 PATHS
				 $ENV{MYSQL_DIR}/lib/${libsuffixDist}
				 $ENV{MYSQL_DIR}/libmysql/${libsuffixBuild}
				 $ENV{MYSQL_DIR}/client/${libsuffixBuild}
				 $ENV{ProgramFiles}/MySQL/*/lib/${libsuffixDist}
				 ${BINDIR32}/MySQL/lib
				 ${BINDIR32}/MySQL/*/lib
				 ${BINDIR32}/MySQL/*/lib/vs12
				 ${BINDIR32}/MySQL/*/lib/vs11
				 ${BINDIR32}/MySQL/*/lib/vs10
				 $ENV{SystemDrive}/MySQL/*/lib/${libsuffixDist})
else (MSVC)
	find_library(MYSQL_LIB NAMES mysqlclient mysqlclient_r
				 PATHS
				 /usr/lib/mysql
				 /usr/local/lib/mysql
				 /usr/local/mysql/lib
				 /usr/local/mysql/lib/mysql
				 /opt/mysql/mysql/lib
				 /opt/mysql/mysql/lib/mysql
				 $ENV{MYSQL_DIR}/libmysql_r/.libs
				 $ENV{MYSQL_DIR}/lib
				 $ENV{MYSQL_DIR}/lib/mysql
				 ${MYSQL_DIR}/lib)

	if (NOT MYSQL_LIB)
		find_library(MARIADB_LIB NAMES mariadbclient
					PATHS
					/usr/lib/mariadb
					/usr/local/lib/mariadb
					/usr/local/mariadb/lib
					/usr/local/mariadb/lib/mariadb
					/opt/mariadb/mariadb/lib
					/opt/mariadb/mariadb/lib/mariadb
					$ENV{MARIADB_DIR}/libmariadb/.libs
					$ENV{MARIADB_DIR}/lib
					$ENV{MARIADB_DIR}/lib/mariadb
					${MARIADB_DIR}/lib)
	endif (NOT MYSQL_LIB)
endif (MSVC)

if (MYSQL_INCLUDE_DIR AND MYSQL_LIB)
	get_filename_component(MYSQL_LIB_DIR ${MYSQL_LIB} PATH)
	set(MYSQL_FOUND TRUE)
	message(STATUS "Found MySQL Include directory: ${MYSQL_INCLUDE_DIR}  library directory: ${MYSQL_LIB_DIR}")
	include_directories(${MYSQL_INCLUDE_DIR})
	link_directories(${MYSQL_LIB_DIR})
elseif((MARIADB_INCLUDE_DIR OR MYSQL_INCLUDE_DIR) AND MARIADB_LIB)
	get_filename_component(MYSQL_LIB_DIR ${MARIADB_LIB} PATH)
	set(MYSQL_FOUND TRUE)
	set(MYSQL_LIB ${MARIADB_LIB})
	if(MARIADB_INCLUDE_DIR)
	  set(MYSQL_INCLUDE_DIR ${MARIADB_INCLUDE_DIR})
	endif(MARIADB_INCLUDE_DIR)
	message(STATUS "Found MariaDB Include directory: ${MYSQL_INCLUDE_DIR}  library directory: ${MYSQL_LIB_DIR}")
	message(STATUS "Use MariaDB for MySQL Support")
	include_directories(${MYSQL_INCLUDE_DIR} )
	link_directories(${MYSQL_LIB_DIR})
else ((MARIADB_INCLUDE_DIR OR MYSQL_INCLUDE_DIR) AND MARIADB_LIB)
	message(STATUS "Couldn't find MySQL or MariaDB")
endif (MYSQL_INCLUDE_DIR AND MYSQL_LIB)
