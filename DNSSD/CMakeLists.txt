# ITNOA

set(DNSSD_IMPLEMENTATION_LIBRARY "")

macro(ADD_AVAHI)
	find_package(Avahi REQUIRED)
	if(AVAHI_FOUND)
		include_directories("${AVAHI_INCLUDE_DIR}")
		message(STATUS "Avahi Support Enabled")
		add_subdirectory( Avahi )
		set(DNSSD_IMPLEMENTATION_LIBRARY "DNSSDAvahi")
	else()
		message(ERROR "Avahi does not found, please make install it")
	endif()
endmacro()

macro(ADD_BONJOUR)
	find_package(Bonjour REQUIRED)
	if(BONJOUR_FOUND)
		if(NOT APPLE)
			include_directories("${BONJOUR_INCLUDE_DIR}")
		endif()
		message(STATUS "Bonjour Support Enabled")
		add_subdirectory( Bonjour )
		set(DNSSD_IMPLEMENTATION_LIBRARY "DNSSDBonjour")
	else()
		message(ERROR "Bonjour does not found, please make install sdk")
	endif()
endmacro()

set(LIBNAME "DNSSD")
set(POCO_LIBNAME "Poco${LIBNAME}")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

if (NOT POCO_STATIC)
    add_definitions(-DTHREADSAFE)
endif (NOT POCO_STATIC)

add_library( "${LIBNAME}" ${LIB_MODE} ${SRCS} )
add_library( "${POCO_LIBNAME}" ALIAS "${LIBNAME}")
set_target_properties( "${LIBNAME}"
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    OUTPUT_NAME ${POCO_LIBNAME}
    DEFINE_SYMBOL DNSSD_EXPORTS
    )

target_link_libraries( "${LIBNAME}" Foundation Net)
target_include_directories( "${LIBNAME}"
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions("${LIBNAME}" PUBLIC ${LIB_MODE_DEFINITIONS})

POCO_INSTALL("${LIBNAME}")
POCO_GENERATE_PACKAGE("${LIBNAME}")

if(ENABLE_DNSSD_DEFAULT)
	if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")	# `UNIX AND NOT APPLE` it is not equal to Linux ;)
		set(LINUX TRUE)
	endif()
	if(LINUX)
		ADD_AVAHI()
	else()
		ADD_BONJOUR()
	endif()
endif()

if(ENABLE_DNSSD_AVHAI)
	ADD_AVAHI()
endif()

if(ENABLE_DNSSD_BONJOUR)
	ADD_BONJOUR()
endif()

if (ENABLE_SAMPLES)
    add_subdirectory(samples)
endif ()
