# Expat XML parser library
# Version: 2.7.3
# Source: https://github.com/libexpat/libexpat/releases/tag/R_2_7_3

if(POCO_UNBUNDLED)
	if (ENABLE_XML)
		find_package(EXPAT REQUIRED)
	else()
		find_package(EXPAT)
	endif()
	if (EXPAT_FOUND)
		set_target_properties(EXPAT::EXPAT PROPERTIES IMPORTED_GLOBAL TRUE)
	endif()
else()
	# Generate expat_config.h from template
	include(TestBigEndian)
	test_big_endian(_EXPAT_BIG_ENDIAN)
	if(_EXPAT_BIG_ENDIAN)
		set(BYTEORDER 4321)
	else()
		set(BYTEORDER 1234)
	endif()

	set(XML_CONTEXT_BYTES 1024)
	set(XML_GE 1)

	# Platform-specific entropy source detection
	if(WIN32)
		# Windows uses rand_s() by default, no additional defines needed
	elseif(APPLE OR CMAKE_SYSTEM_NAME MATCHES ".*BSD")
		set(HAVE_ARC4RANDOM_BUF 1)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
		set(HAVE_GETRANDOM 1)
	elseif(UNIX)
		set(XML_DEV_URANDOM 1)
	endif()

	configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/expat_config.h.cmake
		${CMAKE_CURRENT_BINARY_DIR}/expat_config.h
	)

	# Sources
	file(GLOB SRCS_G "src/*.c")
	POCO_SOURCES(SRCS expat ${SRCS_G})

	# Headers
	file(GLOB_RECURSE HDRS_G "src/*.h")
	POCO_HEADERS(SRCS expat ${HDRS_G})

	# NOTE: We use object library to be able to link it with static or shared libraries
	add_library(_BUNDLED_EXPAT OBJECT EXCLUDE_FROM_ALL ${SRCS})

	set_property(TARGET _BUNDLED_EXPAT PROPERTY POSITION_INDEPENDENT_CODE ON)

	target_compile_definitions(_BUNDLED_EXPAT
		PUBLIC XML_DTD XML_STATIC
		PRIVATE XML_NS HAVE_EXPAT_CONFIG_H
	)

	target_include_directories(_BUNDLED_EXPAT
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		PRIVATE
			${CMAKE_CURRENT_BINARY_DIR}
	)

	add_library(EXPAT::EXPAT ALIAS _BUNDLED_EXPAT)
endif()
