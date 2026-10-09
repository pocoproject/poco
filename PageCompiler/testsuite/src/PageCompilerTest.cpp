//
// PageCompilerTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PageCompilerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/StreamCopier.h"
#include "Poco/InflatingStream.h"
#include "TestHandler.h"
#include "TestHandlerNoForm.h"
#include "StringifyHandler.h"
#include "StringifyHandlerNoForm.h"
#include "StringifyEscapeHandler.h"
#include "StringifyBufferedHandler.h"
#include "StringifyCompressedHandler.h"
#include <sstream>


namespace
{
	template <class H>
	class HandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override
		{
			return new H;
		}
	};


	template <class H>
	std::string get(const std::string& uri, const std::string& acceptEncoding = std::string(), std::string* pContentEncoding = nullptr)
		/// Serves one GET request with a handler of class H on a
		/// loopback HTTP server and returns the response body.
	{
		Poco::Net::ServerSocket socket(Poco::Net::SocketAddress("127.0.0.1", 0));
		Poco::Net::HTTPServer server(new HandlerFactory<H>, socket, new Poco::Net::HTTPServerParams);
		server.start();

		Poco::Net::HTTPClientSession session("127.0.0.1", socket.address().port());
		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri, Poco::Net::HTTPRequest::HTTP_1_1);
		if (!acceptEncoding.empty()) request.set("Accept-Encoding", acceptEncoding);
		session.sendRequest(request);
		Poco::Net::HTTPResponse response;
		std::istream& rs = session.receiveResponse(response);
		std::string body;
		Poco::StreamCopier::copyToString(rs, body);
		if (pContentEncoding != nullptr) *pContentEncoding = response.get("Content-Encoding", "");

		server.stop();
		return body;
	}
}


PageCompilerTest::PageCompilerTest(const std::string& name): CppUnit::TestCase(name)
{
}


PageCompilerTest::~PageCompilerTest()
{
}


void PageCompilerTest::testHandler()
{
	// Verify that the original page generates a concrete, linkable HTTP handler.
	AI::Test::TestHandler handler;
}


void PageCompilerTest::testHandlerNoForm()
{
	// Verify compilation with form disabled and request used in a pre-response scriptlet.
	AI::Test::TestHandlerNoForm handler;
}


void PageCompilerTest::testStringify()
{
	AI::Test::StringifyHandler handler;
	Poco::Net::HTMLForm form;
	form.set("name", "Poco");
	std::ostringstream ostr;

	handler.stringify(ostr, form);

	assertEquals (std::string("<p>Hello, Poco!</p>\n"), ostr.str());
}


void PageCompilerTest::testStringifyNoForm()
{
	AI::Test::StringifyHandlerNoForm handler;
	std::ostringstream ostr;

	handler.stringify(ostr);

	assertEquals (std::string("<p>Hello, World!</p>\n"), ostr.str());
}


void PageCompilerTest::testStringifyEscape()
{
	AI::Test::StringifyEscapeHandler handler;
	Poco::Net::HTMLForm form;
	std::ostringstream ostr;

	handler.stringify(ostr, form);

	assertEquals (std::string("<p>&lt;a&amp;b&gt;</p>\n"), ostr.str());
}


void PageCompilerTest::testHandleRequest()
{
	assertEquals (std::string("<p>Hello, Poco!</p>\n"), get<AI::Test::StringifyHandler>("/?name=Poco"));
}


void PageCompilerTest::testHandleRequestNoForm()
{
	assertEquals (std::string("<p>Hello, World!</p>\n"), get<AI::Test::StringifyHandlerNoForm>("/"));
}


void PageCompilerTest::testHandleRequestEscape()
{
	assertEquals (std::string("<p>&lt;a&amp;b&gt;</p>\n"), get<AI::Test::StringifyEscapeHandler>("/"));
}


void PageCompilerTest::testHandleRequestBuffered()
{
	assertEquals (std::string("<p>Hello, Poco!</p>\n"), get<AI::Test::StringifyBufferedHandler>("/?name=Poco"));
}


void PageCompilerTest::testHandleRequestCompressed()
{
	std::string contentEncoding;
	std::string body = get<AI::Test::StringifyCompressedHandler>("/?name=Poco", std::string(), &contentEncoding);
	assertEquals (std::string("<p>Hello, Poco!</p>\n"), body);
	assertEquals (std::string(), contentEncoding);

	body = get<AI::Test::StringifyCompressedHandler>("/?name=Poco", "gzip", &contentEncoding);
	assertEquals (std::string("gzip"), contentEncoding);
	std::istringstream istr(body);
	Poco::InflatingInputStream inflater(istr, Poco::InflatingStreamBuf::STREAM_GZIP);
	std::string inflated;
	Poco::StreamCopier::copyToString(inflater, inflated);
	assertEquals (std::string("<p>Hello, Poco!</p>\n"), inflated);
}


void PageCompilerTest::setUp()
{
}


void PageCompilerTest::tearDown()
{
}


CppUnit::Test* PageCompilerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PageCompilerTest");

	CppUnit_addTest(pSuite, PageCompilerTest, testHandler);
	CppUnit_addTest(pSuite, PageCompilerTest, testHandlerNoForm);
	CppUnit_addTest(pSuite, PageCompilerTest, testStringify);
	CppUnit_addTest(pSuite, PageCompilerTest, testStringifyNoForm);
	CppUnit_addTest(pSuite, PageCompilerTest, testStringifyEscape);
	CppUnit_addTest(pSuite, PageCompilerTest, testHandleRequest);
	CppUnit_addTest(pSuite, PageCompilerTest, testHandleRequestNoForm);
	CppUnit_addTest(pSuite, PageCompilerTest, testHandleRequestEscape);
	CppUnit_addTest(pSuite, PageCompilerTest, testHandleRequestBuffered);
	CppUnit_addTest(pSuite, PageCompilerTest, testHandleRequestCompressed);

	return pSuite;
}
