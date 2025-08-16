

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

