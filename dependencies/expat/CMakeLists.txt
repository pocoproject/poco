

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
	file(GLOB SRCS_G "src/*.c" "src/*.cpp")
	POCO_SOURCES(SRCS expat ${SRCS_G})

	# Headers
	file(GLOB_RECURSE HDRS_G "src/*.h")
	POCO_HEADERS(SRCS expat ${HDRS_G})

	# NOTE: We use object library to be able to link it with static or shared libraries
	add_library(_BUNDLED_EXPAT OBJECT EXCLUDE_FROM_ALL ${SRCS})

	set_property(TARGET _BUNDLED_EXPAT PROPERTY POSITION_INDEPENDENT_CODE ON)

	target_compile_definitions(
		_BUNDLED_EXPAT
		PUBLIC XML_DTD XML_GE XML_STATIC
		PRIVATE XML_NS HAVE_EXPAT_CONFIG_H
	)

	target_include_directories(_BUNDLED_EXPAT
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		PRIVATE
			${CMAKE_SOURCE_DIR}/Foundation/include
	)

	add_library(EXPAT::EXPAT ALIAS _BUNDLED_EXPAT)
endif()

