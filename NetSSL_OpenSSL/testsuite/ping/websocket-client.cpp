//
// websocket-client.cpp
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Util/Application.h"

#include <iostream>

using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::WebSocket;

class PingClientApp: public Poco::Util::Application
{
public:
	PingClientApp()
	{
		Poco::Net::initializeSSL();
	}

	~PingClientApp() override
	{
		Poco::Net::uninitializeSSL();
	}

	int main(const std::vector<std::string>& args) override
	{
		if (args.size() != 2)
		{
			std::cout << "Usage: " << this->commandName() << " hostname port" << std::endl;
			return 1;
		}
		std::cout << "Connecting to: " << args[0] << ":" << args[1] << std::endl;
		int port = std::stoi(args[1]);

		HTTPSClientSession cs(args[0], port);
		HTTPRequest request(HTTPRequest::HTTP_GET, "/ws");
		HTTPResponse response;
		WebSocket ws(cs, request, response);

		char buffer[1024] = {};
		int flags;

		const std::string payload("PING!");

		while (true)
		{
			std::cout << "Sending: " << payload << std::endl;
			ws.sendFrame(payload.data(), (int) payload.size());
			int n = ws.receiveFrame(buffer, sizeof(buffer), flags);
			std::string response(buffer, n);
			std::cout << "Received: " << response << std::endl;
			Poco::Thread::current()->sleep(1000);
		}

		ws.shutdown();
		ws.receiveFrame(buffer, sizeof(buffer), flags);

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
	PingClientApp app;
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


