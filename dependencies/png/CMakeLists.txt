

if(POCO_UNBUNDLED)
	find_package(PNG REQUIRED)
	set_target_properties(PNG::PNG PROPERTIES IMPORTED_GLOBAL TRUE)
else()

	# Sources
	file(GLOB SRCS_G "src/*.c")
	POCO_SOURCES(SRCS png ${SRCS_G})

	# Headers
	file(GLOB_RECURSE HDRS_G "src/*.h")
	POCO_HEADERS(SRCS png ${HDRS_G})

	# NOTE: We use object library to be able to link it with static or shared libraries
	add_library(_BUNDLED_PNG OBJECT ${SRCS})

	set_property(TARGET _BUNDLED_PNG PROPERTY POSITION_INDEPENDENT_CODE ON)

	# libpng: Do not try to use ARM intrinsics on ARM architecture: source files not included
	target_compile_definitions(_BUNDLED_PNG PRIVATE PNG_ARM_NEON_OPT=0)

	target_include_directories(_BUNDLED_PNG
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	)

	add_library(PNG::PNG ALIAS _BUNDLED_PNG)
endif()

