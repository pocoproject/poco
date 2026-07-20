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
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/StreamCopier.h"
#include "Poco/Thread.h"
#include "Poco/Timespan.h"
#include "Poco/Exception.h"
#include "CppUnit/TestSuite.h"
#include "CppUnit/TestCaller.h"
#include <stdexcept>


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

	// Throws a Poco exception from the handler. HTTPReactorServer sends a 500
	// then re-throws; the exception propagates out to onRead, which must catch
	// it and close the connection instead of leaking it.
	class ThrowPocoRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest&, HTTPServerResponse&)
		{
			throw Poco::ApplicationException("intentional handler failure (poco)");
		}
	};

	// Throws a non-Poco exception; HTTPReactorServer does not catch std::
	// exceptions, so this escapes all the way to onRead's std::exception guard.
	class ThrowStdRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest&, HTTPServerResponse&)
		{
			throw std::runtime_error("intentional handler failure (std)");
		}
	};

	// Sends a large fixed-length body. Used to exercise the send timeout: a
	// client that stops reading makes the server block in send until the
	// timeout fires.
	class BigBodyRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest&, HTTPServerResponse& response)
		{
			const std::size_t chunk = 64 * 1024;
			const int chunks = 512;                 // 32 MiB total
			response.setContentLength(static_cast<Poco::Int64>(chunk) * chunks);
			std::string data(chunk, 'A');
			std::ostream& ostr = response.send();
			for (int i = 0; i < chunks; ++i) ostr.write(data.data(), data.size());
			ostr.flush();
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
			if (request.getURI() == "/throw-poco") return new ThrowPocoRequestHandler;
			if (request.getURI() == "/throw-std")  return new ThrowStdRequestHandler;
			if (request.getURI() == "/big")        return new BigBodyRequestHandler;
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


void HTTPReactorServerTest::testSendTimeoutParam()
{
	// The new opt-in param defaults to disabled (0) so existing TCPServer users
	// are unaffected, and round-trips when set.
	HTTPServerParams::Ptr pParams = new HTTPServerParams;
	assertTrue(pParams->getSendTimeout().totalMicroseconds() == 0);
	pParams->setSendTimeout(Poco::Timespan(5, 0));
	assertTrue(pParams->getSendTimeout() == Poco::Timespan(5, 0));
}

void HTTPReactorServerTest::testClientAbortKeepsServerAlive()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(2);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();
	int port = srv.port();

	// Liveness guard for the new error-close path: abusively abort many
	// connections (connect, send a partial request, close with linger 0 so the
	// OS sends a RST). On a blocking accepted socket the reset makes
	// receiveBytes throw out of onRead, which must run handleClose and leave the
	// server serving. This is a crash/hang/deadlock smoke test - it does not by
	// itself prove the fd-leak/hot-spin is gone (the reactor thread survives a
	// swallowed exception even unpatched). testSendTimeoutClosesStalledClient is
	// the behavioral guard for the fix.
	for (int i = 0; i < 50; ++i)
	{
		Poco::Net::StreamSocket s;
		s.connect(Poco::Net::SocketAddress("127.0.0.1", port));
		s.setLinger(true, 0);                       // close -> RST
		std::string partial("GET / HTTP/1.1\r\nHost: x\r\n");  // no terminating blank line
		s.sendBytes(partial.data(), static_cast<int>(partial.size()));
		s.close();                                  // abortive
	}

	HTTPClientSession cs("127.0.0.1", port);
	cs.setTimeout(Poco::Timespan(10, 0));   // fail fast if a regression wedges the server
	std::string body("still alive");
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setContentLength((int) body.length());
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	std::istream& rs = cs.receiveResponse(response);
	rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());
	assertTrue(rbody == body);
	srv.stop();
}

void HTTPReactorServerTest::testHandlerExceptionKeepsServerAlive()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(2);
	pParams->setReactorMode(true);

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();
	int port = srv.port();

	// Liveness guard: fire requests at handlers that throw a Poco and a non-Poco
	// exception. onRead must catch both and close cleanly, leaving the server
	// serving. Short client timeouts so a regression that fails to close (the
	// pre-fix behavior on the std path, where the socket is never shut) fails
	// this test in seconds instead of stalling on the 60 s default timeout.
	const char* uris[] = { "/throw-poco", "/throw-std" };
	for (const char* uri : uris)
	{
		try
		{
			HTTPClientSession cs("127.0.0.1", port);
			cs.setTimeout(Poco::Timespan(10, 0));
			HTTPRequest request("GET", uri, HTTPMessage::HTTP_1_1);
			request.setContentLength(0);
			cs.sendRequest(request);
			HTTPResponse response;
			std::string rbody;
			std::istream& rs = cs.receiveResponse(response);
			rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());
			// A 500 (poco path) or a reset (std path) are both acceptable; we
			// only care that the server survives, asserted by the request below.
		}
		catch (const Poco::Exception&)
		{
		}
	}

	HTTPClientSession cs("127.0.0.1", port);
	cs.setTimeout(Poco::Timespan(10, 0));
	std::string body("still serving");
	HTTPRequest request("POST", "/", HTTPMessage::HTTP_1_1);
	request.setContentLength((int) body.length());
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	std::istream& rs = cs.receiveResponse(response);
	rbody.assign((std::istreambuf_iterator<char>(rs)), std::istreambuf_iterator<char>());
	assertTrue(rbody == body);
	srv.stop();
}

void HTTPReactorServerTest::testSendTimeoutClosesStalledClient()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	pParams->setMaxThreads(2);
	pParams->setReactorMode(true);
	pParams->setSendTimeout(Poco::Timespan(1, 0));   // 1 s

	Poco::Net::HTTPReactorServer srv(0, pParams, new RequestHandlerFactory);
	srv.start();
	int port = srv.port();

	// Request a 32 MiB body, then STOP reading. The server fills the socket
	// buffers and blocks in send; the send timeout turns that into a bounded
	// wait instead of wedging the worker reactor forever. After the timeout
	// fires the response is abandoned, so when we finally drain we hit EOF
	// quickly having received far less than the full body. Without the timeout
	// (or without a client that eventually reads) the send would block forever.
	Poco::Net::StreamSocket s;
	s.connect(Poco::Net::SocketAddress("127.0.0.1", port));
	std::string req("GET /big HTTP/1.1\r\nHost: x\r\nConnection: close\r\n\r\n");
	s.sendBytes(req.data(), static_cast<int>(req.size()));

	// Give the server's send time to stall and time out (timeout is 1 s).
	Poco::Thread::sleep(3000);

	s.setReceiveTimeout(Poco::Timespan(5, 0));
	std::size_t total = 0;
	bool closed = false;
	char buf[16 * 1024];
	try
	{
		for (;;)
		{
			int n = s.receiveBytes(buf, sizeof(buf));
			if (n <= 0) { closed = true; break; }
			total += static_cast<std::size_t>(n);
			if (total > 48u * 1024 * 1024) break;    // safety bound
		}
	}
	catch (const Poco::Exception&)
	{
		closed = true;                               // reset also means closed
	}
	s.close();

	assertTrue(closed);
	// Truncated well below the 32 MiB body: the stalled send was aborted rather
	// than run to completion once we started reading.
	assertTrue(total < 16u * 1024 * 1024);
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
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testSendTimeoutParam);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testClientAbortKeepsServerAlive);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testHandlerExceptionKeepsServerAlive);
	CppUnit_addTest(pSuite, HTTPReactorServerTest, testSendTimeoutClosesStalledClient);

	return pSuite;
}
