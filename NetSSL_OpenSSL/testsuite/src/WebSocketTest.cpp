//
// WebSocketTest.cpp
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "WebSocketTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Thread.h"


using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::SocketStream;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;


namespace
{
	class WebSocketRequestHandler: public Poco::Net::HTTPRequestHandler
	{
	public:
		WebSocketRequestHandler(std::size_t bufSize = 1024): _bufSize(bufSize)
		{
		}

		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			try
			{
				WebSocket ws(request, response);
				std::unique_ptr<char[]> pBuffer(new char[_bufSize]);
				int flags;
				int n;
				do
				{
					n = ws.receiveFrame(pBuffer.get(), static_cast<int>(_bufSize), flags);
					Poco::Thread::current()->sleep(handleDelay.totalMilliseconds());
					if (n == 0)
						break;
					ws.sendFrame(pBuffer.get(), n, flags);
				}
				while ((flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
			}
			catch (const WebSocketException& exc)
			{
				switch (exc.code())
				{
				case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
					response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
					// fallthrough
				case WebSocket::WS_ERR_NO_HANDSHAKE:
				case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
				case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
					response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
					response.setContentLength(0);
					response.send();
					break;
				}
			}
		}

	public:

		static Poco::Timespan	handleDelay;

	private:
		std::size_t _bufSize;
	};

	Poco::Timespan WebSocketRequestHandler::handleDelay {0};


	class WebSocketRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		WebSocketRequestHandlerFactory(std::size_t bufSize = 1024): _bufSize(bufSize)
		{
		}

		Poco::Net::HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override
		{
			return new WebSocketRequestHandler(_bufSize);
		}

	private:
		std::size_t _bufSize;
	};
}


WebSocketTest::WebSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


WebSocketTest::~WebSocketTest()
{
}


void WebSocketTest::testWebSocketTimeout()
{
	Poco::Net::SecureServerSocket ss(0);
	Poco::Net::HTTPServer server(new WebSocketRequestHandlerFactory, ss, new Poco::Net::HTTPServerParams);
	server.start();

	Poco::Thread::sleep(200);

	HTTPSClientSession cs("127.0.0.1", ss.address().port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/ws");
	HTTPResponse response;
	WebSocket ws(cs, request, response);
	ws.setSendTimeout( Poco::Timespan(2, 0));
	ws.setReceiveTimeout( Poco::Timespan(2, 0));

	Poco::Timestamp sendStart;
	char buffer[1024] = {};
	int flags;
	try
	{
		// Server will take long to process and cause WS timeout
		WebSocketRequestHandler::handleDelay.assign(3, 0);

		std::string payload("x");
		ws.sendFrame(payload.data(), (int) payload.size());
		ws.receiveFrame(buffer, sizeof(buffer), flags);

		failmsg("Data exchange shall time out.");
	}
	catch (const Poco::TimeoutException& te)
	{
		assertTrue(sendStart.elapsed() < Poco::Timespan(4, 0).totalMicroseconds());
	}

	ws.shutdown();
	ws.receiveFrame(buffer, sizeof(buffer), flags);
	server.stop();
}


void WebSocketTest::testWebSocket()
{
	Poco::Net::SecureServerSocket ss(0);
	Poco::Net::HTTPServer server(new WebSocketRequestHandlerFactory, ss, new Poco::Net::HTTPServerParams);
	server.start();

	Poco::Thread::sleep(200);

	HTTPSClientSession cs("127.0.0.1", ss.address().port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/ws");
	HTTPResponse response;
	WebSocket ws(cs, request, response);

	std::string payload("x");
	ws.sendFrame(payload.data(), (int) payload.size());
	char buffer[1024] = {};
	int flags;
	int n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	assertTrue (n == payload.size());
	assertTrue (payload.compare(0, payload.size(), buffer, 0, n) == 0);
	assertTrue (flags == WebSocket::FRAME_TEXT);

	for (int i = 2; i < 20; i++)
	{
		payload.assign(i, 'x');
		ws.sendFrame(payload.data(), (int) payload.size());
		n = ws.receiveFrame(buffer, sizeof(buffer), flags);
		assertTrue (n == payload.size());
		assertTrue (payload.compare(0, payload.size(), buffer, 0, n) == 0);
		assertTrue (flags == WebSocket::FRAME_TEXT);
	}

	for (int i = 125; i < 129; i++)
	{
		payload.assign(i, 'x');
		ws.sendFrame(payload.data(), (int) payload.size());
		n = ws.receiveFrame(buffer, sizeof(buffer), flags);
		assertTrue (n == payload.size());
		assertTrue (payload.compare(0, payload.size(), buffer, 0, n) == 0);
		assertTrue (flags == WebSocket::FRAME_TEXT);
	}

	payload = "Hello, world!";
	ws.sendFrame(payload.data(), (int) payload.size());
	n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	assertTrue (n == payload.size());
	assertTrue (payload.compare(0, payload.size(), buffer, 0, n) == 0);
	assertTrue (flags == WebSocket::FRAME_TEXT);

	payload = "Hello, universe!";
	ws.sendFrame(payload.data(), (int) payload.size(), WebSocket::FRAME_BINARY);
	n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	assertTrue (n == payload.size());
	assertTrue (payload.compare(0, payload.size(), buffer, 0, n) == 0);
	assertTrue (flags == WebSocket::FRAME_BINARY);

	ws.shutdown();
	n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	assertTrue (n == 2);
	assertTrue ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_CLOSE);

	server.stop();
}


void WebSocketTest::testWebSocketLarge()
{
	const int msgSize = 64000;

	Poco::Net::SecureServerSocket ss(0);
	Poco::Net::HTTPServer server(new WebSocketRequestHandlerFactory(msgSize), ss, new Poco::Net::HTTPServerParams);
	server.start();

	Poco::Thread::sleep(200);

	HTTPSClientSession cs("127.0.0.1", ss.address().port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/ws");
	HTTPResponse response;
	WebSocket ws(cs, request, response);
	ws.setSendBufferSize(msgSize);
	ws.setReceiveBufferSize(msgSize);
	std::string payload(msgSize, 'x');
	SocketStream sstr(ws);
	sstr << payload;
	sstr.flush();

	char buffer[msgSize + 1] = {};
	int flags;
	int n = 0;
	do
	{
		n += ws.receiveFrame(buffer + n, sizeof(buffer) - n, flags);
	} while (n > 0 && n < msgSize);

	assertTrue (n == payload.size());
	assertTrue (payload.compare(0, payload.size(), buffer, 0, n) == 0);

	server.stop();
}


void WebSocketTest::setUp()
{
}


void WebSocketTest::tearDown()
{
}


CppUnit::Test* WebSocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("WebSocketTest");

	CppUnit_addTest(pSuite, WebSocketTest, testWebSocket);
	CppUnit_addTest(pSuite, WebSocketTest, testWebSocketTimeout);
	CppUnit_addTest(pSuite, WebSocketTest, testWebSocketLarge);

	return pSuite;
}
