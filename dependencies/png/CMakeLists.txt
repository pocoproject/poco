

if(POCO_UNBUNDLED)	
	if (ENABLE_PDF)
		find_package(PNG REQUIRED)
	else()
		find_package(PNG)
	endif()
	if (PNG_FOUND)
		set_target_properties(PNG::PNG PROPERTIES IMPORTED_GLOBAL TRUE)
	endif()
else()

	# Sources
	file(GLOB SRCS_G "src/*.c")
	POCO_SOURCES(SRCS png ${SRCS_G})

	# Headers
	file(GLOB_RECURSE HDRS_G "src/*.h")
	POCO_HEADERS(SRCS png ${HDRS_G})

	# NOTE: We use object library to be able to link it with static or shared libraries
	add_library(_BUNDLED_PNG OBJECT EXCLUDE_FROM_ALL ${SRCS})

	set_property(TARGET _BUNDLED_PNG PROPERTY POSITION_INDEPENDENT_CODE ON)

	# libpng: Do not try to use ARM intrinsics on ARM architecture: source files not included
	target_compile_definitions(_BUNDLED_PNG PRIVATE PNG_ARM_NEON_OPT=0)

	target_include_directories(_BUNDLED_PNG
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	)

	add_library(PNG::PNG ALIAS _BUNDLED_PNG)
endif()

