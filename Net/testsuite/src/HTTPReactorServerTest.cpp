#include "HTTPReactorServerTest.h"
#include "Poco/Net/HTTPReactorServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/StreamCopier.h"
#include "CppUnit/TestSuite.h"
#include "CppUnit/TestCaller.h"


using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::StreamCopier;

namespace
{
	class EchoBodyRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			if (request.getChunkedTransferEncoding())
				response.setChunkedTransferEncoding(true);
			else if (request.getContentLength() != HTTPMessage::UNKNOWN_CONTENT_LENGTH && request.getContentLength() > 0)
				response.setContentLength(request.getContentLength());
			else if (request.getContentLength() == 0)
				response.setContentLength(0);
			response.setContentType(request.getContentType());
			
			if (request.getContentLength() > 0 || request.getChunkedTransferEncoding())
			{
				std::istream& istr = request.stream();
				std::ostream& ostr = response.send();
				StreamCopier::copyStream(istr, ostr);
				ostr.flush();
			}
			else
			{
				response.send();
			}
		}
	};

	class RequestHandlerFactory: public HTTPRequestHandlerFactory
	{
	public:
		HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
		{
			if (request.getURI() == "/unimpl")
			{
				return nullptr;
			}
			return new EchoBodyRequestHandler;
		}
	};
}

HTTPReactorServerTest::HTTPReactorServerTest(const std::string& name): CppUnit::TestCase(name)
{
}

HTTPReactorServerTest::~HTTPReactorServerTest()
{
}

void HTTPReactorServerTest::testBodyWithContentLength()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	std::string body("Hello World");
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setContentLength((int) body.length());
	request.setContentType("text/plain");
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	std::istream& responseStream = cs.receiveResponse(response);
	rbody.assign((std::istreambuf_iterator<char>(responseStream)), std::istreambuf_iterator<char>());
	
	assertTrue(response.getContentLength() == body.size());
	assertTrue(rbody == body);

	srv.stop();
}

void HTTPReactorServerTest::testBodyWithChunkedTransferEncoding()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	std::string body("Testing chunked");
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setContentType("text/plain");
	request.setChunkedTransferEncoding(true);
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	std::istream& rs = cs.receiveResponse(response); rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());

	assertTrue(response.getChunkedTransferEncoding());
	assertTrue(rbody == body);
	srv.stop();
}

void HTTPReactorServerTest::testBodyReadTwice()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	std::string body("Testing body twice");
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setContentLength((int) body.length());
	request.setContentType("text/plain");
	cs.sendRequest(request) << body;
	HTTPResponse response;

	std::istream& istr = cs.receiveResponse(response);

	char buffer1[32];
	istr.read(buffer1, 10);
	int n1 = istr.gcount();
	char buffer2[32];
	istr.read(buffer2, 50);
	int n2 = istr.gcount();

	std::string fullBody = std::string(buffer1, n1) + std::string(buffer2, n2);

	assertTrue(fullBody == body);
	srv.stop();
}

void HTTPReactorServerTest::testMultipleRequests()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	for (int i = 0; i < 3; ++i)
	{
		HTTPClientSession cs("127.0.0.1", port);
		std::string body("Request " + std::to_string(i + 1));
		HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
		request.setContentLength((int) body.length());
		cs.sendRequest(request) << body;
		HTTPResponse response;
		std::string rbody;
		std::istream& rs = cs.receiveResponse(response); rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());
		assertTrue(rbody == body);
	}
	srv.stop();
}

void HTTPReactorServerTest::testKeepAliveRequests()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(true);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	cs.setKeepAlive(true);

	for (int i = 0; i < 2; ++i)
	{
		std::string body("KA " + std::to_string(i + 1));
		HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
		request.setContentLength((int) body.length());
		cs.sendRequest(request) << body;
		HTTPResponse response;
		std::string rbody;
		std::istream& rs = cs.receiveResponse(response); rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());
		assertTrue(response.getKeepAlive());
		assertTrue(rbody == body);
	}

	cs.setKeepAlive(false);
	std::string body("Final");
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setContentLength((int) body.length());
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	std::istream& rs = cs.receiveResponse(response); rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());
	assertTrue(!response.getKeepAlive());
	assertTrue(rbody == body);
	srv.stop();
}

void HTTPReactorServerTest::testStickyBufferWithBody()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	std::string body(5000, 'x');
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setContentLength((int) body.length());
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	std::istream& rs = cs.receiveResponse(response); rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());

	assertTrue(rbody == body);
	srv.stop();
}

void HTTPReactorServerTest::testEmptyBody()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	std::string body = "";
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setContentLength(0);
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	std::istream& rs = cs.receiveResponse(response); rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());

	assertTrue(rbody.empty());
	srv.stop();
}

