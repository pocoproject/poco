# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

add_library(CppUnit ${SRCS})
add_library(Poco::CppUnit ALIAS CppUnit)
set_target_properties(CppUnit
	PROPERTIES
	VERSION "1" SOVERSION "1"
	OUTPUT_NAME CppUnit
	DEFINE_SYMBOL CppUnit_EXPORTS
)
target_link_libraries(CppUnit PUBLIC Poco::Foundation)
target_include_directories(CppUnit
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(NOT BUILD_SHARED_LIBS)
	target_compile_definitions(CppUnit
		PUBLIC
			POCO_STATIC
	)
elseif(MINGW)
	target_compile_definitions(CppUnit
		PUBLIC
			_DLL)
endif()
