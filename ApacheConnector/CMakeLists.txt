# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

add_library(mod_poco SHARED ${SRCS})
set_target_properties(mod_poco
	PROPERTIES
	PREFIX ""
	SUFFIX ".so"
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
set(DEPLIBS Poco::Util Poco::Net Apache::Apr Apache::Aprutil)
if(WIN32)
	list(APPEND DEPLIBS Apache::httpd)
endif(WIN32)
target_link_libraries(mod_poco PUBLIC ${DEPLIBS})

file(MAKE_DIRECTORY "include/Poco")
POCO_INSTALL(mod_poco)

if(ENABLE_TESTS)
	add_subdirectory(samples)
endif()
