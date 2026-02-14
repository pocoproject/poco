# zlib compression library
# Version: 1.3.1.2
# Source: https://github.com/madler/zlib/releases/tag/v1.3.1.2
#
# Only the core inflate/deflate/crc32/adler32 sources are bundled.
# The gz* file-based API (gzclose.c, gzlib.c, gzread.c, gzwrite.c)
# and uncompress convenience wrapper (uncompr.c) are omitted because
# Poco only uses the low-level streaming API.

if(POCO_UNBUNDLED)
	find_package(ZLIB REQUIRED)
	set_target_properties(ZLIB::ZLIB PROPERTIES IMPORTED_GLOBAL TRUE)
else()

	# Sources
	file(GLOB SRCS_G "src/*.c")
	POCO_SOURCES(SRCS zlib ${SRCS_G})

	# Headers
	file(GLOB_RECURSE HDRS_G "src/*.h")
	POCO_HEADERS(SRCS zlib ${HDRS_G})

	# NOTE: We use object library to be able to link it with static or shared libraries
	add_library(_BUNDLED_ZLIB OBJECT EXCLUDE_FROM_ALL ${SRCS})

	set_property(TARGET _BUNDLED_ZLIB PROPERTY POSITION_INDEPENDENT_CODE ON)

	target_include_directories(_BUNDLED_ZLIB
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	)

	add_library(ZLIB::ZLIB ALIAS _BUNDLED_ZLIB)
endif()

