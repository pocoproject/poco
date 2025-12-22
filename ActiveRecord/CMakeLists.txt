# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(ActiveRecord ${SRCS})
add_library(Poco::ActiveRecord ALIAS ActiveRecord)
set_target_properties(ActiveRecord
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoActiveRecord
	DEFINE_SYMBOL ActiveRecordLib_EXPORTS
)

target_link_libraries(ActiveRecord PUBLIC Poco::Data Poco::Foundation)
target_include_directories(ActiveRecord
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

POCO_INSTALL(ActiveRecord)
POCO_GENERATE_PACKAGE(ActiveRecord)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
