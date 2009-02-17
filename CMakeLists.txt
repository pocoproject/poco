cmake_minimum_required(VERSION 2.6.0)

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "3")
set(CPACK_PACKAGE_VERSION_PATCH "99")

#set(COMMON_CXX "-Wall -Wno-sign-compare")
set(DEBUG_CXX_FLAGS   "${COMMON_CXX} ${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(RELEASE_CXX_FLAGS "${COMMON_CXX} ${CMAKE_CXX_FLAGS_MINSIZEREL}")

# Uncomment from next two lines to force statitc or dynamic library, default is autodetection
if(POCO_STATIC)
    set( LIB_MODE STATIC )
else(POCO_STATIC)
    set( LIB_MODE SHARED )
endif(POCO_STATIC)

# Set local include path
include_directories( CppUnit/include Foundation/include XML/include Net/include NetSSL_OpenSSL/include Util/include Data/include WebWidgets/include Zip/include)

include(CheckTypeSize)
include(FindCygwin)
include(FindOpenSSL)

include(contrib/cmake/FindMySQL.cmake)
include(contrib/cmake/FindODBC.cmake)

# OS Detection
if(CMAKE_SYSTEM MATCHES "Windows")
 add_definitions( -DPOCO_OS_FAMILY_WINDOWS)
else (CMAKE_SYSTEM MATCHES "Windows")
 add_definitions( -DPOCO_OS_FAMILY_UNIX )
endif(CMAKE_SYSTEM MATCHES "Windows")

# Standard 'must be' defines
add_definitions( -D_XOPEN_SOURCE=500 -D_REENTRANT -D_THREAD_SAFE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 )

# UTF-8 detection (POCO_WIN32_UTF8)

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
add_subdirectory(ApacheConnector)
