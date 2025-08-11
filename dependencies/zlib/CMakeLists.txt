

# Sources
file(GLOB SRCS_G "src/*.c")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

add_library(_BUNDLED_ZLIB STATIC ${SRCS})

target_include_directories(_BUNDLED_ZLIB
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if(POCO_UNBUNDLED)
	find_package(ZLIB REQUIRED)
	set_target_properties(ZLIB::ZLIB PROPERTIES IMPORTED_GLOBAL TRUE)
else()
	add_library(ZLIB::ZLIB ALIAS _BUNDLED_ZLIB)
endif()

