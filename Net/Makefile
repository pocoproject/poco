#
# Makefile
#
# $Id: //poco/1.4/Net/Makefile#3 $
#
# Makefile for Poco Net
#

include $(POCO_BASE)/build/rules/global

SHAREDOPT_CXX   += -DNet_EXPORTS

objects = \
	DNS HTTPResponse HostEntry Socket \
	DatagramSocket HTTPServer IPAddress SocketAddress \
	HTTPBasicCredentials HTTPCookie HTMLForm MediaType DialogSocket \
	DatagramSocketImpl FilePartSource HTTPServerConnection MessageHeader \
	HTTPChunkedStream HTTPServerConnectionFactory MulticastSocket SocketStream \
	HTTPClientSession HTTPServerParams MultipartReader StreamSocket SocketImpl \
	HTTPFixedLengthStream HTTPServerRequest HTTPServerRequestImpl MultipartWriter StreamSocketImpl \
	HTTPHeaderStream HTTPServerResponse HTTPServerResponseImpl NameValueCollection TCPServer \
	HTTPMessage HTTPServerSession NetException TCPServerConnection HTTPBufferAllocator \
	HTTPAuthenticationParams HTTPCredentials HTTPDigestCredentials \
	HTTPRequest HTTPSession HTTPSessionInstantiator HTTPSessionFactory NetworkInterface  \
	HTTPRequestHandler HTTPStream HTTPIOStream ServerSocket TCPServerDispatcher TCPServerConnectionFactory \
	HTTPRequestHandlerFactory HTTPStreamFactory ServerSocketImpl TCPServerParams \
	QuotedPrintableEncoder QuotedPrintableDecoder StringPartSource \
	FTPClientSession FTPStreamFactory PartHandler PartSource NullPartHandler \
	SocketReactor SocketNotifier SocketNotification AbstractHTTPRequestHandler \
	MailRecipient MailMessage MailStream SMTPClientSession POP3ClientSession \
	RawSocket RawSocketImpl ICMPClient ICMPEventArgs ICMPPacket ICMPPacketImpl \
	ICMPSocket ICMPSocketImpl ICMPv4PacketImpl \
	RemoteSyslogChannel RemoteSyslogListener \
	WebSocket WebSocketImpl

target         = PocoNet
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
