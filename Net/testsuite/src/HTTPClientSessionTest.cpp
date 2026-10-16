//
// HTTPClientSessionTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPClientSessionTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPBufferAllocator.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/StreamCopier.h"
#include "Poco/Stopwatch.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/Buffer.h"
#include "HTTPTestServer.h"
#include <future>
#include <istream>
#include <ostream>
#include <sstream>


using Poco::Net::HTTPBufferAllocator;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSession;
using Poco::Net::ProxyConfig;
using Poco::Net::ProxyAuthentication;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::MessageException;
using Poco::Net::ServerSocket;
using Poco::Net::Socket;
using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;
using Poco::StreamCopier;
using Poco::Stopwatch;
using Poco::InvalidArgumentException;
using Poco::IllegalStateException;
using Poco::File;
using Poco::Path;


namespace
{
	class TunnelClientSession: public HTTPClientSession
		/// Gives the tests access to proxyTunnel().
	{
	public:
		using HTTPClientSession::HTTPClientSession;
		using HTTPClientSession::proxyTunnel;
	};

	class KeepAliveRequestHandler: public HTTPRequestHandler
		/// Answers HEAD /keepAlive, GET /small, GET /large and HEAD /large;
		/// the last one closes the connection.
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override
		{
			const bool head = request.getMethod() == HTTPRequest::HTTP_HEAD;
			if (head && request.getURI() == "/keepAlive")
			{
				response.setContentType("text/plain");
				response.setContentLength(static_cast<std::streamsize>(HTTPTestServer::SMALL_BODY.size()));
				response.send();
			}
			else if (!head && request.getURI() == "/small")
			{
				response.sendBuffer(HTTPTestServer::SMALL_BODY.data(), HTTPTestServer::SMALL_BODY.size());
			}
			else if (!head && request.getURI() == "/large")
			{
				response.setChunkedTransferEncoding(true);
				response.send() << HTTPTestServer::LARGE_BODY;
			}
			else if (head && request.getURI() == "/large")
			{
				response.setContentType("text/plain");
				response.setContentLength(static_cast<std::streamsize>(HTTPTestServer::LARGE_BODY.size()));
				response.setKeepAlive(false);
				response.send();
			}
		}
	};

	class KeepAliveRequestHandlerFactory: public HTTPRequestHandlerFactory
	{
	public:
		HTTPRequestHandler* createRequestHandler(const HTTPServerRequest&) override
		{
			return new KeepAliveRequestHandler;
		}
	};

	class BufferedSession: public HTTPClientSession
		/// Gives the test access to the buffered read of HTTPSession.
	{
	public:
		explicit BufferedSession(const StreamSocket& socket):
			HTTPClientSession(socket)
		{
		}

		int nextChar()
		{
			return get();
		}
	};

	class FailedSendSession: public HTTPClientSession
		/// Lets the test record a send error as HTTPSession::write() does.
		/// Every send after failSend() fails.
	{
	public:
		FailedSendSession(const std::string& host, Poco::UInt16 port):
			HTTPClientSession(host, port)
		{
		}

		void failSend()
		{
			setException(Poco::TimeoutException("Send timed out"));
			_failed = true;
		}

	protected:
		int write(const char* buffer, std::streamsize length) override
		{
			if (_failed) throw Poco::TimeoutException("Send timed out");
			return HTTPClientSession::write(buffer, length);
		}

	private:
		bool _failed = false;
	};

	class ShortSendSession: public HTTPClientSession
		/// Sends all but the last 5 bytes of the first write, as a send
		/// on a non-blocking socket with a full send buffer does.
	{
	public:
		ShortSendSession(const std::string& host, Poco::UInt16 port):
			HTTPClientSession(host, port)
		{
		}

		int shortWriteLength() const
		{
			return _shortWriteLength;
		}

	protected:
		int write(const char* buffer, std::streamsize length) override
		{
			if (_shortWriteLength > 0) return HTTPClientSession::write(buffer, length);
			_shortWriteLength = HTTPClientSession::write(buffer, length - 5);
			return _shortWriteLength;
		}

	private:
		int _shortWriteLength = 0;
	};
}


HTTPClientSessionTest::HTTPClientSessionTest(const std::string& name): CppUnit::TestCase(name)
{
}


HTTPClientSessionTest::~HTTPClientSessionTest()
{
}


void HTTPClientSessionTest::testGetSmall()
{
	HTTPTestServer srv;
	HTTPClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/small");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPTestServer::SMALL_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPTestServer::SMALL_BODY);
}


void HTTPClientSessionTest::testGetSmallUnix()
{
#if defined(POCO_HAS_UNIX_SOCKET)
#if POCO_OS == POCO_OS_ANDROID
	File socketFile("/data/local/tmp/SocketTest.sock");
#elif defined(POCO_OS_FAMILY_WINDOWS)
	File socketFile(Path::tempHome() + "SocketTest.sock");
#else
	File socketFile("/tmp/SocketTest.sock");
#endif // POCO_OS == POCO_OS_ANDROID
	if (socketFile.exists()) socketFile.remove();
	HTTPTestServer srv(socketFile.path());
	HTTPClientSession s(socketFile.path());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/small");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue(response.getContentLength() == HTTPTestServer::SMALL_BODY.length());
	assertTrue(response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue(ostr.str() == HTTPTestServer::SMALL_BODY);
#endif // POCO_HAS_UNIX_SOCKET
}


void HTTPClientSessionTest::testGetLarge()
{
	HTTPTestServer srv;
	HTTPClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/large");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPTestServer::LARGE_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPTestServer::LARGE_BODY);
}


void HTTPClientSessionTest::testHead()
{
	HTTPTestServer srv;
	HTTPClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_HEAD, "/large");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPTestServer::LARGE_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	assertTrue (StreamCopier::copyStream(rs, ostr) == 0);
}


void HTTPClientSessionTest::testPostSmallIdentity()
{
	HTTPTestServer srv;
	HTTPClientSession s("127.0.0.1", srv.port());
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


void HTTPClientSessionTest::testPostLargeIdentity()
{
	HTTPTestServer srv;
	HTTPClientSession s("127.0.0.1", srv.port());
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


void HTTPClientSessionTest::testPostSmallChunked()
{
	HTTPTestServer srv;
	HTTPClientSession s("127.0.0.1", srv.port());
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


void HTTPClientSessionTest::testPostLargeChunked()
{
	HTTPTestServer srv;
	HTTPClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_POST, "/echo");
	std::string body(16000, 'x');
	request.setChunkedTransferEncoding(true);
	std::ostream& os = s.sendRequest(request);
	os << body;
	os.flush();
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getChunkedTransferEncoding());
	assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr, 16000);
	assertTrue (ostr.str() == body);
}


