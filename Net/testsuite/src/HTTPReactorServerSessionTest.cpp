#include "HTTPReactorServerSessionTest.h"
#include "Poco/Net/HTTPReactorServerSession.h"
#include "CppUnit/TestSuite.h"
#include "CppUnit/TestCaller.h"


HTTPReactorServerSessionTest::HTTPReactorServerSessionTest(const std::string& name) : CppUnit::TestCase(name)
{
}

HTTPReactorServerSessionTest::~HTTPReactorServerSessionTest()
{
}

void HTTPReactorServerSessionTest::setUp()
{
}

void HTTPReactorServerSessionTest::tearDown()
{
}

void HTTPReactorServerSessionTest::testCheckRequestComplete()
{
	std::string  buf("GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 0\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunked()
{
	std::string   buf("GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(!buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedWithTrailer()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\nTrailer: Content-MD5\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(!buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedWithTrailerAndBody()
{
	std::string buf(
		"GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\nTrailer: Content-MD5\r\n\r\n0\r\nContent-MD5:xxxxx\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedWithBody()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n0\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedWithStickyBody()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nHello\r\n5\r\nWorld\r\n0\r\n\r\n");
	int len = buf.length();
	buf.append(buf);
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.length() == len);
}

CppUnit::Test* HTTPReactorServerSessionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPReactorServerSessionTest");

	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestComplete);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunked);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunkedWithTrailer);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunkedWithTrailerAndBody);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunkedWithBody);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteChunkedWithStickyBody);

	return pSuite;
}