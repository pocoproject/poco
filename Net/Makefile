#
# Makefile
#
# $Id: //poco/1.1.0/Net/Makefile#1 $
#
# Makefile for Poco Net
#

include $(POCO_BASE)/build/rules/global

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
	HTTPRequest HTTPSession NetworkInterface TCPServerConnectionFactory \
	HTTPRequestHandler HTTPStream HTTPIOStream ServerSocket TCPServerDispatcher \
	HTTPRequestHandlerFactory HTTPStreamFactory ServerSocketImpl TCPServerParams \
	QuotedPrintableEncoder QuotedPrintableDecoder StringPartSource \
	FTPClientSession FTPStreamFactory PartHandler PartSource NullPartHandler \
	SocketReactor SocketNotifier SocketNotification \
	MailRecipient MailMessage MailStream SMTPClientSession POP3ClientSession

target         = PocoNet
target_version = 1
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