void HTTPClientSessionTest::testKeepAlive()
{
	ServerSocket svs(SocketAddress("127.0.0.1", 0));
	HTTPServer srv(new KeepAliveRequestHandlerFactory, svs, new HTTPServerParams);
	srv.start();
	try
	{
		HTTPClientSession s("127.0.0.1", srv.port());
		s.setKeepAlive(true);
		HTTPRequest request(HTTPRequest::HTTP_HEAD, "/keepAlive", HTTPMessage::HTTP_1_1);
		s.sendRequest(request);
		HTTPResponse response;
		std::istream& rs1 = s.receiveResponse(response);
		assertTrue (response.getContentLength() == HTTPTestServer::SMALL_BODY.length());
		assertTrue (response.getContentType() == "text/plain");
		assertTrue (response.getKeepAlive());
		std::ostringstream ostr1;
		assertTrue (StreamCopier::copyStream(rs1, ostr1) == 0);

		request.setMethod(HTTPRequest::HTTP_GET);
		request.setURI("/small");
		s.sendRequest(request);
		std::istream& rs2 = s.receiveResponse(response);
		assertTrue (response.getContentLength() == HTTPTestServer::SMALL_BODY.length());
		assertTrue (response.getKeepAlive());
		std::ostringstream ostr2;
		StreamCopier::copyStream(rs2, ostr2);
		assertTrue (ostr2.str() == HTTPTestServer::SMALL_BODY);

		request.setMethod(HTTPRequest::HTTP_GET);
		request.setURI("/large");
		s.sendRequest(request);
		std::istream& rs3 = s.receiveResponse(response);
		assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
		assertTrue (response.getChunkedTransferEncoding());
		assertTrue (response.getKeepAlive());
		std::ostringstream ostr3;
		StreamCopier::copyStream(rs3, ostr3);
		assertTrue (ostr3.str() == HTTPTestServer::LARGE_BODY);

		request.setMethod(HTTPRequest::HTTP_HEAD);
		request.setURI("/large");
		s.sendRequest(request);
		std::istream& rs4 = s.receiveResponse(response);
		assertTrue (response.getContentLength() == HTTPTestServer::LARGE_BODY.length());
		assertTrue (response.getContentType() == "text/plain");
		assertTrue (!response.getKeepAlive());
		std::ostringstream ostr4;
		assertTrue (StreamCopier::copyStream(rs4, ostr4) == 0);
		assertTrue (srv.totalConnections() == 1);
		srv.stop();
	}
	catch (...)
	{
		srv.stop();
		throw;
	}
}


void HTTPClientSessionTest::testTrailer()
{
	HTTPTestServer srv;
	HTTPClientSession s("127.0.0.1", srv.port());
	s.setKeepAlive(true);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/trailer", HTTPMessage::HTTP_1_1);
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPTestServer::LARGE_BODY);
	assertTrue (!s.responseTrailer().empty());
	assertTrue (s.responseTrailer().get("Trailer-1") == "Value 1");
	assertTrue (s.responseTrailer().get("Trailer-2") == "Value 2");
}


void HTTPClientSessionTest::testProxy()
{
	HTTPTestServer srv;
	HTTPClientSession s("www.somehost.com");
	s.setProxy("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/large");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPTestServer::LARGE_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPTestServer::LARGE_BODY);
}


void HTTPClientSessionTest::testProxyAuth()
{
	HTTPTestServer srv;
	HTTPClientSession s("www.somehost.com");
	s.setProxy("127.0.0.1", srv.port());
	s.setProxyCredentials("user", "pass");
	HTTPRequest request(HTTPRequest::HTTP_GET, "/large");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPTestServer::LARGE_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPTestServer::LARGE_BODY);
	std::string r = srv.lastRequest();
	assertTrue (r.find("Proxy-Authorization: Basic dXNlcjpwYXNz\r\n") != std::string::npos);
}


void HTTPClientSessionTest::testBypassProxy()
{
	ProxyConfig proxyConfig;
	proxyConfig.host = "proxy.domain.com";
	proxyConfig.port = 80;
	proxyConfig.nonProxyHosts = "localhost|127\\.0\\.0\\.1";

	HTTPClientSession s1("127.0.0.1", 80);
	s1.setProxyConfig(proxyConfig);
	assertTrue (s1.bypassProxy());

	HTTPClientSession s2("127.0.0.1", 80);
	s2.setProxyConfig(proxyConfig);
	assertTrue (s2.bypassProxy());

	HTTPClientSession s3("www.appinf.com", 80);
	s3.setProxyConfig(proxyConfig);
	assertTrue (!s3.bypassProxy());
}


void HTTPClientSessionTest::testExpectContinue()
{
	HTTPTestServer srv;
	HTTPClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_POST, "/expect");
	std::string body("this is a random request body\r\n0\r\n");
	request.setContentLength((int) body.length());
	request.setExpectContinue(true);
	s.sendRequest(request) << body;
	HTTPResponse response;
	assertTrue (s.peekResponse(response));
	assertTrue (response.getStatus() == HTTPResponse::HTTP_CONTINUE);
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
	assertTrue (response.getContentLength() == body.length());
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == body);
}


void HTTPClientSessionTest::testExpectContinueFail()
{
	HTTPTestServer srv;
	HTTPClientSession s("127.0.0.1", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_POST, "/fail");
	std::string body("this is a random request body\r\n0\r\n");
	request.setContentLength((int) body.length());
	request.setExpectContinue(true);
	s.sendRequest(request) << body;
	HTTPResponse response;
	assertTrue (!s.peekResponse(response));
	assertTrue (response.getStatus() == HTTPResponse::HTTP_BAD_REQUEST);
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_BAD_REQUEST);
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str().empty());
}


void HTTPClientSessionTest::testProxyConfig()
{
	ProxyConfig config;
	assertTrue (config.host.empty());
	assertTrue (config.port == HTTPSession::HTTP_PORT);
	assertTrue (config.protocol == "http");
	assertTrue (config.tunnel == true);
	assertTrue (config.username.empty());
	assertTrue (config.password.empty());
	assertTrue (config.nonProxyHosts.empty());
	assertTrue (config.authMethod == ProxyAuthentication::Basic);
}


void HTTPClientSessionTest::testSetProxyProtocolValidation()
{
	HTTPClientSession s("www.example.com");

	try
	{
		s.setProxy("proxy", 80, "ftp", true);
		fail("must throw InvalidArgumentException");
	}
	catch (InvalidArgumentException&)
	{
	}

	try
	{
		s.setProxy("proxy", 80, "", true);
		fail("must throw InvalidArgumentException");
	}
	catch (InvalidArgumentException&)
	{
	}

	try
	{
		s.setProxyProtocol("socks");
		fail("must throw InvalidArgumentException");
	}
	catch (InvalidArgumentException&)
	{
	}

	s.setProxy("proxy", 80, "http", true);
	assertTrue (s.getProxyProtocol() == "http");

	// HTTPS proxy is only supported by HTTPSClientSession; plain
	// HTTPClientSession must reject protocol "https".
	try
	{
		s.setProxy("proxy", 443, "https", true);
		fail("must throw InvalidArgumentException");
	}
	catch (InvalidArgumentException&)
	{
	}

	try
	{
		s.setProxyProtocol("https");
		fail("must throw InvalidArgumentException");
	}
	catch (InvalidArgumentException&)
	{
	}
}


