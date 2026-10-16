//
// HTTPSClientSessionTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPSClientSessionTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Event.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include "DialogServer.h"
#include "HTTPSTestServer.h"
#include <atomic>
#include <iostream>
#include <sstream>


using namespace Poco::Net;
using Poco::Util::Application;
using Poco::StreamCopier;
using Poco::Thread;


class TestRequestHandler: public HTTPRequestHandler
	/// Return a HTML document with the current date and time.
{
public:
	TestRequestHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		response.setChunkedTransferEncoding(true);
		response.setContentType(request.getContentType());
		std::ostream& ostr = response.send();
		Poco::StreamCopier::copyStream(request.stream(), ostr);
	}

};


class TestRequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
	TestRequestHandlerFactory()
	{
	}

	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		return new TestRequestHandler();
	}
};


class TLSConnectProxy: public Poco::Runnable
	/// A proxy that serves one TLS connection. It answers the CONNECT
	/// request with 200, then relays the decrypted bytes to the given
	/// port on this host and the bytes from there back.
{
public:
	enum Tunnel
	{
		TUNNEL_RELAY,
			/// The proxy relays the bytes in both directions.
		TUNNEL_CLOSE,
			/// The proxy closes the connection after its answer,
			/// without a close_notify alert.
		TUNNEL_STALL,
			/// The proxy keeps the connection open and relays nothing.
		TUNNEL_CORRUPT,
			/// As TUNNEL_RELAY, but after arm() it flips one bit in the
			/// next bytes from the target.
		TUNNEL_CUT
			/// As TUNNEL_RELAY, but after arm() it relays the next bytes
			/// from the target and then closes the TLS connection with a
			/// close_notify alert.
	};

	explicit TLSConnectProxy(Poco::UInt16 targetPort, Tunnel tunnel = TUNNEL_RELAY):
		_socket(SocketAddress("127.0.0.1", 0)),
		_targetPort(targetPort),
		_tunnel(tunnel),
		_thread("TLSConnectProxy"),
		_stop(false),
		_armed(false)
	{
		_thread.start(*this);
	}

	~TLSConnectProxy()
	{
		_stop = true;
		_thread.join();
	}

	Poco::UInt16 port() const
	{
		return _socket.address().port();
	}

	const std::string& connectRequest() const
		/// Returns the request the proxy received. Valid once the
		/// client has an answer from the target.
	{
		return _connectRequest;
	}

	void arm()
		/// Starts the action of TUNNEL_CORRUPT or TUNNEL_CUT.
	{
		_armed = true;
	}

	bool waitForTunnelData(long milliseconds)
		/// Waits until data from the client arrives through the
		/// tunnel of TUNNEL_STALL.
	{
		return _tunnelData.tryWait(milliseconds);
	}

	void run()
	{
		const Poco::Timespan span(50000);
		try
		{
			while (!_socket.poll(span, Socket::SELECT_READ))
			{
				if (_stop) return;
			}
			StreamSocket connection = _socket.acceptConnection();
			connection.setReceiveTimeout(Poco::Timespan(5, 0));
			SecureStreamSocket client = SecureStreamSocket::attach(connection, SSLManager::instance().defaultServerContext());
			char buffer[4096];
			while (_connectRequest.find("\r\n\r\n") == std::string::npos)
			{
				int n = client.receiveBytes(buffer, sizeof(buffer));
				if (n <= 0) return;
				_connectRequest.append(buffer, n);
			}
			StreamSocket target(SocketAddress("127.0.0.1", _targetPort));
			const std::string established("HTTP/1.1 200 Connection established\r\n\r\n");
			client.sendBytes(established.data(), static_cast<int>(established.size()));
			if (_tunnel == TUNNEL_CLOSE)
			{
				// Only the sending direction is shut down: data that arrives
				// after the receiving direction is shut down resets the
				// connection on Linux and Windows.
				connection.shutdownSend();
				// The descriptor stays open: closing it could reset the
				// connection, and the client would see that instead.
				while (!_stop) Thread::sleep(static_cast<long>(span.totalMilliseconds()));
				return;
			}
			if (_tunnel == TUNNEL_STALL)
			{
				// Nothing is read: the connection would end with a close_notify
				// alert of the client.
				while (!_stop && !client.poll(span, Socket::SELECT_READ))
				{
				}
				_tunnelData.set();
				while (!_stop) Thread::sleep(static_cast<long>(span.totalMilliseconds()));
				return;
			}
			while (!_stop)
			{
				// decrypted data is not seen by poll()
				if (client.available() > 0 || client.poll(span, Socket::SELECT_READ))
				{
					int n = client.receiveBytes(buffer, sizeof(buffer));
					if (n <= 0) return;
					target.sendBytes(buffer, n);
				}
				if (target.poll(span, Socket::SELECT_READ))
				{
					int n = target.receiveBytes(buffer, sizeof(buffer));
					if (n <= 0) return;
					// The flag is read before the bytes are relayed: the client
					// may answer them with arm().
					const bool armed = _armed.exchange(false);
					if (armed && _tunnel == TUNNEL_CORRUPT) buffer[n - 1] ^= 1;
					client.sendBytes(buffer, n);
					if (armed && _tunnel == TUNNEL_CUT)
					{
						// the descriptor stays open, as with TUNNEL_CLOSE
						client.shutdown();
						while (!_stop) Thread::sleep(static_cast<long>(span.totalMilliseconds()));
						return;
					}
				}
			}
		}
		catch (Poco::Exception& exc)
		{
			std::cerr << "TLSConnectProxy: " << exc.displayText() << std::endl;
		}
	}

private:
	ServerSocket _socket;
	Poco::UInt16 _targetPort;
	Tunnel _tunnel;
	Poco::Thread _thread;
	std::atomic<bool> _stop;
	std::atomic<bool> _armed;
	Poco::Event _tunnelData;
	std::string _connectRequest;
};


