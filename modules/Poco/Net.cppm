//
// Net.cppm
//
// C++ module file
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_NET
#include "Poco/Net/AbstractHTTPRequestHandler.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/AcceptCertificateHandler.h"
#ifdef ENABLE_NETSSL_WIN
#include "Poco/Net/AutoSecBufferDesc.h"
#endif
#include "Poco/Net/CertificateHandlerFactory.h"
#include "Poco/Net/CertificateHandlerFactoryMgr.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/Context.h"
#endif
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/DatagramSocketImpl.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/DNS.h"
#include "Poco/Net/EscapeHTMLStream.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/Net/FTPClientSession.h"
#ifdef ENABLE_SSL_OPENSSL
#include "Poco/Net/FTPSClientSession.h"
#include "Poco/Net/FTPSStreamFactory.h"
#endif
#include "Poco/Net/FTPStreamFactory.h"
#include "Poco/Net/HostEntry.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPAuthenticationParams.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/HTTPBasicStreamBuf.h"
#include "Poco/Net/HTTPBufferAllocator.h"
#include "Poco/Net/HTTPChunkedStream.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/HTTPDigestCredentials.h"
#include "Poco/Net/HTTPFixedLengthStream.h"
#include "Poco/Net/HTTPHeaderStream.h"
#include "Poco/Net/HTTPIOStream.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/HTTPNTLMCredentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPResponse.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/HTTPSClientSession.h"
#endif
#include "Poco/Net/HTTPServerConnectionFactory.h"
#include "Poco/Net/HTTPServerConnection.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerResponseImpl.h"
#include "Poco/Net/HTTPServerSession.h"
#include "Poco/Net/HTTPSessionFactory.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/HTTPSessionInstantiator.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/HTTPSSessionInstantiator.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#endif
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPStream.h"
#include "Poco/Net/ICMPClient.h"
#include "Poco/Net/ICMPEventArgs.h"
#include "Poco/Net/ICMPPacket.h"
#include "Poco/Net/ICMPPacketImpl.h"
#include "Poco/Net/ICMPSocket.h"
#include "Poco/Net/ICMPSocketImpl.h"
#include "Poco/Net/ICMPv4PacketImpl.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/InvalidCertificateHandler.h"
#endif
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/IPAddressImpl.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/KeyFileHandler.h"
#endif
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/MailStream.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/MulticastSocket.h"
#include "Poco/Net/MultipartReader.h"
#include "Poco/Net/MultipartWriter.h"
#include "Poco/Net/MultiSocketPoller.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/Net.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/NetSSL.h"
#endif
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Net/NTLMCredentials.h"
#include "Poco/Net/NTPClient.h"
#include "Poco/Net/NTPEventArgs.h"
#include "Poco/Net/NTPPacket.h"
#include "Poco/Net/NullPartHandler.h"
#include "Poco/Net/OAuth10Credentials.h"
#include "Poco/Net/OAuth20Credentials.h"
#include "Poco/Net/ParallelSocketAcceptor.h"
#include "Poco/Net/ParallelSocketReactor.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/PartSource.h"
#include "Poco/Net/PartStore.h"
#include "Poco/Net/PollSet.h"
#include "Poco/Net/POP3ClientSession.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/PrivateKeyFactory.h"
#include "Poco/Net/PrivateKeyFactoryMgr.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#endif
#include "Poco/Net/QuotedPrintableDecoder.h"
#include "Poco/Net/QuotedPrintableEncoder.h"
#include "Poco/Net/RawSocket.h"
#include "Poco/Net/RawSocketImpl.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/RejectCertificateHandler.h"
#endif
#include "Poco/Net/RemoteSyslogChannel.h"
#include "Poco/Net/RemoteSyslogListener.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/SecureServerSocketImpl.h"
#include "Poco/Net/SecureSMTPClientSession.h"
#include "Poco/Net/SecureSocketImpl.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureStreamSocketImpl.h"
#endif
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/ServerSocketImpl.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/Session.h"
#endif
#include "Poco/Net/SingleSocketPoller.h"
#include "Poco/Net/SMTPChannel.h"
#include "Poco/Net/SMTPClientSession.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketAddressImpl.h"
#include "Poco/Net/SocketConnector.h"
#include "Poco/Net/SocketDefs.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/SocketImpl.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketNotifier.h"
#include "Poco/Net/SocketProactor.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketStream.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/SSLException.h"
#include "Poco/Net/SSLManager.h"
#endif
#include "Poco/Net/SSPINTLMCredentials.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/StreamSocketImpl.h"
#include "Poco/Net/StringPartSource.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerDispatcher.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/UDPClient.h"
#include "Poco/Net/UDPHandler.h"
#include "Poco/Net/UDPServer.h"
#include "Poco/Net/UDPServerParams.h"
#include "Poco/Net/UDPSocketReader.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/Utility.h"
#include "Poco/Net/VerificationErrorArgs.h"
#endif
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/WebSocketImpl.h"
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
#include "Poco/Net/X509Certificate.h"
#endif
#endif

