# Expat XML parser library
# Version: 2.7.4
# Source: https://github.com/libexpat/libexpat/releases/tag/R_2_7_4

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
	# Sources
	file(GLOB SRCS_G "src/*.c")
	POCO_SOURCES(SRCS expat ${SRCS_G})

	# Headers
	file(GLOB_RECURSE HDRS_G "src/*.h")
	POCO_HEADERS(SRCS expat ${HDRS_G})

	# NOTE: We use object library to be able to link it with static or shared libraries
	add_library(_BUNDLED_EXPAT OBJECT EXCLUDE_FROM_ALL ${SRCS})

	set_property(TARGET _BUNDLED_EXPAT PROPERTY POSITION_INDEPENDENT_CODE ON)

	# Platform-specific entropy source for Expat's hash salt generation
	if(WIN32)
		# Windows uses rand_s() by default
	elseif(APPLE OR CMAKE_SYSTEM_NAME MATCHES ".*BSD")
		target_compile_definitions(_BUNDLED_EXPAT PRIVATE HAVE_ARC4RANDOM_BUF)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
		target_compile_definitions(_BUNDLED_EXPAT PRIVATE HAVE_GETRANDOM)
	elseif(UNIX)
		target_compile_definitions(_BUNDLED_EXPAT PRIVATE XML_DEV_URANDOM)
	endif()

	target_compile_definitions(_BUNDLED_EXPAT
		PUBLIC XML_DTD XML_STATIC
		PRIVATE XML_NS HAVE_EXPAT_CONFIG_H
	)

	target_include_directories(_BUNDLED_EXPAT
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	)

	add_library(EXPAT::EXPAT ALIAS _BUNDLED_EXPAT)
endif()
