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

add_library(Crypto ${SRCS})
add_library(Poco::Crypto ALIAS Crypto)
set_target_properties(Crypto
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoCrypto
	DEFINE_SYMBOL Crypto_EXPORTS
)

target_link_libraries(Crypto PUBLIC Poco::Foundation OpenSSL::SSL OpenSSL::Crypto)
target_include_directories(Crypto
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(MSVC)
	if(OPENSSL_USE_STATIC_LIBS)
		target_link_libraries(Crypto PUBLIC ws2_32.lib Crypt32.lib)
	endif()
endif()

POCO_INSTALL(Crypto)
POCO_GENERATE_PACKAGE(Crypto)

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