void HTTPClientSessionTest::testSetProxyConfigValidation()
{
	HTTPClientSession s("www.example.com");

	ProxyConfig badConfig;
	badConfig.host = "proxy";
	badConfig.protocol = "ftp";

	try
	{
		s.setProxyConfig(badConfig);
		fail("must throw InvalidArgumentException");
	}
	catch (InvalidArgumentException&)
	{
	}

	ProxyConfig goodConfig;
	goodConfig.host = "proxy";
	goodConfig.protocol = "http";
	s.setProxyConfig(goodConfig);
	assertTrue (s.getProxyHost() == "proxy");

	// HTTPS proxy is only supported by HTTPSClientSession; plain
	// HTTPClientSession must reject a ProxyConfig with protocol "https".
	ProxyConfig httpsConfig;
	httpsConfig.host = "proxy";
	httpsConfig.port = 443;
	httpsConfig.protocol = "https";
	try
	{
		s.setProxyConfig(httpsConfig);
		fail("must throw InvalidArgumentException");
	}
	catch (InvalidArgumentException&)
	{
	}
}


void HTTPClientSessionTest::testProxySetters()
{
	HTTPClientSession s("www.example.com");

	s.setProxyHost("proxy.example.com");
	assertTrue (s.getProxyHost() == "proxy.example.com");

	s.setProxyPort(8080);
	assertTrue (s.getProxyPort() == 8080);

	s.setProxyProtocol("http");
	assertTrue (s.getProxyProtocol() == "http");

	s.setProxyTunnel(false);
	assertTrue (s.isProxyTunnel() == false);

	s.setProxyCredentials("user", "pass");
	assertTrue (s.getProxyUsername() == "user");
	assertTrue (s.getProxyPassword() == "pass");

	ProxyConfig config;
	config.host = "other-proxy.com";
	config.port = 3128;
	config.protocol = "http";
	config.tunnel = true;
	config.username = "admin";
	config.password = "secret";
	s.setProxyConfig(config);
	assertTrue (s.getProxyHost() == "other-proxy.com");
	assertTrue (s.getProxyPort() == 3128);
	assertTrue (s.getProxyProtocol() == "http");
	assertTrue (s.isProxyTunnel() == true);
	assertTrue (s.getProxyUsername() == "admin");
	assertTrue (s.getProxyPassword() == "secret");
}


void HTTPClientSessionTest::testProxyRequestPrefix()
{
	HTTPTestServer srv;

	HTTPClientSession s1("www.somehost.com", 80);
	s1.setProxy("127.0.0.1", srv.port());
	HTTPRequest request1(HTTPRequest::HTTP_GET, "/large");
	s1.sendRequest(request1);
	HTTPResponse response1;
	std::istream& rs1 = s1.receiveResponse(response1);
	std::ostringstream ostr1;
	StreamCopier::copyStream(rs1, ostr1);
	std::string r1 = srv.lastRequest();
	// Port 80 should NOT appear in the prefix
	assertTrue (r1.find("GET http://www.somehost.com/large") != std::string::npos);
	assertTrue (r1.find(":80") == std::string::npos);

	HTTPTestServer srv2;
	HTTPClientSession s2("www.somehost.com", 8080);
	s2.setProxy("127.0.0.1", srv2.port());
	HTTPRequest request2(HTTPRequest::HTTP_GET, "/large");
	s2.sendRequest(request2);
	HTTPResponse response2;
	std::istream& rs2 = s2.receiveResponse(response2);
	std::ostringstream ostr2;
	StreamCopier::copyStream(rs2, ostr2);
	std::string r2 = srv2.lastRequest();
	// Non-default port should appear in the prefix
	assertTrue (r2.find("GET http://www.somehost.com:8080/large") != std::string::npos);
}


void HTTPClientSessionTest::testProxyNonTunnel()
{
	HTTPTestServer srv;
	HTTPClientSession s("www.somehost.com");
	s.setProxy("127.0.0.1", srv.port(), "http", false);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/large");
	s.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPTestServer::LARGE_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == HTTPTestServer::LARGE_BODY);
	std::string r = srv.lastRequest();
	assertTrue (r.find("GET http://www.somehost.com/large") != std::string::npos);
}


void HTTPClientSessionTest::testGlobalProxyConfig()
{
	ProxyConfig globalConfig;
	globalConfig.host = "global-proxy.example.com";
	globalConfig.port = 3128;
	globalConfig.protocol = "http";
	HTTPClientSession::setGlobalProxyConfig(globalConfig);

	HTTPClientSession s1("www.example.com");
	assertTrue (s1.getProxyHost() == "global-proxy.example.com");
	assertTrue (s1.getProxyPort() == 3128);

	// Per-session config overrides global
	ProxyConfig localConfig;
	localConfig.host = "local-proxy.example.com";
	localConfig.port = 8080;
	localConfig.protocol = "http";
	s1.setProxyConfig(localConfig);
	assertTrue (s1.getProxyHost() == "local-proxy.example.com");
	assertTrue (s1.getProxyPort() == 8080);

	// Reset global config
	ProxyConfig emptyConfig;
	HTTPClientSession::setGlobalProxyConfig(emptyConfig);

	HTTPClientSession s2("www.example.com");
	assertTrue (s2.getProxyHost().empty());
}


void HTTPClientSessionTest::testBypassProxyExtended()
{
	ProxyConfig config;
	config.host = "proxy.domain.com";
	config.port = 80;
	config.protocol = "http";
	config.tunnel = false;
	config.nonProxyHosts = "localhost|127\\.0\\.0\\.1";

	HTTPClientSession s1("127.0.0.1", 80);
	s1.setProxyConfig(config);
	assertTrue (s1.bypassProxy());

	HTTPClientSession s2("www.appinf.com", 80);
	s2.setProxyConfig(config);
	assertTrue (!s2.bypassProxy());

	// Empty nonProxyHosts disables bypass
	config.nonProxyHosts = "";
	HTTPClientSession s3("127.0.0.1", 80);
	s3.setProxyConfig(config);
	assertTrue (!s3.bypassProxy());
}


void HTTPClientSessionTest::testChunkedFramingErrorReconnects()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	StreamSocket first = server.acceptConnection();
	const std::string malformed(
		"HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
		"5x\r\n"
		"HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nstale");
	first.sendBytes(malformed.data(), static_cast<int>(malformed.size()));
	HTTPResponse response;
	std::istream& rs1 = cs.receiveResponse(response);
	std::string body;
	StreamCopier::copyToString(rs1, body);
	assertTrue (rs1.bad());

	assertNewConnectionServes(cs, server, request);
}


