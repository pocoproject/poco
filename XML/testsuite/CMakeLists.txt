set( TEST_SRCS
src/AttributesImplTest.cpp
src/ChildNodesTest.cpp
src/DOMTestSuite.cpp
src/DocumentTest.cpp
src/DocumentTypeTest.cpp
src/Driver.cpp
src/ElementTest.cpp
src/EventTest.cpp
src/NamePoolTest.cpp
src/NameTest.cpp
src/NamespaceSupportTest.cpp
src/NodeAppenderTest.cpp
src/NodeIteratorTest.cpp
src/NodeTest.cpp
src/ParserWriterTest.cpp
src/SAXParserTest.cpp
src/SAXTestSuite.cpp
src/TextTest.cpp
src/TreeWalkerTest.cpp
src/XMLTestSuite.cpp
src/XMLWriterTest.cpp
)

#include_directories()

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT} PocoXML PocoFoundation CppUnit)

