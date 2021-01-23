# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindAPR
# -------
#
# Find Apache Portable Runtime
#
# This will define the following variables::
#
#   APR_FOUND           - True if the system has the libraries
#   APR_INCLUDE_DIRS    - where to find the headers
#   APR_LIBRARIES       - where to find the libraries
#   APR_DEFINITIONS     - compile definitions
#
# and the following imported targets::
#
#   Apache::Apr   - The library
#
# Hints:
# Set ``APR_ROOT_DIR`` to the root directory of an installation.
#
include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)
pkg_check_modules(PC_APR QUIET apr-1)

find_path(APR_INCLUDE_DIR apr.h
	HINTS
		${APR_ROOT_DIR}/include
		${APR_ROOT_INCLUDE_DIRS}
	PATHS
		${PC_APR_INCLUDE_DIRS}
		/usr/local/include
		/usr/include
	PATH_SUFFIXES
		apr-1
		apr-1.0
)

find_library(APR_LIBRARY
  NAMES apr-1 ${APR_NAMES}
  HINTS
	${APR_ROOT_DIR}/lib
	${APR_ROOT_LIBRARY_DIRS}
  PATHS
	${PC_APR_LIBRARY_DIRS}
	/usr/lib
	/usr/local/lib
  )

set(APR_VERSION ${PC_APR_VERSION})

find_package_handle_standard_args(APR
  FOUND_VAR APR_FOUND
  REQUIRED_VARS
	APR_INCLUDE_DIR
	APR_LIBRARY
  VERSION_VAR APR_VERSION
)

if(APR_FOUND)
  set(APR_LIBRARIES ${APR_LIBRARY})
  set(APR_INCLUDE_DIRS ${APR_INCLUDE_DIR})
  set(APR_DEFINITIONS ${PC_APR_CFLAGS_OTHER})

  # Deprecated declarations.
  SET (NATIVE_APR_INCLUDE_PATH ${APR_INCLUDE_DIR} )
  GET_FILENAME_COMPONENT (NATIVE_APR_LIB_PATH ${APR_LIBRARY} PATH)
endif()

if(APR_FOUND AND NOT TARGET Apache::Apr)
  add_library(Apache::Apr UNKNOWN IMPORTED)
  set_target_properties(Apache::Apr PROPERTIES
	IMPORTED_LOCATION "${APR_LIBRARY}"
	INTERFACE_COMPILE_OPTIONS "${PC_APR_CFLAGS_OTHER}"
	INTERFACE_INCLUDE_DIRECTORIES "${APR_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(
  APR_LIBRARY
  APR_INCLUDE_DIR
)
