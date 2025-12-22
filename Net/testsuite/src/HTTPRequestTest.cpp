//
// HTTPRequestTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPRequestTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/NetException.h"
#include <sstream>


using Poco::Net::HTTPRequest;
using Poco::Net::HTTPMessage;
using Poco::Net::MessageException;
using Poco::Net::NameValueCollection;


HTTPRequestTest::HTTPRequestTest(const std::string& name): CppUnit::TestCase(name)
{
}


HTTPRequestTest::~HTTPRequestTest()
{
}


void HTTPRequestTest::testWrite1()
{
	HTTPRequest request;
	std::ostringstream ostr;
	request.write(ostr);
	std::string s = ostr.str();
	assertTrue (s == "GET / HTTP/1.0\r\n\r\n");
}


void HTTPRequestTest::testWrite2()
{
	HTTPRequest request(HTTPRequest::HTTP_HEAD, "/index.html", HTTPMessage::HTTP_1_1);
	request.setHost("localhost", 80);
	request.setKeepAlive(true);
	request.set("User-Agent", "Poco");
	std::ostringstream ostr;
	request.write(ostr);
	std::string s = ostr.str();
	assertTrue (s == "HEAD /index.html HTTP/1.1\r\nHost: localhost\r\nConnection: Keep-Alive\r\nUser-Agent: Poco\r\n\r\n");
}


void HTTPRequestTest::testWrite3()
{
	HTTPRequest request(HTTPRequest::HTTP_POST, "/test.cgi", HTTPMessage::HTTP_1_1);
	request.setHost("localhost", 8000);
	request.setKeepAlive(false);
	request.set("User-Agent", "Poco");
	request.setContentLength(100);
	request.setContentType("text/plain");
	std::ostringstream ostr;
	request.write(ostr);
	std::string s = ostr.str();
	assertTrue (s == "POST /test.cgi HTTP/1.1\r\nHost: localhost:8000\r\nConnection: Close\r\nUser-Agent: Poco\r\nContent-Length: 100\r\nContent-Type: text/plain\r\n\r\n");
}


void HTTPRequestTest::testWrite4()
{
	HTTPRequest request(HTTPRequest::HTTP_HEAD, "/index.html", HTTPMessage::HTTP_1_1);
	request.setHost("fe80::1", 88);
	request.setKeepAlive(true);
	request.set("User-Agent", "Poco");
	std::ostringstream ostr;
	request.write(ostr);
	std::string s = ostr.str();
	assertTrue (s == "HEAD /index.html HTTP/1.1\r\nHost: [fe80::1]:88\r\nConnection: Keep-Alive\r\nUser-Agent: Poco\r\n\r\n");
}


void HTTPRequestTest::testRead1()
{
	std::string s("GET / HTTP/1.0\r\n\r\n");
	std::istringstream istr(s);
	HTTPRequest request;
	request.read(istr);
	assertTrue (request.getMethod() == HTTPRequest::HTTP_GET);
	assertTrue (request.getURI() == "/");
	assertTrue (request.getVersion() == HTTPMessage::HTTP_1_0);
	assertTrue (request.empty());
	assertTrue (istr.get() == -1);
}


void HTTPRequestTest::testRead2()
{
	std::string s("HEAD /index.html HTTP/1.1\r\nConnection: Keep-Alive\r\nHost: localhost\r\nUser-Agent: Poco\r\n\r\n");
	std::istringstream istr(s);
	HTTPRequest request;
	request.read(istr);
	assertTrue (request.getMethod() == HTTPRequest::HTTP_HEAD);
	assertTrue (request.getURI() == "/index.html");
	assertTrue (request.getVersion() == HTTPMessage::HTTP_1_1);
	assertTrue (request.size() == 3);
	assertTrue (request["Connection"] == "Keep-Alive");
	assertTrue (request["Host"] == "localhost");
	assertTrue (request["User-Agent"] == "Poco");
	assertTrue (istr.get() == -1);
}


void HTTPRequestTest::testRead3()
{
	std::string s("POST /test.cgi HTTP/1.1\r\nConnection: Close\r\nContent-Length: 100\r\nContent-Type: text/plain\r\nHost: localhost:8000\r\nUser-Agent: Poco\r\n\r\n");
	std::istringstream istr(s);
	HTTPRequest request;
	request.read(istr);
	assertTrue (request.getMethod() == HTTPRequest::HTTP_POST);
	assertTrue (request.getURI() == "/test.cgi");
	assertTrue (request.getVersion() == HTTPMessage::HTTP_1_1);
	assertTrue (request.size() == 5);
	assertTrue (request["Connection"] == "Close");
	assertTrue (request["Host"] == "localhost:8000");
	assertTrue (request["User-Agent"] == "Poco");
	assertTrue (request.getContentType() == "text/plain");
	assertTrue (request.getContentLength() == 100);
	assertTrue (istr.get() == -1);
}


