//
// HTTPSStreamFactoryTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPSStreamFactoryTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#include "Poco/Net/NetException.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/StreamCopier.h"
#include "HTTPSTestServer.h"
#include <sstream>
#include <memory>


using Poco::Net::HTTPSStreamFactory;
using Poco::Net::NetException;
using Poco::Net::HTTPException;
using Poco::Util::Application;
using Poco::URI;
using Poco::StreamCopier;


HTTPSStreamFactoryTest::HTTPSStreamFactoryTest(const std::string& name): CppUnit::TestCase(name)
{
}


HTTPSStreamFactoryTest::~HTTPSStreamFactoryTest()
{
}


void HTTPSStreamFactoryTest::testNoRedirect()
{
	HTTPSTestServer server;
	HTTPSStreamFactory factory;
	URI uri("https://127.0.0.1/large");
	uri.setPort(server.port());
	std::unique_ptr<std::istream> pStr(factory.open(uri));
	std::ostringstream ostr;
	StreamCopier::copyStream(*pStr.get(), ostr);
	assertTrue (ostr.str() == HTTPSTestServer::LARGE_BODY);
}


void HTTPSStreamFactoryTest::testEmptyPath()
{
	HTTPSTestServer server;
	HTTPSStreamFactory factory;
	URI uri("https://127.0.0.1");
	uri.setPort(server.port());
	std::unique_ptr<std::istream> pStr(factory.open(uri));
	std::ostringstream ostr;
	StreamCopier::copyStream(*pStr.get(), ostr);
	assertTrue (ostr.str() == HTTPSTestServer::SMALL_BODY);
}


void HTTPSStreamFactoryTest::testRedirect()
{
	HTTPSTestServer server;
	HTTPSStreamFactory factory;
	URI uri("https://127.0.0.1/redirect");
	uri.setPort(server.port());
	std::unique_ptr<std::istream> pStr(factory.open(uri));
	std::ostringstream ostr;
	StreamCopier::copyStream(*pStr.get(), ostr);
	assertTrue (ostr.str() == HTTPSTestServer::LARGE_BODY);
}


void HTTPSStreamFactoryTest::testRedirectCredentials()
{
	// The credentials of the URI ("user:secret", base64 "dXNlcjpzZWNyZXQ=")
	// go to the redirect target only when it has the same origin.
	HTTPSTestServer server;
	HTTPSTestServer otherServer;
	HTTPSStreamFactory factory;

	URI uri("https://user:secret@127.0.0.1/authRedirect?to=/auth");
	uri.setPort(server.port());
	std::unique_ptr<std::istream> pStr(factory.open(uri));
	std::ostringstream ostr;
	StreamCopier::copyStream(*pStr.get(), ostr);
	assertTrue (ostr.str() == HTTPSTestServer::SMALL_BODY);
	assertTrue (server.lastRequest().find("Authorization: Basic dXNlcjpzZWNyZXQ=") != std::string::npos);

	// A 401 and then a redirect to another origin.
	const std::string otherLocation("https://127.0.0.1:" + std::to_string(otherServer.port()) + "/auth");
	uri = URI("https://user:secret@127.0.0.1/authRedirect?to=" + otherLocation);
	uri.setPort(server.port());
	pStr.reset(factory.open(uri));
	assertTrue (otherServer.lastRequest().substr(0, 9) == "GET /auth");
	assertTrue (otherServer.lastRequest().find("dXNlcjpzZWNyZXQ=") == std::string::npos);

	// A redirect to another origin before any 401.
	uri = URI("https://user:secret@127.0.0.1/moved?to=" + otherLocation);
	uri.setPort(server.port());
	pStr.reset(factory.open(uri));
	assertTrue (otherServer.lastRequest().substr(0, 9) == "GET /auth");
	assertTrue (otherServer.lastRequest().find("dXNlcjpzZWNyZXQ=") == std::string::npos);
}


void HTTPSStreamFactoryTest::testProxy()
{
	HTTPSTestServer server;
	HTTPSStreamFactory factory(
		Application::instance().config().getString("testsuite.proxy.host"),
		Application::instance().config().getInt("testsuite.proxy.port")
	);
	URI uri("https://secure.appinf.com/public/poco/NetSSL.txt");
	std::unique_ptr<std::istream> pStr(factory.open(uri));
	std::ostringstream ostr;
	StreamCopier::copyStream(*pStr.get(), ostr);
	assertTrue (ostr.str().length() > 0);
}


void HTTPSStreamFactoryTest::testError()
{
	HTTPSTestServer server;
	HTTPSStreamFactory factory;
	URI uri("https://127.0.0.1/notfound");
	uri.setPort(server.port());
	try
	{
		factory.open(uri);
		fail("not found - must throw");
	}
	catch (HTTPException& exc)
	{
		std::string m = exc.displayText();
	}
}


void HTTPSStreamFactoryTest::setUp()
{
}


void HTTPSStreamFactoryTest::tearDown()
{
}


CppUnit::Test* HTTPSStreamFactoryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPSStreamFactoryTest");

	CppUnit_addTest(pSuite, HTTPSStreamFactoryTest, testNoRedirect);
	CppUnit_addTest(pSuite, HTTPSStreamFactoryTest, testEmptyPath);
	CppUnit_addTest(pSuite, HTTPSStreamFactoryTest, testRedirect);
	CppUnit_addTest(pSuite, HTTPSStreamFactoryTest, testRedirectCredentials);
	CppUnit_addTest(pSuite, HTTPSStreamFactoryTest, testProxy);
	CppUnit_addTest(pSuite, HTTPSStreamFactoryTest, testError);

	return pSuite;
}
