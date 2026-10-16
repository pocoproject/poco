//
// HTTPClientSessionPoolTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPClientSessionPoolTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPClientSessionPool.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPSessionFactory.h"
#include "Poco/Net/HTTPSessionInstantiator.h"
#include "Poco/Net/ProxyConfig.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Any.h"
#include "Poco/StreamCopier.h"
#include "Poco/Thread.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include "Poco/URI.h"
#include <atomic>
#include <thread>
#include <utility>
#include <vector>


using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPClientSessionPool;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPSessionFactory;
using Poco::Net::HTTPSessionInstantiator;
using Poco::Net::ProxyAuthentication;
using Poco::Net::ProxyConfig;
using Poco::Net::ServerSocket;
using Poco::Net::Socket;
using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;
using Poco::StreamCopier;


namespace
{
	class HelloRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override
		{
			response.setContentLength(5);
			response.send() << "hello";
		}
	};

	class HelloRequestHandlerFactory: public HTTPRequestHandlerFactory
	{
	public:
		HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override
		{
			return new HelloRequestHandler;
		}
	};

	class NTLMProxyRequestHandler: public HTTPRequestHandler
		/// Answers an NTLM negotiate message with a challenge and other requests with "hello".
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override
		{
			std::string scheme;
			std::string authInfo;
			if (request.hasProxyCredentials()) request.getProxyCredentials(scheme, authInfo);
			// Base64 of the NTLM signature and message type 1.
			if (authInfo.compare(0, 12, "TlRMTVNTUAAB") == 0)
			{
				response.setStatusAndReason(HTTPResponse::HTTP_PROXY_AUTHENTICATION_REQUIRED);
				// A challenge message that requests NTLMv2 and carries no target information.
				response.set("Proxy-Authenticate", "NTLM TlRMTVNTUAACAAAAAAAAAAAAAAACAggAASNFZ4mrze8=");
				response.setContentLength(0);
				response.send();
			}
			else
			{
				response.setContentLength(5);
				response.send() << "hello";
			}
		}
	};

	class NTLMProxyRequestHandlerFactory: public HTTPRequestHandlerFactory
	{
	public:
		HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override
		{
			return new NTLMProxyRequestHandler;
		}
	};

	class HTTPFactory: public HTTPSessionFactory
	{
	public:
		HTTPFactory()
		{
			registerProtocol("http", new HTTPSessionInstantiator);
		}
	};

	class ShortKeepAliveInstantiator: public HTTPSessionInstantiator
		/// Creates sessions with a short keep-alive timeout, one second by default.
	{
	public:
		explicit ShortKeepAliveInstantiator(const Poco::Timespan& keepAliveTimeout = Poco::Timespan(1, 0)):
			_keepAliveTimeout(keepAliveTimeout)
		{
		}

		HTTPClientSession* createClientSession(const Poco::URI& uri) override
		{
			HTTPClientSession* pSession = HTTPSessionInstantiator::createClientSession(uri);
			pSession->setKeepAliveTimeout(_keepAliveTimeout);
			return pSession;
		}

	private:
		Poco::Timespan _keepAliveTimeout;
	};

	Poco::URI localURI(Poco::UInt16 port)
	{
		return Poco::URI("http://127.0.0.1:" + std::to_string(port) + "/");
	}

	std::string readBody(std::istream& rs)
	{
		std::string body;
		StreamCopier::copyToString(rs, body);
		return body;
	}
}


HTTPClientSessionPoolTest::HTTPClientSessionPoolTest(const std::string& name): CppUnit::TestCase(name)
{
}


HTTPClientSessionPoolTest::~HTTPClientSessionPoolTest()
{
}


std::istream& HTTPClientSessionPoolTest::exchange(HTTPClientSession& session, ServerSocket& server, StreamSocket& peer, const std::string& response)
{
	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	return exchange(session, request, server, peer, response);
}


