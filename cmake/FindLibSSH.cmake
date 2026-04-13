# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindLibSSH
# ----------
#
# Find the libssh library.
#
# This will define the following variables::
#
#   LIBSSH_FOUND           - True if the system has libssh
#   LIBSSH_INCLUDE_DIRS    - where to find libssh/libssh.h
#   LIBSSH_LIBRARIES       - the libraries to link against
#   LIBSSH_VERSION         - the version of libssh found
#
# Hints:
# Set ``LIBSSH_ROOT_DIR`` to the root directory of an installation.
#

include(FindPackageHandleStandardArgs)

# Try pkg-config first (Linux, macOS with Homebrew)
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
	pkg_check_modules(PC_LIBSSH QUIET libssh)
endif()

find_path(LIBSSH_INCLUDE_DIR libssh/libssh.h
	HINTS
		${LIBSSH_ROOT_DIR}/include
		${PC_LIBSSH_INCLUDE_DIRS}
	PATHS
		/usr/include
		/usr/local/include
		/opt/homebrew/include
		$ENV{LIBSSH_ROOT_DIR}/include
)

find_library(LIBSSH_LIBRARY
	NAMES ssh libssh
	HINTS
		${LIBSSH_ROOT_DIR}/lib
		${PC_LIBSSH_LIBRARY_DIRS}
	PATHS
		/usr/lib
		/usr/local/lib
		/opt/homebrew/lib
		$ENV{LIBSSH_ROOT_DIR}/lib
)

if(PC_LIBSSH_VERSION)
	set(LIBSSH_VERSION ${PC_LIBSSH_VERSION})
elseif(LIBSSH_INCLUDE_DIR)
	file(STRINGS "${LIBSSH_INCLUDE_DIR}/libssh/libssh.h" _libssh_version_str
		REGEX "^#define[\t ]+LIBSSH_VERSION[\t ]+\"[^\"]*\"")
	if(_libssh_version_str)
		string(REGEX REPLACE "^#define[\t ]+LIBSSH_VERSION[\t ]+\"([^\"]*)\".*" "\\1"
			LIBSSH_VERSION "${_libssh_version_str}")
	endif()
endif()

find_package_handle_standard_args(LibSSH
	REQUIRED_VARS LIBSSH_LIBRARY LIBSSH_INCLUDE_DIR
	VERSION_VAR LIBSSH_VERSION
)

if(LIBSSH_FOUND)
	set(LIBSSH_LIBRARIES ${LIBSSH_LIBRARY})
	set(LIBSSH_INCLUDE_DIRS ${LIBSSH_INCLUDE_DIR})
endif()

mark_as_advanced(LIBSSH_INCLUDE_DIR LIBSSH_LIBRARY)
