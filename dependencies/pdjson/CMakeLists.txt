

# Sources
file(GLOB SRCS_G "src/*.c")
POCO_SOURCES(SRCS pdjson ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS(SRCS pdjson ${HDRS_G})

# NOTE: We use object library to be able to link it with static or shared libraries
add_library(_BUNDLED_PDJSON OBJECT ${SRCS})

set_property(TARGET _BUNDLED_PDJSON PROPERTY POSITION_INDEPENDENT_CODE ON)

if(WIN32)
	target_compile_definitions(_BUNDLED_PDJSON PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_include_directories(_BUNDLED_PDJSON
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

add_library(PDJson::PDJson ALIAS _BUNDLED_PDJSON)
