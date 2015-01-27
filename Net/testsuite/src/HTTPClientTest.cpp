//
// HTTPClientTest.cpp
//
// $Id: //poco/1.4/Net/testsuite/src/HTTPClientTest.cpp#2 $
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPClientTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPClient.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "HTTPTestServer.h"
#include "Poco/Delegate.h"
#include "Poco/Thread.h"
#include "Poco/SharedPtr.h"
#include <istream>
#include <ostream>
#include <sstream>


using Poco::Net::HTTPClient;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPEventArgs;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Delegate;
using Poco::Thread;
using Poco::SharedPtr;


HTTPClientTest::HTTPClientTest(const std::string& name): CppUnit::TestCase(name)
{
}


void HTTPClientTest::onResponse(const void* pSender, HTTPEventArgs& args)
{
	_contentType = args.response().getContentType();
	_contentLength = static_cast<int>(args.response().getContentLength());
	_chunked = args.response().getChunkedTransferEncoding();
	_keepAlive = args.response().getKeepAlive();
	_body = args.body();
	_done = true;
}


void HTTPClientTest::onError(const void* pSender, HTTPEventArgs& args)
{
	_error = args.response().getStatus();
	_done = true;
}


HTTPClientTest::~HTTPClientTest()
{
}


void HTTPClientTest::testGetSmall()
{
	HTTPTestServer srv;
	HTTPClient s("localhost", srv.port());
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	s.sendGet("/small");
	while (!_done) Thread::sleep(10);
	assert(_contentLength == HTTPTestServer::SMALL_BODY.length());
	assert(_contentType == "text/plain");
	assert(!_chunked);
	assert(_body == HTTPTestServer::SMALL_BODY);
}


void HTTPClientTest::testGetLarge()
{
	HTTPTestServer srv;
	HTTPClient s("localhost", srv.port());
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	s.sendGet("/large");
	while (!_done) Thread::sleep(10);
	assert(_contentLength == HTTPTestServer::LARGE_BODY.length());
	assert(_contentType == "text/plain");
	assert(!_chunked);
	assert(_body == HTTPTestServer::LARGE_BODY);
}


void HTTPClientTest::testHead()
{
	HTTPTestServer srv;
	HTTPClient s("localhost", srv.port());
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	s.sendHead("/large");
	while (!_done) Thread::sleep(10);
	assert(_contentLength == HTTPTestServer::LARGE_BODY.length());
	assert(_contentType == "text/plain");
	assert(!_chunked);
}


void HTTPClientTest::testPostSmallIdentity()
{
	HTTPTestServer srv;
	HTTPClient s("localhost", srv.port());
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	std::string body("this is a random request body\r\n0\r\n");
	s.sendPost("/echo", body);
	while (!_done) Thread::sleep(10);
	assert(_contentLength == body.length());
	assert(!_chunked);
	assert(_body == body);
}


void HTTPClientTest::testPostLargeIdentity()
{
	HTTPTestServer srv;
	HTTPClient s("localhost", srv.port());
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	std::string body(8000, 'x');
	body.append("\r\n0\r\n");
	s.sendPost("/echo", body);
	while (!_done) Thread::sleep(10);
	assert(_contentLength == body.length());
	assert(!_chunked);
	assert(_body == body);
}


