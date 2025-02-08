# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

# Platform Specific
POCO_SOURCES_AUTO_PLAT(SRCS WIN32 src/wepoll.c)
POCO_HEADERS_AUTO(SRCS src/wepoll.h)

if (MSVC OR MINGW)
	set(HAVE_SENDFILE ON)
else()
	include(CheckIncludeFiles)
	include(CheckSymbolExists)
	check_include_files(sys/sendfile.h HAVE_SYS_SENDFILE_H)
	if(HAVE_SYS_SENDFILE_H)
		check_symbol_exists(sendfile sys/sendfile.h HAVE_SENDFILE)
		if (NOT HAVE_SENDFILE)
			check_symbol_exists(sendfile64 sys/sendfile.h HAVE_SENDFILE64)
		endif()
	else()
		# BSD version
		check_symbol_exists(sendfile "sys/types.h;sys/socket.h;sys/uio.h" HAVE_SENDFILE)
	endif()
endif()

if (HAVE_SENDFILE OR HAVE_SENDFILE64)
	message(STATUS "OS has native sendfile function")
	add_compile_definitions(POCO_HAVE_SENDFILE)
endif()

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(Net ${SRCS})
add_library(Poco::Net ALIAS Net)
set_target_properties(Net
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoNet
	DEFINE_SYMBOL Net_EXPORTS
)

target_link_libraries(Net PUBLIC Poco::Foundation)
# Windows need additional libraries
if(WIN32)
	target_link_libraries(Net PUBLIC "iphlpapi.lib")
	target_link_libraries(Net PUBLIC "ws2_32.lib")
	if (MINGW)
		target_link_libraries(Net PUBLIC "mswsock.lib")
	endif()
endif(WIN32)

target_include_directories(Net
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(Net)
POCO_GENERATE_PACKAGE(Net)

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()

if(ENABLE_FUZZING)
	add_subdirectory(fuzzing)
endif()