export module Poco.Net;

export namespace Poco::Net {
	#ifdef ENABLE_NET
	using Poco::Net::AbstractHTTPRequestHandler;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::AcceptCertificateHandler;
	#endif
	using Poco::Net::AddressFamily;
	using Poco::Net::AddressFamilyMismatchException;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	#ifdef ENABLE_NETSSL_WIN
	using Poco::Net::AutoSecBufferDesc;
	#endif
	using Poco::Net::CertificateHandlerFactory;
	using Poco::Net::CertificateHandlerFactoryImpl;
	using Poco::Net::CertificateHandlerFactoryMgr;
	using Poco::Net::CertificateHandlerFactoryRegistrar;
	using Poco::Net::CertificateValidationException;
	#endif
	using Poco::Net::ConnectionAbortedException;
	using Poco::Net::ConnectionRefusedException;
	using Poco::Net::ConnectionResetException;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::ConsoleCertificateHandler;
	using Poco::Net::Context;
	#endif
	using Poco::Net::DNS;
	using Poco::Net::DNSException;
	using Poco::Net::DatagramSocket;
	using Poco::Net::DatagramSocketImpl;
	using Poco::Net::DialogSocket;
	using Poco::Net::ErrorNotification;
	using Poco::Net::EscapeHTMLIOS;
	using Poco::Net::EscapeHTMLOutputStream;
	using Poco::Net::EscapeHTMLStreamBuf;
	using Poco::Net::FTPClientSession;
	using Poco::Net::FTPException;
	using Poco::Net::FTPPasswordProvider;
	#ifdef ENABLE_NETSSL_OPENSSL
	using Poco::Net::FTPSClientSession;
	using Poco::Net::FTPSStreamFactory;
	#endif
	using Poco::Net::FTPStreamFactory;
	using Poco::Net::FilePartSource;
	using Poco::Net::FilePartStore;
	using Poco::Net::FilePartStoreFactory;
	using Poco::Net::HTMLForm;
	using Poco::Net::HTMLFormException;
	using Poco::Net::HTTPAuthenticationParams;
	using Poco::Net::HTTPBasicCredentials;
	using Poco::Net::HTTPBufferAllocator;
	using Poco::Net::HTTPChunkedIOS;
	using Poco::Net::HTTPChunkedInputStream;
	using Poco::Net::HTTPChunkedOutputStream;
	using Poco::Net::HTTPChunkedStreamBuf;
	using Poco::Net::HTTPClientSession;
	using Poco::Net::HTTPCookie;
	using Poco::Net::HTTPCredentials;
	using Poco::Net::HTTPDigestCredentials;
	using Poco::Net::HTTPException;
	using Poco::Net::HTTPFixedLengthIOS;
	using Poco::Net::HTTPFixedLengthInputStream;
	using Poco::Net::HTTPFixedLengthOutputStream;
	using Poco::Net::HTTPFixedLengthStreamBuf;
	using Poco::Net::HTTPHeaderIOS;
	using Poco::Net::HTTPHeaderInputStream;
	using Poco::Net::HTTPHeaderOutputStream;
	using Poco::Net::HTTPHeaderStreamBuf;
	using Poco::Net::HTTPIOS;
	using Poco::Net::HTTPInputStream;
	using Poco::Net::HTTPMessage;
	using Poco::Net::HTTPNTLMCredentials;
	using Poco::Net::HTTPOutputStream;
	using Poco::Net::HTTPRequest;
	using Poco::Net::HTTPRequestHandler;
	using Poco::Net::HTTPRequestHandlerFactory;
	using Poco::Net::HTTPResponse;
	using Poco::Net::HTTPResponseIOS;
	using Poco::Net::HTTPResponseStream;
	using Poco::Net::HTTPResponseStreamBuf;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::HTTPSClientSession;
	using Poco::Net::HTTPSSessionInstantiator;
	using Poco::Net::HTTPSStreamFactory;
	#endif
	using Poco::Net::HTTPServer;
	using Poco::Net::HTTPServerConnection;
	using Poco::Net::HTTPServerConnectionFactory;
	using Poco::Net::HTTPServerParams;
	using Poco::Net::HTTPServerRequest;
	using Poco::Net::HTTPServerRequestImpl;
	using Poco::Net::HTTPServerResponse;
	using Poco::Net::HTTPServerResponseImpl;
	using Poco::Net::HTTPServerSession;
	using Poco::Net::HTTPSession;
	using Poco::Net::HTTPSessionFactory;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::HTTPSSessionInstantiator;
	#endif
	using Poco::Net::HTTPStreamBuf;
	using Poco::Net::HTTPStreamFactory;
	using Poco::Net::HostEntry;
	using Poco::Net::HostNotFoundException;
	using Poco::Net::ICMPClient;
	using Poco::Net::ICMPEventArgs;
	using Poco::Net::ICMPException;
	using Poco::Net::ICMPFragmentationException;
	using Poco::Net::ICMPPacket;
	using Poco::Net::ICMPPacketImpl;
	using Poco::Net::ICMPSocket;
	using Poco::Net::ICMPSocketImpl;
	using Poco::Net::ICMPv4PacketImpl;
	using Poco::Net::IPAddress;
	using Poco::Net::IdleNotification;
	using Poco::Net::InterfaceNotFoundException;
	using Poco::Net::InvalidAddressException;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::InvalidCertificateException;
	using Poco::Net::InvalidCertificateHandler;
	#endif
	using Poco::Net::InvalidSocketException;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::KeyConsoleHandler;
	using Poco::Net::KeyFileHandler;
	#endif
	using Poco::Net::MailIOS;
	using Poco::Net::MailInputStream;
	using Poco::Net::MailMessage;
	using Poco::Net::MailOutputStream;
	using Poco::Net::MailRecipient;
	using Poco::Net::MailStreamBuf;
	using Poco::Net::MediaType;
	using Poco::Net::MessageException;
	using Poco::Net::MessageHeader;
	using Poco::Net::MulticastSocket;
	using Poco::Net::MultipartException;
	using Poco::Net::MultipartIOS;
	using Poco::Net::MultipartInputStream;
	using Poco::Net::MultipartReader;
	using Poco::Net::MultipartSource;
	using Poco::Net::MultipartStreamBuf;
	using Poco::Net::MultipartWriter;
	using Poco::Net::NTLMContext;
	using Poco::Net::NTLMCredentials;
	using Poco::Net::NTPClient;
	using Poco::Net::NTPEventArgs;
	using Poco::Net::NTPException;
	using Poco::Net::NTPPacket;
	using Poco::Net::NameValueCollection;
	using Poco::Net::NetException;
	using Poco::Net::NetworkInterface;
	using Poco::Net::NoAddressFoundException;
	using Poco::Net::NoMessageException;
	using Poco::Net::NotAuthenticatedException;
	using Poco::Net::NullPartHandler;
	using Poco::Net::OAuth10Credentials;
	using Poco::Net::OAuth20Credentials;
	using Poco::Net::POP3ClientSession;
	using Poco::Net::POP3Exception;
	using Poco::Net::ParallelSocketAcceptor;
	using Poco::Net::ParallelSocketReactor;
	using Poco::Net::PartHandler;
	using Poco::Net::PartSource;
	using Poco::Net::PartStore;
	using Poco::Net::PartStoreFactory;
	using Poco::Net::PollSet;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::PrivateKeyFactory;
	using Poco::Net::PrivateKeyFactoryImpl;
	using Poco::Net::PrivateKeyFactoryMgr;
	using Poco::Net::PrivateKeyFactoryRegistrar;
	using Poco::Net::PrivateKeyPassphraseHandler;
	#endif
	using Poco::Net::QuotedPrintableDecoder;
	using Poco::Net::QuotedPrintableDecoderBuf;
	using Poco::Net::QuotedPrintableEncoderIOS;
	using Poco::Net::QuotedPrintableEncoder;
	using Poco::Net::QuotedPrintableEncoderBuf;
	using Poco::Net::QuotedPrintableEncoderIOS;
	using Poco::Net::RawSocket;
	using Poco::Net::RawSocketImpl;
	using Poco::Net::ReadableNotification;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::RejectCertificateHandler;
	#endif
	using Poco::Net::RemoteSyslogChannel;
	using Poco::Net::RemoteSyslogListener;
	using Poco::Net::SMTPChannel;
	using Poco::Net::SMTPClientSession;
	using Poco::Net::SMTPException;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::SSLConnectionUnexpectedlyClosedException;
	using Poco::Net::SSLContextException;
	using Poco::Net::SSLException;
	using Poco::Net::SSLManager;
	#endif
	using Poco::Net::SSPINTLMCredentials;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::SecureSMTPClientSession;
	using Poco::Net::SecureServerSocket;
	using Poco::Net::SecureServerSocketImpl;
	using Poco::Net::SecureStreamSocket;
	using Poco::Net::SecureSocketImpl;
	#endif
	using Poco::Net::ServerSocket;
	using Poco::Net::ServerSocketImpl;
	using Poco::Net::ServiceNotFoundException;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::Session;
	#endif
	using Poco::Net::ShutdownNotification;
	using Poco::Net::Socket;
	using Poco::Net::SocketAcceptor;
	using Poco::Net::SocketAddress;
	using Poco::Net::SocketConnector;
	using Poco::Net::SocketIOS;
	using Poco::Net::SocketImpl;
	using Poco::Net::SocketInputStream;
	using Poco::Net::SocketNotification;
	using Poco::Net::SocketNotifier;
	using Poco::Net::SocketOutputStream;
	using Poco::Net::SocketProactor;
	using Poco::Net::SocketReactor;
	using Poco::Net::SocketStream;
	using Poco::Net::SocketStreamBuf;
	using Poco::Net::StreamSocket;
	using Poco::Net::StreamSocketImpl;
	using Poco::Net::StringPartSource;
	using Poco::Net::TCPServer;
	using Poco::Net::TCPServerConnection;
	using Poco::Net::TCPServerConnectionFactory;
	using Poco::Net::TCPServerConnectionFactoryImpl;
	using Poco::Net::TCPServerConnectionFilter;
	using Poco::Net::TCPServerDispatcher;
	using Poco::Net::TCPServerParams;
	using Poco::Net::TimeoutNotification;
	using Poco::Net::UDPClient;
	using Poco::Net::UDPServerParams;
	using Poco::Net::UDPSocketReader;
	using Poco::Net::UnsupportedFamilyException;
	using Poco::Net::UnsupportedRedirectException;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::Utility;
	using Poco::Net::VerificationErrorArgs;
	#endif
	using Poco::Net::WebSocket;
	using Poco::Net::WebSocketException;
	using Poco::Net::WebSocketImpl;
	using Poco::Net::WritableNotification;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::X509Certificate;
	#endif

	using Poco::Net::SocketBufVec;
	using Poco::Net::UDPMsgSizeT;
	
	using Poco::Net::HTTPBasicStreamBuf;
	using Poco::Net::UDPMultiServer;
	using Poco::Net::UDPServer;

	using Poco::Net::SocketBufVecSize;
	using Poco::Net::htmlize;
	using Poco::Net::initializeNetwork;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::initializeSSL;
	#endif
	using Poco::Net::operator<<;
	using Poco::Net::operator>>;
	using Poco::Net::swap;
	using Poco::Net::uninitializeNetwork;
	#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	using Poco::Net::uninitializeSSL;
	#endif

	namespace Impl {
		using Poco::Net::Impl::IPAddressImpl;
		using Poco::Net::Impl::IPv4AddressImpl;
		using Poco::Net::Impl::IPv4SocketAddressImpl;
		using Poco::Net::Impl::IPv6AddressImpl;
		using Poco::Net::Impl::IPv6SocketAddressImpl;
		using Poco::Net::Impl::LocalSocketAddressImpl;
		using Poco::Net::Impl::SocketAddressImpl;
	}
	#endif
}
