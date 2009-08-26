set(LIBNAME "PocoODBC")
set(LIBNAMED "${LIBNAME}d")

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
	PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS}
	VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}
	SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
target_link_libraries( ${LIBNAME} PocoData PocoFoundation ${ODBC_LINK_FLAGS})

add_library( ${LIBNAMED} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAMED}  
	PROPERTIES COMPILE_FLAGS "${DEBUG_CXX_FLAGS}" 
	VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}
	SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
target_link_libraries( ${LIBNAMED} PocoDatad PocoFoundationd ${ODBC_LINK_FLAGS})

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )
    
install(
    TARGETS ${LIBNAME} ${LIBNAMED}
    DESTINATION lib
    )

add_subdirectory( testsuite )
