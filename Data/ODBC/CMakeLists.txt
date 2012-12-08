set(LIBNAME "PocoDataODBC")

set(SRCS 
src/Binder.cpp
src/ConnectionHandle.cpp
src/Connector.cpp
src/EnvironmentHandle.cpp
src/Extractor.cpp
src/ODBCException.cpp
src/ODBCMetaColumn.cpp
src/ODBCStatementImpl.cpp
src/Parameter.cpp
src/Preparator.cpp
src/SessionImpl.cpp
src/TypeInfo.cpp
src/Unicode.cpp
src/Utility.cpp
)

add_definitions( ${ODBC_CFLAGS} )

include_directories( include )

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} PocoData PocoFoundation ${ODBC_LINK_FLAGS})

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )
    
install(
    TARGETS ${LIBNAME}
    DESTINATION lib
    )

if (ENABLE_TESTS)
  add_subdirectory(testsuite)
endif ()
