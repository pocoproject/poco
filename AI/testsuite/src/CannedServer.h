//
// CannedServer.h
//
// An in-test HTTP server on a loopback port of its own that answers every
// request with the response it is given and keeps the requests it got. The
// provider tests use it in place of a model API.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AITest_CannedServer_INCLUDED
#define AITest_CannedServer_INCLUDED


#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/StreamCopier.h"
#include "Poco/Condition.h"
#include "Poco/Event.h"
#include "Poco/Mutex.h"
#include "Poco/Timestamp.h"
#include <atomic>
#include <memory>
#include <string>
#include <vector>


class CannedServer
	/// Answers every request with the canned response. Not copyable.
{
public:
	struct Response
	{
		int status = 200;
		std::string contentType;
		std::string body;
		bool chunked = false;
			/// Sends the body with chunked transfer encoding.
		bool waitBeforeHeaders = false;
			/// Sends nothing until the server stops or the client goes away.
		std::size_t pauseAt = std::string::npos;
			/// Sends the body up to this offset, then waits until the server
			/// stops or the client goes away.
		bool stallBeforeBody = false;
			/// Reads next to nothing of the request and sends nothing until the
			/// server stops or the client goes away: an upload blocks.
		std::vector<std::pair<std::string, std::string>> headers;
			/// More headers of the response.
	};

	explicit CannedServer(const Response& response):
		_pState(std::make_shared<State>()),
		_socket(Poco::Net::SocketAddress("127.0.0.1", 0)),
		_server(new Factory(_pState), _socket, new Poco::Net::HTTPServerParams)
	{
		_pState->response = response;
		_server.start();
	}

	CannedServer(const std::string& contentType, const std::string& body):
		CannedServer(makeResponse(200, contentType, body))
	{
	}

	~CannedServer()
	{
		// Wakes the handlers that wait and waits until none runs.
		_pState->stopping.set();
		_server.stopAll(true);
		Poco::Mutex::ScopedLock lock(_pState->activeMutex);
		while (_pState->active > 0)
		{
			if (!_pState->idle.tryWait(_pState->activeMutex, 10000)) break;
		}
	}

	static Response makeResponse(int status, const std::string& contentType, const std::string& body)
	{
		Response response;
		response.status = status;
		response.contentType = contentType;
		response.body = body;
		return response;
	}

	std::string baseUrl() const
		/// Returns the base URL to give a provider.
	{
		return "http://127.0.0.1:" + std::to_string(_socket.address().port()) + "/v1";
	}

	int requests() const
		/// Returns the number of requests the server has read.
	{
		return _pState->count.load();
	}

	std::string path() const
		/// Returns the path of the last request.
	{
		Poco::FastMutex::ScopedLock lock(_pState->mutex);
		return _pState->path;
	}

	std::string request(std::size_t index = std::string::npos) const
		/// Returns the body of the request with the given index, or of the last.
	{
		Poco::FastMutex::ScopedLock lock(_pState->mutex);
		if (_pState->bodies.empty()) return std::string();
		if (index == std::string::npos || index >= _pState->bodies.size()) return _pState->bodies.back();
		return _pState->bodies[index];
	}

	std::string header(const std::string& name) const
		/// Returns a header of the last request, or an empty string.
	{
		Poco::FastMutex::ScopedLock lock(_pState->mutex);
		return _pState->headers.get(name, "");
	}

	bool waitForRequest(long milliseconds)
		/// Waits until a request has arrived; false on timeout.
	{
		return _pState->arrived.tryWait(milliseconds);
	}

	bool waitForRequests(int count, long milliseconds)
		/// Waits until count requests have arrived; false on timeout.
	{
		const Poco::Timestamp start;
		Poco::Mutex::ScopedLock lock(_pState->activeMutex);
		while (_pState->count.load() < count)
		{
			const long left = milliseconds - static_cast<long>(start.elapsed() / 1000);
			if (left <= 0) return false;
			(void) _pState->idle.tryWait(_pState->activeMutex, left);
		}
		return true;
	}

private:
	struct State
	{
		Response response;
		std::atomic<int> count{0};
		mutable Poco::FastMutex mutex;
		std::string path;
		std::vector<std::string> bodies;
		Poco::Net::NameValueCollection headers;
		Poco::Event arrived{Poco::Event::EVENT_MANUALRESET};
		Poco::Event stopping{Poco::Event::EVENT_MANUALRESET};
		Poco::Mutex activeMutex;
		Poco::Condition idle;
		int active = 0;
	};

	class Handler: public Poco::Net::HTTPRequestHandler
	{
	public:
		explicit Handler(std::shared_ptr<State> pState):
			_pState(std::move(pState))
		{
		}

		void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override
		{
			{
				Poco::Mutex::ScopedLock lock(_pState->activeMutex);
				++_pState->active;
			}
			try
			{
				serve(request, response);
			}
			catch (...)
			{
			}
			Poco::Mutex::ScopedLock lock(_pState->activeMutex);
			--_pState->active;
			_pState->idle.broadcast();
		}

	private:
		void serve(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
		{
			Response canned;
			{
				Poco::FastMutex::ScopedLock lock(_pState->mutex);
				canned = _pState->response;
			}
			std::string requestBody;
			if (!canned.stallBeforeBody)
			{
				Poco::StreamCopier::copyToString(request.stream(), requestBody);
			}
			{
				Poco::FastMutex::ScopedLock lock(_pState->mutex);
				_pState->path = request.getURI();
				_pState->bodies.push_back(requestBody);
				_pState->headers = request;
			}
			{
				Poco::Mutex::ScopedLock lock(_pState->activeMutex);
				++_pState->count;
				_pState->idle.broadcast();
			}
			_pState->arrived.set();

			if (canned.waitBeforeHeaders || canned.stallBeforeBody)
			{
				waitForEnd(request);
				return;
			}
			response.setStatus(static_cast<Poco::Net::HTTPResponse::HTTPStatus>(canned.status));
			response.setContentType(canned.contentType);
			for (const auto& header: canned.headers)
			{
				response.set(header.first, header.second);
			}
			if (canned.chunked || canned.pauseAt != std::string::npos)
			{
				response.setChunkedTransferEncoding(true);
			}
			else
			{
				response.setContentLength(static_cast<std::streamsize>(canned.body.size()));
			}
			std::ostream& out = response.send();
			if (canned.pauseAt != std::string::npos)
			{
				out << canned.body.substr(0, canned.pauseAt);
				out.flush();
				waitForEnd(request);
				return;
			}
			out << canned.body;
		}

		void waitForEnd(Poco::Net::HTTPServerRequest& request)
			/// Waits until the server stops or the client closes the connection.
		{
			Poco::Net::StreamSocket& socket = static_cast<Poco::Net::HTTPServerRequestImpl&>(request).socket();
			for (;;)
			{
				if (_pState->stopping.tryWait(20)) return;
				try
				{
					if (socket.poll(Poco::Timespan(0), Poco::Net::Socket::SELECT_READ | Poco::Net::Socket::SELECT_ERROR))
					{
						char c;
						if (socket.receiveBytes(&c, 1) <= 0) return;
					}
				}
				catch (...)
				{
					return;
				}
			}
		}

		std::shared_ptr<State> _pState;
	};

	class Factory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		explicit Factory(std::shared_ptr<State> pState):
			_pState(std::move(pState))
		{
		}

		Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override
		{
			return new Handler(_pState);
		}

	private:
		std::shared_ptr<State> _pState;
	};

	CannedServer(const CannedServer&) = delete;
	CannedServer& operator = (const CannedServer&) = delete;

	std::shared_ptr<State> _pState;
	Poco::Net::ServerSocket _socket;
	Poco::Net::HTTPServer _server;
};


#endif // AITest_CannedServer_INCLUDED
