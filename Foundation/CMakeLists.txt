# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

# Platform Specific
POCO_SOURCES_AUTO_PLAT( SRCS UNIX src/SyslogChannel.cpp )
POCO_HEADERS_AUTO( SRCS include/Poco/SyslogChannel.h )

# For Windows CE we need to disable these
if(WINCE)
POCO_SOURCES_AUTO_PLAT( SRCS OFF
	src/WindowsConsoleChannel.cpp
	src/EventLogChannel.cpp
	)
else()
POCO_SOURCES_AUTO_PLAT( SRCS WIN32
	src/WindowsConsoleChannel.cpp
	src/EventLogChannel.cpp
	)
endif()

# Messages
POCO_MESSAGES( SRCS Logging src/pocomsg.mc)

# If POCO_UNBUNDLED is enabled we try to find the required packages
# The configuration will fail if the packages are not found
if (POCO_UNBUNDLED)
	find_package(PCRE REQUIRED)
	find_package(ZLIB REQUIRED)

	#HACK: Unicode.cpp requires functions from these files. The can't be taken from the library
	POCO_SOURCES( SRCS RegExp
		src/pcre_ucd.c
		src/pcre_tables.c
	)

else()
	# pcre
	POCO_SOURCES( SRCS pcre
		src/pcre_config.c
		src/pcre_byte_order.c
		src/pcre_chartables.c
		src/pcre_compile.c
		src/pcre_exec.c
		src/pcre_fullinfo.c
		src/pcre_globals.c
		src/pcre_maketables.c
		src/pcre_newline.c
		src/pcre_ord2utf8.c
		src/pcre_study.c
		src/pcre_tables.c
		src/pcre_dfa_exec.c
		src/pcre_get.c
		src/pcre_jit_compile.c
		src/pcre_refcount.c
		src/pcre_string_utils.c
		src/pcre_version.c
		src/pcre_ucd.c
		src/pcre_valid_utf8.c
		src/pcre_xclass.c
	)

	# zlib
	POCO_HEADERS( SRCS zlib
		include/Poco/zconf.h
		include/Poco/zlib.h
		)

	POCO_SOURCES( SRCS zlib
		src/adler32.c
		src/compress.c
		src/crc32.c
		src/deflate.c
		src/infback.c
		src/inffast.c
		src/inflate.c
		src/inftrees.c
		src/trees.c
		src/zutil.c
	)
endif (POCO_UNBUNDLED)


add_library(Foundation ${SRCS})
add_library(Poco::Foundation ALIAS Foundation)
set_target_properties(Foundation
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoFoundation
	DEFINE_SYMBOL Foundation_EXPORTS
	)

target_link_libraries(Foundation PUBLIC ${PCRE_LIBRARIES} ${ZLIB_LIBRARIES})
target_include_directories(Foundation
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
	)

target_compile_definitions(Foundation
  PUBLIC
    $<$<CONFIG:Debug>:_DEBUG>
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(Foundation
        PUBLIC
            POCO_STATIC
    )
endif()
  
if(WIN32)
	target_compile_definitions(Foundation PUBLIC POCO_NO_AUTOMATIC_LIBS POCO_OS_FAMILY_WINDOWS UNICODE _UNICODE __LCC__)  #__LCC__ define used by MySQL.h
	target_link_libraries(Foundation PUBLIC iphlpapi)
endif()

if (CYGWIN)
  target_compile_definitions(Foundation PUBLIC POCO_NO_FPENVIRONMENT POCO_NO_WSTRING _XOPEN_SOURCE=500 __BSD_VISIBLE)
else (CYGWIN)
	if (UNIX AND NOT ANDROID )
	  target_compile_definitions(Foundation PUBLIC POCO_OS_FAMILY_UNIX)
	  if (APPLE)
		target_compile_definitions(Foundation PUBLIC POCO_HAVE_IPv6 POCO_NO_STAT64)
		target_link_libraries(Foundation PUBLIC ${CMAKE_DL_LIBS})
	  else (APPLE)
		target_compile_definitions(Foundation PUBLIC _REENTRANT _THREAD_SAFE _LARGEFILE64_SOURCE _FILE_OFFSET_BITS=64)
		if (QNX)
		  target_compile_definitions(Foundation PUBLIC POCO_HAVE_FD_POLL)
		  target_link_libraries(Foundation PUBLIC m socket)
		else (QNX)
		  target_compile_definitions(Foundation PUBLIC _XOPEN_SOURCE=500 POCO_HAVE_FD_EPOLL)
		  target_link_libraries(Foundation PUBLIC pthread ${CMAKE_DL_LIBS} rt)
		endif (QNX)
	  endif (APPLE)
	endif(UNIX AND NOT ANDROID )
endif (CYGWIN)

if (CMAKE_SYSTEM MATCHES "SunOS")
  target_compile_definitions(Foundation
	  PUBLIC
		POCO_OS_FAMILY_UNIX
		_XOPEN_SOURCE=500
		_REENTRANT
		_THREAD_SAFE
		_LARGEFILE64_SOURCE
		_FILE_OFFSET_BITS=64
	)
  target_link_libraries(Foundation PUBLIC pthread socket xnet nsl resolv rt ${CMAKE_DL_LIBS})
endif()

if(CMAKE_COMPILER_IS_MINGW)
  target_compile_definitions(Foundation
	  PUBLIC
		WC_NO_BEST_FIT_CHARS=0x400
		POCO_WIN32_UTF8
		_WIN32
		MINGW32
		WINVER=0x500
		ODBCVER=0x0300
		POCO_THREAD_STACK_SIZE
	)
endif()

# SunPro C++
if(${CMAKE_CXX_COMPILER_ID} MATCHES "SunPro")
	target_compile_definitions(Foundation PUBLIC _BSD_SOURCE)
	target_compile_options(Foundation PUBLIC -library=stlport4)
endif()

# iOS
if (IOS)
  target_compile_definitions(Foundation
	  PUBLIC
		POCO_HAVE_IPv6
		POCO_NO_FPENVIRONMENT
		POCO_NO_STAT64
		POCO_NO_SHAREDLIBS
		POCO_NO_NET_IFTYPES
	)
endif()

#Android
if (ANDROID)
	target_compile_definitions(Foundation
		PUBLIC
			POCO_NO_FPENVIRONMENT
			POCO_NO_WSTRING
			POCO_NO_SHAREDMEMORY
	)
	target_link_libraries(Foundation PUBLIC log)
endif()

if(POCO_UNBUNDLED)
	target_include_directories(Foundation PUBLIC "${PCRE_INCLUDE_DIRS}" "${ZLIB_INCLUDE_DIRS}")
	target_compile_definitions(Foundation PUBLIC POCO_UNBUNDLED)
endif()

POCO_INSTALL(Foundation)
POCO_GENERATE_PACKAGE(Foundation)

if (ENABLE_TESTS)
	add_subdirectory(samples)
	add_subdirectory(testsuite)
endif ()