void HTTPClientSessionTest::testInvalidRequestKeepsConnection()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	StreamSocket connection = server.acceptConnection();
	connection.setReceiveTimeout(Poco::Timespan(10, 0));
	const std::string valid("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nfresh");
	connection.sendBytes(valid.data(), static_cast<int>(valid.size()));
	HTTPResponse response;
	std::istream& rs = cs.receiveResponse(response);
	std::string body;
	StreamCopier::copyToString(rs, body);
	char buffer[1024];
	assertTrue (connection.receiveBytes(buffer, sizeof(buffer)) > 0);

	HTTPRequest invalid(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	invalid.set("X-Field", "a\r\nX-Injected: 1");
	try
	{
		cs.sendRequest(invalid);
		fail("invalid request must throw");
	}
	catch (MessageException&)
	{
	}
	assertTrue (cs.connected());

	cs.sendRequest(request).flush();
	const int n = connection.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n).compare(0, 15, "GET / HTTP/1.1\r") == 0);
}


void HTTPClientSessionTest::assertNewConnectionServes(HTTPClientSession& session, ServerSocket& server, HTTPRequest& request)
{
	session.sendRequest(request);
	assertTrue (server.poll(Poco::Timespan(5, 0), Socket::SELECT_READ));
	StreamSocket connection = server.acceptConnection();
	const std::string valid("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nfresh");
	connection.sendBytes(valid.data(), static_cast<int>(valid.size()));
	HTTPResponse response;
	std::istream& rs = session.receiveResponse(response);
	std::string body;
	StreamCopier::copyToString(rs, body);
	assertTrue (body == "fresh");
}


std::string HTTPClientSessionTest::receiveHeader(StreamSocket& peer)
{
	peer.setReceiveTimeout(Poco::Timespan(5, 0));
	std::string received;
	char buffer[1024];
	try
	{
		while (received.find("\r\n\r\n") == std::string::npos)
		{
			const int n = peer.receiveBytes(buffer, sizeof(buffer));
			if (n <= 0) break;
			received.append(buffer, static_cast<std::size_t>(n));
		}
	}
	catch (Poco::TimeoutException&)
	{
		failmsg("the client did not send a request header");
	}
	return received;
}


void HTTPClientSessionTest::testIncompleteFixedLengthResponse()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	StreamSocket ss = server.acceptConnection();
	const std::string truncated("HTTP/1.1 200 OK\r\nContent-Length: 100\r\n\r\n0123456789");
	ss.sendBytes(truncated.data(), static_cast<int>(truncated.size()));
	ss.shutdownSend();
	HTTPResponse response;
	std::istream& rs = cs.receiveResponse(response);
	std::ostringstream ostr;
	assertTrue (StreamCopier::copyStream(rs, ostr) == 10);
	assertTrue (ostr.str() == "0123456789");
	assertTrue (dynamic_cast<const MessageException*>(cs.networkException()) != nullptr);
	assertTrue (!cs.responseComplete());
}


void HTTPClientSessionTest::testCompleteFixedLengthResponse()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	assertTrue (!cs.responseComplete());
	StreamSocket ss = server.acceptConnection();
	const std::string complete("HTTP/1.1 200 OK\r\nContent-Length: 10\r\n\r\n0123456789");
	ss.sendBytes(complete.data(), static_cast<int>(complete.size()));
	HTTPResponse response;
	std::istream& rs = cs.receiveResponse(response);
	std::ostringstream ostr;
	assertTrue (StreamCopier::copyStream(rs, ostr) == 10);
	assertTrue (ostr.str() == "0123456789");
	assertTrue (cs.networkException() == nullptr);
	assertTrue (cs.responseComplete());
}


void HTTPClientSessionTest::testUnreadFixedLengthResponseReconnects()
{
	// The last byte of the body is not sent, so the client cannot skip the rest.
	const std::string embedded("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nstale");
	assertReconnectAfterPartialRead("HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(embedded.size() + 1) + "\r\n\r\n" + embedded, 0);
}


void HTTPClientSessionTest::testUnreadBufferedFixedLengthResponseKeepsConnection()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	StreamSocket ss = server.acceptConnection();
	// The body is sent with the header, so it is in the session buffer when the header has been read.
	const std::string small("HTTP/1.1 404 Not Found\r\nContent-Length: 5\r\n\r\nstale");
	ss.sendBytes(small.data(), static_cast<int>(small.size()));
	HTTPResponse response;
	cs.receiveResponse(response);
	assertTrue (!cs.responseComplete());

	cs.sendRequest(request);
	assertTrue (!server.poll(Poco::Timespan(), Socket::SELECT_READ));
	const std::string valid("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nfresh");
	ss.sendBytes(valid.data(), static_cast<int>(valid.size()));
	std::istream& rs = cs.receiveResponse(response);
	std::string body;
	StreamCopier::copyToString(rs, body);
	assertTrue (body == "fresh");
}


void HTTPClientSessionTest::testStrayResponseAfterBodyReconnects()
{
	enum class StrayResponse
	{
		SAME_SEGMENT_BODY_UNREAD,
		SAME_SEGMENT_BODY_READ,
		SEPARATE_AFTER_BODY_READ
	};
	const std::string stale("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nstale");
	const std::string bogus("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nbogus");
	for (StrayResponse stray: {StrayResponse::SAME_SEGMENT_BODY_UNREAD, StrayResponse::SAME_SEGMENT_BODY_READ, StrayResponse::SEPARATE_AFTER_BODY_READ})
	{
		ServerSocket server(SocketAddress("127.0.0.1", 0));
		HTTPClientSession cs("127.0.0.1", server.address().port());
		cs.setKeepAlive(true);
		cs.setTimeout(Poco::Timespan(10, 0));
		HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
		cs.sendRequest(request).flush();
		StreamSocket first = server.acceptConnection();
		receiveHeader(first);
		// A second response without a request follows the body, in the same segment or after the body was read.
		const std::string responses = stray == StrayResponse::SEPARATE_AFTER_BODY_READ ? stale : stale + bogus;
		first.sendBytes(responses.data(), static_cast<int>(responses.size()));
		HTTPResponse response;
		std::istream& rs = cs.receiveResponse(response);
		if (stray != StrayResponse::SAME_SEGMENT_BODY_UNREAD)
		{
			std::string body;
			StreamCopier::copyToString(rs, body);
			assertTrue (body == "stale");
		}
		if (stray == StrayResponse::SEPARATE_AFTER_BODY_READ)
		{
			first.sendBytes(bogus.data(), static_cast<int>(bogus.size()));
			// The stray response is in the socket, not in the session buffer.
			assertTrue (cs.socket().poll(Poco::Timespan(5, 0), Socket::SELECT_READ));
		}

		assertNewConnectionServes(cs, server, request);
	}
}


