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
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include "Poco/Buffer.h"
#include "Poco/Event.h"
#include "Poco/Mutex.h"
#include "Poco/SharedPtr.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include <atomic>


using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::SocketStream;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;
using Poco::Net::ConnectionAbortedException;
using Poco::IOException;


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
				Poco::Buffer<char> buffer(_bufSize);
				int flags;
				int n;
				do
				{
					n = ws.receiveFrame(buffer.begin(), static_cast<int>(buffer.size()), flags);
					if (n > 0) ws.sendFrame(buffer.begin(), n, flags);
				}
				while (n > 0 && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
			}
			catch (WebSocketException& exc)
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
			catch (ConnectionAbortedException&)
			{
			}
			catch (IOException&)
			{
			}
		}

	private:
		std::size_t _bufSize;
	};

	class WebSocketRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		WebSocketRequestHandlerFactory(std::size_t bufSize = 1024): _bufSize(bufSize)
		{
		}

		Poco::Net::HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
		{
			return new WebSocketRequestHandler(_bufSize);
		}

	private:
		std::size_t _bufSize;
	};

	struct SingleFrameState: public Poco::RefCountedObject
		/// Shared with SingleFrameRequestHandler: the WebSocket it reads from,
		/// so that a reader which does not return can still be released, and
		/// the outcome of that read. Reference counted, because a reader that
		/// never returns outlives the test that started it.
	{
		using Ptr = Poco::AutoPtr<SingleFrameState>;

		Poco::FastMutex mutex;
		Poco::SharedPtr<WebSocket> pWebSocket;
		Poco::Event upgraded;
		Poco::Event done;
		std::atomic<int> received{-1};
		std::atomic<int> flags{-1};
		std::atomic<int> errorCode{-1};
			/// The code of the WebSocketException caught by the reader,
			/// or -1 if either no exception was thrown or a non-WebSocket
			/// exception was.
		bool blocking = true;
			/// Whether the reader reads the frame in blocking mode. A
			/// non-blocking reader retries until the frame is complete, the
			/// connection is reported closed, or its own deadline passes.
		Poco::UInt8 allowedRSV = 0;
		int bufferSize = 64;
	};

	class SingleFrameRequestHandler: public Poco::Net::HTTPRequestHandler
		/// Upgrades the connection, then reads one frame and reports what
		/// that read returned.
	{
	public:
		SingleFrameRequestHandler(SingleFrameState::Ptr pState): _pState(pState)
		{
		}

		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			Poco::SharedPtr<WebSocket> pWebSocket = new WebSocket(request, response);
			pWebSocket->setAllowedRSVBits(_pState->allowedRSV);
			if (!_pState->blocking) pWebSocket->setBlocking(false);
			{
				Poco::FastMutex::ScopedLock lock(_pState->mutex);
				_pState->pWebSocket = pWebSocket;
			}
			_pState->upgraded.set();

			Poco::Buffer<char> buffer(_pState->bufferSize);
			int flags = 0;
			try
			{
				int n = pWebSocket->receiveFrame(buffer.begin(), static_cast<int>(buffer.size()), flags);
				if (!_pState->blocking)
				{
					// A non-blocking read answers with a negative value while
					// the frame is incomplete. Retry until it is complete or
					// the connection is reported closed.
					Poco::Timestamp start;
					while (n < 0 && !start.isElapsed(10*1000000))
					{
						Poco::Thread::sleep(1);
						n = pWebSocket->receiveFrame(buffer.begin(), static_cast<int>(buffer.size()), flags);
					}
				}
				_pState->received = n;
				_pState->flags = flags;
			}
			catch (WebSocketException& exc)
			{
				_pState->received = -1;
				_pState->errorCode = exc.code();
			}
			catch (Poco::Exception&)
			{
				_pState->received = -1;
			}
			_pState->done.set();
		}

	private:
		SingleFrameState::Ptr _pState;
	};

	class SingleFrameRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		SingleFrameRequestHandlerFactory(SingleFrameState::Ptr pState): _pState(pState)
		{
		}

		Poco::Net::HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
		{
			return new SingleFrameRequestHandler(_pState);
		}

	private:
		SingleFrameState::Ptr _pState;
	};
}


WebSocketTest::WebSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


WebSocketTest::~WebSocketTest()
{
}


