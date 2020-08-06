# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindApache2
# -------
#
# Find Apache2 Runtime
#
# This will define the following variables::
#
#   APACHE2_FOUND           - True if the system has the Apache2 library
#   APACHE2_INCLUDE_DIRS    - where to find httpd.h, etc.
#
# Hints:
# Set ``APACHE2_ROOT_DIR`` to the root directory of an Apache installation.
#
include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)
pkg_check_modules(PC_APACHE2 QUIET apache2)

if(WIN32)
	set(APACHE2_PRIMARY_INCLUDE_HINT "${APACHE2_ROOT_DIR}/include")
else(WIN32)
	set(APACHE2_PRIMARY_INCLUDE_HINT "${APACHE2_ROOT_DIR}/include/apache2")
endif(WIN32)

find_path(APACHE2_INCLUDE_DIR httpd.h
	HINTS
		${APACHE2_PRIMARY_INCLUDE_HINT}
		${APACHE2_ROOT_INCLUDE_DIRS}
	PATHS
		${PC_APACHE2_INCLUDE_DIRS}
		/usr/local/include/apache2
		/usr/include/apache2
)

set(APACHE2_REQUIRED_VARS APACHE2_INCLUDE_DIR)
if(WIN32)
	find_library(APACHE2_LIBRARY
		NAMES libhttpd  ${APACHE2_NAMES}
		HINTS
			${APACHE2_ROOT_DIR}/lib
			${APACHE2_ROOT_LIBRARY_DIRS}
		PATHS
			${PC_APACHE2_LIBRARY_DIRS}
			/usr/lib
			/usr/local/lib
	)
	list(APPEND APACHE2_REQUIRED_VARS APACHE2_LIBRARY)
endif(WIN32)

set(APACHE2_VERSION ${PC_APACHE2_VERSION})

find_package_handle_standard_args(Apache2
	FOUND_VAR APACHE2_FOUND
	REQUIRED_VARS ${APACHE2_REQUIRED_VARS}
	VERSION_VAR APACHE2_VERSION
)

if(APACHE2_FOUND)
	set(APACHE2_INCLUDE_DIRS ${APACHE2_INCLUDE_DIR})
	set(APACHE2_DEFINITIONS ${PC_APACHE2_CFLAGS_OTHER})
endif()

if(WIN32)
  if(APACHE2_FOUND AND NOT TARGET Apache::httpd)
    add_library(Apache::httpd UNKNOWN IMPORTED)
    set_target_properties(Apache::httpd PROPERTIES
	IMPORTED_LOCATION "${APACHE2_LIBRARY}"
	INTERFACE_COMPILE_OPTIONS "${PC_APACHE2_CFLAGS_OTHER}"
	INTERFACE_INCLUDE_DIRECTORIES "${APACHE2_INCLUDE_DIR}"
    )
  endif()
endif()

mark_as_advanced(
	APACHE2_INCLUDE_DIR
	APACHE2_LIBRARY
)
