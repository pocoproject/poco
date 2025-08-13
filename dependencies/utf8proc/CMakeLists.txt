
if(POCO_UNBUNDLED)
	find_package(Utf8Proc REQUIRED)
	set_target_properties(Utf8Proc::Utf8Proc PROPERTIES IMPORTED_GLOBAL TRUE)
else()

	# Sources
	file(GLOB SRCS_G "src/*.c")
	list(FILTER SRCS_G EXCLUDE REGEX ".*utf8proc_data.c")

	POCO_SOURCES(SRCS utf8proc ${SRCS_G})

	# Headers
	file(GLOB_RECURSE HDRS_G "src/*.h")
	POCO_HEADERS(SRCS utf8proc ${HDRS_G})

	# NOTE: We use object library to be able to link it with static or shared libraries
	add_library(_BUNDLED_UTF8PROC OBJECT ${SRCS})

	set_property(TARGET _BUNDLED_UTF8PROC PROPERTY POSITION_INDEPENDENT_CODE ON)

	target_compile_definitions(_BUNDLED_UTF8PROC PUBLIC UTF8PROC_STATIC)

	target_include_directories(_BUNDLED_UTF8PROC
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	)

	add_library(Utf8Proc::Utf8Proc ALIAS _BUNDLED_UTF8PROC)
endif()

