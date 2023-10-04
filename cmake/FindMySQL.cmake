# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindMySQL
# -------
#
# Find MySQL Runtime
#
# This will define the following variables::
#
#   MYSQL_FOUND           - True if the system has the libraries
#   MYSQL_INCLUDE_DIRS    - where to find the headers
#   MYSQL_LIBRARIES       - where to find the libraries
#   MYSQL_DEFINITIONS     - compile definitons
#
# Hints:
# Set ``MYSQL_ROOT_DIR`` to the root directory of an installation.
#
include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)
pkg_check_modules(PC_MYSQL QUIET mysqlclient)
pkg_check_modules(PC_MARIADB QUIET mariadb)

SET(BINDIR32_ENV_NAME "ProgramFiles(x86)")
SET(BINDIR32 $ENV{${BINDIR32_ENV_NAME}})

find_path(MYSQL_INCLUDE_DIR mysql/mysql.h
	HINTS
		${MYSQL_ROOT_DIR}/include
		${MYSQL_ROOT_INCLUDE_DIRS}
	PATHS
		${PC_MYSQL_INCLUDE_DIRS}
		${PC_MARIADB_INCLUDE_DIRS}
		/usr/include
		/usr/local/include
		/opt/mysql/mysql/include
		/usr/local/mysql/include
		$ENV{MYSQL_INCLUDE_DIR}
		$ENV{MYSQL_DIR}/include
		$ENV{ProgramFiles}/MySQL/*/include
		${BINDIR32}/MySQL/*/include
		$ENV{SystemDrive}/MySQL/*/include
		$ENV{MARIADB_INCLUDE_DIR}
		$ENV{MARIADB_DIR}/include
		${MARIADB_INCLUDE_DIR}
		${MARIADB_DIR}/include
	PATH_SUFFIXES
		mysql
		mariadb
)

if (MSVC)
	if (CMAKE_BUILD_TYPE STREQUAL Debug)
		set(libsuffixDist debug)
		set(libsuffixBuild Debug)
	else (CMAKE_BUILD_TYPE STREQUAL Debug)
		set(libsuffixDist opt)
		set(libsuffixBuild Release)
		set(WIN_MYSQL_DEFINITONS " -DDBUG_OFF")
	endif (CMAKE_BUILD_TYPE STREQUAL Debug)

	find_library(MYSQL_LIBRARY NAMES mysqlclient
				 HINTS
					${MYSQL_ROOT_DIR}/lib
					${MYSQL_ROOT_LIBRARY_DIRS}
				 PATHS
					 ${PC_MYSQL_LIBRARY_DIRS}
					 ${PC_MARIADB_LIBRARY_DIRS}
					 $ENV{MYSQL_DIR}/lib
					 $ENV{MYSQL_DIR}/libmysql
					 $ENV{MYSQL_DIR}/client
					 $ENV{ProgramFiles}/MySQL/*/lib
					 ${BINDIR32}/MySQL/*/lib
					 $ENV{SystemDrive}/MySQL/*/lib
				 PATH_SUFFIXES
				 	vs12
				 	vs11
				 	vs10
				 	${libsuffixDist}
				 	${libsuffixBuild}
	)
else()
	find_library(MYSQL_LIBRARY NAMES mysqlclient mysqlclient_r mariadbclient
				 HINTS
					${MYSQL_ROOT_DIR}/lib
					${MYSQL_ROOT_LIBRARY_DIRS}
				 PATHS
					 ${PC_MYSQL_LIBRARY_DIRS}
					 ${PC_MARIADB_LIBRARY_DIRS}
					 /usr/lib
					 /usr/local/lib
					 /usr/local/mysql/lib
					 /opt/mysql/mysql/lib
					 $ENV{MYSQL_DIR}/libmysql_r/.libs
					 $ENV{MYSQL_DIR}/lib
					 ${MYSQL_DIR}/lib
				 PATH_SUFFIXES
					mysql
					mariadb
		)
endif()

set(MYSQL_VERSION ${PC_MYSQL_VERSION})

find_package_handle_standard_args(MySQL
  FOUND_VAR MYSQL_FOUND
  REQUIRED_VARS
	MYSQL_INCLUDE_DIR
	MYSQL_LIBRARY
	VERSION_VAR MYSQL_VERSION
)

if(MYSQL_FOUND)
	set(MYSQL_LIBRARIES ${MYSQL_LIBRARY})
	set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR})
	set(MYSQL_DEFINITIONS "${PC_MYSQL_CFLAGS_OTHER}${WIN_MYSQL_DEFINITONS}")
endif()

if(MYSQL_FOUND AND NOT TARGET MySQL::client)
  add_library(MySQL::client UNKNOWN IMPORTED)
  set_target_properties(MySQL::client PROPERTIES
	IMPORTED_LOCATION "${MYSQL_LIBRARY}"
	INTERFACE_COMPILE_OPTIONS "${PC_MYSQL_CFLAGS_OTHER}${WIN_MYSQL_DEFINITONS}"
	INTERFACE_INCLUDE_DIRECTORIES "${MYSQL_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(
  MYSQL_LIBRARY
  MYSQL_INCLUDE_DIR
)
