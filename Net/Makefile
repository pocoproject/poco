#
# Makefile
#
# $Id: //poco/1.2/Net/Makefile#1 $
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
	HTTPFixedLengthStream HTTPServerRequest MultipartWriter StreamSocketImpl \
	HTTPHeaderStream HTTPServerResponse NameValueCollection TCPServer \
	HTTPMessage HTTPServerSession NetException TCPServerConnection HTTPBufferAllocator \
	HTTPRequest HTTPSession HTTPSessionInstantiator HTTPSessionFactory NetworkInterface TCPServerConnectionFactory \
	HTTPRequestHandler HTTPStream HTTPIOStream ServerSocket TCPServerDispatcher \
	HTTPRequestHandlerFactory HTTPStreamFactory ServerSocketImpl TCPServerParams \
	QuotedPrintableEncoder QuotedPrintableDecoder StringPartSource \
	FTPClientSession FTPStreamFactory PartHandler PartSource NullPartHandler \
	SocketReactor SocketNotifier SocketNotification \
	MailRecipient MailMessage MailStream SMTPClientSession POP3ClientSession \
	RawSocket RawSocketImpl ICMPClient ICMPEventArgs ICMPPacket ICMPPacketImpl \
	ICMPSocket ICMPSocketImpl ICMPv4PacketImpl

target         = PocoNet
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