namespace
{
	class HeldBackHTTPSServer: public Poco::Runnable
		/// An HTTPS server for one connection. It reads the request,
		/// sends the header of its response after sendHeader() and the
		/// body after sendBody(), and closes the connection when it is
		/// destroyed, so that its close_notify alert does not follow the
		/// body at once.
	{
	public:
		HeldBackHTTPSServer(const std::string& header, const std::string& body):
			_socket(SocketAddress("127.0.0.1", 0)),
			_header(header),
			_body(body),
			_thread("HeldBackHTTPSServer"),
			_stop(false)
		{
			_thread.start(*this);
		}

		~HeldBackHTTPSServer()
		{
			_stop = true;
			_headerReleased.set();
			_bodyReleased.set();
			_stopped.set();
			_thread.join();
		}

		Poco::UInt16 port() const
		{
			return _socket.address().port();
		}

		void sendHeader()
		{
			_headerReleased.set();
		}

		void sendBody()
		{
			_bodyReleased.set();
		}

		void run()
		{
			const Poco::Timespan span(50000);
			const Poco::Timespan releaseTimeout(5, 0);
			try
			{
				while (!_socket.poll(span, Socket::SELECT_READ))
				{
					if (_stop) return;
				}
				StreamSocket connection = _socket.acceptConnection();
				connection.setReceiveTimeout(Poco::Timespan(5, 0));
				SecureStreamSocket client = SecureStreamSocket::attach(connection, SSLManager::instance().defaultServerContext());
				std::string request;
				char buffer[1024];
				while (request.find("\r\n\r\n") == std::string::npos)
				{
					int n = client.receiveBytes(buffer, sizeof(buffer));
					if (n <= 0) return;
					request.append(buffer, n);
				}
				const long releaseWait = static_cast<long>(releaseTimeout.totalMilliseconds());
				if (!_headerReleased.tryWait(releaseWait) || _stop) return;
				client.sendBytes(_header.data(), static_cast<int>(_header.size()));
				if (!_bodyReleased.tryWait(releaseWait) || _stop) return;
				client.sendBytes(_body.data(), static_cast<int>(_body.size()));
				(void) _stopped.tryWait(releaseWait);
			}
			catch (Poco::Exception& exc)
			{
				std::cerr << "HeldBackHTTPSServer: " << exc.displayText() << std::endl;
			}
		}

	private:
		ServerSocket _socket;
		std::string _header;
		std::string _body;
		Poco::Thread _thread;
		std::atomic<bool> _stop;
		Poco::Event _headerReleased;
		Poco::Event _bodyReleased;
		Poco::Event _stopped;
	};
}