std::istream& HTTPClientSessionPoolTest::exchange(HTTPClientSession& session, HTTPRequest& request, ServerSocket& server, StreamSocket& peer, const std::string& response)
{
	const bool connected = session.connected();
	session.setTimeout(Poco::Timespan(10, 0));
	session.sendRequest(request);
	if (!connected) peer = server.acceptConnection();
	peer.sendBytes(response.data(), static_cast<int>(response.size()));
	HTTPResponse httpResponse;
	return session.receiveResponse(httpResponse);
}


void HTTPClientSessionPoolTest::testReuseAfterCompleteResponse()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	HTTPClientSession* pSession = nullptr;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
		pSession = &handle.session();
		assertTrue (handle->getKeepAlive());
		assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello")) == "hello");
	}
	assertTrue (pool.idleSessions() == 1);

	HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
	assertTrue (pool.idleSessions() == 0);
	assertTrue (&handle.session() == pSession);
	assertTrue (handle->connected());
	assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nagain")) == "again");
	assertTrue (!server.poll(Poco::Timespan(), Socket::SELECT_READ));
}


void HTTPClientSessionPoolTest::testReuseAfterChunkedResponse()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
		std::istream& rs = exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\n0\r\n\r\n");
		std::string body(5, '\0');
		rs.read(body.data(), static_cast<std::streamsize>(body.size()));
		assertTrue (rs.gcount() == 5 && body == "hello");
	}
	assertTrue (pool.idleSessions() == 1);

	HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
	assertTrue (handle->connected());
	assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nagain")) == "again");
}


void HTTPClientSessionPoolTest::testUnreadResponseNotKept()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
		// The rest of a body that has been received would be skipped.
		exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nContent-Length: 10\r\n\r\nhello");
	}
	assertTrue (pool.idleSessions() == 0);
}


void HTTPClientSessionPoolTest::testIncompleteRequestBodyNotKept()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
		handle->setTimeout(Poco::Timespan(10, 0));
		HTTPRequest request(HTTPRequest::HTTP_POST, "/", HTTPMessage::HTTP_1_1);
		request.setContentLength(10);
		handle->sendRequest(request) << "hello";
		peer = server.acceptConnection();
		const std::string response("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello");
		peer.sendBytes(response.data(), static_cast<int>(response.size()));
		HTTPResponse httpResponse;
		assertTrue (readBody(handle->receiveResponse(httpResponse)) == "hello");
	}
	assertTrue (pool.idleSessions() == 0);
}


void HTTPClientSessionPoolTest::testConnectionCloseNotKept()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
		assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: 5\r\n\r\nhello")) == "hello");
	}
	assertTrue (pool.idleSessions() == 0);
}


void HTTPClientSessionPoolTest::testSwitchingProtocolsNotReused()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
	assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 101 Switching Protocols\r\nConnection: Upgrade\r\nUpgrade: websocket\r\n\r\n")).empty());
	assertTrue (!handle->canReuse());

	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	handle->sendRequest(request);
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
}


void HTTPClientSessionPoolTest::testConnectTunnelNotReused()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
	handle->setTimeout(Poco::Timespan(10, 0));
	HTTPRequest connectRequest(HTTPRequest::HTTP_CONNECT, "example.com:443", HTTPMessage::HTTP_1_1);
	handle->sendRequest(connectRequest);
	StreamSocket peer = server.acceptConnection();
	const std::string response("HTTP/1.1 200 Connection established\r\nContent-Length: 0\r\n\r\n");
	peer.sendBytes(response.data(), static_cast<int>(response.size()));
	HTTPResponse httpResponse;
	assertTrue (readBody(handle->receiveResponse(httpResponse)).empty());
	assertTrue (!handle->canReuse());

	HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
	handle->sendRequest(request);
	assertTrue (server.poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
}


