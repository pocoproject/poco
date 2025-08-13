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

add_library(PDF ${SRCS})

# TODO: Currently only bundled is supported, in future this should also be possible
# with an unbundled version of hpdf

target_link_libraries(PDF PRIVATE "$<BUILD_LOCAL_INTERFACE:HPDF::HPDF>")

add_library(Poco::PDF ALIAS PDF)
set_target_properties(PDF
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoPDF
	DEFINE_SYMBOL PDF_EXPORTS
)

target_link_libraries(PDF PUBLIC Poco::XML Poco::Util)
target_include_directories(PDF
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(PDF)
POCO_GENERATE_PACKAGE(PDF)

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()

