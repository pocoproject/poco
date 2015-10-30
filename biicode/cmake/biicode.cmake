# Append biicode module directory to CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake
                                           ${CMAKE_CURRENT_SOURCE_DIR}/biicode/cmake)

include(BiiPocoMacros)

FILTER_BAD_DEPENDENCIES()

# Actually create targets: EXEcutables and libraries.
ADD_BIICODE_TARGETS()

option(POCO_STATIC
  "Set to OFF|ON (default is OFF) to control build of POCO as STATIC library" ON)

# Uncomment from next two lines to force statitc or dynamic library, default is autodetection
if(POCO_STATIC)
    target_compile_definitions(${BII_BLOCK_TARGET} INTERFACE  -DPOCO_STATIC -DPOCO_NO_AUTOMATIC_LIBS)
    set( LIB_MODE STATIC )
    message(STATUS "Building static libraries")
else(POCO_STATIC)
    set( LIB_MODE SHARED )
    message(STATUS "Building dynamic libraries")
endif(POCO_STATIC)

# OS Detection
if(WIN32)
  target_compile_definitions(${BII_BLOCK_TARGET} INTERFACE  -DUNICODE -D_UNICODE)
  #set(SYSLIBS iphlpapi gdi32 odbc32)
endif(WIN32)

if (UNIX AND NOT ANDROID )
  # Standard 'must be' defines
  if (APPLE)
    target_compile_definitions(${BII_BLOCK_TARGET} INTERFACE  -DPOCO_HAVE_IPv6 -DPOCO_NO_STAT64)
    set(SYSLIBS  dl)
  else (APPLE)
    target_compile_definitions(${BII_BLOCK_TARGET} INTERFACE  -D_XOPEN_SOURCE=500 -D_REENTRANT -D_THREAD_SAFE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64)
    set(SYSLIBS  pthread dl rt)
  endif (APPLE)
endif(UNIX AND NOT ANDROID )

if (CMAKE_COMPILER_IS_MINGW)
  target_compile_definitions(${BII_BLOCK_TARGET} INTERFACE -DWC_NO_BEST_FIT_CHARS=0x400  -DPOCO_WIN32_UTF8)
  target_compile_definitions(${BII_BLOCK_TARGET} INTERFACE -D_WIN32 -DMINGW32 -DWINVER=0x500 -DODBCVER=0x0300 -DPOCO_THREAD_STACK_SIZE)
endif (CMAKE_COMPILER_IS_MINGW)

# Collect the built libraries and include dirs, the will be used to create the PocoConfig.cmake file
ENABLE_LIBRARIES()

include(Foundation)
if(ENABLE_XML)
  include(XML)
endif()
if(ENABLE_PDF)
  include(PDF)
endif()

#NetSSL

if(WIN32 AND ENABLE_NETSSL_WIN)
    include(NetSSL_Win)
endif(WIN32 AND ENABLE_NETSSL_WIN)

find_package(OpenSSL)
if(OPENSSL_FOUND)
    target_include_directories(${BII_BLOCK_TARGET} INTERFACE "${OPENSSL_INCLUDE_DIR}")
    if(ENABLE_NETSSL)
      target_link_libraries(${BII_BLOCK_TARGET} INTERFACE  ${OPENSSL_SSL_LIBRARY})
    endif()
    if(ENABLE_CRYPTO)
      include(Crypto)
    endif()
endif(OPENSSL_FOUND)

if(ENABLE_DATA)
  include(Data)
endif()

find_package(APR)
find_package(Apache2)
if(APRUTIL_FOUND AND APACHE_FOUND)
    target_include_directories(${BII_BLOCK_TARGET} INTERFACE  "${APACHE_INCLUDE_DIR}" "${APRUTIL_INCLUDE_DIR}" )
endif(APRUTIL_FOUND AND APACHE_FOUND)
