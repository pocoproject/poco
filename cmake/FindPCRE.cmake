# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindPCRE
-------

Finds the PCRE library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Pcre::Pcre``
  The PCRE library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``PCRE_FOUND``
  True if the system has the PCRE library.
``PCRE_VERSION``
  The version of the PCRE library which was found.
``PCRE_INCLUDE_DIRS``
  Include directories needed to use PCRE.
``PCRE_LIBRARIES``
  Libraries needed to link to PCRE.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``PCRE_INCLUDE_DIR``
  The directory containing ``foo.h``.
``PCRE_LIBRARY``
  The path to the PCRE library.

Hints
^^^^^

``PCRE_ROOT_DIR``
  The path to the root directory of a PCRE installation.
``PCRE_ROOT_INCLUDE_DIRS``
  The path to the include directory of a PCRE installation.
``PCRE_ROOT_LIBRARY_DIRS``
  The path to the library directory of a PCRE installation.

#]=======================================================================]#

include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)
pkg_check_modules(PC_PCRE QUIET pcre)

find_path(PCRE_INCLUDE_DIR
  NAMES pcre.h
  HINTS
        ${PCRE_ROOT_DIR}/include
        ${PCRE_ROOT_INCLUDE_DIRS}
  PATHS 
        ${PC_PCRE_INCLUDE_DIRS}
  DOC "Specify the include directory containing pcre.h"
)

find_library(PCRE_LIBRARY
  NAMES pcre
  HINTS
        ${PCRE_ROOT_DIR}/lib
        ${PCRE_ROOT_LIBRARY_DIRS}
  PATHS
        ${PC_PCRE_LIBRARY_DIRS}
  DOC "Specify the lib directory containing pcre"
)

set(PCRE_VERSION ${PC_PCRE_VERSION})

find_package_handle_standard_args(PCRE
  FOUND_VAR PCRE_FOUND
  REQUIRED_VARS
    PCRE_LIBRARY
    PCRE_INCLUDE_DIR
  VERSION_VAR PCRE_VERSION
)

if(PCRE_FOUND)
  set(PCRE_LIBRARIES ${PCRE_LIBRARY})
  set(PCRE_INCLUDE_DIRS ${PCRE_INCLUDE_DIR})
  set(PCRE_DEFINITIONS ${PC_PCRE_CFLAGS_OTHER})
endif()

if(PCRE_FOUND AND NOT TARGET Pcre::Pcre)
  add_library(Pcre::Pcre UNKNOWN IMPORTED)
  set_target_properties(Pcre::Pcre PROPERTIES
    IMPORTED_LOCATION "${PCRE_LIBRARY}"
    INTERFACE_COMPILE_OPTIONS "${PC_PCRE_CFLAGS_OTHER}"
    INTERFACE_INCLUDE_DIRECTORIES "${PCRE_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(
  PCRE_INCLUDE_DIR
  PCRE_LIBRARY
)
