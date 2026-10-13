# Sources
list(APPEND SRCS_G "src/SerialPort.cpp")
if(WIN32)
	list(APPEND SRCS_G "src/SerialPort_WIN32.cpp")
else()
	list(APPEND SRCS_G "src/SerialPort_POSIX.cpp")
endif()
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
list(APPEND HDRS_G "include/Poco/Serial/Serial.h")
list(APPEND HDRS_G "include/Poco/Serial/SerialPort.h")
if(WIN32)
	list(APPEND HDRS_G "include/Poco/Serial/SerialPort_WIN32.h")
else()
	list(APPEND HDRS_G "include/Poco/Serial/SerialPort_POSIX.h")
endif()
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(Serial ${SRCS})
add_library(Poco::Serial ALIAS Serial)
set_target_properties(Serial
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoSerial
	DEFINE_SYMBOL Serial_EXPORTS
)

target_link_libraries(Serial PUBLIC Poco::Foundation)

target_include_directories(Serial
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(Serial)
POCO_GENERATE_PACKAGE(Serial)