void HTTPClientSessionTest::testServerCloseAfterResponseReconnects()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request).flush();
	StreamSocket first = server.acceptConnection();
	// Read before closing: closing a socket with unread data resets the connection instead of closing it.
	receiveHeader(first);
	const std::string valid("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nstale");
	first.sendBytes(valid.data(), static_cast<int>(valid.size()));
	HTTPResponse response;
	std::istream& rs = cs.receiveResponse(response);
	std::string body;
	StreamCopier::copyToString(rs, body);
	assertTrue (body == "stale");
	first.close();
	assertTrue (cs.socket().poll(Poco::Timespan(5, 0), Socket::SELECT_READ));

	assertNewConnectionServes(cs, server, request);
}


void HTTPClientSessionTest::testUnreadChunkedResponseReconnects()
{
	// The server has not sent the next chunk yet; the client must not wait for it.
	assertReconnectAfterPartialRead("HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\n", 5);
}


void HTTPClientSessionTest::testMalformedUnreadChunkReconnects()
{
	assertReconnectAfterPartialRead("HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\nzz\r\n", 5);
}


void HTTPClientSessionTest::testUnreadCloseDelimitedResponseReconnects()
{
	assertReconnectAfterPartialRead("HTTP/1.1 200 OK\r\n\r\nhello", 0);
}


void HTTPClientSessionTest::testUnreadChunkedTerminatorKeepsConnection()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	StreamSocket ss = server.acceptConnection();
	const std::string chunked("HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\n0\r\nTrailer-1: Value 1\r\n\r\n");
	ss.sendBytes(chunked.data(), static_cast<int>(chunked.size()));
	HTTPResponse response;
	std::istream& rs1 = cs.receiveResponse(response);
	char buffer[5];
	rs1.read(buffer, sizeof(buffer));
	assertTrue (std::string(buffer, static_cast<std::size_t>(rs1.gcount())) == "hello");
	assertTrue (!cs.responseComplete());

	cs.sendRequest(request);
	assertTrue (!server.poll(Poco::Timespan(), Socket::SELECT_READ));
	assertTrue (cs.responseTrailer().get("Trailer-1", "") == "Value 1");
	const std::string valid("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nfresh");
	ss.sendBytes(valid.data(), static_cast<int>(valid.size()));
	std::istream& rs2 = cs.receiveResponse(response);
	std::string body;
	StreamCopier::copyToString(rs2, body);
	assertTrue (body == "fresh");
}


void HTTPClientSessionTest::testProxyTunnelIgnoresPreviousResponse()
{
	ServerSocket proxy(SocketAddress("127.0.0.1", 0));
	TunnelClientSession cs("www.somehost.com", 80);
	cs.setProxy("127.0.0.1", proxy.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request).flush();
	StreamSocket first = proxy.acceptConnection();
	// Read before closing: on Windows, closing a socket with unread data resets the connection.
	receiveHeader(first);
	// A close-delimited body and Connection: close; neither may close the next tunnel.
	const std::string closing("HTTP/1.1 401 Unauthorized\r\nConnection: close\r\n\r\nno");
	first.sendBytes(closing.data(), static_cast<int>(closing.size()));
	first.close();
	HTTPResponse response;
	std::istream& rs = cs.receiveResponse(response);
	std::string body;
	StreamCopier::copyToString(rs, body);
	assertTrue (body == "no");

	cs.reset();
	auto accepted = std::async(std::launch::async, [this, &proxy]()
	{
		StreamSocket connection;
		if (proxy.poll(Poco::Timespan(5, 0), Socket::SELECT_READ))
		{
			connection = proxy.acceptConnection();
			receiveHeader(connection);
			const std::string established("HTTP/1.1 200 Connection established\r\n\r\n");
			connection.sendBytes(established.data(), static_cast<int>(established.size()));
		}
		return connection;
	});
	cs.proxyTunnel();
	StreamSocket tunnel = accepted.get();

	cs.sendRequest(request).flush();
	assertTrue (!proxy.poll(Poco::Timespan(), Socket::SELECT_READ));
	char buffer[1024];
	const int n = tunnel.receiveBytes(buffer, sizeof(buffer));
	assertTrue (n > 0);
	assertTrue (std::string(buffer, n).compare(0, 4, "GET ") == 0);
}


void HTTPClientSessionTest::testMalformedContentLengthResponse()
{
	assertFramingRejected("Content-Length: 1,0\r\n");
	assertFramingRejected("Content-Length: -5\r\n");
	assertFramingRejected("Content-Length: +10\r\n");
	assertFramingRejected("Content-Length: \r\n");
	assertFramingRejected("Content-Length: 99999999999999999999\r\n");
}


void HTTPClientSessionTest::testDuplicateContentLengthResponse()
{
	assertFramingRejected("Content-Length: 0\r\nContent-Length: 40\r\n");
}


void HTTPClientSessionTest::testDuplicateTransferEncodingResponse()
{
	assertFramingRejected("Transfer-Encoding: chunked\r\nTransfer-Encoding: chunked\r\n");
	assertFramingRejected("transfer-encoding: chunked\r\nTRANSFER-ENCODING: gzip\r\n");
}


void HTTPClientSessionTest::testContentLengthAndTransferEncodingResponse()
{
	assertFramingRejected("Content-Length: 0\r\nTransfer-Encoding: chunked\r\n");
}


void HTTPClientSessionTest::testNonChunkedTransferEncodingResponseReadsToClose()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	StreamSocket first = server.acceptConnection();
	const std::string gzipped("HTTP/1.1 200 OK\r\nTransfer-Encoding: gzip\r\n\r\nHTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nstale");
	first.sendBytes(gzipped.data(), static_cast<int>(gzipped.size()));
	first.shutdownSend();
	HTTPResponse response;
	std::istream& rs1 = cs.receiveResponse(response);
	std::string body;
	StreamCopier::copyToString(rs1, body);
	assertTrue (body == "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nstale");

	assertNewConnectionServes(cs, server, request);
}


void HTTPClientSessionTest::testHeadResponseIgnoresFramingHeaders()
{
	assertFramingIgnored(HTTPRequest::HTTP_HEAD, "/");
}


void HTTPClientSessionTest::testConnectResponseIgnoresFramingHeaders()
{
	assertFramingIgnored(HTTPRequest::HTTP_CONNECT, "www.somehost.com:80");
}


