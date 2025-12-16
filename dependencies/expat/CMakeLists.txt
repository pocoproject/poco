# Expat XML parser library
# Version: 2.7.3
# Source: https://github.com/libexpat/libexpat/releases/tag/R_2_7_3
#
# Note: src/xmlparse.cpp is generated from expat's xmlparse.c with POCO-specific modifications.
# Use the patch_expat_xmlparse target to regenerate it after upgrading expat.

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
	# Exclude original xmlparse.c (we use the patched xmlparse.cpp)
	list(FILTER SRCS_G EXCLUDE REGEX ".*xmlparse\\.c$")
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

# Custom target to regenerate xmlparse.cpp from original xmlparse.c
# This target is NOT included in ALL - run manually with: cmake --build . --target patch_expat_xmlparse
add_custom_target(patch_expat_xmlparse
	COMMAND ${CMAKE_COMMAND} -E echo "Patching xmlparse.c for POCO..."
	COMMAND ${CMAKE_COMMAND} -E copy xmlparse.c xmlparse.c.bak
	COMMAND patch xmlparse.c < ${CMAKE_CURRENT_SOURCE_DIR}/xmlparse_poco.patch
	COMMAND ${CMAKE_COMMAND} -E rename xmlparse.c xmlparse.cpp
	COMMAND ${CMAKE_COMMAND} -E rename xmlparse.c.bak xmlparse.c
	COMMAND ${CMAKE_COMMAND} -E echo "Done. xmlparse.cpp has been updated."
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src
	COMMENT "Applying POCO patches to expat xmlparse.c"
)
