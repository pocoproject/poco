//
// websocket-server.cpp
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/WebSocket.h"
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
#include "Poco/Util/Application.h"

#include <iostream>

using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
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

		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override
		{
			try
			{
				WebSocket ws(request, response);
				std::unique_ptr<char[]> pBuffer(new char[_bufSize]);
				static const std::string pong("PONG!");
				int flags;
				int n;
				do
				{
					n = ws.receiveFrame(pBuffer.get(), static_cast<int>(_bufSize), flags);
					if (n == 0)
						break;

					std::string message(pBuffer.get(), n);
					std::cout << "Received: " << message << " from " << request.clientAddress().toString() << std::endl;
					ws.sendFrame(pong.c_str(), pong.size(), flags);
					std::cout << "Replied: " << pong << std::endl;
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

	private:
		std::size_t _bufSize;
	};

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


class PingServerApp: public Poco::Util::Application
{
public:
	PingServerApp()
	{
		Poco::Net::initializeSSL();
	}

	~PingServerApp() override
	{
		Poco::Net::uninitializeSSL();
	}

	int main(const std::vector<std::string>& args) override
	{
		std::cout << "Starting server" << std::endl;

		Poco::Net::SecureServerSocket ss(0);
		Poco::Net::HTTPServer server(new WebSocketRequestHandlerFactory, ss, new Poco::Net::HTTPServerParams);

		server.start();

		Poco::Thread::sleep(200);

		std::cout << "Listening: " << ss.address().toString() << std::endl;
		std::cout << "Serving requests. Press a key to stop." << std::endl;
		std::cin.get();

		server.stop();

		return 0;
	}

	void setup(int argc, char** argv)
	{
		init(argc, argv);
	}

protected:
	void initialize(Poco::Util::Application& self) override
	{
		loadConfiguration(); // load default configuration files, if present
		Poco::Util::Application::initialize(self);
	}

};

int main(int argc, char** argv)
{
	PingServerApp app;
	try
	{
		app.setup(argc, argv);
		return app.run();
	}
	catch (Poco::Exception& exc)
	{
		std::cout << exc.displayText() << std::endl;
		return 1;
	}
}