void HTTPClientSessionPoolTest::testSettingsRestored()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	HTTPClientSession* pSession = nullptr;
	Poco::Timespan connectTimeout;
	Poco::Timespan sendTimeout;
	Poco::Timespan receiveTimeout;
	Poco::Timespan keepAliveTimeout;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
		pSession = &handle.session();
		connectTimeout = handle->getConnectTimeout();
		sendTimeout = handle->getSendTimeout();
		receiveTimeout = handle->getReceiveTimeout();
		keepAliveTimeout = handle->getKeepAliveTimeout();
		handle->requestTrailer().set("X-Trailer", "secret");
		handle->setKeepAliveTimeout(Poco::Timespan(3600, 0));
		handle->attachSessionData(Poco::Any(42));
		// exchange() sets other timeouts.
		assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello")) == "hello");
	}
	assertTrue (pool.idleSessions() == 1);

	HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
	assertTrue (&handle.session() == pSession);
	assertTrue (handle->requestTrailer().empty());
	assertTrue (handle->getConnectTimeout() == connectTimeout);
	assertTrue (handle->getSendTimeout() == sendTimeout);
	assertTrue (handle->getReceiveTimeout() == receiveTimeout);
	assertTrue (handle->getKeepAliveTimeout() == keepAliveTimeout);
	assertTrue (handle->sessionData().empty());
}


void HTTPClientSessionPoolTest::testChangedProxyNotKept()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
		handle->setProxy("127.0.0.1", server.address().port(), "http", false);
		assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello")) == "hello");
		assertTrue (handle->canReuse());
	}
	assertTrue (pool.idleSessions() == 0);
}


void HTTPClientSessionPoolTest::testChangedHostNotKept()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(Poco::URI("http://localhost:" + std::to_string(server.address().port()) + "/"));
		handle->setHost("127.0.0.1");
		assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello")) == "hello");
		assertTrue (handle->canReuse());
	}
	assertTrue (pool.idleSessions() == 0);

	const ServerSocket other(SocketAddress("127.0.0.1", 0));
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(localURI(other.address().port()));
		handle->setPort(server.address().port());
		assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello")) == "hello");
		assertTrue (handle->canReuse());
	}
	assertTrue (pool.idleSessions() == 0);
}


void HTTPClientSessionPoolTest::testChangedProxyCredentialsNotKept()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	ProxyConfig proxyConfig;
	proxyConfig.host = "127.0.0.1";
	proxyConfig.port = server.address().port();
	proxyConfig.tunnel = false;
	proxyConfig.username = "pool";
	proxyConfig.password = "pool";
	factory.setProxyConfig(proxyConfig);
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(Poco::URI("http://example.com/"));
		handle->setProxyCredentials("user", "secret");
		assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello")) == "hello");
		assertTrue (handle->canReuse());
	}
	assertTrue (pool.idleSessions() == 0);
}


void HTTPClientSessionPoolTest::testNTLMProxySessionNotKept()
{
	HTTPServer srv(new NTLMProxyRequestHandlerFactory, ServerSocket(SocketAddress("127.0.0.1", 0)), new HTTPServerParams);
	srv.start();
	HTTPFactory factory;
	ProxyConfig proxyConfig;
	proxyConfig.host = "127.0.0.1";
	proxyConfig.port = srv.port();
	proxyConfig.tunnel = false;
	proxyConfig.username = "user";
	proxyConfig.password = "secret";
	proxyConfig.authMethod = ProxyAuthentication::NTLM;
	factory.setProxyConfig(proxyConfig);
	HTTPClientSessionPool pool(factory);
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(Poco::URI("http://example.com/"));
		handle->setTimeout(Poco::Timespan(10, 0));
		HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
		handle->sendRequest(request);
		HTTPResponse response;
		assertTrue (readBody(handle->receiveResponse(response)) == "hello");
		assertTrue (handle->canReuse());
	}
	srv.stop();
	assertTrue (srv.totalConnections() == 1);
	assertTrue (pool.idleSessions() == 0);
}


