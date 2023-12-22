# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

add_library(mod_poco SHARED ${SRCS})
set_target_properties(mod_poco
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	DEFINE_SYMBOL ApacheHandlers_EXPORTS)
target_include_directories(mod_poco
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		${APACHE2_INCLUDE_DIRS}
		${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(mod_poco PUBLIC Poco::Util Poco::Net Apache::Apr Apache::Aprutil)

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()
