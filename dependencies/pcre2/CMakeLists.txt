

if(POCO_UNBUNDLED)
	find_package(PCRE2 REQUIRED)
	set_target_properties(Pcre2::Pcre2 PROPERTIES IMPORTED_GLOBAL TRUE)

	# HACK: Foundation Unicode.cpp requires functions from these files.
	# They can't be used directly from the library
	set(
		EMBEDDED_PCRE2_SOURCES
			${CMAKE_CURRENT_SOURCE_DIR}/src/pcre2_ucd.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/pcre2_tables.c
		CACHE INTERNAL "Embedded PCRE2 sources"
	)

	set(
		EMBEDDED_PCRE2_INCLUDE
			${CMAKE_CURRENT_SOURCE_DIR}/src
		CACHE INTERNAL "Embedded PCRE2 include directory"
	)

else()

	# Sources
	file(GLOB SRCS_G "src/*.c")

	# Exclude JIT files (not included in object files)
	list(FILTER SRCS_G EXCLUDE REGEX ".*pcre2_jit_compile.c")
	list(FILTER SRCS_G EXCLUDE REGEX ".*pcre2_jit_match.c")
	list(FILTER SRCS_G EXCLUDE REGEX ".*pcre2_jit_misc.c")
	list(FILTER SRCS_G EXCLUDE REGEX ".*pcre2_jit_test.c")
	# Exclude test/tool files
	list(FILTER SRCS_G EXCLUDE REGEX ".*pcre2test.c")
	list(FILTER SRCS_G EXCLUDE REGEX ".*pcre2posix_test.c")
	list(FILTER SRCS_G EXCLUDE REGEX ".*pcre2grep.c")
	list(FILTER SRCS_G EXCLUDE REGEX ".*pcre2demo.c")
	list(FILTER SRCS_G EXCLUDE REGEX ".*pcre2_dftables.c")
	list(FILTER SRCS_G EXCLUDE REGEX ".*pcre2_fuzzsupport.c")

	POCO_SOURCES(SRCS pcre2 ${SRCS_G})

	# Headers
	file(GLOB_RECURSE HDRS_G "src/*.h")
	POCO_HEADERS(SRCS pcre2 ${HDRS_G})

	# NOTE: We use object library to be able to link it with static or shared libraries
	add_library(_BUNDLED_PCRE2 OBJECT EXCLUDE_FROM_ALL ${SRCS})

	set_property(TARGET _BUNDLED_PCRE2 PROPERTY POSITION_INDEPENDENT_CODE ON)

	target_compile_definitions(_BUNDLED_PCRE2 PUBLIC PCRE2_STATIC HAVE_CONFIG_H)

	target_include_directories(_BUNDLED_PCRE2
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	)

	add_library(Pcre2::Pcre2 ALIAS _BUNDLED_PCRE2)
endif()