void HTTPClientSessionPoolTest::testConnectionAuthenticatedSessionNotKept()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	const std::string response("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello");
	const std::vector<std::pair<std::string, std::string>> credentials = {
		{HTTPRequest::AUTHORIZATION, "NTLM TlRMTVNTUAADAAAA"},
		{HTTPRequest::AUTHORIZATION, "Negotiate YIIB"},
		{HTTPRequest::AUTHORIZATION, "Kerberos YIIB"},
		{HTTPRequest::AUTHORIZATION, " NTLM TlRMTVNTUAADAAAA"},
		{HTTPRequest::PROXY_AUTHORIZATION, "NTLM TlRMTVNTUAADAAAA"}
	};
	for (const auto& [header, value]: credentials)
	{
		StreamSocket peer;
		{
			HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
			HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
			request.set(header, value);
			assertTrue (readBody(exchange(handle.session(), request, server, peer, response)) == "hello");
		}
		assertTrue (pool.idleSessions() == 0);
	}

	{
		StreamSocket peer;
		{
			HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
			HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
			request.set(HTTPRequest::AUTHORIZATION, "Basic dXNlcjpwYXNz");
			request.add(HTTPRequest::AUTHORIZATION, "NTLM TlRMTVNTUAADAAAA");
			assertTrue (readBody(exchange(handle.session(), request, server, peer, response)) == "hello");
		}
		assertTrue (pool.idleSessions() == 0);
	}

	{
		StreamSocket peer;
		{
			HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
			HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
			request.set(HTTPRequest::AUTHORIZATION, "Basic dXNlcjpwYXNz");
			assertTrue (readBody(exchange(handle.session(), request, server, peer, response)) == "hello");
		}
		assertTrue (pool.idleSessions() == 1);
		pool.clear();
	}

	StreamSocket peer;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
		HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
		request.set(HTTPRequest::AUTHORIZATION, "NTLM TlRMTVNTUAADAAAA");
		assertTrue (readBody(exchange(handle.session(), request, server, peer, response)) == "hello");
		handle->reset();
		assertTrue (readBody(exchange(handle.session(), server, peer, response)) == "hello");
	}
	assertTrue (pool.idleSessions() == 1);
}


void HTTPClientSessionPoolTest::testClosedIdleConnectionNotReused()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	HTTPClientSession* pSession = nullptr;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
		pSession = &handle.session();
		assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello")) == "hello");
	}
	assertTrue (pool.idleSessions() == 1);

	peer.close();
	// The idle session is not in use; waiting on its socket only observes the close.
	assertTrue (pSession->socket().poll(Poco::Timespan(10, 0), Socket::SELECT_READ));
	HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
	assertTrue (!handle->connected());
	assertTrue (pool.idleSessions() == 0);
}


