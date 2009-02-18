set(LIBNAME "PocoNet")
set(LIBNAMED "${LIBNAME}d")

set( BASE_SRCS
src/AbstractHTTPRequestHandler.cpp
src/DNS.cpp
src/DatagramSocket.cpp
src/DatagramSocketImpl.cpp
src/DialogSocket.cpp
src/FTPClientSession.cpp
src/FTPStreamFactory.cpp
src/FilePartSource.cpp
src/HTMLForm.cpp
src/HTTPBasicCredentials.cpp
src/HTTPBufferAllocator.cpp
src/HTTPChunkedStream.cpp
src/HTTPClientSession.cpp
src/HTTPCookie.cpp
src/HTTPFixedLengthStream.cpp
src/HTTPHeaderStream.cpp
src/HTTPIOStream.cpp
src/HTTPMessage.cpp
src/HTTPRequest.cpp
src/HTTPRequestHandler.cpp
src/HTTPRequestHandlerFactory.cpp
src/HTTPResponse.cpp
src/HTTPServer.cpp
src/HTTPServerConnection.cpp
src/HTTPServerConnectionFactory.cpp
src/HTTPServerParams.cpp
src/HTTPServerRequest.cpp
src/HTTPServerRequestImpl.cpp
src/HTTPServerResponse.cpp
src/HTTPServerResponseImpl.cpp
src/HTTPServerSession.cpp
src/HTTPSession.cpp
src/HTTPSessionFactory.cpp
src/HTTPSessionInstantiator.cpp
src/HTTPStream.cpp
src/HTTPStreamFactory.cpp
src/HostEntry.cpp
src/ICMPClient.cpp
src/ICMPEventArgs.cpp
src/ICMPPacket.cpp
src/ICMPPacketImpl.cpp
src/ICMPSocket.cpp
src/ICMPSocketImpl.cpp
src/ICMPv4PacketImpl.cpp
src/IPAddress.cpp
src/MailMessage.cpp
src/MailRecipient.cpp
src/MailStream.cpp
src/MediaType.cpp
src/MessageHeader.cpp
src/MulticastSocket.cpp
src/MultipartReader.cpp
src/MultipartWriter.cpp
src/NameValueCollection.cpp
src/NetException.cpp
src/NetworkInterface.cpp
src/NullPartHandler.cpp
src/POP3ClientSession.cpp
src/PartHandler.cpp
src/PartSource.cpp
src/QuotedPrintableDecoder.cpp
src/QuotedPrintableEncoder.cpp
src/RawSocket.cpp
src/RawSocketImpl.cpp
src/RemoteSyslogChannel.cpp
src/RemoteSyslogListener.cpp
src/SMTPClientSession.cpp
src/ServerSocket.cpp
src/ServerSocketImpl.cpp
src/Socket.cpp
src/SocketAddress.cpp
src/SocketImpl.cpp
src/SocketNotification.cpp
src/SocketNotifier.cpp
src/SocketReactor.cpp
src/SocketStream.cpp
src/StreamSocket.cpp
src/StreamSocketImpl.cpp
src/StringPartSource.cpp
src/TCPServer.cpp
src/TCPServerConnection.cpp
src/TCPServerConnectionFactory.cpp
src/TCPServerDispatcher.cpp
src/TCPServerParams.cpp
)

set( WIN_SRCS
)

set( LIN_SRCS
)

if(CMAKE_SYSTEM MATCHES "Windows")
 set(SRCS ${BASE_SRCS} ${WIN_SRCS})
else (CMAKE_SYSTEM MATCHES "Windows")
 set(SRCS ${BASE_SRCS} ${LIN_SRCS})
endif(CMAKE_SYSTEM MATCHES "Windows")

if (NOT POCO_STATIC)
  add_definitions(-DNet_EXPORTS)
endif (NOT POCO_STATIC)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES COMPILE_FLAGS ${DEBUG_CXX_FLAGS}
	VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}
	SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
target_link_libraries( ${LIBNAME} )

add_library( ${LIBNAMED} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAMED}  
	PROPERTIES COMPILE_FLAGS "${RELEASE_CXX_FLAGS}" 
	VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}
	SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
target_link_libraries( ${LIBNAMED} )

install(
    DIRECTORY include/Poco
    DESTINATION include
    )
    
install(
    TARGETS ${LIBNAME} ${LIBNAMED}
    DESTINATION lib
    )

#add_subdirectory( testsuite )
