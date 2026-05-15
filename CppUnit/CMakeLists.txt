# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

add_library(CppUnit ${SRCS})
add_library(Poco::CppUnit ALIAS CppUnit)
set_target_properties(CppUnit
	PROPERTIES
	VERSION "1" SOVERSION "1"
	OUTPUT_NAME CppUnit
	DEFINE_SYMBOL CppUnit_EXPORTS
)
target_include_directories(CppUnit
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		# Pull Foundation's include dirs in *without* linking Foundation:
		# TestRunner.h needs Poco/Exception.h visible (consumed by the
		# CppUnitMain / CppUnitPocoExceptionText macros), and CppUnit's own
		# sources include TestRunner.h transitively. The macro expansions
		# live at consumer call sites, so libCppUnit.so itself never
		# references Poco::Exception symbols and doesn't need to link
		# Foundation; each consumer brings its own Poco link line.
		$<TARGET_PROPERTY:Poco::Foundation,INTERFACE_INCLUDE_DIRECTORIES>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)
if(WIN32)
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_definitions(CppUnit PUBLIC POCO_NO_AUTOMATIC_LIBS)
	endif()
endif()

if(NOT BUILD_SHARED_LIBS)
	target_compile_definitions(CppUnit PUBLIC POCO_STATIC)
elseif (MINGW)
	target_compile_definitions(CppUnit PUBLIC _DLL)
endif()

if(ENABLE_INSTALL_CPPUNIT)
	install(
		DIRECTORY include/CppUnit
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		COMPONENT Devel
	)

	install(
		TARGETS CppUnit
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
		INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)
endif()