HTTPSClientSessionTest::HTTPSClientSessionTest(const std::string& name): CppUnit::TestCase(name)
{
}


HTTPSClientSessionTest::~HTTPSClientSessionTest()
{
}


void HTTPSClientSessionTest::testFromSocket()
{
	HTTPSTestServer srv;
	SecureStreamSocket sss("localhost");
	HTTPSClientSession s(sss, "127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/small");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPSTestServer::SMALL_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPSTestServer::SMALL_BODY);
}


void HTTPSClientSessionTest::testGetSmall()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/small");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPSTestServer::SMALL_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPSTestServer::SMALL_BODY);
}


void HTTPSClientSessionTest::testSourceAddress()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("127.0.0.1", srv.port());
	s.setSourceAddress(SocketAddress("127.0.0.1", 0));
	// without keep-alive the second request binds and connects again
	for (int i = 0; i < 2; ++i)
	{
		HTTPRequest request(HTTPRequest::HTTP_GET, "/small");
		s.sendRequest(request);
		HTTPResponse response;
		std::istream& rs = s.receiveResponse(response);
		assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
		std::ostringstream ostr;
		StreamCopier::copyStream(rs, ostr);
		assertTrue (ostr.str() == HTTPSTestServer::SMALL_BODY);
	}
}


void HTTPSClientSessionTest::testGetLarge()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/large");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPSTestServer::LARGE_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPSTestServer::LARGE_BODY);
}


void HTTPSClientSessionTest::testHead()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_HEAD, "/large");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPSTestServer::LARGE_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	assertTrue (StreamCopier::copyStream(rs, ostr) == 0);
}


void HTTPSClientSessionTest::testPostSmallIdentity()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_POST, "/echo");
	std::string body("this is a random request body\r\n0\r\n");
	request.setContentLength((int) body.length());
	s.sendRequest(request) << body;
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == body.length());
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == body);
}


void HTTPSClientSessionTest::testPostLargeIdentity()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_POST, "/echo");
	std::string body(8000, 'x');
	body.append("\r\n0\r\n");
	request.setContentLength((int) body.length());
	s.sendRequest(request) << body;
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == body.length());
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == body);
}


void HTTPSClientSessionTest::testPostSmallChunked()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_POST, "/echo");
	std::string body("this is a random request body");
	request.setChunkedTransferEncoding(true);
	s.sendRequest(request) << body;
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getChunkedTransferEncoding());
	assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == body);
}


void HTTPSClientSessionTest::testPostLargeChunked()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_POST, "/echo");
	std::string body(16000, 'x');
	request.setChunkedTransferEncoding(true);
	s.sendRequest(request) << body;
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getChunkedTransferEncoding());
	assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == body);
}