void HTTPClientSessionTest::testTimeoutAppliedToConnectedSocket()
{
	HTTPTestServer srv;
	HTTPClientSession s("127.0.0.1", srv.port());
	s.setKeepAlive(true);
	HTTPRequest request(HTTPRequest::HTTP_HEAD, "/keepAlive", HTTPMessage::HTTP_1_1);
	s.sendRequest(request);
	HTTPResponse response;
	s.receiveResponse(response);
	assertTrue (response.getKeepAlive());
	assertTrue (s.connected());

	s.setTimeout(Poco::Timespan(3, 0), Poco::Timespan(4, 0), Poco::Timespan(5, 0));
	assertTrue (s.socket().getSendTimeout() == Poco::Timespan(4, 0));
	assertTrue (s.socket().getReceiveTimeout() == Poco::Timespan(5, 0));

	s.setSendTimeout(Poco::Timespan(6, 0));
	s.setReceiveTimeout(Poco::Timespan(7, 0));
	assertTrue (s.socket().getSendTimeout() == Poco::Timespan(6, 0));
	assertTrue (s.socket().getReceiveTimeout() == Poco::Timespan(7, 0));

	// POSIX systems reject a negative microsecond part with EDOM; Windows takes whole milliseconds and accepts it.
	const Poco::Timespan invalidTimeout(0, -1);
	bool sendRejected = false;
	try
	{
		s.setSendTimeout(invalidTimeout);
	}
	catch (Poco::IOException&)
	{
		sendRejected = true;
	}
	bool rejected = false;
	try
	{
		s.setTimeout(Poco::Timespan(3, 0), invalidTimeout, Poco::Timespan(8, 0));
	}
	catch (Poco::IOException&)
	{
		rejected = true;
	}
	assertTrue (rejected == sendRejected);
	assertTrue (s.socket().getReceiveTimeout() == Poco::Timespan(8, 0));

	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket socket(SocketAddress("127.0.0.1", server.address().port()));
	socket.setSendTimeout(Poco::Timespan(10, 0));
	socket.setReceiveTimeout(Poco::Timespan(10, 0));
	HTTPClientSession adopted(socket);
	adopted.setTimeout(Poco::Timespan(60, 0));
	assertTrue (adopted.socket().getSendTimeout() == Poco::Timespan(60, 0));
	assertTrue (adopted.socket().getReceiveTimeout() == Poco::Timespan(60, 0));
}


void HTTPClientSessionTest::testNonBlockingReceiveWithoutData()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket client(server.address());
	StreamSocket peer = server.acceptConnection();
	client.setBlocking(false);
	BufferedSession session(client);
	assertTrue (session.nextChar() == std::char_traits<char>::eof());
	Poco::Buffer<char> buffer(0);
	session.drainBuffer(buffer);
	assertTrue (buffer.size() == 0);

	peer.sendBytes("abc", 3);
	assertTrue (client.poll(Poco::Timespan(5, 0), Socket::SELECT_READ));
	assertTrue (session.nextChar() == 'a');
}


void HTTPClientSessionTest::testFailedBodySendReconnects()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(1, 0));
	cs.setKeepAliveTimeout(Poco::Timespan(30, 0));
	StreamSocket first = sendBodyUntilFailure(cs, server);
	HTTPResponse response;
	try
	{
		cs.receiveResponse(response);
		failmsg("must throw TimeoutException");
	}
	catch (Poco::TimeoutException&)
	{
	}
	assertTrue (!cs.connected());
	assertBodyOnlyUntilClosed(first);

	HTTPRequest request(HTTPRequest::HTTP_GET, "/second", HTTPMessage::HTTP_1_1);
	assertNewConnectionServes(cs, server, request);
}


void HTTPClientSessionTest::testFailedBodySendClosesAdoptedSocket()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket socket(server.address());
	HTTPClientSession cs(socket);
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(2, 0));
	StreamSocket first = sendBodyUntilFailure(cs, server);
	HTTPRequest request(HTTPRequest::HTTP_GET, "/second", HTTPMessage::HTTP_1_1);
	Stopwatch sw;
	sw.start();
	try
	{
		cs.sendRequest(request);
		failmsg("must throw TimeoutException");
	}
	catch (Poco::TimeoutException&)
	{
	}
	// Sending the buffered body again on the stalled connection would take another send timeout.
	assertTrue (sw.elapsedSeconds() < 1);
	assertTrue (!cs.connected());
	assertBodyOnlyUntilClosed(first);
}


void HTTPClientSessionTest::testIncompleteRequestBodyClosesAdoptedSocket()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket socket(server.address());
	HTTPClientSession cs(socket);
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(5, 0));
	HTTPRequest request(HTTPRequest::HTTP_POST, "/upload", HTTPMessage::HTTP_1_1);
	request.setContentLength(10);
	cs.sendRequest(request) << "AAAAA";
	StreamSocket first = server.acceptConnection();
	const std::string early("HTTP/1.1 413 Payload Too Large\r\nContent-Length: 0\r\n\r\n");
	first.sendBytes(early.data(), static_cast<int>(early.size()));
	HTTPResponse response;
	cs.receiveResponse(response);

	HTTPRequest next(HTTPRequest::HTTP_GET, "/second", HTTPMessage::HTTP_1_1);
	try
	{
		cs.sendRequest(next);
		failmsg("must throw NetException");
	}
	catch (Poco::Net::NetException& exc)
	{
		assertTrue (exc.message().find("request body") != std::string::npos);
	}
	assertTrue (!cs.connected());
	assertBodyOnlyUntilClosed(first);
}


void HTTPClientSessionTest::testUnreadResponseBodyClosesAdoptedSocket()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	StreamSocket socket(server.address());
	HTTPClientSession cs(socket);
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(5, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/first", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	StreamSocket first = server.acceptConnection();
	// The last byte of the body is not sent, so the client cannot skip the rest.
	const std::string reply("HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\n0123456789");
	first.sendBytes(reply.data(), static_cast<int>(reply.size()));
	HTTPResponse response;
	cs.receiveResponse(response);

	HTTPRequest next(HTTPRequest::HTTP_GET, "/second", HTTPMessage::HTTP_1_1);
	try
	{
		cs.sendRequest(next);
		failmsg("must throw NetException");
	}
	catch (Poco::Net::NetException& exc)
	{
		assertTrue (exc.message().find("response body") != std::string::npos);
	}
	assertTrue (!cs.connected());
	const std::string received = receiveUntilClosed(first);
	assertTrue (received.find("/first") != std::string::npos);
	assertTrue (received.find("/second") == std::string::npos);
}