void HTTPReactorServerTest::testLargeBody()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	std::string body(50000, 'A');
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setContentLength((int) body.length());
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	std::istream& rs = cs.receiveResponse(response); rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());

	assertTrue(rbody == body);
	srv.stop();
}

void HTTPReactorServerTest::testChunkedMultipleChunks()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	std::string body(20000, 'B');
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setChunkedTransferEncoding(true);
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	std::istream& rs = cs.receiveResponse(response); rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());

	assertTrue(response.getChunkedTransferEncoding());
	assertTrue(rbody == body);
	srv.stop();
}

void HTTPReactorServerTest::testMixedRequests()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(true);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	cs.setKeepAlive(true);

	std::string body1("First");
	HTTPRequest request1("POST", "/", HTTPMessage::HTTP_1_1);
	request1.setContentLength((int) body1.length());
	cs.sendRequest(request1) << body1;
	HTTPResponse response1;
	std::string rbody1;
	cs.receiveResponse(response1) >> rbody1;
	assertTrue(rbody1 == body1);

	HTTPRequest request2("POST", "/", HTTPMessage::HTTP_1_1);
	request2.setChunkedTransferEncoding(true);
	std::string body2("Second");
	cs.sendRequest(request2) << body2;
	HTTPResponse response2;
	std::string rbody2;
	cs.receiveResponse(response2) >> rbody2;
	assertTrue(response2.getChunkedTransferEncoding());
	assertTrue(rbody2 == body2);
	srv.stop();
}

void HTTPReactorServerTest::testContentLengthZero()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	HTTPRequest request("GET", "/");
	request.setContentLength(0);
	cs.sendRequest(request);
	HTTPResponse response;
	std::istream& rs = cs.receiveResponse(response);
	std::string rbody;
	
	rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());
	
	assertTrue(rbody.empty());
	srv.stop();
}

void HTTPReactorServerTest::testChunkedWithEmptyChunks()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(1);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setChunkedTransferEncoding(true);
	std::string body = "";
	cs.sendRequest(request) << body;

	HTTPResponse response;
	std::string rbody;
	std::istream& rs = cs.receiveResponse(response);
	rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());

	assertTrue(response.getChunkedTransferEncoding());
	assertTrue(rbody.empty());
	srv.stop();
}

void HTTPReactorServerTest::testConcurrentRequests()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	for (int i = 0; i < 5; ++i)
	{
		HTTPClientSession cs("127.0.0.1", port);
		std::string body("Req " + std::to_string(i));
		HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
		request.setContentLength((int) body.length());
		cs.sendRequest(request) << body;
		HTTPResponse response;
		std::string rbody;
		std::istream& rs = cs.receiveResponse(response);
		rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());
		assertTrue(rbody == body);
	}
	srv.stop();
}

void HTTPReactorServerTest::testUseSelfReactor()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setReactorMode(true);
	pParams->setUseSelfReactor(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	std::string body("Test SelfReactor");
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setContentLength((int) body.length());
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	std::istream& rs = cs.receiveResponse(response);
	rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());
	assertTrue(response.getContentLength() == body.size());
	assertTrue(rbody == body);
	srv.stop();
}

void HTTPReactorServerTest::testNotImplementedResponseWithKeepAlive()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(true);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();

	int port = srv.port();

	HTTPClientSession cs("127.0.0.1", port);
	cs.setKeepAlive(true);
	HTTPRequest request("GET", "/unimpl", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	HTTPResponse response;
	cs.receiveResponse(response);
	assertTrue(response.getStatus() == Poco::Net::HTTPResponse::HTTP_NOT_IMPLEMENTED);
	assertTrue(!response.getKeepAlive());
	assertTrue(cs.getKeepAlive());

	HTTPRequest request2("GET", "/", HTTPMessage::HTTP_1_1);
	request2.setContentLength(0);
	cs.sendRequest(request2);
	std::string rbody;
	std::istream& rs = cs.receiveResponse(response);
	rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());
	assertTrue(response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK);
	assertTrue(response.getKeepAlive());
	assertTrue(rbody.empty());
	srv.stop();
}


void HTTPReactorServerTest::setUp()
{
}

void HTTPReactorServerTest::tearDown()
{
}

CppUnit::Test* HTTPReactorServerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPReactorServerTest");

	CppUnit_addTest(pSuite, HTTPReactorServerTest, testBodyWithContentLength);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testBodyWithChunkedTransferEncoding);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testBodyReadTwice);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testMultipleRequests);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testKeepAliveRequests);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testStickyBufferWithBody);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testEmptyBody);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testLargeBody);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testChunkedMultipleChunks);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testMixedRequests);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testContentLengthZero);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testChunkedWithEmptyChunks);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testConcurrentRequests);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testUseSelfReactor);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testNotImplementedResponseWithKeepAlive);

	return pSuite;
}
