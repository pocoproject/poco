PROJECT(Poco)

cmake_minimum_required(VERSION 2.8.0)

set(SHARED_LIBRARY_VERSION "13")

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "5")
set(CPACK_PACKAGE_VERSION_PATCH "0")
SET(COMPLETE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
SET(RELEASE_NAME "Unstable-trunk")
SET(PROJECT_VERSION ${COMPLETE_VERSION})

#################################################################################
# Setup C/C++ compiler options
#################################################################################

if(NOT MSVC_IDE)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release" FORCE)
  endif()
  message(STATUS "Setting Poco build type - ${CMAKE_BUILD_TYPE}")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "")
    set( CMAKE_BUILD_TYPE "RelWithDebInfo" )
endif ()

# http://www.cmake.org/Wiki/CMake_Useful_Variables :
# CMAKE_BUILD_TYPE 
#    Choose the type of build. CMake has default flags for these: 
#
#    * None (CMAKE_C_FLAGS or CMAKE_CXX_FLAGS used)
#    * Debug (CMAKE_C_FLAGS_DEBUG or CMAKE_CXX_FLAGS_DEBUG)
#    * Release (CMAKE_C_FLAGS_RELEASE or CMAKE_CXX_FLAGS_RELEASE)
#    * RelWithDebInfo (CMAKE_C_FLAGS_RELWITHDEBINFO or CMAKE_CXX_FLAGS_RELWITHDEBINFO
#    * MinSizeRel (CMAKE_C_FLAGS_MINSIZEREL or CMAKE_CXX_FLAGS_MINSIZEREL) 
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Debug output enabled")
  set(LIB_EXT "d" CACHE STRING "Set debug library postfix" FORCE)
else ()
  message(STATUS "Optimized output enabled")
  set(LIB_EXT "" CACHE STRING "Set debug library postfix" FORCE)
endif ()

option(ENABLE_TESTS
  "Set to OFF|ON (default is OFF) to control build of POCO tests & samples" OFF)

option(POCO_STATIC
  "Set to OFF|ON (default is OFF) to control build of POCO as STATIC library" OFF)

# Uncomment from next two lines to force statitc or dynamic library, default is autodetection
if(POCO_STATIC)
    set( LIB_MODE STATIC )
    message(STATUS "Building static libraries")
else(POCO_STATIC)
    set( LIB_MODE SHARED )
    message(STATUS "Building dynamic libraries")
endif(POCO_STATIC)

IF (ENABLE_TESTS)
  include(CTest)
  ENABLE_TESTING()
  message(STATUS "Building with unittests & samples")
ELSE ()
  message(STATUS "Building without tests & samples")
ENDIF ()

# Set local include path
include_directories( CppUnit/include Foundation/include XML/include Net/include NetSSL_OpenSSL/include Util/include Data/include WebWidgets/include Zip/include Crypto/include Web/include)

include(CheckTypeSize)
include(FindCygwin)
include(FindOpenSSL)
#include(CMakeDetermineCompilerId)

include(contrib/cmake/FindMySQL.cmake)
include(contrib/cmake/FindAPR.cmake)
include(contrib/cmake/FindApache2.cmake)

# OS Detection
if(CMAKE_SYSTEM MATCHES "Windows")
 
 add_definitions( -DPOCO_OS_FAMILY_WINDOWS)
 set(SYSLIBS iphlpapi gdi32 odbc32)
 
 if (CMAKE_C_COMPILER_ID MATCHES "MSVC")
        message(STATUS "XXX: MS Visual Compiler detected")
 endif (CMAKE_C_COMPILER_ID MATCHES "MSVC")

endif(CMAKE_SYSTEM MATCHES "Windows")

if (CMAKE_SYSTEM MATCHES "Linux")
  add_definitions( -DPOCO_OS_FAMILY_UNIX )
  # Standard 'must be' defines
  add_definitions( -D_XOPEN_SOURCE=500 -D_REENTRANT -D_THREAD_SAFE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64)
  set(SYSLIBS  pthread dl rt)
endif(CMAKE_SYSTEM MATCHES "Linux")

if (CMAKE_SYSTEM MATCHES "SunOS")
  add_definitions( -DPOCO_OS_FAMILY_UNIX )
  # Standard 'must be' defines
  add_definitions( -D_XOPEN_SOURCE=500 -D_REENTRANT -D_THREAD_SAFE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 )
  set(SYSLIBS  pthread socket xnet nsl resolv rt dl)
endif(CMAKE_SYSTEM MATCHES "SunOS")

if (CMAKE_COMPILER_IS_MINGW)
    add_definitions(-DWC_NO_BEST_FIT_CHARS=0x400  -DPOCO_WIN32_UTF8)
    add_definitions(-mno-cygwin -D_WIN32 -DMINGW32 -DWINVER=0x500 -DODBCVER=0x0300 -DPOCO_THREAD_STACK_SIZE -DFoundation_Config_INCLUDED )
  link_directories(/usr/local/lib /usr/lib)
  include_directories(/usr/local/include /usr/include)
endif (CMAKE_COMPILER_IS_MINGW)

if (CMAKE_COMPILER_IS_CYGWIN)
#    add_definitions(-DWC_NO_BEST_FIT_CHARS=0x400  -DPOCO_WIN32_UTF8)
endif (CMAKE_COMPILER_IS_CYGWIN)

# SunPro C++
if (${CMAKE_CXX_COMPILER_ID} MATCHES "SunPro")
  add_definitions( -D_BSD_SOURCE -library=stlport4)
endif (${CMAKE_CXX_COMPILER_ID} MATCHES "SunPro")

IF (ENABLE_TESTS)
   add_subdirectory(CppUnit)
ENDIF ()

add_subdirectory(Foundation)
add_subdirectory(XML)
add_subdirectory(Util)
add_subdirectory(Net)
#add_subdirectory(Web)

# OPENSSL_SSL_LIBRARY
if(OPENSSL_FOUND)
    add_subdirectory(NetSSL_OpenSSL)
    add_subdirectory(Crypto)
endif(OPENSSL_FOUND)

add_subdirectory(Data)
add_subdirectory(WebWidgets)
add_subdirectory(Zip)

if(APRUTIL_FOUND AND APACHE_FOUND)
    add_subdirectory(ApacheConnector)
endif(APRUTIL_FOUND AND APACHE_FOUND)

#############################################################
# Uninstall stuff see: http://www.vtk.org/Wiki/CMake_FAQ
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/contrib/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#############################################################
# Enable packaging

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Poco Libraries")
SET(CPACK_PACKAGE_VENDOR "Applied Informatics Software Engineering GmbH")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "/usr/local")

INCLUDE(CPack)

message(STATUS "CMake ${CMAKE_VERSION} successfully configured ${PROJECT_NAME} using ${CMAKE_GENERATOR} generator")

message(STATUS "C_FLAGS:  =${CMAKE_C_FLAGS}")
message(STATUS "CXX_FLAGS:=${CMAKE_CXX_FLAGS}")

