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
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "HTTPTestServer.h"
#include <istream>
#include <ostream>
#include <sstream>


using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSession;
using Poco::Net::ProxyConfig;
using Poco::Net::ProxyAuthentication;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::InvalidArgumentException;
using Poco::IllegalStateException;
using Poco::File;
using Poco::Path;


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
	HTTPTestServer srv;
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
	std::istream& rs4= s.receiveResponse(response);
	assertTrue (response.getContentLength() == HTTPTestServer::LARGE_BODY.length());
	assertTrue (response.getContentType() == "text/plain");
	assertTrue (!response.getKeepAlive());
	std::ostringstream ostr4;
	assertTrue (StreamCopier::copyStream(rs4, ostr4) == 0);
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

	s.setProxy("proxy", 80, "https", true);
	assertTrue (s.getProxyProtocol() == "https");
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

	goodConfig.protocol = "https";
	s.setProxyConfig(goodConfig);
	assertTrue (s.getProxyProtocol() == "https");
}


void HTTPClientSessionTest::testProxySetters()
{
	HTTPClientSession s("www.example.com");

	s.setProxyHost("proxy.example.com");
	assertTrue (s.getProxyHost() == "proxy.example.com");

	s.setProxyPort(8080);
	assertTrue (s.getProxyPort() == 8080);

	s.setProxyProtocol("https");
	assertTrue (s.getProxyProtocol() == "https");

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
	config.protocol = "https";
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

	return pSuite;
}
