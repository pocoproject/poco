# Expat XML parser library
# Version: 2.8.1
# Source: https://github.com/libexpat/libexpat/releases/tag/R_2_8_1

if(POCO_UNBUNDLED)
	if (ENABLE_XML)
		find_package(EXPAT REQUIRED)
		set_target_properties(EXPAT::EXPAT PROPERTIES IMPORTED_GLOBAL TRUE)
	endif()
else()
	# Core sources -- exclude the per-entropy-source random_*.c files;
	# those are added conditionally below to mirror the upstream
	# CMakeLists gating (see expat-2.8.1/CMakeLists.txt).
	file(GLOB SRCS_G "src/*.c")
	list(FILTER SRCS_G EXCLUDE REGEX "/random_.*\\.c$")
	POCO_SOURCES(SRCS expat ${SRCS_G})

	# Headers
	file(GLOB_RECURSE HDRS_G "src/*.h")
	POCO_HEADERS(SRCS expat ${HDRS_G})

	# Platform-specific entropy source for Expat's hash salt generation.
	# Each random_*.c file is only meaningful when its corresponding
	# HAVE_* macro is defined; compiling otherwise yields undefined
	# symbols (e.g. SYS_getrandom on macOS).
	set(_BUNDLED_EXPAT_RANDOM_SRCS "")
	if(WIN32)
		# Windows uses rand_s() by default
		list(APPEND _BUNDLED_EXPAT_RANDOM_SRCS src/random_rand_s.c)
	elseif(APPLE OR CMAKE_SYSTEM_NAME MATCHES ".*BSD")
		list(APPEND _BUNDLED_EXPAT_RANDOM_SRCS src/random_arc4random_buf.c)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
		list(APPEND _BUNDLED_EXPAT_RANDOM_SRCS src/random_getrandom.c)
	elseif(UNIX)
		list(APPEND _BUNDLED_EXPAT_RANDOM_SRCS src/random_dev_urandom.c)
	endif()

	# NOTE: We use object library to be able to link it with static or shared libraries
	add_library(_BUNDLED_EXPAT OBJECT EXCLUDE_FROM_ALL ${SRCS} ${_BUNDLED_EXPAT_RANDOM_SRCS})

	set_property(TARGET _BUNDLED_EXPAT PROPERTY POSITION_INDEPENDENT_CODE ON)

	# Match each entropy source file with the macro its body checks for.
	if(WIN32)
		# random_rand_s.c body is unconditional
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