void HTTPClientSessionPoolTest::testExpiredIdleSessionRemoved()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPSessionFactory factory;
	factory.registerProtocol("http", new ShortKeepAliveInstantiator);
	HTTPClientSessionPool pool(factory);
	StreamSocket peer;
	{
		HTTPClientSessionPool::Handle handle = pool.acquire(localURI(server.address().port()));
		assertTrue (readBody(exchange(handle.session(), server, peer, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello")) == "hello");
	}
	const Poco::Timestamp released;
	assertTrue (pool.idleSessions() == 1);

	// One second after the release, the keep-alive timeout has expired and the
	// idle sessions of other endpoints are checked again.
	const Poco::Timespan interval(1, 0);
	while (interval >= released.elapsed()) Poco::Thread::sleep(50);
	HTTPClientSessionPool::Handle handle = pool.acquire(localURI(1));
	assertTrue (pool.idleSessions() == 0);
}


void HTTPClientSessionPoolTest::testExpiredIdleSessionOutOfOrderRemoved()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPSessionFactory factory;
	const Poco::Timespan keepAliveTimeout(2, 0);
	factory.registerProtocol("http", new ShortKeepAliveInstantiator(keepAliveTimeout));
	HTTPClientSessionPool pool(factory, 3);
	const std::string response("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello");
	StreamSocket peerA;
	StreamSocket peerB;
	StreamSocket peerC;
	Poco::Timestamp lastRequestA;
	Poco::Timestamp lastRequestB;
	{
		HTTPClientSessionPool::Handle handleC = pool.acquire(localURI(server.address().port()));
		{
			HTTPClientSessionPool::Handle handleA = pool.acquire(localURI(server.address().port()));
			assertTrue (readBody(exchange(handleA.session(), server, peerA, response)) == "hello");
			lastRequestA = handleA->getLastRequest();
			// Half the keep-alive timeout: A is still valid when it is returned, and B when C is returned.
			const Poco::Timespan offset(keepAliveTimeout.totalMicroseconds() / 2);
			while (offset >= lastRequestA.elapsed()) Poco::Thread::sleep(50);
			{
				HTTPClientSessionPool::Handle handleB = pool.acquire(localURI(server.address().port()));
				assertTrue (readBody(exchange(handleB.session(), server, peerB, response)) == "hello");
				lastRequestB = handleB->getLastRequest();
			}
		}
		assertTrue (pool.idleSessions() == 2);

		// A, returned last, expires while B, returned before it, is still valid.
		while (keepAliveTimeout > lastRequestA.elapsed()) Poco::Thread::sleep(50);
		assertTrue (keepAliveTimeout > lastRequestB.elapsed());
		// Every release checks the idle sessions of its endpoint for expiry,
		// independent of the schedule of the sweep over all endpoints.
		assertTrue (readBody(exchange(handleC.session(), server, peerC, response)) == "hello");
	}
	assertTrue (pool.idleSessions() == 2);
}


void HTTPClientSessionPoolTest::testIdleSessionLimit()
{
	ServerSocket server(SocketAddress("127.0.0.1", 0));
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory, 1);
	StreamSocket peer1;
	StreamSocket peer2;
	{
		HTTPClientSessionPool::Handle handle1 = pool.acquire(localURI(server.address().port()));
		HTTPClientSessionPool::Handle handle2 = pool.acquire(localURI(server.address().port()));
		assertTrue (readBody(exchange(handle1.session(), server, peer1, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello")) == "hello");
		assertTrue (readBody(exchange(handle2.session(), server, peer2, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello")) == "hello");
	}
	assertTrue (pool.idleSessions() == 1);
}


void HTTPClientSessionPoolTest::testConcurrentAcquire()
{
	constexpr int THREADS = 4;
	constexpr int REQUESTS = 25;
	HTTPServer srv(new HelloRequestHandlerFactory, ServerSocket(SocketAddress("127.0.0.1", 0)), new HTTPServerParams);
	srv.start();
	HTTPFactory factory;
	HTTPClientSessionPool pool(factory);
	const Poco::URI uri(localURI(srv.port()));
	std::atomic<int> failures(0);
	std::vector<std::thread> threads;
	for (int t = 0; t < THREADS; ++t)
	{
		threads.emplace_back([&pool, &uri, &failures]()
		{
			for (int i = 0; i < REQUESTS; ++i)
			{
				try
				{
					HTTPClientSessionPool::Handle handle = pool.acquire(uri);
					handle->setTimeout(Poco::Timespan(10, 0));
					HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);
					handle->sendRequest(request);
					HTTPResponse response;
					if (readBody(handle->receiveResponse(response)) != "hello") ++failures;
				}
				catch (Poco::Exception&)
				{
					++failures;
				}
			}
		});
	}
	for (auto& thread: threads) thread.join();
	srv.stop();
	assertTrue (failures == 0);
	assertTrue (srv.totalConnections() <= THREADS);
	assertTrue (pool.idleSessions() >= 1 && pool.idleSessions() <= THREADS);
}


void HTTPClientSessionPoolTest::setUp()
{
}


void HTTPClientSessionPoolTest::tearDown()
{
}


CppUnit::Test* HTTPClientSessionPoolTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPClientSessionPoolTest");

	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testReuseAfterCompleteResponse);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testReuseAfterChunkedResponse);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testUnreadResponseNotKept);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testIncompleteRequestBodyNotKept);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testConnectionCloseNotKept);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testSwitchingProtocolsNotReused);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testConnectTunnelNotReused);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testSettingsRestored);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testChangedProxyNotKept);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testChangedHostNotKept);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testChangedProxyCredentialsNotKept);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testNTLMProxySessionNotKept);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testConnectionAuthenticatedSessionNotKept);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testClosedIdleConnectionNotReused);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testExpiredIdleSessionRemoved);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testExpiredIdleSessionOutOfOrderRemoved);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testIdleSessionLimit);
	CppUnit_addTest(pSuite, HTTPClientSessionPoolTest, testConcurrentAcquire);

	return pSuite;
}
