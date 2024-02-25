# Sources
file(GLOB SRCS_G ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
POCO_SOURCES_AUTO(DATA_TEST_LIB_SRCS ${SRCS_G})

# Headers
file(GLOB HDRS_G ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
POCO_HEADERS_AUTO(DATA_TEST_LIB_SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND DATA_TEST_LIB_SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(DataTest ${DATA_TEST_LIB_SRCS})
add_library(Poco::DataTest ALIAS DataTest)
set_target_properties(DataTest
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoDataTest
	DEFINE_SYMBOL DataTest_EXPORTS
)

target_link_libraries(DataTest PUBLIC Poco::Data CppUnit)
target_include_directories(DataTest
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Data/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
