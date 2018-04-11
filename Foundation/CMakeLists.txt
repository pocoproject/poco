# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

# Platform Specific
POCO_SOURCES_AUTO_PLAT( SRCS UNIX src/SyslogChannel.cpp )
POCO_HEADERS_AUTO( SRCS include/Poco/SyslogChannel.h )

POCO_SOURCES_AUTO_PLAT( SRCS ANDROID src/AndroidLogChannel.cpp )
POCO_HEADERS_AUTO( SRCS include/Poco/AndroidLogChannel.h )

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


add_library(Foundation ${LIB_MODE} ${SRCS})
add_library(Poco::Foundation ALIAS Foundation)
set_target_properties(Foundation
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoFoundation
	DEFINE_SYMBOL Foundation_EXPORTS
	)

target_link_libraries(Foundation PUBLIC ${SYSLIBS} ${PCRE_LIBRARIES} ${ZLIB_LIBRARIES})
if(WIN32)
	target_link_libraries(Foundation PUBLIC iphlpapi)
endif()
if(ANDROID)
	target_link_libraries(Foundation PUBLIC log)
endif()
target_include_directories(Foundation
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
	)
target_compile_definitions(Foundation PUBLIC ${LIB_MODE_DEFINITIONS})
if(POCO_UNBUNDLED)
	target_include_directories(Foundation PUBLIC "${PCRE_INCLUDE_DIRS}" "${ZLIB_INCLUDE_DIRS}")
	target_compile_definitions(Foundation PUBLIC POCO_UNBUNDLED)
endif()

if (NOT POCO_ENABLE_WSTRING)
	target_compile_definitions(Foundation PUBLIC POCO_NO_WSTRING)
endif()

if (NOT POCO_ENABLE_FPENVIRONMENT)
	target_compile_definitions(Foundation PUBLIC POCO_NO_FPENVIRONMENT)
endif()

POCO_INSTALL(Foundation)
POCO_GENERATE_PACKAGE(Foundation)

if (POCO_ENABLE_TESTS)
	add_subdirectory( testsuite )
endif ()
if (POCO_ENABLE_SAMPLES)
	add_subdirectory( samples )
endif ()