void WebSocketTest::testWebSocket()
{
	Poco::Net::ServerSocket ss(0);
	Poco::Net::HTTPServer server(new WebSocketRequestHandlerFactory, ss, new Poco::Net::HTTPServerParams);
	server.start();

	Poco::Thread::sleep(200);

	HTTPClientSession cs("127.0.0.1", ss.address().port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/ws", HTTPRequest::HTTP_1_1);
	HTTPResponse response;
	WebSocket ws0 = WebSocket(cs, request, response);
	WebSocket ws(std::move(ws0));
#ifdef POCO_NEW_STATE_ON_MOVE
	assertTrue(ws0.impl() == nullptr);
#endif

	std::string payload("x");
	ws.sendFrame(payload.data(), (int) payload.size());
	char buffer[1024] = {};
	int flags;
	int n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	assertTrue (n == payload.size());
	assertTrue (payload.compare(0, payload.size(), buffer, n) == 0);
	assertTrue (flags == WebSocket::FRAME_TEXT);

	for (int i = 2; i < 20; i++)
	{
		payload.assign(i, 'x');
		ws.sendFrame(payload.data(), (int) payload.size());
		n = ws.receiveFrame(buffer, sizeof(buffer), flags);
		assertTrue (n == payload.size());
		assertTrue (payload.compare(0, payload.size(), buffer, n) == 0);
		assertTrue (flags == WebSocket::FRAME_TEXT);

		ws.sendFrame(payload.data(), (int) payload.size());
		Poco::Buffer<char> pocobuffer(0);
		assertTrue(0 == pocobuffer.size());
		n = ws.receiveFrame(pocobuffer, flags);
		assertTrue (n == payload.size());
		assertTrue (n == pocobuffer.size());
		assertTrue (payload.compare(0, payload.size(), pocobuffer.begin(), n) == 0);
		assertTrue (flags == WebSocket::FRAME_TEXT);
	}

	for (int i = 125; i < 129; i++)
	{
		payload.assign(i, 'x');
		ws.sendFrame(payload.data(), (int) payload.size());
		n = ws.receiveFrame(buffer, sizeof(buffer), flags);
		assertTrue (n == payload.size());
		assertTrue (payload.compare(0, payload.size(), buffer, n) == 0);
		assertTrue (flags == WebSocket::FRAME_TEXT);

		ws.sendFrame(payload.data(), (int) payload.size());
		Poco::Buffer<char> pocobuffer(0);
		n = ws.receiveFrame(pocobuffer, flags);
		assertTrue (n == payload.size());
		assertTrue (payload.compare(0, payload.size(), pocobuffer.begin(), n) == 0);
		assertTrue (flags == WebSocket::FRAME_TEXT);
	}

	payload = "Hello, world!";
	ws.sendFrame(payload.data(), (int) payload.size());
	n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	assertTrue (n == payload.size());
	assertTrue (payload.compare(0, payload.size(), buffer, n) == 0);
	assertTrue (flags == WebSocket::FRAME_TEXT);

	payload = "Hello, universe!";
	ws.sendFrame(payload.data(), (int) payload.size(), WebSocket::FRAME_BINARY);
	n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	assertTrue (n == payload.size());
	assertTrue (payload.compare(0, payload.size(), buffer, n) == 0);
	assertTrue (flags == WebSocket::FRAME_BINARY);

	ws.shutdown();
	n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	assertTrue (n == 2);
	assertTrue ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_CLOSE);

	ws.close();
	server.stop();
}


