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

add_library(Prometheus ${SRCS})
add_library(Poco::Prometheus ALIAS Prometheus)
set_target_properties(Prometheus
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoPrometheus
	DEFINE_SYMBOL Prometheus_EXPORTS
)

target_link_libraries(Prometheus PUBLIC Poco::Net)
target_include_directories(Prometheus
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(Prometheus)
POCO_GENERATE_PACKAGE(Prometheus)

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()

