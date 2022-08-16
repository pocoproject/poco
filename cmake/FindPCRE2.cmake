# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindPCRE2
---------

Finds the PCRE2 library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Pcre2::Pcre2``
  The PCRE2 library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``PCRE2_FOUND``
  True if the system has the PCRE2 library.
``PCRE2_VERSION``
  The version of the PCRE2 library which was found.
``PCRE2_INCLUDE_DIRS``
  Include directories needed to use PCRE2.
``PCRE2_LIBRARIES``
  Libraries needed to link to PCRE2.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``PCRE2_INCLUDE_DIR``
  The directory containing ``pcre2.h``.
``PCRE2_LIBRARY``
  The path to the PCRE2 library.

Hints
^^^^^

``PCRE2_ROOT_DIR``
  The path to the root directory of a PCRE2 installation.
``PCRE2_ROOT_INCLUDE_DIRS``
  The path to the include directory of a PCRE2 installation.
``PCRE2_ROOT_LIBRARY_DIRS``
  The path to the library directory of a PCRE2 installation.

#]=======================================================================]#

include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)
pkg_check_modules(PC_PCRE2 QUIET pcre2)

find_path(PCRE2_INCLUDE_DIR
  NAMES pcre2.h
  HINTS
        ${PCRE2_ROOT_DIR}/include
        ${PCRE2_ROOT_INCLUDE_DIRS}
  PATHS
        ${PC_PCRE2_INCLUDE_DIRS}
  DOC "Specify the include directory containing pcre2.h"
)

find_library(PCRE2_LIBRARY
  NAMES pcre2-8
  HINTS
        ${PCRE2_ROOT_DIR}/lib
        ${PCRE2_ROOT_LIBRARY_DIRS}
  PATHS
        ${PC_PCRE2_LIBRARY_DIRS}
  DOC "Specify the lib directory containing pcre2"
)

set(PCRE2_VERSION ${PC_PCRE2_VERSION})

find_package_handle_standard_args(PCRE2
  FOUND_VAR PCRE2_FOUND
  REQUIRED_VARS
    PCRE2_LIBRARY
    PCRE2_INCLUDE_DIR
  VERSION_VAR PCRE2_VERSION
)

if(PCRE2_FOUND)
  set(PCRE2_LIBRARIES ${PCRE2_LIBRARY})
  set(PCRE2_INCLUDE_DIRS ${PCRE2_INCLUDE_DIR})
  set(PCRE2_DEFINITIONS ${PC_PCRE2_CFLAGS_OTHER})
endif()

if(PCRE2_FOUND AND NOT TARGET Pcre2::Pcre2)
  add_library(Pcre2::Pcre2 UNKNOWN IMPORTED)
  set_target_properties(Pcre2::Pcre2 PROPERTIES
    IMPORTED_LOCATION "${PCRE2_LIBRARY}"
    INTERFACE_COMPILE_OPTIONS "${PC_PCRE2_CFLAGS_OTHER}"
    INTERFACE_INCLUDE_DIRECTORIES "${PCRE2_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(
  PCRE2_INCLUDE_DIR
  PCRE2_LIBRARY
)
