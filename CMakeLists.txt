PROJECT(poco)

cmake_minimum_required(VERSION 2.4.8)

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "3")
set(CPACK_PACKAGE_VERSION_PATCH "99")
SET(COMPLETE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
SET(RELEASE_NAME "Unstable-trunk")
SET(PROJECT_VERSION ${COMPLETE_VERSION})

#set(COMMON_CXX "-Wall -Wno-sign-compare")
set(DEBUG_CXX_FLAGS   "${COMMON_CXX} ${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(RELEASE_CXX_FLAGS "${COMMON_CXX} ${CMAKE_CXX_FLAGS_MINSIZEREL}")

# Uncomment from next two lines to force statitc or dynamic library, default is autodetection
if(POCO_STATIC)
    set( LIB_MODE STATIC )
else(POCO_STATIC)
    set( LIB_MODE SHARED )
endif(POCO_STATIC)

# whether unit tests should be build
SET (ENABLE_TESTS FALSE CACHE BOOL "Build unit tests?")

IF (ENABLE_TESTS)
  ENABLE_TESTING()
ENDIF (ENABLE_TESTS)

# Set local include path
include_directories( CppUnit/include Foundation/include XML/include Net/include NetSSL_OpenSSL/include Util/include Data/include WebWidgets/include Zip/include)

include(CheckTypeSize)
include(FindCygwin)
include(FindOpenSSL)

include(contrib/cmake/FindMySQL.cmake)
include(contrib/cmake/FindAPR.cmake)
include(contrib/cmake/FindApache2.cmake)

# OS Detection
if(CMAKE_SYSTEM MATCHES "Windows")
 add_definitions( -DPOCO_OS_FAMILY_WINDOWS -DPOCO_WIN32_UTF8)
  set(SYSLIBS iphlpapi gdi32)
endif(CMAKE_SYSTEM MATCHES "Windows")

if (CMAKE_SYSTEM MATCHES "Linux")
  add_definitions( -DPOCO_OS_FAMILY_UNIX )
  # Standard 'must be' defines
  add_definitions( -D_XOPEN_SOURCE=500 -D_REENTRANT -D_THREAD_SAFE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 )
  set(SYSLIBS  pthread dl rt)
endif(CMAKE_SYSTEM MATCHES "Linux")

if (CMAKE_SYSTEM MATCHES "SunOS")
  add_definitions( -DPOCO_OS_FAMILY_UNIX )
  # Standard 'must be' defines
  add_definitions( -D_XOPEN_SOURCE=500 -D_REENTRANT -D_THREAD_SAFE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 )
  set(SYSLIBS  pthread socket xnet nsl resolv rt dl)
endif(CMAKE_SYSTEM MATCHES "SunOS")

if (CMAKE_COMPILER_IS_MINGW)
    add_definitions(-DWC_NO_BEST_FIT_CHARS=0x400)
    add_definitions(-mno-cygwin -D_WIN32 -DMINGW32 -DWINVER=0x500 -DODBCVER=0x0300 -DPOCO_THREAD_STACK_SIZE -DFoundation_Config_INCLUDED )
  link_directories(/usr/local/lib /usr/lib)
  include_directories(/usr/local/include /usr/include)
endif (CMAKE_COMPILER_IS_MINGW)

if (CMAKE_COMPILER_IS_CYGWIN)
#    add_definitions(-DWC_NO_BEST_FIT_CHARS=0x400)
endif (CMAKE_COMPILER_IS_CYGWIN)

add_subdirectory(CppUnit)
add_subdirectory(Foundation)
add_subdirectory(XML)
add_subdirectory(Util)
add_subdirectory(Net)

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