void HTTPClientTest::testPostSmallChunked()
{
	HTTPTestServer srv;
	HTTPClient s("localhost", srv.port());
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	SharedPtr<HTTPRequest> request = new HTTPRequest(HTTPRequest::HTTP_POST, "/echo");
	std::string body("this is a random request body");
	request->setChunkedTransferEncoding(true);
	s.sendRequest(request, body);
	while (!_done) Thread::sleep(10);
	assert(_contentLength == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	assert(_chunked);
	assert(_body == body);
}


void HTTPClientTest::testPostLargeChunked()
{
	HTTPTestServer srv;
	HTTPClient s("localhost", srv.port());
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	SharedPtr<HTTPRequest> request = new HTTPRequest(HTTPRequest::HTTP_POST, "/echo");
	std::string body(16000, 'x');
	request->setChunkedTransferEncoding(true);
	s.sendRequest(request, body);
	while (!_done) Thread::sleep(10);
	assert(_contentLength == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	assert(_chunked);
	assert(_body == body);
}


void HTTPClientTest::testPostSmallClose()
{
	HTTPTestServer srv;
	HTTPClient s("localhost", srv.port());
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	SharedPtr<HTTPRequest> request = new HTTPRequest(HTTPRequest::HTTP_POST, "/echo");
	std::string body("this is a random request body");
	s.sendRequest(request, body);
	while (!_done) Thread::sleep(10);
	assert(_contentLength == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	assert(!_chunked);
	assert(_body == body);
}


void HTTPClientTest::testPostLargeClose()
{
	HTTPTestServer srv;
	HTTPClient s("localhost", srv.port());
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	SharedPtr<HTTPRequest> request = new HTTPRequest(HTTPRequest::HTTP_POST, "/echo");
	std::string body(8000, 'x');
	s.sendRequest(request, body);
	while (!_done) Thread::sleep(10);
	assert(_contentLength == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	assert(!_chunked);
	assert(_body == body);
}


void HTTPClientTest::testKeepAlive()
{
	HTTPTestServer srv;
	HTTPClient s("localhost", srv.port());
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	s.setKeepAlive(true);
	SharedPtr<HTTPRequest> request = new HTTPRequest(HTTPRequest::HTTP_HEAD, "/keepAlive", HTTPMessage::HTTP_1_1);
	s.sendRequest(request);
	while (!_done) Thread::sleep(10);
	//HTTPResponse response;
	//std::istream& rs1 = s.receiveResponse(response);
	assert (_contentLength == HTTPTestServer::SMALL_BODY.length());
	assert (_contentType == "text/plain");
	assert (_keepAlive);
	assert(_body.empty());
	//std::ostringstream ostr1;
	//assert (StreamCopier::copyStream(rs1, ostr1) == 0);

	setUp();

	request->setMethod(HTTPRequest::HTTP_GET);
	request->setURI("/small");
	s.sendRequest(request);
	while (!_done) Thread::sleep(10);
	//std::istream& rs2 = s.receiveResponse(response);
	assert (_contentLength == HTTPTestServer::SMALL_BODY.length());
	assert (_keepAlive);
	//std::ostringstream ostr2;
	//StreamCopier::copyStream(rs2, ostr2);
	assert (_body == HTTPTestServer::SMALL_BODY);

	setUp();

	request->setMethod(HTTPRequest::HTTP_GET);
	request->setURI("/large");
	s.sendRequest(request);
	while (!_done) Thread::sleep(10);
	//std::istream& rs3 = s.receiveResponse(response);
	assert (_contentLength == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	assert (_chunked);
	assert (_keepAlive);
	std::ostringstream ostr3;
	//StreamCopier::copyStream(rs3, ostr3);
	assert (_body == HTTPTestServer::LARGE_BODY);

	setUp();

	request->setMethod(HTTPRequest::HTTP_HEAD);
	request->setURI("/large");
	s.sendRequest(request);
	while (!_done) Thread::sleep(10);
	//std::istream& rs4= s.receiveResponse(response);
	assert (_contentLength == HTTPTestServer::LARGE_BODY.length());
	assert (_contentType == "text/plain");
	assert (!_keepAlive);
	assert(_body.empty());
}


void HTTPClientTest::testProxy()
{
	HTTPTestServer srv;
	HTTPClient s("www.somehost.com");
	s.setProxy("localhost", srv.port());
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	SharedPtr<HTTPRequest> request = new HTTPRequest(HTTPRequest::HTTP_GET, "/large");
	s.sendRequest(request);
	while (!_done) Thread::sleep(10);
	assert(_contentLength == HTTPTestServer::LARGE_BODY.length());
	assert(_contentType == "text/plain");
	assert(!_chunked);
	assert(_body == HTTPTestServer::LARGE_BODY);
}


void HTTPClientTest::testProxyAuth()
{
	HTTPTestServer srv;
	HTTPClient s("www.somehost.com");
	s.setProxy("localhost", srv.port());
	s.setProxyCredentials("user", "pass");
	s.httpResponse += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onResponse);
	SharedPtr<HTTPRequest> request = new HTTPRequest(HTTPRequest::HTTP_GET, "/large");
	s.sendRequest(request);
	while (!_done) Thread::sleep(10);
	assert(_contentLength == HTTPTestServer::LARGE_BODY.length());
	assert(_contentType == "text/plain");
	assert(!_chunked);
	assert(_body == HTTPTestServer::LARGE_BODY);
}


void HTTPClientTest::testBypassProxy()
{
	HTTPClientSession::ProxyConfig proxyConfig;
	proxyConfig.host = "proxy.domain.com";
	proxyConfig.port = 80;
	proxyConfig.nonProxyHosts = "localhost|127\\.0\\.0\\.1";
	
	HTTPClient s1("localhost", 80);
	s1.setProxyConfig(proxyConfig);
	assert (s1.bypassProxy());
	
	HTTPClient s2("127.0.0.1", 80);
	s2.setProxyConfig(proxyConfig);
	assert (s2.bypassProxy());
	
	HTTPClient s3("www.appinf.com", 80);
	s3.setProxyConfig(proxyConfig);
	assert (!s3.bypassProxy());
}


void HTTPClientTest::testNotFound()
{
	HTTPTestServer srv;
	HTTPClient s("localhost", srv.port());
	s.httpError += Delegate<HTTPClientTest, HTTPEventArgs>(this, &HTTPClientTest::onError);
	s.sendGet("/notfound");
	while (!_done) Thread::sleep(10);
	assert(_error == HTTPResponse::HTTP_NOT_FOUND);
}


void HTTPClientTest::setUp()
{
	_contentType.clear();
	_contentLength = HTTPMessage::UNKNOWN_CONTENT_LENGTH;
	_chunked = false;
	_keepAlive = false;
	_body.clear();
	_error = HTTPResponse::HTTP_OK;
	_done = false;
}


void HTTPClientTest::tearDown()
{
}


CppUnit::Test* HTTPClientTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPClientTest");

	CppUnit_addTest(pSuite, HTTPClientTest, testGetSmall);
	CppUnit_addTest(pSuite, HTTPClientTest, testGetLarge);
	CppUnit_addTest(pSuite, HTTPClientTest, testHead);
	CppUnit_addTest(pSuite, HTTPClientTest, testPostSmallIdentity);
	CppUnit_addTest(pSuite, HTTPClientTest, testPostLargeIdentity);
	CppUnit_addTest(pSuite, HTTPClientTest, testPostSmallChunked);
	CppUnit_addTest(pSuite, HTTPClientTest, testPostLargeChunked);
	CppUnit_addTest(pSuite, HTTPClientTest, testPostSmallClose);
	CppUnit_addTest(pSuite, HTTPClientTest, testPostLargeClose);
	CppUnit_addTest(pSuite, HTTPClientTest, testKeepAlive);
	CppUnit_addTest(pSuite, HTTPClientTest, testProxy);
	CppUnit_addTest(pSuite, HTTPClientTest, testProxyAuth);
	CppUnit_addTest(pSuite, HTTPClientTest, testBypassProxy);
	CppUnit_addTest(pSuite, HTTPClientTest, testNotFound);

	return pSuite;
}
