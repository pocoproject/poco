//
// HTTPResponseTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPResponseTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/NetException.h"
#include <sstream>


using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPCookie;
using Poco::Net::MessageException;


HTTPResponseTest::HTTPResponseTest(const std::string& name): CppUnit::TestCase(name)
{
}


HTTPResponseTest::~HTTPResponseTest()
{
}


void HTTPResponseTest::testWrite1()
{
	HTTPResponse response;
	std::ostringstream ostr;
	response.write(ostr);
	std::string s = ostr.str();
	assertTrue (s == "HTTP/1.0 200 OK\r\n\r\n");
}


void HTTPResponseTest::testWrite2()
{
	HTTPResponse response(HTTPMessage::HTTP_1_1, HTTPResponse::HTTP_MOVED_PERMANENTLY);
	response.set("Location", "http://www.appinf.com/index.html");
	response.set("Server", "Poco/1.0");
	std::ostringstream ostr;
	response.write(ostr);
	std::string s = ostr.str();
	assertTrue (s == "HTTP/1.1 301 Moved Permanently\r\nLocation: http://www.appinf.com/index.html\r\nServer: Poco/1.0\r\n\r\n");
}


void HTTPResponseTest::testRead1()
{
	std::string s("HTTP/1.1 500 Internal Server Error\r\n\r\n");
	std::istringstream istr(s);
	HTTPResponse response;
	response.read(istr);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
	assertTrue (response.getReason() == "Internal Server Error");
	assertTrue (response.getVersion() == HTTPMessage::HTTP_1_1);
	assertTrue (response.empty());
	assertTrue (istr.get() == -1);
}


void HTTPResponseTest::testRead2()
{
	std::string s("HTTP/1.0 301 Moved Permanently\r\nLocation: http://www.appinf.com/index.html\r\nServer: Poco/1.0\r\n\r\n");
	std::istringstream istr(s);
	HTTPResponse response;
	response.read(istr);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_MOVED_PERMANENTLY);
	assertTrue (response.getReason() == "Moved Permanently");
	assertTrue (response.getVersion() == HTTPMessage::HTTP_1_0);
	assertTrue (response.size() == 2);
	assertTrue (response["Location"] == "http://www.appinf.com/index.html");
	assertTrue (response["Server"] == "Poco/1.0");
	assertTrue (istr.get() == -1);
}


void HTTPResponseTest::testRead3()
{
	std::string s("HTTP/1.1 200 \r\nContent-Length: 0\r\n\r\n");
	std::istringstream istr(s);
	HTTPResponse response;
	response.read(istr);
	assertTrue (response.getVersion() == HTTPMessage::HTTP_1_1);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
	assertTrue (response.getReason() == "");
	assertTrue (response.size() == 1);
	assertTrue (response.getContentLength() == 0);
	assertTrue (istr.get() == -1);
}


void HTTPResponseTest::testInvalid1()
{
	std::string s(256, 'x');
	std::istringstream istr(s);
	HTTPResponse response;
	try
	{
		response.read(istr);
		fail("inavalid response - must throw");
	}
	catch (MessageException&)
	{
	}
}


void HTTPResponseTest::testInvalid2()
{
	std::string s("HTTP/1.1 200 ");
	s.append(1000, 'x');
	s.append("\r\n\r\n");
	std::istringstream istr(s);
	HTTPResponse response;
	try
	{
		response.read(istr);
		fail("inavalid response - must throw");
	}
	catch (MessageException&)
	{
	}
}


void HTTPResponseTest::testInvalid3()
{
	std::string s("HTTP/1.0 ");
	s.append(8000, 'x');
	s.append("\r\n\r\n");
	std::istringstream istr(s);
	HTTPResponse response;
	try
	{
		response.read(istr);
		fail("inavalid response - must throw");
	}
	catch (MessageException&)
	{
	}
}