void HTTPClientSessionTest::testDataAfterResponseClosesAdoptedSocket()
{
	for (const bool peerCloses: {true, false})
	{
		ServerSocket server(SocketAddress("127.0.0.1", 0));
		StreamSocket socket(server.address());
		HTTPClientSession cs(socket);
		cs.setKeepAlive(true);
		cs.setTimeout(Poco::Timespan(5, 0));
		HTTPRequest request(HTTPRequest::HTTP_GET, "/first", HTTPMessage::HTTP_1_1);
		cs.sendRequest(request).flush();
		StreamSocket first = server.acceptConnection();
		// Read before closing: closing a socket with unread data resets the connection instead of closing it.
		receiveHeader(first);
		const std::string reply("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello");
		first.sendBytes(reply.data(), static_cast<int>(reply.size()));
		HTTPResponse response;
		std::istream& rs = cs.receiveResponse(response);
		std::string body;
		StreamCopier::copyToString(rs, body);
		assertTrue (body == "hello");
		if (peerCloses)
		{
			first.close();
		}
		else
		{
			const std::string unsolicited("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nbogus");
			first.sendBytes(unsolicited.data(), static_cast<int>(unsolicited.size()));
		}
		assertTrue (cs.socket().poll(Poco::Timespan(5, 0), Socket::SELECT_READ));

		HTTPRequest next(HTTPRequest::HTTP_GET, "/second", HTTPMessage::HTTP_1_1);
		try
		{
			cs.sendRequest(next);
			failmsg("must throw NetException");
		}
		catch (Poco::Net::NetException& exc)
		{
			assertTrue (exc.message().find("closed the connection after it") != std::string::npos);
		}
		assertTrue (!cs.connected());
	}
}


void HTTPClientSessionTest::testPeekResponseAfterFailedSendCloses()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	FailedSendSession cs("127.0.0.1", server.address().port());
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_POST, "/upload", HTTPMessage::HTTP_1_1);
	request.setContentLength(5);
	request.setExpectContinue(true);
	cs.sendRequest(request);
	StreamSocket peer = server.acceptConnection();
	cs.failSend();
	HTTPResponse response;
	try
	{
		cs.peekResponse(response);
		failmsg("must throw TimeoutException");
	}
	catch (Poco::TimeoutException&)
	{
	}
	assertTrue (!cs.connected());
}


void HTTPClientSessionTest::testFailedBodySendNotRepeated()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setTimeout(Poco::Timespan(2, 0));
	StreamSocket peer = sendBodyUntilFailure(cs, server);
	Stopwatch sw;
	sw.start();
	HTTPResponse response;
	try
	{
		cs.receiveResponse(response);
		failmsg("must throw TimeoutException");
	}
	catch (Poco::TimeoutException&)
	{
	}
	// Sending the buffered body again on the stalled connection would take another send timeout.
	assertTrue (sw.elapsedSeconds() < 1);
	assertTrue (!cs.connected());
}


void HTTPClientSessionTest::testIncompleteRequestBodyReconnects()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(5, 0));
	HTTPRequest request(HTTPRequest::HTTP_POST, "/upload", HTTPMessage::HTTP_1_1);
	request.setContentLength(10);
	cs.sendRequest(request) << "AAAAA";
	StreamSocket first = server.acceptConnection();
	const std::string early("HTTP/1.1 413 Payload Too Large\r\nContent-Length: 0\r\n\r\n");
	first.sendBytes(early.data(), static_cast<int>(early.size()));
	HTTPResponse response;
	cs.receiveResponse(response);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_REQUEST_ENTITY_TOO_LARGE);

	HTTPRequest next(HTTPRequest::HTTP_GET, "/second", HTTPMessage::HTTP_1_1);
	cs.sendRequest(next);
	assertTrue (server.poll(Poco::Timespan(5, 0), Socket::SELECT_READ));
	assertBodyOnlyUntilClosed(first);
}


void HTTPClientSessionTest::testIncompleteRequestBodyWithoutResponseReconnects()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(5, 0));
	HTTPRequest request(HTTPRequest::HTTP_POST, "/upload", HTTPMessage::HTTP_1_1);
	request.setContentLength(10);
	cs.sendRequest(request) << "AAAAA";
	StreamSocket first = server.acceptConnection();

	HTTPRequest next(HTTPRequest::HTTP_GET, "/second", HTTPMessage::HTTP_1_1);
	cs.sendRequest(next);
	assertTrue (server.poll(Poco::Timespan(5, 0), Socket::SELECT_READ));
	assertBodyOnlyUntilClosed(first);
}


void HTTPClientSessionTest::testShortRequestWriteClosesConnection()
{
	// The short write happens in the final flush.
	assertTrue (sendWithShortWrite("0123456789") == "01234");

	// A body larger than the stream buffer makes the short write happen when the buffer is full.
	std::string body;
	for (int i = 0; i < 2 * HTTPBufferAllocator::BUFFER_SIZE; ++i)
		body += static_cast<char>('a' + i % 23);
	const std::string received = sendWithShortWrite(body);
	assertTrue (!received.empty() && received.size() < body.size());
	assertTrue (received == body.substr(0, received.size()));
}


void HTTPClientSessionTest::assertFramingRejected(const std::string& headers)
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	StreamSocket first = server.acceptConnection();
	const std::string malformed("HTTP/1.1 200 OK\r\n" + headers + "\r\nHTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nstale");
	first.sendBytes(malformed.data(), static_cast<int>(malformed.size()));
	HTTPResponse response;
	try
	{
		cs.receiveResponse(response);
		failmsg("must throw MessageException: " + headers);
	}
	catch (MessageException&)
	{
	}
	assertTrue (!cs.connected());

	assertNewConnectionServes(cs, server, request);
}


void HTTPClientSessionTest::assertFramingIgnored(const std::string& method, const std::string& uri)
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(method, uri, HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	StreamSocket connection = server.acceptConnection();
	const std::string reply("HTTP/1.1 200 OK\r\nContent-Length: 5\r\nContent-Length: 7\r\nTransfer-Encoding: chunked\r\n\r\n");
	connection.sendBytes(reply.data(), static_cast<int>(reply.size()));
	HTTPResponse response;
	std::istream& rs = cs.receiveResponse(response);
	std::string body;
	StreamCopier::copyToString(rs, body);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
	assertTrue (body.empty());
}


void HTTPClientSessionTest::assertReconnectAfterPartialRead(const std::string& firstResponse, std::streamsize bodyBytes)
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setKeepAlive(true);
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	StreamSocket first = server.acceptConnection();
	first.sendBytes(firstResponse.data(), static_cast<int>(firstResponse.size()));
	HTTPResponse response;
	std::istream& rs1 = cs.receiveResponse(response);
	std::string partial(static_cast<std::size_t>(bodyBytes), '\0');
	rs1.read(partial.data(), bodyBytes);
	assertTrue (rs1.gcount() == bodyBytes);

	Stopwatch sw;
	sw.start();
	assertNewConnectionServes(cs, server, request);
	assertTrue (sw.elapsedSeconds() < 2);
}


