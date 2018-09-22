# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindMySQL
# -------
#
# Find ODBC Runtime
#
# This will define the following variables::
#
#   ODBC_FOUND           - True if the system has the libraries
#   ODBC_INCLUDE_DIRS    - where to find the headers
#   ODBC_LIBRARIES       - where to find the libraries
#   ODBC_DEFINITIONS     - compile definitons
#
# Hints:
# Set ``ODBC_ROOT_DIR`` to the root directory of an installation.
#
include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)
pkg_check_modules(PC_ODBC QUIET odbc)

find_path(ODBC_INCLUDE_DIR
	NAMES sql.h
	HINTS
		${ODBC_ROOT_DIR}/include
	PATHS
		${PC_ODBC_INCLUDE_DIRS}
		/usr/include
		/usr/local/include
		/usr/local/odbc/include
		/usr/local/iodbc/include
		"C:/Program Files/ODBC/include"
		"C:/Program Files/Microsoft SDKs/Windows/v7.0/include"
		"C:/Program Files/Microsoft SDKs/Windows/v6.0a/include"
		"C:/ODBC/include"
	PATH_SUFFIXES
		odbc
		iodbc
	DOC "Specify the directory containing sql.h."
)

find_library(ODBC_LIBRARY
	NAMES iodbc odbc odbcinst odbc32
	HINTS
		${ODBC_ROOT_DIR}/lib
	PATHS
		${PC_ODBC_LIBRARY_DIRS}
		/usr/lib
		/usr/local/lib
		/usr/local/odbc/lib
		"C:/Program Files/ODBC/lib"
		"C:/ODBC/lib/debug"
		"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib"
	PATH_SUFIXES
		odbc
	DOC "Specify the ODBC driver manager library here."
)

# MinGW find usually fails... TODO(Bjoe) Verify if this is not set via find_package(ODBC)
if(MINGW)
	set(ODBC_INCLUDE_DIR ".")
	set(ODBC_LIBRARY odbc32)
endif()
if(WIN32 AND NOT WINCE)
	set(ODBC_LIBRARY "odbc32" "odbccp32") # TODO(Bjoe) Verify if this is not set via find_package(ODBC)
endif()

find_package_handle_standard_args(ODBC
	FOUND_VAR ODBC_FOUND
	REQUIRED_VARS
		ODBC_LIBRARY
		ODBC_INCLUDE_DIR
	VERSION_VAR ODBC_VERSION
)

if(ODBC_FOUND)
  set(ODBC_LIBRARIES ${ODBC_LIBRARY})
  set(ODBC_INCLUDE_DIRS ${ODBC_INCLUDE_DIR})
  set(ODBC_DEFINITIONS ${PC_ODBC_CFLAGS_OTHER})
endif()

if(ODBC_FOUND AND NOT TARGET odbc::odbc)
  add_library(odbc::odbc UNKNOWN IMPORTED)
  set_target_properties(odbc::odbc PROPERTIES
	IMPORTED_LOCATION "${ODBC_LIBRARY}"
	INTERFACE_COMPILE_OPTIONS "${PC_ODBC_CFLAGS_OTHER}"
	INTERFACE_INCLUDE_DIRECTORIES "${ODBC_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(ODBC_LIBRARY ODBC_INCLUDE_DIR)
