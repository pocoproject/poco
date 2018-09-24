# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindAPRUTIL
# -------
#
# Find Apache Portable Runtime
#
# This will define the following variables::
#
#   APRUTIL_FOUND           - True if the system has the libraries
#   APRUTIL_INCLUDE_DIRS    - where to find the headers
#   APRUTIL_LIBRARIES       - where to find the libraries
#   APRUTIL_DEFINITIONS     - compile definitons
#
# Hints:
# Set ``APRUTIL_ROOT_DIR`` to the root directory of an installation.
#
include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)
pkg_check_modules(PC_APRUTIL QUIET apr-util-1)

find_path(APRUTIL_INCLUDE_DIR apu.h
	HINTS
		${APRUTIL_ROOT_DIR}/include
		${APRUTIL_ROOT_INCLUDE_DIRS}
	PATHS
		${PC_APRUTIL_INCLUDE_DIRS}
		/usr/local/include
		/usr/include
	PATH_SUFFIXES
		apr-1
		apr-1.0
)

find_library(APRUTIL_LIBRARY
  NAMES aprutil-1 ${APRUTIL_NAMES}
  HINTS
	${APRUTIL_ROOT_DIR}/lib
	${APRUTIL_ROOT_LIBRARY_DIRS}
  PATHS
	${PC_APRUTIL_LIBRARY_DIRS}
	/usr/lib
	/usr/local/lib
)

set(APRUTIL_VERSION ${PC_APRUTIL_VERSION})

find_package_handle_standard_args(APRUTIL
  FOUND_VAR APRUTIL_FOUND
  REQUIRED_VARS
	APRUTIL_INCLUDE_DIR
	APRUTIL_LIBRARY
	VERSION_VAR APRUTIL_VERSION
)

if(APRUTIL_FOUND)
	set(APRUTIL_LIBRARIES ${APRUTIL_LIBRARY})
	set(APRUTIL_INCLUDE_DIRS ${APRUTIL_INCLUDE_DIR})
	set(APRUTIL_DEFINITIONS ${PC_APRUTIL_CFLAGS_OTHER})

	# Deprecated declarations.
	SET (NATIVE_APRUTIL_INCLUDE_PATH ${APRUTIL_INCLUDE_DIR} )
	GET_FILENAME_COMPONENT (NATIVE_APRUTIL_LIB_PATH ${APRUTIL_LIBRARY} PATH)
endif()

if(APRUTIL_FOUND AND NOT TARGET Apache::Aprutil)
  add_library(Apache::Aprutil UNKNOWN IMPORTED)
  set_target_properties(Apache::Aprutil PROPERTIES
	IMPORTED_LOCATION "${APRUTIL_LIBRARY}"
	INTERFACE_COMPILE_OPTIONS "${PC_APRUTIL_CFLAGS_OTHER}"
	INTERFACE_INCLUDE_DIRECTORIES "${APRUTIL_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(
  APRUTIL_LIBRARY
  APRUTIL_INCLUDE_DIR
)