void HTTPClientSessionTest::testFixedLengthResponseInPartsNonBlocking()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPClientSession cs("127.0.0.1", server.address().port());
	cs.setTimeout(Poco::Timespan(10, 0));
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	StreamSocket ss = server.acceptConnection();
	const std::string head("HTTP/1.1 200 OK\r\nContent-Length: 10\r\n\r\n01234");
	ss.sendBytes(head.data(), static_cast<int>(head.size()));
	HTTPResponse response;
	std::istream& rs = cs.receiveResponse(response);
	cs.socket().setBlocking(false);
	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);
	assertTrue (cs.networkException() == nullptr);

	rs.clear();
	cs.socket().setBlocking(true);
	const std::string tail("56789");
	ss.sendBytes(tail.data(), static_cast<int>(tail.size()));
	StreamCopier::copyStream(rs, ostr);
	assertTrue (ostr.str() == "0123456789");
	assertTrue (cs.networkException() == nullptr);
}


StreamSocket HTTPClientSessionTest::sendBodyUntilFailure(HTTPClientSession& session, ServerSocket& server)
{
	const Poco::Int64 contentLength = 64 * 1024 * 1024;
	HTTPRequest request(HTTPRequest::HTTP_POST, "/upload", HTTPMessage::HTTP_1_1);
	request.setContentLength64(contentLength);
	std::ostream& body = session.sendRequest(request);
	StreamSocket peer = server.acceptConnection();
	const std::string chunk(64 * 1024, 'A');
	Poco::Int64 written = 0;
	while (body.good() && written < contentLength)
	{
		body.write(chunk.data(), static_cast<std::streamsize>(chunk.size()));
		written += static_cast<Poco::Int64>(chunk.size());
	}
	assertTrue (!body.good());
	return peer;
}


std::string HTTPClientSessionTest::receiveUntilClosed(StreamSocket& peer)
{
	peer.setReceiveTimeout(Poco::Timespan(5, 0));
	std::string received;
	char buffer[64 * 1024];
	try
	{
		int n = 0;
		while ((n = peer.receiveBytes(buffer, sizeof(buffer))) > 0)
			received.append(buffer, static_cast<std::size_t>(n));
	}
	catch (Poco::TimeoutException&)
	{
		failmsg("the client did not close the connection");
	}
	return received;
}


void HTTPClientSessionTest::assertBodyOnlyUntilClosed(StreamSocket& peer)
{
	const std::string received = receiveUntilClosed(peer);
	const std::size_t headerEnd = received.find("\r\n\r\n");
	assertTrue (headerEnd != std::string::npos);
	assertTrue (received.find_first_not_of('A', headerEnd + 4) == std::string::npos);
}


std::string HTTPClientSessionTest::sendWithShortWrite(const std::string& body)
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	ShortSendSession cs("127.0.0.1", server.address().port());
	cs.setTimeout(Poco::Timespan(5, 0));
	auto peer = std::async(std::launch::async, [&server]()
	{
		StreamSocket connection = server.acceptConnection();
		connection.setReceiveTimeout(Poco::Timespan(2, 0));
		std::string received;
		char buffer[1024];
		try
		{
			int n = 0;
			while ((n = connection.receiveBytes(buffer, sizeof(buffer))) > 0)
				received.append(buffer, static_cast<std::size_t>(n));
		}
		catch (Poco::TimeoutException&)
		{
		}
		try
		{
			// A response that the client would accept if it kept the connection.
			const std::string reply("HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
			connection.sendBytes(reply.data(), static_cast<int>(reply.size()));
		}
		catch (Poco::Exception&)
		{
		}
		return received;
	});
	HTTPRequest request(HTTPRequest::HTTP_POST, "/upload", HTTPMessage::HTTP_1_1);
	request.setContentLength(static_cast<std::streamsize>(body.size()));
	cs.sendRequest(request) << body;
	HTTPResponse response;
	try
	{
		cs.receiveResponse(response);
		failmsg("must throw NetException");
	}
	catch (Poco::Net::NetException& exc)
	{
		assertTrue (exc.message().find("Incomplete message write") != std::string::npos);
	}
	assertTrue (!cs.connected());
	const std::string received = peer.get();
	assertTrue (received.size() == static_cast<std::size_t>(cs.shortWriteLength()));
	const std::size_t headerEnd = received.find("\r\n\r\n");
	assertTrue (headerEnd != std::string::npos);
	assertTrue (received.compare(0, 6, "POST /") == 0);
	assertTrue (received.find("POST /", 1) == std::string::npos);
	return received.substr(headerEnd + 4);
}


void HTTPClientSessionTest::setUp()
{
}


void HTTPClientSessionTest::tearDown()
{
}


CppUnit::Test* HTTPClientSessionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPClientSessionTest");

	CppUnit_addTest(pSuite, HTTPClientSessionTest, testGetSmall);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testGetSmallUnix);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testGetLarge);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testHead);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testPostSmallIdentity);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testPostLargeIdentity);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testPostSmallChunked);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testPostLargeChunked);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testKeepAlive);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testTrailer);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testProxy);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testProxyAuth);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testBypassProxy);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testExpectContinue);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testExpectContinueFail);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testProxyConfig);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testSetProxyProtocolValidation);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testSetProxyConfigValidation);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testProxySetters);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testProxyRequestPrefix);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testProxyNonTunnel);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testGlobalProxyConfig);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testBypassProxyExtended);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testChunkedFramingErrorReconnects);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testInvalidRequestKeepsConnection);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testIncompleteFixedLengthResponse);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testCompleteFixedLengthResponse);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testFixedLengthResponseInPartsNonBlocking);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testUnreadFixedLengthResponseReconnects);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testUnreadBufferedFixedLengthResponseKeepsConnection);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testStrayResponseAfterBodyReconnects);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testServerCloseAfterResponseReconnects);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testUnreadChunkedResponseReconnects);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testMalformedUnreadChunkReconnects);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testUnreadCloseDelimitedResponseReconnects);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testUnreadChunkedTerminatorKeepsConnection);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testProxyTunnelIgnoresPreviousResponse);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testMalformedContentLengthResponse);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testDuplicateContentLengthResponse);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testDuplicateTransferEncodingResponse);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testContentLengthAndTransferEncodingResponse);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testNonChunkedTransferEncodingResponseReadsToClose);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testHeadResponseIgnoresFramingHeaders);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testConnectResponseIgnoresFramingHeaders);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testTimeoutAppliedToConnectedSocket);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testNonBlockingReceiveWithoutData);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testFailedBodySendReconnects);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testFailedBodySendClosesAdoptedSocket);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testIncompleteRequestBodyClosesAdoptedSocket);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testUnreadResponseBodyClosesAdoptedSocket);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testDataAfterResponseClosesAdoptedSocket);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testPeekResponseAfterFailedSendCloses);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testFailedBodySendNotRepeated);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testIncompleteRequestBodyReconnects);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testIncompleteRequestBodyWithoutResponseReconnects);
	CppUnit_addTest(pSuite, HTTPClientSessionTest, testShortRequestWriteClosesConnection);

	return pSuite;
}
