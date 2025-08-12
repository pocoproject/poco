

# Sources
file(GLOB SRCS_G "src/*.c")
POCO_SOURCES(SRCS wepoll ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS(SRCS wepoll ${HDRS_G})

add_library(_BUNDLED_WEPOLL STATIC ${SRCS})

target_include_directories(_BUNDLED_WEPOLL
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

add_library(WEPoll::WEPoll ALIAS _BUNDLED_WEPOLL)
