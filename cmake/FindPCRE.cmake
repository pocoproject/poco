#
# - Find pcre
# Find the native PCRE includes and library
#
#  PCRE_INCLUDE_DIRS - where to find pcre.h, etc.
#  PCRE_LIBRARIES    - List of libraries when using pcre.
#  PCRE_FOUND        - True if pcre found.
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

MARK_AS_ADVANCED( PCRE_LIBRARIES PCRE_INCLUDE_DIRS )
mark_as_advanced(
  PCRE_INCLUDE_DIR
  PCRE_LIBRARY
)
