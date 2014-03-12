set( TEST_SRCS
src/DNSTest.cpp
src/DatagramSocketTest.cpp
src/DialogServer.cpp
src/DialogSocketTest.cpp
src/Driver.cpp
src/EchoServer.cpp
src/FTPClientSessionTest.cpp
src/FTPClientTestSuite.cpp
src/FTPStreamFactoryTest.cpp
src/HTMLFormTest.cpp
src/HTMLTestSuite.cpp
src/HTTPClientSessionTest.cpp
src/HTTPClientTestSuite.cpp
src/HTTPCookieTest.cpp
src/HTTPCredentialsTest.cpp
src/HTTPRequestTest.cpp
src/HTTPResponseTest.cpp
src/HTTPServerTest.cpp
src/HTTPServerTestSuite.cpp
src/HTTPStreamFactoryTest.cpp
src/HTTPTestServer.cpp
src/HTTPTestSuite.cpp
src/ICMPClientTest.cpp
src/ICMPClientTestSuite.cpp
src/ICMPSocketTest.cpp
src/IPAddressTest.cpp
src/MailMessageTest.cpp
src/MailStreamTest.cpp
src/MailTestSuite.cpp
src/MediaTypeTest.cpp
src/MessageHeaderTest.cpp
src/MessagesTestSuite.cpp
src/MulticastEchoServer.cpp
src/MulticastSocketTest.cpp
src/MultipartReaderTest.cpp
src/MultipartWriterTest.cpp
src/NameValueCollectionTest.cpp
src/NetCoreTestSuite.cpp
src/NetTestSuite.cpp
src/NetworkInterfaceTest.cpp
src/NTPClientTest.cpp
src/NTPClientTestSuite.cpp
src/POP3ClientSessionTest.cpp
src/QuotedPrintableTest.cpp
src/RawSocketTest.cpp
src/ReactorTestSuite.cpp
src/SMTPClientSessionTest.cpp
src/SocketAddressTest.cpp
src/SocketReactorTest.cpp
src/SocketStreamTest.cpp
src/SocketTest.cpp
src/SocketsTestSuite.cpp
src/SyslogTest.cpp
src/TCPServerTest.cpp
src/TCPServerTestSuite.cpp
src/UDPEchoServer.cpp
src/WebSocketTest.cpp
src/WebSocketTestSuite.cpp
)

#include_directories()

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT} PocoNet PocoUtil PocoXML PocoFoundation CppUnit)