void HTTPSClientSessionTest::testPostLargeChunkedKeepAlive()
{
	SecureServerSocket svs(32322);
	HTTPServer srv(new TestRequestHandlerFactory(), svs, new HTTPServerParams());
	srv.start();
	try
	{
		HTTPSClientSession s("127.0.0.1", srv.port());
		s.setKeepAlive(true);
		for (int i = 0; i < 10; ++i)
		{
			HTTPRequest request(HTTPRequest::HTTP_POST, "/keepAlive", HTTPMessage::HTTP_1_1);
			std::string body(16000, 'x');
			request.setChunkedTransferEncoding(true);
			s.sendRequest(request) << body;
			HTTPResponse response;
			std::istream& rs = s.receiveResponse(response);
			assertTrue (response.getChunkedTransferEncoding());
			assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
			std::ostringstream ostr;
			StreamCopier::copyStream(rs, ostr);
			assertTrue (ostr.str() == body);
		}
		srv.stop();
	}
	catch (...)
	{
		srv.stop();
		throw;
	}
}


void HTTPSClientSessionTest::testKeepAlive()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("127.0.0.1", srv.port());
	s.setKeepAlive(true);
	HTTPRequest request(HTTPRequest::HTTP_HEAD, "/keepAlive", HTTPMessage::HTTP_1_1);
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs1 = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPSTestServer::SMALL_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	assertTrue (response.getKeepAlive());
	std::ostringstream ostr1;
	assertTrue (StreamCopier::copyStream(rs1, ostr1) == 0);

	request.setMethod(HTTPRequest::HTTP_GET);
	request.setURI("/small");
	s.sendRequest(request);
	std::istream& rs2 = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPSTestServer::SMALL_BODY.length());
	assertTrue (response.getKeepAlive());
	std::ostringstream ostr2;
	StreamCopier::copyStream(rs2, ostr2);
	assertTrue (ostr2.str() == HTTPSTestServer::SMALL_BODY);

	request.setMethod(HTTPRequest::HTTP_GET);
	request.setURI("/large");
	s.sendRequest(request);
	std::istream& rs3 = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	assertTrue (response.getChunkedTransferEncoding());
	assertTrue (response.getKeepAlive());
	std::ostringstream ostr3;
	StreamCopier::copyStream(rs3, ostr3);
	assertTrue (ostr3.str() == HTTPSTestServer::LARGE_BODY);

	request.setMethod(HTTPRequest::HTTP_HEAD);
	request.setURI("/large");
	s.sendRequest(request);
	std::istream& rs4 = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPSTestServer::LARGE_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	assertTrue (!response.getKeepAlive());
	std::ostringstream ostr4;
	assertTrue (StreamCopier::copyStream(rs4, ostr4) == 0);
}


void HTTPSClientSessionTest::testMultipleSSLInit()
{

	auto initSSL = []()
	{
		initializeSSL();
		Poco::SharedPtr<InvalidCertificateHandler> ptrCert = new AcceptCertificateHandler(false);
		Context::Ptr context(new Context(Context::CLIENT_USE, "", "", "",
				Context::VerificationMode::VERIFY_STRICT, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
			)
		);
		SSLManager::instance().initializeClient(nullptr, ptrCert, context);
	};

	auto deinitSSL = []()
	{
		uninitializeSSL();
	};

	try
	{
		initSSL();
		deinitSSL();

		initSSL();

		HTTPSClientSession session("secure.appinf.com");
		HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
		(void)session.sendRequest(request);
		deinitSSL();
	}
	catch(...)
	{
		failmsg("Double SSL init failed");
	}
}


void HTTPSClientSessionTest::testInterop()
{
	HTTPSClientSession s("secure.appinf.com");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/public/poco/NetSSL.txt");
	s.sendRequest(request);
	X509Certificate cert = s.serverCertificate();
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	std::string str(ostr.str());
	assertTrue (str == "This is a test file for NetSSL.\n");
	assertTrue (cert.commonName().find(".appinf.com") != std::string::npos);
}


void HTTPSClientSessionTest::testProxy()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("secure.appinf.com");
	s.setProxy(
		Application::instance().config().getString("testsuite.proxy.host"),
		Application::instance().config().getInt("testsuite.proxy.port")
	);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/public/poco/NetSSL.txt");
	s.sendRequest(request);
	X509Certificate cert = s.serverCertificate();
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	std::string str(ostr.str());
	assertTrue (str == "This is a test file for NetSSL.\n");
	assertTrue (cert.commonName().find(".appinf.com") != std::string::npos);
}


