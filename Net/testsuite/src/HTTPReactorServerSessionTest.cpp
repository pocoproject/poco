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

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ntransfer-encoding: chunked\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session1.checkRequestComplete());
	session1.popCompletedRequest();
	assertTrue(!buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedWithTrailer()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\nTrailer: Content-MD5\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(!buf.empty());

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ntransfer-encoding: chunked\r\ntrailer: content-md5\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session1.checkRequestComplete());
	session1.popCompletedRequest();
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

	buf.assign(
		"GET / HTTP/1.1\r\nhost: localhost\r\ntransfer-encoding: chunked\r\ntrailer: content-md5\r\n\r\n0\r\ncontent-md5:xxxxx\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session1.checkRequestComplete());
	session1.popCompletedRequest();
	assertTrue(buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteChunkedWithBody()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n0\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.empty());

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ntransfer-encoding: chunked\r\n\r\n0\r\n\r\n");
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session1.checkRequestComplete());
	session1.popCompletedRequest();
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

void HTTPReactorServerSessionTest::testCheckRequestCompleteContentLength()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nHello");
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.empty());

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ncontent-length: 5\r\n\r\nHello");
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session1.checkRequestComplete());
	session1.popCompletedRequest();
	assertTrue(buf.empty());
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteContentLengthIncomplete()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nHel");
	int len = buf.length();
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.size() == len);

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ncontent-length: 5\r\n\r\nHel");
	len = buf.length();
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(!session1.checkRequestComplete());
	session1.popCompletedRequest();
	assertTrue(buf.size() == len);
}

void HTTPReactorServerSessionTest::testCheckRequestCompleteContentLengthWithStickyBody()
{
	std::string buf("GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nHello");
	int len = buf.length();
	buf.append(buf);
	Poco::Net::HTTPReactorServerSession session(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session.checkRequestComplete());
	session.popCompletedRequest();
	assertTrue(buf.length() == len);

	buf.assign("GET / HTTP/1.1\r\nhost: localhost\r\ncontent-length: 5\r\n\r\nHello");
	len = buf.length();
	buf.append(buf);
	Poco::Net::HTTPReactorServerSession session1(Poco::Net::StreamSocket(), buf, nullptr);
	assertTrue(session1.checkRequestComplete());
	session1.popCompletedRequest();
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
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteContentLength);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteContentLengthIncomplete);
	CppUnit_addTest(pSuite, HTTPReactorServerSessionTest, testCheckRequestCompleteContentLengthWithStickyBody);

	return pSuite;
}