set(LIBNAME "CppUnit")

set(SRCS
    src/CppUnitException.cpp
    src/TestCase.cpp
    src/TestDecorator.cpp
    src/TestFailure.cpp
    src/TestResult.cpp
    src/TestRunner.cpp
    src/TestSuite.cpp
    src/TextTestResult.cpp
)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME}
	PROPERTIES
	VERSION "1"
	SOVERSION "1")
target_link_libraries( ${LIBNAME} )