void HTTPSClientSessionTest::testCachedSession()
{
	// ensure OpenSSL machinery is fully setup
	Context::Ptr pDefaultServerContext = SSLManager::instance().defaultServerContext();
	Context::Ptr pDefaultClientContext = SSLManager::instance().defaultClientContext();

	Context::Ptr pServerContext = new Context(
		Context::SERVER_USE,
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.privateKeyFile"),
		Application::instance().config().getString("openSSL.server.caConfig"),
		Context::VERIFY_NONE,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pServerContext->enableSessionCache(true, "TestSuite");
	pServerContext->setSessionTimeout(10);
	pServerContext->setSessionCacheSize(1000);
	pServerContext->disableStatelessSessionResumption();

	HTTPSTestServer srv(pServerContext);

	Context::Ptr pClientContext = new Context(
		Context::CLIENT_USE,
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.privateKeyFile"),
		Application::instance().config().getString("openSSL.client.caConfig"),
		Context::VERIFY_RELAXED,
		9,
		true,
		"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	pClientContext->enableSessionCache(true);

	HTTPSClientSession s1("127.0.0.1", srv.port(), pClientContext);
	HTTPRequest request1(HTTPRequest::HTTP_GET, "/small");
	s1.sendRequest(request1);
	Session::Ptr pSession1 = s1.sslSession();
	HTTPResponse response1;
	std::istream& rs1 = s1.receiveResponse(response1);
	assertTrue (response1.getContentLength() == HTTPSTestServer::SMALL_BODY.length());
	assertTrue (response1.getContentType() == "text/plain");
	std::ostringstream ostr1;
	StreamCopier::copyStream(rs1, ostr1);
	assertTrue (ostr1.str() == HTTPSTestServer::SMALL_BODY);

	HTTPSClientSession s2("127.0.0.1", srv.port(), pClientContext, pSession1);
	HTTPRequest request2(HTTPRequest::HTTP_GET, "/small");
	s2.sendRequest(request2);
	HTTPResponse response2;
	std::istream& rs2 = s2.receiveResponse(response2);
	assertTrue (response2.getContentLength() == HTTPSTestServer::SMALL_BODY.length());
	assertTrue (response2.getContentType() == "text/plain");
	std::ostringstream ostr2;
	StreamCopier::copyStream(rs2, ostr2);
	assertTrue (ostr2.str() == HTTPSTestServer::SMALL_BODY);

	HTTPRequest request3(HTTPRequest::HTTP_GET, "/small");
	s2.sendRequest(request3);
	HTTPResponse response3;
	std::istream& rs3 = s2.receiveResponse(response3);
	assertTrue (response3.getContentLength() == HTTPSTestServer::SMALL_BODY.length());
	assertTrue (response3.getContentType() == "text/plain");
	std::ostringstream ostr3;
	StreamCopier::copyStream(rs3, ostr3);
	assertTrue (ostr3.str() == HTTPSTestServer::SMALL_BODY);

	Thread::sleep(15000); // wait for session to expire
	pServerContext->flushSessionCache();

	HTTPRequest request4(HTTPRequest::HTTP_GET, "/small");
	s2.sendRequest(request4);
	HTTPResponse response4;
	std::istream& rs4 = s2.receiveResponse(response4);
	assertTrue (response4.getContentLength() == HTTPSTestServer::SMALL_BODY.length());
	assertTrue (response4.getContentType() == "text/plain");
	std::ostringstream ostr4;
	StreamCopier::copyStream(rs4, ostr4);
	assertTrue (ostr4.str() == HTTPSTestServer::SMALL_BODY);
}


void HTTPSClientSessionTest::testUnknownContentLength()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/nolength");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPSTestServer::SMALL_BODY);
}


void HTTPSClientSessionTest::testServerAbort()
{
	HTTPSTestServer srv;
	HTTPSClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/nolength/connection/abort");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPSTestServer::SMALL_BODY);
	assertTrue (dynamic_cast<const Poco::Net::SSLConnectionUnexpectedlyClosedException*>(
			 s.networkException()) != nullptr );
}


void HTTPSClientSessionTest::testProxyConfig()
{
	HTTPSClientSession s("www.example.com");
	assertTrue (s.isProxyTunnel() == true);
	assertTrue (s.getProxyProtocol() == "http");
	assertTrue (s.getProxyHost().empty());
}


void HTTPSClientSessionTest::testProxySetters()
{
	HTTPSClientSession s("www.example.com");

	s.setProxy("proxy.example.com", 3128, "https", false);
	assertTrue (s.getProxyHost() == "proxy.example.com");
	assertTrue (s.getProxyPort() == 3128);
	assertTrue (s.getProxyProtocol() == "https");
	assertTrue (s.isProxyTunnel() == false);

	s.setProxyTunnel(true);
	assertTrue (s.isProxyTunnel() == true);

	s.setProxyTunnel(false);
	assertTrue (s.isProxyTunnel() == false);
}


void HTTPSClientSessionTest::testStalledPeerTimeout()
{
	// DialogServer accepts the connection and, with no response queued, never writes a byte,
	// so the TLS handshake never receives a ServerHello. connectSSL() runs that handshake with
	// the socket timeout set to the connection timeout, so it has to fail there; leaving the
	// session looking connected makes the first request pay a receive timeout on top of it.
	DialogServer srv;
	const Poco::Timespan connectTimeout(1, 0);
	const Poco::Timespan requestTimeout(5, 0);

	HTTPSClientSession s("127.0.0.1", srv.port());
	s.setTimeout(connectTimeout, requestTimeout, requestTimeout);

	HTTPRequest request(HTTPRequest::HTTP_GET, "/");
	Poco::Timestamp tsStart;
	try
	{
		s.sendRequest(request);
		HTTPResponse response;
		s.receiveResponse(response);
		fail("a peer that never answers must not produce a response");
	}
	catch (Poco::TimeoutException&)
	{
	}
	assertTrue (tsStart.elapsed() < (connectTimeout + requestTimeout).totalMicroseconds());
}


void HTTPSClientSessionTest::testHttpsProxyTunnel()
{
	// The TLS session with the target runs inside the TLS session with the
	// proxy, which relays only what it decrypts: a handshake sent outside of
	// that session reaches nobody.
	HTTPSTestServer srv;
	TLSConnectProxy proxy(srv.port());
	const Poco::Timespan timeout(5, 0);

	HTTPSClientSession s("www.appinf.com", srv.port());
	s.setTimeout(timeout);
	s.setProxy("127.0.0.1", proxy.port(), "https", true);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/large");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPSTestServer::LARGE_BODY);
	assertTrue (proxy.connectRequest().find("CONNECT www.appinf.com:" + std::to_string(srv.port()) + " ") == 0);
	assertTrue (srv.lastRequest().find("GET /large ") == 0);
}


void HTTPSClientSessionTest::testHttpsProxyTunnelVerifiesTargetName()
{
	// The target is named with a loopback address literal and the peer address
	// is a loopback address as well, but it is the one of the proxy: the host
	// name of the certificate of the target is still checked.
	HTTPSTestServer srv;
	TLSConnectProxy proxy(srv.port());
	const Poco::Timespan timeout(5, 0);

	HTTPSClientSession s("127.0.0.1", srv.port());
	s.setTimeout(timeout);
	s.setProxy("127.0.0.1", proxy.port(), "https", true);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/large");
	try
	{
		s.sendRequest(request);
		fail("the certificate of the target is not issued for 127.0.0.1");
	}
	catch (CertificateValidationException&)
	{
	}
}


void HTTPSClientSessionTest::testHttpsProxyTunnelClosed()
{
	// The proxy closes the connection when the handshake with the target
	// begins. The handshake ends with the exception of the TLS connection
	// with the proxy, and does not wait for the timeout of the session.
	HTTPSTestServer srv;
	TLSConnectProxy proxy(srv.port(), TLSConnectProxy::TUNNEL_CLOSE);
	const Poco::Timespan timeout(5, 0);

	HTTPSClientSession s("www.appinf.com", srv.port());
	s.setTimeout(timeout);
	s.setProxy("127.0.0.1", proxy.port(), "https", true);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/large");
	Poco::Timestamp tsStart;
	try
	{
		s.sendRequest(request);
		fail("a closed tunnel must not accept a request");
	}
	catch (SSLConnectionUnexpectedlyClosedException&)
	{
	}
	assertTrue (tsStart.elapsed() < timeout.totalMicroseconds());
}


void HTTPSClientSessionTest::testHttpsProxyTunnelStalled()
{
	// The proxy relays nothing, so the handshake with the target ends
	// with the timeout of the session.
	HTTPSTestServer srv;
	TLSConnectProxy proxy(srv.port(), TLSConnectProxy::TUNNEL_STALL);
	const Poco::Timespan timeout(1, 0);
	const Poco::Timespan bound(5, 0);

	HTTPSClientSession s("www.appinf.com", srv.port());
	s.setTimeout(timeout);
	s.setProxy("127.0.0.1", proxy.port(), "https", true);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/large");
	Poco::Timestamp tsStart;
	try
	{
		s.sendRequest(request);
		fail("a stalled tunnel must not accept a request");
	}
	catch (Poco::TimeoutException&)
	{
	}
	assertTrue (tsStart.elapsed() >= timeout.totalMicroseconds());
	assertTrue (tsStart.elapsed() < bound.totalMicroseconds());
}


void HTTPSClientSessionTest::testHttpsProxyTunnelCorruptedRecord()
{
	// A record of the target that is corrupted in the tunnel is a TLS
	// error of the connection with the target, not the end of the body.
	const std::string body("corrupted in the tunnel");
	HeldBackHTTPSServer srv("HTTP/1.0 200 OK\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n", body);
	TLSConnectProxy proxy(srv.port(), TLSConnectProxy::TUNNEL_CORRUPT);
	const Poco::Timespan timeout(5, 0);

	HTTPSClientSession s("www.appinf.com", srv.port());
	s.setTimeout(timeout);
	s.setProxy("127.0.0.1", proxy.port(), "https", true);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/");
	s.sendRequest(request);
	srv.sendHeader();
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
	proxy.arm();
	srv.sendBody();
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str().empty());
	assertTrue (dynamic_cast<const SSLException*>(s.networkException()) != nullptr);
}