void WebSocketTest::testWebSocketLarge()
{
	const int msgSize = 64000;

	Poco::Net::ServerSocket ss(0);
	Poco::Net::HTTPServer server(new WebSocketRequestHandlerFactory(msgSize), ss, new Poco::Net::HTTPServerParams);
	server.start();

	Poco::Thread::sleep(200);

	HTTPClientSession cs("127.0.0.1", ss.address().port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/ws", HTTPRequest::HTTP_1_1);
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
	assertTrue (payload.compare(0, payload.size(), buffer, n) == 0);

	ws.close();
	server.stop();
}


void WebSocketTest::testOneLargeFrame(int msgSize)
{
	Poco::Net::ServerSocket ss(0);
	Poco::Net::HTTPServer server(new WebSocketRequestHandlerFactory(msgSize), ss, new Poco::Net::HTTPServerParams);
	server.start();

	Poco::Thread::sleep(200);

	HTTPClientSession cs("127.0.0.1", ss.address().port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/ws", HTTPRequest::HTTP_1_1);
	HTTPResponse response;
	WebSocket ws(cs, request, response);
	ws.setSendBufferSize(msgSize);
	ws.setReceiveBufferSize(msgSize);
	std::string payload(msgSize, 'x');

	ws.sendFrame(payload.data(), msgSize);

	Poco::Buffer<char> buffer(msgSize);
	int flags;
	int n;

	n = ws.receiveFrame(buffer.begin(), static_cast<int>(buffer.size()), flags);
	assertTrue (n == payload.size());
	assertTrue (payload.compare(0, payload.size(), buffer.begin(), n) == 0);

	ws.sendFrame(payload.data(), msgSize);

	Poco::Buffer<char> pocobuffer(0);

	n = ws.receiveFrame(pocobuffer, flags);
	assertTrue (n == payload.size());
	assertTrue (payload.compare(0, payload.size(), pocobuffer.begin(), n) == 0);

	ws.close();
	server.stop();
}


void WebSocketTest::testWebSocketLargeInOneFrame()
{
	testOneLargeFrame(64000);
	testOneLargeFrame(70000);
}


void WebSocketTest::testPeerCloseAfterPartialHeader()
{
	peerCloseAfterPartialHeader(true);
}


void WebSocketTest::testPeerCloseAfterPartialHeaderNB()
{
	peerCloseAfterPartialHeader(false);
}


void WebSocketTest::peerCloseAfterPartialHeader(bool blocking)
{
	SingleFrameState::Ptr pState = new SingleFrameState;
	pState->blocking = blocking;
	Poco::Net::ServerSocket ss(0);
	Poco::Net::HTTPServer server(new SingleFrameRequestHandlerFactory(pState), ss, new Poco::Net::HTTPServerParams);
	server.start();

	// The handshake is done by hand so that the frame following it can be
	// sent in pieces.
	HTTPClientSession cs("127.0.0.1", ss.address().port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/ws", HTTPRequest::HTTP_1_1);
	request.set("Connection", "Upgrade");
	request.set("Upgrade", "websocket");
	request.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
	request.set("Sec-WebSocket-Key", "dGhlIHNhbXBsZSBub25jZQ==");
	cs.setKeepAlive(true);
	cs.sendRequest(request);
	HTTPResponse response;
	cs.receiveResponse(response);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_SWITCHING_PROTOCOLS);
	Poco::Net::StreamSocket ws = cs.detachSocket();
	assertTrue (pState->upgraded.tryWait(10000));

	// The first two bytes of a masked text frame carrying four bytes of
	// payload. The four mask bytes that would complete its header never
	// arrive, because the peer closes instead.
	const char partialHeader[2] = {'\x81', '\x84'};
	ws.sendBytes(partialHeader, sizeof(partialHeader));
	// shutdownSend() rather than close(), which would send a reset if the
	// socket still held unread data
	ws.shutdownSend();

	// The peer is gone, so the header can never be completed: the read must
	// report the closed connection rather than wait for the rest of it.
	bool returned = pState->done.tryWait(10000);
	if (!returned)
	{
		// Release the reader, so that neither this server's shutdown nor
		// the rest of the suite waits for a thread that never returns. The
		// state is reference counted, so a reader that stays stuck through
		// this keeps what it writes to alive.
		Poco::FastMutex::ScopedLock lock(pState->mutex);
		if (pState->pWebSocket) pState->pWebSocket->close();
		pState->done.tryWait(10000);
	}
	assertTrue (returned);
	assertTrue (pState->received == 0);
	assertTrue (pState->flags == 0);

	ws.close();
	server.stop();
}


void WebSocketTest::testWebSocketNB()
{
	Poco::Net::ServerSocket ss(0);
	Poco::Net::HTTPServer server(new WebSocketRequestHandlerFactory(256*1024), ss, new Poco::Net::HTTPServerParams);
	server.start();
	
	Poco::Thread::sleep(200);
	
	HTTPClientSession cs("127.0.0.1", ss.address().port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/ws", HTTPRequest::HTTP_1_1);
	HTTPResponse response;
	WebSocket ws(cs, request, response);
	ws.setBlocking(false);

	int flags;
	char buffer[256*1024] = {};
	int n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	assertTrue (n < 0);

	std::string payload("x");
	n = ws.sendFrame(payload.data(), (int) payload.size());
	assertTrue (n > 0);
	if (ws.poll(1000000, Poco::Net::Socket::SELECT_READ))
	{
		n = ws.receiveFrame(buffer, sizeof(buffer), flags);
		while (n < 0)
		{
			n = ws.receiveFrame(buffer, sizeof(buffer), flags);
		}
	}
	assertTrue (n == payload.size());
	assertTrue (payload.compare(0, payload.size(), buffer, n) == 0);
	assertTrue (flags == WebSocket::FRAME_TEXT);

	ws.setSendBufferSize(256*1024);
	ws.setReceiveBufferSize(256*1024);

	payload.assign(256000, 'z');
	n = ws.sendFrame(payload.data(), (int) payload.size());
	assertTrue (n > 0);
	if (ws.poll(1000000, Poco::Net::Socket::SELECT_READ))
	{
		n = ws.receiveFrame(buffer, sizeof(buffer), flags);
		while (n < 0)
		{
			n = ws.receiveFrame(buffer, sizeof(buffer), flags);
		}
	}
	assertTrue (n == payload.size());
	assertTrue (payload.compare(0, payload.size(), buffer, n) == 0);
	assertTrue (flags == WebSocket::FRAME_TEXT);
	
	n = ws.shutdown();
	assertTrue (n > 0);

	n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	while (n < 0)
	{
		n = ws.receiveFrame(buffer, sizeof(buffer), flags);
	}
	assertTrue (n == 2);
	assertTrue ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_CLOSE);
	
	ws.close();
	server.stop();
}


int WebSocketTest::sendServerFrame(const std::string& frameBytes, Poco::UInt8 allowedRSV, int bufferSize, int* pFlags)
{
	SingleFrameState::Ptr pState = new SingleFrameState;
	pState->allowedRSV = allowedRSV;
	pState->bufferSize = bufferSize;
	Poco::Net::ServerSocket ss(0);
	Poco::Net::HTTPServer server(new SingleFrameRequestHandlerFactory(pState), ss, new Poco::Net::HTTPServerParams);
	server.start();

	HTTPClientSession cs("127.0.0.1", ss.address().port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/ws", HTTPRequest::HTTP_1_1);
	request.set("Upgrade", "websocket");
	request.set("Connection", "Upgrade");
	request.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
	request.set("Sec-WebSocket-Key", "dGhlIHNhbXBsZSBub25jZQ==");
	cs.setKeepAlive(true);
	cs.sendRequest(request);
	HTTPResponse response;
	cs.receiveResponse(response);
	assertTrue (response.getStatus() == HTTPResponse::HTTP_SWITCHING_PROTOCOLS);
	Poco::Net::StreamSocket sock = cs.detachSocket();
	assertTrue (pState->upgraded.tryWait(10000));

	sock.sendBytes(frameBytes.data(), static_cast<int>(frameBytes.size()));
	sock.shutdownSend();

	assertTrue (pState->done.tryWait(10000));
	int res = pState->received;
	int err = pState->errorCode;
	if (pFlags) *pFlags = pState->flags;

	sock.close();
	server.stop();

	return (res == -1) ? err : res;
}


void WebSocketTest::testMalformedFrames()
{
	// 1. Server rejects unmasked client frame
	{
		const std::string unmaskedFrame = "\x81\x01\x41";
		assertEqual (WebSocket::WS_ERR_CORRUPT_FRAME, sendServerFrame(unmaskedFrame));
	}

	// 2. Server rejects non-zero RSV bits by default
	{
		// RSV1 (0xC1) with mask
		const std::string rsv1Frame = "\xC1\x81\x01\x02\x03\x04\x40";
		assertEqual (WebSocket::WS_ERR_CORRUPT_FRAME, sendServerFrame(rsv1Frame));

		// RSV2 (0xA1) with mask
		const std::string rsv2Frame = "\xA1\x81\x01\x02\x03\x04\x40";
		assertEqual (WebSocket::WS_ERR_CORRUPT_FRAME, sendServerFrame(rsv2Frame));

		// RSV3 (0x91) with mask
		const std::string rsv3Frame = "\x91\x81\x01\x02\x03\x04\x40";
		assertEqual (WebSocket::WS_ERR_CORRUPT_FRAME, sendServerFrame(rsv3Frame));
	}

	// 3. Server accepts allowed RSV bits configured via setAllowedRSVBits
	{
		const std::string rsv1Frame = "\xC1\x81\x01\x02\x03\x04\x40";
		int flags = 0;
		int n = sendServerFrame(rsv1Frame, WebSocket::FRAME_FLAG_RSV1, 256, &flags);
		assertEqual (1, n);
		assertTrue ((flags & WebSocket::FRAME_FLAG_RSV1) != 0);
	}

	// 4. Server rejects reserved opcodes
	{
		// Reserved non-control opcode 0x03
		const std::string opcode3 = "\x83\x81\x01\x02\x03\x04\x40";
		assertEqual (WebSocket::WS_ERR_CORRUPT_FRAME, sendServerFrame(opcode3));

		// Reserved control opcode 0x0B
		const std::string opcodeB = "\x8B\x81\x01\x02\x03\x04\x40";
		assertEqual (WebSocket::WS_ERR_CORRUPT_FRAME, sendServerFrame(opcodeB));
	}

	// 5. Server rejects fragmented control frame (PING with FIN=0: 0x09)
	{
		const std::string fragPing = "\x09\x81\x01\x02\x03\x04\x40";
		assertEqual (WebSocket::WS_ERR_CORRUPT_FRAME, sendServerFrame(fragPing));
	}

	// 6. Control frame boundary: 125-byte accepted, 126/127-byte rejected
	{
		// 125-byte PING (accepted)
		std::string ping125;
		ping125.push_back('\x89');
		ping125.push_back('\xFD'); // 125 with mask bit
		ping125.append("\x01\x02\x03\x04", 4);
		ping125.append(125, '\x00');
		assertEqual (125, sendServerFrame(ping125, 0, 256));

		// 126-byte PING with 16-bit length (rejected)
		std::string ping126;
		ping126.push_back('\x89');
		ping126.push_back('\xFE'); // 126 with mask bit
		ping126.push_back('\x00');
		ping126.push_back('\x7E'); // 126
		ping126.append("\x01\x02\x03\x04", 4);
		ping126.append(126, '\x00');
		assertEqual (WebSocket::WS_ERR_CORRUPT_FRAME, sendServerFrame(ping126, 0, 256));

		// 64-bit length 127 encoding on control frame (rejected)
		std::string ping127;
		ping127.push_back('\x89');
		ping127.push_back('\xFF'); // 127 with mask bit
		ping127.append(8, '\x00');
		ping127.append("\x01\x02\x03\x04", 4);
		assertEqual (WebSocket::WS_ERR_CORRUPT_FRAME, sendServerFrame(ping127, 0, 256));
	}

	// 7. Client rejects masked frame from server
	{
		class MaskedFrameRequestHandler: public Poco::Net::HTTPRequestHandler
		{
		public:
			void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
			{
				try
				{
					response.setStatusAndReason(HTTPResponse::HTTP_SWITCHING_PROTOCOLS);
					response.set("Upgrade", "websocket");
					response.set("Connection", "Upgrade");
					std::string key = request.get("Sec-WebSocket-Key", "");
					response.set("Sec-WebSocket-Accept", WebSocket::computeAccept(key));
					Poco::Net::StreamSocket sock = response.detachSocket();

					// Send masked frame to client (illegal per RFC 6455)
					const char maskedFrame[] = {'\x81', '\x81', '\x01', '\x02', '\x03', '\x04', '\x40'};
					sock.sendBytes(maskedFrame, sizeof(maskedFrame));
					sock.shutdownSend();
					sock.close();
				}
				catch (Poco::Exception&)
				{
				}
			}
		};

		class MaskedFrameRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
		{
		public:
			Poco::Net::HTTPRequestHandler* createRequestHandler(const HTTPServerRequest&)
			{
				return new MaskedFrameRequestHandler;
			}
		};

		Poco::Net::ServerSocket ss(0);
		Poco::Net::HTTPServer server(new MaskedFrameRequestHandlerFactory, ss, new Poco::Net::HTTPServerParams);
		server.start();

		HTTPClientSession cs("127.0.0.1", ss.address().port());
		HTTPRequest request(HTTPRequest::HTTP_GET, "/ws", HTTPRequest::HTTP_1_1);
		HTTPResponse response;
		try
		{
			WebSocket ws(cs, request, response);
			char buffer[64];
			int flags = 0;
			ws.receiveFrame(buffer, sizeof(buffer), flags);
			fail("expected WebSocketException for masked frame on client");
		}
		catch (WebSocketException& exc)
		{
			assertEqual (WebSocket::WS_ERR_CORRUPT_FRAME, exc.code());
		}

		server.stop();
	}
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
	CppUnit_addTest(pSuite, WebSocketTest, testWebSocketLarge);
	CppUnit_addTest(pSuite, WebSocketTest, testWebSocketLargeInOneFrame);
	CppUnit_addTest(pSuite, WebSocketTest, testWebSocketNB);
	CppUnit_addTest(pSuite, WebSocketTest, testPeerCloseAfterPartialHeader);
	CppUnit_addTest(pSuite, WebSocketTest, testPeerCloseAfterPartialHeaderNB);
	CppUnit_addTest(pSuite, WebSocketTest, testMalformedFrames);

	return pSuite;
}
