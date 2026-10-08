//
// CannedServer.h
//
// An in-test HTTP server on a loopback port of its own that answers every
// request with the same response and keeps the last request it got. The
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
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/StreamCopier.h"
#include "Poco/Mutex.h"
#include <memory>
#include <string>


class CannedServer
{
public:
	CannedServer(const std::string& contentType, const std::string& body):
		_pState(std::make_shared<State>()),
		_socket(Poco::Net::SocketAddress("127.0.0.1", 0)),
		_server(new Factory(_pState), _socket, new Poco::Net::HTTPServerParams)
	{
		_pState->contentType = contentType;
		_pState->body = body;
		_server.start();
	}

	~CannedServer()
	{
		_server.stopAll(true);
	}

	std::string baseUrl() const
		/// Returns the base URL to give a provider.
	{
		return "http://127.0.0.1:" + std::to_string(_socket.address().port()) + "/v1";
	}

	std::string path() const
		/// Returns the path of the last request.
	{
		Poco::FastMutex::ScopedLock lock(_pState->mutex);
		return _pState->path;
	}

	std::string request() const
		/// Returns the body of the last request.
	{
		Poco::FastMutex::ScopedLock lock(_pState->mutex);
		return _pState->request;
	}

	std::string header(const std::string& name) const
		/// Returns a header of the last request, or an empty string.
	{
		Poco::FastMutex::ScopedLock lock(_pState->mutex);
		return _pState->headers.get(name, "");
	}

private:
	struct State
	{
		Poco::FastMutex mutex;
		std::string contentType;
		std::string body;
		std::string path;
		std::string request;
		Poco::Net::NameValueCollection headers;
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
			std::string requestBody;
			Poco::StreamCopier::copyToString(request.stream(), requestBody);
			std::string contentType;
			std::string body;
			{
				Poco::FastMutex::ScopedLock lock(_pState->mutex);
				_pState->path = request.getURI();
				_pState->request = requestBody;
				_pState->headers = request;
				contentType = _pState->contentType;
				body = _pState->body;
			}
			response.setContentType(contentType);
			response.setContentLength(static_cast<std::streamsize>(body.size()));
			response.send() << body;
		}

	private:
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

	std::shared_ptr<State> _pState;
	Poco::Net::ServerSocket _socket;
	Poco::Net::HTTPServer _server;
};


#endif // AITest_CannedServer_INCLUDED