void HTTPResponseTest::testCookies()
{
	HTTPResponse response;
	HTTPCookie cookie1("cookie1", "value1");
	response.addCookie(cookie1);
	std::vector<HTTPCookie> cookies;
	response.getCookies(cookies);
	assertTrue (cookies.size() == 1);
	assertTrue (cookie1.getVersion() == cookies[0].getVersion());
	assertTrue (cookie1.getName() == cookies[0].getName());
	assertTrue (cookie1.getValue() == cookies[0].getValue());
	assertTrue (cookie1.getComment() == cookies[0].getComment());
	assertTrue (cookie1.getDomain() == cookies[0].getDomain());
	assertTrue (cookie1.getPath() == cookies[0].getPath());
	assertTrue (cookie1.getSecure() == cookies[0].getSecure());
	assertTrue (cookie1.getMaxAge() == cookies[0].getMaxAge());

	HTTPCookie cookie2("cookie2", "value2");
	cookie2.setVersion(1);
	cookie2.setMaxAge(42);
	cookie2.setSecure(true);
	response.addCookie(cookie2);
	response.getCookies(cookies);
	assertTrue (cookies.size() == 2);
	HTTPCookie cookie2a;
	if (cookies[0].getName() == cookie2.getName())
		cookie2a = cookies[0];
	else
		cookie2a = cookies[1];
	assertTrue (cookie2.getVersion() == cookie2a.getVersion());
	assertTrue (cookie2.getName() == cookie2a.getName());
	assertTrue (cookie2.getValue() == cookie2a.getValue());
	assertTrue (cookie2.getComment() == cookie2a.getComment());
	assertTrue (cookie2.getDomain() == cookie2a.getDomain());
	assertTrue (cookie2.getPath() == cookie2a.getPath());
	assertTrue (cookie2.getSecure() == cookie2a.getSecure());
	assertTrue (cookie2.getMaxAge() == cookie2a.getMaxAge());

	HTTPResponse response2;
	response2.add("Set-Cookie", "name1=value1");
	response2.add("Set-cookie", "name2=value2");
	cookies.clear();
	response2.getCookies(cookies);
	assertTrue (cookies.size() == 2);
	assertTrue (((cookies[0].getName() == "name1") && (cookies[1].getName() == "name2"))
	     || ((cookies[0].getName() == "name2") && (cookies[1].getName() == "name1")));
}


void HTTPResponseTest::testReplaceCookie()
{
	HTTPResponse response;
	HTTPCookie cookie1("cookie1", "value1");
	response.replaceCookie(cookie1); // cookie does not exist, will add cookie
	std::vector<HTTPCookie> cookies;
	response.getCookies(cookies);
	assertTrue (cookies.size() == 1);
	assertTrue (cookie1.getName() == cookies[0].getName());
	assertTrue (cookie1.getValue() == cookies[0].getValue());

	HTTPCookie cookie1new("cookie1", "value2");
	response.replaceCookie(cookie1new);
	cookies.clear();
	response.getCookies(cookies);
	assertTrue (cookies.size() == 1);
	assertTrue (cookie1new.getName() == cookies[0].getName());
	assertTrue (cookie1new.getValue() == cookies[0].getValue());
}


void HTTPResponseTest::testRemoveCookie()
{
	HTTPResponse response;
	HTTPCookie cookie1("cookie1", "value1");
	response.addCookie(cookie1); 
	std::vector<HTTPCookie> cookies;
	response.getCookies(cookies);
	assertTrue (cookies.size() == 1);
	assertTrue (cookie1.getName() == cookies[0].getName());
	assertTrue (cookie1.getValue() == cookies[0].getValue());

	response.removeCookie("cookie1");
	cookies.clear();
	response.getCookies(cookies);
	assertTrue (cookies.size() == 0);

	response.removeCookie("cookie2"); // should do nothing
}


void HTTPResponseTest::setUp()
{
}


void HTTPResponseTest::tearDown()
{
}


CppUnit::Test* HTTPResponseTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPResponseTest");

	CppUnit_addTest(pSuite, HTTPResponseTest, testWrite1);
	CppUnit_addTest(pSuite, HTTPResponseTest, testWrite2);
	CppUnit_addTest(pSuite, HTTPResponseTest, testRead1);
	CppUnit_addTest(pSuite, HTTPResponseTest, testRead2);
	CppUnit_addTest(pSuite, HTTPResponseTest, testRead3);
	CppUnit_addTest(pSuite, HTTPResponseTest, testInvalid1);
	CppUnit_addTest(pSuite, HTTPResponseTest, testInvalid2);
	CppUnit_addTest(pSuite, HTTPResponseTest, testInvalid3);
	CppUnit_addTest(pSuite, HTTPResponseTest, testCookies);
	CppUnit_addTest(pSuite, HTTPResponseTest, testReplaceCookie);
	CppUnit_addTest(pSuite, HTTPResponseTest, testRemoveCookie);

	return pSuite;
}