void HTTPSClientSessionTest::testHttpsProxyTunnelClosedInBody()
{
	// The proxy closes the TLS connection with the client with a
	// close_notify alert before the target has sent the whole body. The
	// TLS connection with the target ends without its close_notify alert,
	// which is an unexpected close, not the end of the body.
	const std::string body("first half");
	HeldBackHTTPSServer srv("HTTP/1.0 200 OK\r\nContent-Length: " + std::to_string(2*body.size()) + "\r\n\r\n", body);
	TLSConnectProxy proxy(srv.port(), TLSConnectProxy::TUNNEL_CUT);
	const Poco::Timespan timeout(5, 0);

	HTTPSClientSession s("www.appinf.com", srv.port());
	s.setTimeout(timeout);
	s.setProxy("127.0.0.1", proxy.port(), "https", true);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/");
	s.sendRequest(request);
	srv.sendHeader();
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
	proxy.arm();
	srv.sendBody();
	Poco::Timestamp tsStart;
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == body);
	assertTrue (dynamic_cast<const SSLConnectionUnexpectedlyClosedException*>(s.networkException()) != nullptr);
	assertTrue (tsStart.elapsed() < timeout.totalMicroseconds());
}


void HTTPSClientSessionTest::testHttpsProxyTunnelCloseEndsHandshake()
{
	// close() of another thread ends a handshake through a tunnel that
	// waits for the answer of the target: the TCP connection with the
	// proxy is shut down in both directions. A TLS shutdown of the
	// connection with the proxy would not end the wait, and close() would
	// wait for the receive timeout.
	//
	// HTTPSClientSession has the socket only once the handshake is complete,
	// so the tunnel is set up here.
	HTTPSTestServer srv;
	TLSConnectProxy proxy(srv.port(), TLSConnectProxy::TUNNEL_STALL);
	const Poco::Timespan timeout(30, 0);
	const Poco::Timespan bound(3, 0);

	SecureStreamSocket tunnel(SocketAddress("127.0.0.1", proxy.port()));
	tunnel.setReceiveTimeout(timeout);
	const std::string connectRequest("CONNECT 127.0.0.1:" + std::to_string(srv.port()) + " HTTP/1.1\r\n\r\n");
	tunnel.sendBytes(connectRequest.data(), static_cast<int>(connectRequest.size()));
	std::string answer;
	char buffer[256];
	while (answer.find("\r\n\r\n") == std::string::npos)
	{
		int n = tunnel.receiveBytes(buffer, sizeof(buffer));
		assertTrue (n > 0);
		answer.append(buffer, n);
	}
	assertTrue (answer.find("HTTP/1.1 200 ") == 0);

	// attach() does not start the handshake on a non-blocking socket
	tunnel.setBlocking(false);
	SecureStreamSocket ss = SecureStreamSocket::attach(tunnel, SSLManager::instance().defaultClientContext());
	ss.setBlocking(true);
	ss.setReceiveTimeout(timeout);

	Poco::Thread handshake;
	handshake.startFunc([&ss]
	{
		try
		{
			(void) ss.completeHandshake();
		}
		catch (Poco::Exception&)
		{
		}
	});
	// The client hello has arrived at the proxy: the handshake waits for the answer.
	const bool waiting = proxy.waitForTunnelData(static_cast<long>(bound.totalMilliseconds()));
	Poco::Timestamp tsStart;
	try
	{
		ss.close();
	}
	catch (...)
	{
		handshake.join();
		throw;
	}
	const Poco::Timestamp::TimeDiff closeTime = tsStart.elapsed();
	const bool ended = handshake.tryJoin(static_cast<long>(bound.totalMilliseconds()));
	// bounded by the receive timeout
	if (!ended) handshake.join();
	assertTrue (waiting);
	assertTrue (ended);
	assertTrue (closeTime < bound.totalMicroseconds());
}


void HTTPSClientSessionTest::setUp()
{
}


void HTTPSClientSessionTest::tearDown()
{
}


CppUnit::Test* HTTPSClientSessionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPSClientSessionTest");

	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testFromSocket);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testGetSmall);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testSourceAddress);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testGetLarge);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testHead);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testPostSmallIdentity);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testPostLargeIdentity);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testPostSmallChunked);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testPostLargeChunked);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testPostLargeChunkedKeepAlive);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testKeepAlive);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testMultipleSSLInit);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testInterop);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testProxy);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testCachedSession);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testUnknownContentLength);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testServerAbort);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testProxyConfig);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testProxySetters);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testStalledPeerTimeout);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testHttpsProxyTunnel);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testHttpsProxyTunnelVerifiesTargetName);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testHttpsProxyTunnelClosed);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testHttpsProxyTunnelStalled);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testHttpsProxyTunnelCorruptedRecord);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testHttpsProxyTunnelClosedInBody);
	CppUnit_addTest(pSuite, HTTPSClientSessionTest, testHttpsProxyTunnelCloseEndsHandshake);

	return pSuite;
}