void HTTPRequestTest::testRead4()
{
	std::string s("POST /test.cgi HTTP/1.1\r\nConnection: Close\r\nContent-Length:   100  \r\nContent-Type: text/plain\r\nHost: localhost:8000\r\nUser-Agent: Poco\r\n\r\n");
	std::istringstream istr(s);
	HTTPRequest request;
	request.read(istr);
	assertTrue (request.getMethod() == HTTPRequest::HTTP_POST);
	assertTrue (request.getURI() == "/test.cgi");
	assertTrue (request.getVersion() == HTTPMessage::HTTP_1_1);
	assertTrue (request.size() == 5);
	assertTrue (request["Connection"] == "Close");
	assertTrue (request["Host"] == "localhost:8000");
	assertTrue (request["User-Agent"] == "Poco");
	assertTrue (request.getContentType() == "text/plain");
	assertTrue (request.getContentLength() == 100);
	assertTrue (istr.get() == -1);
}


void HTTPRequestTest::testInvalid1()
{
	std::string s(256, 'x');
	std::istringstream istr(s);
	HTTPRequest request;
	try
	{
		request.read(istr);
		fail("inavalid request - must throw");
	}
	catch (MessageException&)
	{
	}
}


void HTTPRequestTest::testInvalid2()
{
	std::string s("GET ");
	s.append(8000, 'x');
	s.append("HTTP/1.0");
	std::istringstream istr(s);
	HTTPRequest request;
	try
	{
		request.read(istr);
		fail("inavalid request - must throw");
	}
	catch (MessageException&)
	{
	}
}


void HTTPRequestTest::testInvalid3()
{
	std::string s("GET / HTTP/1.10");
	std::istringstream istr(s);
	HTTPRequest request;
	try
	{
		request.read(istr);
		fail("inavalid request - must throw");
	}
	catch (MessageException&)
	{
	}
}


void HTTPRequestTest::testCookies()
{
	HTTPRequest request1;
	NameValueCollection cookies1;
	cookies1.add("cookie1", "value1");
	request1.setCookies(cookies1);
	assertTrue (request1["Cookie"] == "cookie1=value1");

	HTTPRequest request2;
	NameValueCollection cookies2;
	cookies2.add("cookie2", "value2");
	cookies2.add("cookie3", "value3");
	request2.setCookies(cookies2);
	assertTrue (request2["Cookie"] == "cookie2=value2; cookie3=value3");

	request1.setCookies(cookies2);
	NameValueCollection cookies3;
	request1.getCookies(cookies3);
	assertTrue (cookies3.size() == 3);
	assertTrue (cookies3["cookie1"] == "value1");
	assertTrue (cookies3["cookie2"] == "value2");
	assertTrue (cookies3["cookie3"] == "value3");

	HTTPRequest request3;
	request3.add("Cookie", "cookie1=value1");
	request3.add("cookie", "cookie2=value2");
	NameValueCollection cookies4;
	request3.getCookies(cookies4);
	assertTrue (cookies4.size() == 2);
	assertTrue (cookies4["cookie1"] == "value1");
	assertTrue (cookies4["cookie2"] == "value2");
}


void HTTPRequestTest::setUp()
{
}


void HTTPRequestTest::tearDown()
{
}


CppUnit::Test* HTTPRequestTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPRequestTest");

	CppUnit_addTest(pSuite, HTTPRequestTest, testWrite1);
	CppUnit_addTest(pSuite, HTTPRequestTest, testWrite2);
	CppUnit_addTest(pSuite, HTTPRequestTest, testWrite3);
	CppUnit_addTest(pSuite, HTTPRequestTest, testWrite4);
	CppUnit_addTest(pSuite, HTTPRequestTest, testRead1);
	CppUnit_addTest(pSuite, HTTPRequestTest, testRead2);
	CppUnit_addTest(pSuite, HTTPRequestTest, testRead3);
	CppUnit_addTest(pSuite, HTTPRequestTest, testRead4);
	CppUnit_addTest(pSuite, HTTPRequestTest, testInvalid1);
	CppUnit_addTest(pSuite, HTTPRequestTest, testInvalid2);
	CppUnit_addTest(pSuite, HTTPRequestTest, testInvalid3);
	CppUnit_addTest(pSuite, HTTPRequestTest, testCookies);

	return pSuite;
}
