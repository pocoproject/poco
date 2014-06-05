set(LIBNAME "PocoNet")

set( BASE_SRCS 
  src/AbstractHTTPRequestHandler.cpp
  src/DatagramSocket.cpp
  src/DatagramSocketImpl.cpp
  src/DialogSocket.cpp
  src/DNS.cpp
  src/FilePartSource.cpp
  src/FTPClientSession.cpp
  src/FTPStreamFactory.cpp
  src/HostEntry.cpp
  src/HTMLForm.cpp
  src/HTTPAuthenticationParams.cpp
  src/HTTPBasicCredentials.cpp
  src/HTTPBufferAllocator.cpp
  src/HTTPChunkedStream.cpp
  src/HTTPClientSession.cpp
  src/HTTPCookie.cpp
  src/HTTPCredentials.cpp
  src/HTTPDigestCredentials.cpp
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
  src/ICMPClient.cpp
  src/ICMPEventArgs.cpp
  src/ICMPPacket.cpp
  src/ICMPPacketImpl.cpp
  src/ICMPSocket.cpp
  src/ICMPSocketImpl.cpp
  src/ICMPv4PacketImpl.cpp
  src/IPAddress.cpp
  src/IPAddressImpl.cpp
  src/MailMessage.cpp
  src/MailRecipient.cpp
  src/MailStream.cpp
  src/MediaType.cpp
  src/MessageHeader.cpp
  src/MulticastSocket.cpp
  src/MultipartReader.cpp
  src/MultipartWriter.cpp
  src/NameValueCollection.cpp
  src/Net.cpp
  src/NetException.cpp
  src/NetworkInterface.cpp
  src/NTPClient.cpp
  src/NTPEventArgs.cpp
  src/NTPPacket.cpp
  src/NullPartHandler.cpp
  src/PartHandler.cpp
  src/PartSource.cpp
  src/PartStore.cpp
  src/POP3ClientSession.cpp
  src/QuotedPrintableDecoder.cpp
  src/QuotedPrintableEncoder.cpp
  src/RawSocket.cpp
  src/RawSocketImpl.cpp
  src/RemoteSyslogChannel.cpp
  src/RemoteSyslogListener.cpp
  src/ServerSocket.cpp
  src/ServerSocketImpl.cpp
  src/SMTPChannel.cpp
  src/SMTPClientSession.cpp
  src/Socket.cpp
  src/SocketAddress.cpp
  src/SocketAddressImpl.cpp
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
  src/WebSocket.cpp
  src/WebSocketImpl.cpp
)

set( WIN_SRCS
)

set( LIN_SRCS
)

if(CMAKE_SYSTEM MATCHES "Windows")
 set(SRCS ${BASE_SRCS} ${WIN_SRCS})
 set(SYSLIBS ${SYSLIBS} "ws2_32.lib" "iphlpapi.lib") 
else (CMAKE_SYSTEM MATCHES "Windows")
 set(SRCS ${BASE_SRCS} ${LIN_SRCS})
endif(CMAKE_SYSTEM MATCHES "Windows")

if (NOT POCO_STATIC)
  add_definitions(-DNet_EXPORTS)
endif (NOT POCO_STATIC)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} PocoFoundation ${SYSLIBS})

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )
    
install(
    TARGETS ${LIBNAME}
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    RUNTIME DESTINATION bin
    )

if (ENABLE_TESTS)
  add_subdirectory(samples)
  add_subdirectory(testsuite)
endif ()

