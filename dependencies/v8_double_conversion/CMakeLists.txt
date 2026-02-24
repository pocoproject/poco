# Google double-conversion library
# Version: 3.3.1
# Source: https://github.com/google/double-conversion/releases/tag/v3.3.1

# Sources
file(GLOB SRCS_G "src/*.c" "src/*.cc")
POCO_SOURCES(SRCS v8_double_conversion ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS(SRCS v8_double_conversion ${HDRS_G})

add_library(_BUNDLED_V8_DOUBLE_CONVERSION OBJECT EXCLUDE_FROM_ALL ${SRCS})

set_property(TARGET _BUNDLED_V8_DOUBLE_CONVERSION PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(_BUNDLED_V8_DOUBLE_CONVERSION
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

add_library(V8::DoubleConversion ALIAS _BUNDLED_V8_DOUBLE_CONVERSION)
