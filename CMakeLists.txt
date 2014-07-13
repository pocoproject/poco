# POCO_BUILD_TYPE
# POCO_STATIC
# POCO_UNBUNDLED
# POCO_NO_LOCALE

project(Poco)

cmake_minimum_required(VERSION 2.8.0)

file(STRINGS "${CMAKE_SOURCE_DIR}/libversion" SHARED_LIBRARY_VERSION)

# Read the version information from the VERSION file
file (STRINGS "${CMAKE_SOURCE_DIR}/VERSION" PACKAGE_VERSION )
message(STATUS "Poco package version: ${PACKAGE_VERSION}")
string(REGEX REPLACE "([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9])+\\.[0-9]+" "\\1" CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" CPACK_PACKAGE_VERSION_PATCH ${PACKAGE_VERSION})

set(COMPLETE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(RELEASE_NAME "Unstable-trunk")
set(PROJECT_VERSION ${COMPLETE_VERSION})

# Put the libaries and binaries that get built into directories at the
# top of the build tree rather than in hard-to-find leaf
# directories. This simplifies manual testing and the use of the build
# tree rather than installed Boost libraries.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Windows DLLs are "runtime" for CMake. Output them to "bin" like the Visual Studio projects do.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Append our module directory to CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#################################################################################
# Setup C/C++ compiler options
#################################################################################

if(NOT MSVC_IDE)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
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

# For Debug build types, append a "d" to the library names.
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Set debug library postfix" FORCE)

option(ENABLE_TESTS
  "Set to OFF|ON (default is OFF) to control build of POCO tests & samples" OFF)

option(POCO_STATIC
  "Set to OFF|ON (default is OFF) to control build of POCO as STATIC library" OFF)

option(POCO_UNBUNDLED
  "Set to OFF|ON (default is OFF) to control linking dependencies as external" OFF)

# Uncomment from next two lines to force statitc or dynamic library, default is autodetection
if(POCO_STATIC)
    add_definitions( -DPOCO_STATIC -DPOCO_NO_AUTOMATIC_LIBS)
    set( LIB_MODE STATIC )
    message(STATUS "Building static libraries")
else(POCO_STATIC)
    set( LIB_MODE SHARED )
    message(STATUS "Building dynamic libraries")
endif(POCO_STATIC)

if (ENABLE_TESTS)
  include(CTest)
  enable_testing()
  message(STATUS "Building with unittests & samples")
else ()
  message(STATUS "Building without tests & samples")
endif ()

if (POCO_UNBUNDLED)
  add_definitions( -DPOCO_UNBUNDLED)
  message(STATUS "Build with using external sqlite, libz, pcre, expat ...")
else ()
  message(STATUS "Build with using internal copy of sqlite, libz, pcre, expat, ...")
endif ()

# Set local include path
include_directories( CppUnit/include Foundation/include XML/include Net/include NetSSL_OpenSSL/include Util/include Data/include Data/MySQL/include Data/SQLite/include Data/ODBC/include Zip/include Crypto/include Web/include JSON/include MongoDB/include PDF/include SevenZip/include)

include(CheckTypeSize)
find_package(Cygwin)

#include(CMakeDetermineCompilerId)

# OS Detection
if(CMAKE_SYSTEM MATCHES "Windows")

 add_definitions( -DPOCO_OS_FAMILY_WINDOWS)
 set(SYSLIBS iphlpapi gdi32 odbc32)

 if (CMAKE_C_COMPILER_ID MATCHES "MSVC")
        message(STATUS "XXX: MS Visual Compiler detected")
 endif (CMAKE_C_COMPILER_ID MATCHES "MSVC")

endif(CMAKE_SYSTEM MATCHES "Windows")

if (CMAKE_SYSTEM MATCHES "Linux" AND NOT ANDROID )
  add_definitions( -DPOCO_OS_FAMILY_UNIX )
  # Standard 'must be' defines
  add_definitions( -D_XOPEN_SOURCE=500 -D_REENTRANT -D_THREAD_SAFE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64)
  set(SYSLIBS  pthread dl rt)
endif(CMAKE_SYSTEM MATCHES "Linux" AND NOT ANDROID )

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

# iOS
if (IOS)
  add_definitions( -DPOCO_HAVE_IPv6 -DPOCO_NO_FPENVIRONMENT -DPOCO_NO_STAT64 -DPOCO_NO_SHAREDLIBS -DPOCO_NO_NET_IFTYPES )
endif(IOS)

#ANDROID
if (ANDROID)
  add_definitions( -DPOCO_ANDROID -DPOCO_NO_FPENVIRONMENT -DPOCO_NO_WSTRING -DPOCO_NO_SHAREDMEMORY )
endif()

if (ENABLE_TESTS)
   add_subdirectory(CppUnit)
endif ()

add_subdirectory(Foundation)
add_subdirectory(XML)
add_subdirectory(JSON)
add_subdirectory(MongoDB)
add_subdirectory(PDF)
add_subdirectory(Util)
add_subdirectory(Net)

# OPENSSL_SSL_LIBRARY
find_package(OpenSSL)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    add_subdirectory(NetSSL_OpenSSL)
    add_subdirectory(Crypto)
endif(OPENSSL_FOUND)

add_subdirectory(Data)
add_subdirectory(SevenZip)
add_subdirectory(Zip)

#find_package(ARP)
#find_package(Apache2)
if(APRUTIL_FOUND AND APACHE_FOUND)
    add_subdirectory(ApacheConnector)
endif(APRUTIL_FOUND AND APACHE_FOUND)

#############################################################
# Uninstall stuff see: http://www.vtk.org/Wiki/CMake_FAQ
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#############################################################
# Enable packaging

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Poco Libraries")
set(CPACK_PACKAGE_VENDOR "Applied Informatics Software Engineering GmbH")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "/usr/local")

include(CPack)

message(STATUS "CMake ${CMAKE_VERSION} successfully configured ${PROJECT_NAME} using ${CMAKE_GENERATOR} generator")
message(STATUS "Installation target path: ${CMAKE_INSTALL_PREFIX}")

message(STATUS "C_FLAGS:  =${CMAKE_C_FLAGS}")
message(STATUS "CXX_FLAGS:=${CMAKE_CXX_FLAGS}")

