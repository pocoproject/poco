//
// MCPHTTPServer.cpp
//
// A sample MCP server exposing two trivial tools (echo, ping) over the
// Streamable HTTP transport. Also provides a --selftest mode that serves on
// an ephemeral port and drives the HTTP client through discovery, the tool
// list and a tool call.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/ToolResult.h"
#include "Poco/AI/MCP/HTTP/ServerRequestHandler.h"
#include "Poco/AI/MCP/HTTP/HTTPClient.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/OptionException.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/NumberParser.h"
#include "Poco/URI.h"
#include <iostream>
#include <ostream>
#include <string>
#include <vector>


using Poco::Util::Application;
using Poco::Util::ServerApplication;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;


namespace
{


const std::string ENDPOINT_PATH("/mcp");


std::string firstText(const Poco::AI::MCP::ToolResult& result)
{
	Poco::JSON::Array::Ptr pContent = result.content();
	if (pContent != nullptr && pContent->size() > 0)
	{
		Poco::JSON::Object::Ptr pBlock = pContent->getObject(0);
		if (pBlock != nullptr && pBlock->has("text"))
		{
			return pBlock->getValue<std::string>("text");
		}
	}
	return std::string();
}


void registerTools(Poco::AI::MCP::Server& server)
{
	// echo: returns its "message" argument.
	Poco::JSON::Object::Ptr messageProp = new Poco::JSON::Object;
	messageProp->set("type", "string");
	messageProp->set("description", "The message to echo back.");

	Poco::JSON::Object::Ptr echoProps = new Poco::JSON::Object;
	echoProps->set("message", messageProp);

	Poco::JSON::Array::Ptr echoRequired = new Poco::JSON::Array;
	echoRequired->add(std::string("message"));

	Poco::JSON::Object::Ptr echoSchema = new Poco::JSON::Object;
	echoSchema->set("type", "object");
	echoSchema->set("properties", echoProps);
	echoSchema->set("required", echoRequired);

	server.registerTool("echo", "Echo back the provided message.", echoSchema,
		[](const Poco::JSON::Object::Ptr& pArgs)
		{
			Poco::AI::MCP::ToolResult result;
			result.addText(pArgs->optValue<std::string>("message", ""));
			return result;
		});

	// ping: returns "pong".
	Poco::JSON::Object::Ptr pingSchema = new Poco::JSON::Object;
	pingSchema->set("type", "object");
	pingSchema->set("properties", Poco::JSON::Object::Ptr(new Poco::JSON::Object));

	server.registerTool("ping", "Return the string 'pong'.", pingSchema,
		[](const Poco::JSON::Object::Ptr&)
		{
			Poco::AI::MCP::ToolResult result;
			result.addText("pong");
			return result;
		});
}


} // anonymous namespace


class MCPHTTPSampleServer: public ServerApplication
{
public:
	MCPHTTPSampleServer():
		_host("127.0.0.1"),
		_port(8080),
		_selftest(false),
		_helpRequested(false)
	{
	}

protected:
	void defineOptions(OptionSet& options) override
	{
		ServerApplication::defineOptions(options);

		options.addOption(Option("help", "h", "Display help and exit.")
			.required(false).repeatable(false)
			.callback(Poco::Util::OptionCallback<MCPHTTPSampleServer>(this, &MCPHTTPSampleServer::handleHelp)));
		options.addOption(Option("host", "", "The address to listen on; 127.0.0.1 by default.")
			.required(false).repeatable(false).argument("address")
			.callback(Poco::Util::OptionCallback<MCPHTTPSampleServer>(this, &MCPHTTPSampleServer::handleHost)));
		options.addOption(Option("port", "p", "The port to listen on; 8080 by default.")
			.required(false).repeatable(false).argument("port")
			.callback(Poco::Util::OptionCallback<MCPHTTPSampleServer>(this, &MCPHTTPSampleServer::handlePort)));
		options.addOption(Option("origin", "", "An origin whose pages may call the server, e.g. http://localhost:3000; repeatable.")
			.required(false).repeatable(true).argument("origin")
			.callback(Poco::Util::OptionCallback<MCPHTTPSampleServer>(this, &MCPHTTPSampleServer::handleOrigin)));
		options.addOption(Option("selftest", "", "Serve on an ephemeral port, run the HTTP client against it and exit.")
			.required(false).repeatable(false)
			.callback(Poco::Util::OptionCallback<MCPHTTPSampleServer>(this, &MCPHTTPSampleServer::handleSelfTest)));
	}

	void handleHelp(const std::string&, const std::string&)
	{
		_helpRequested = true;
		HelpFormatter formatter(options());
		formatter.setCommand(commandName());
		formatter.setUsage("OPTIONS");
		formatter.setHeader("A sample MCP server exposing echo and ping tools over HTTP.");
		formatter.format(std::cout);
		std::cout << "\n"
			"The MCP endpoint is http://<host>:<port>" << ENDPOINT_PATH << " (protocol revision 2026-07-28).\n"
			"See the README next to this sample for curl examples and client configuration.\n";
		stopOptionsProcessing();
	}

	void handleHost(const std::string&, const std::string& value)
	{
		_host = value;
	}

	void handlePort(const std::string&, const std::string& value)
	{
		unsigned port = 0;
		if (!Poco::NumberParser::tryParseUnsigned(value, port) || port > 65535)
		{
			throw Poco::Util::InvalidArgumentException("Not a port number", value);
		}
		_port = static_cast<Poco::UInt16>(port);
	}

	void handleOrigin(const std::string&, const std::string& value)
	{
		_origins.push_back(value);
	}

	void handleSelfTest(const std::string&, const std::string&)
	{
		_selftest = true;
	}

	int main(const std::vector<std::string>&) override
	{
		if (_helpRequested)
		{
			return Application::EXIT_OK;
		}

		Poco::AI::MCP::Server server;
		server.setServerInfo(serverInfo());
		server.setInstructions("A sample MCP server with echo and ping tools.");
		registerTools(server);

		Poco::AI::MCP::HTTP::ServerTransport::Params params;
		params.allowedOrigins = _origins;
		Poco::Net::ServerSocket socket(Poco::Net::SocketAddress(_host, _selftest ? 0 : _port));
		const Poco::UInt16 port = socket.address().port();
		Poco::Net::HTTPServer httpServer(new Poco::AI::MCP::HTTP::ServerRequestHandlerFactory(server, ENDPOINT_PATH, params), socket, new Poco::Net::HTTPServerParams);
		httpServer.start();

		Poco::URI endpoint;
		endpoint.setScheme("http");
		endpoint.setHost(_host);
		endpoint.setPort(port);
		endpoint.setPath(ENDPOINT_PATH);

		int status = Application::EXIT_OK;
		if (_selftest)
		{
			status = runSelfTest(endpoint);
		}
		else
		{
			logger().information("MCP endpoint: " + endpoint.toString());
			waitForTerminationRequest();
		}
		httpServer.stopAll(true);
		return status;
	}

private:
	static Poco::AI::MCP::Implementation serverInfo()
	{
		Poco::AI::MCP::Implementation info;
		info.name = "mcp-http-sample";
		info.version = "1.0.0";
		return info;
	}

	int runSelfTest(const Poco::URI& endpoint)
	{
		bool ok = true;
		try
		{
			Poco::AI::MCP::HTTP::HTTPClient client(endpoint);
			const Poco::AI::MCP::ServerDescription description = client.discover();
			const std::vector<Poco::AI::MCP::Tool> tools = client.listTools();
			Poco::JSON::Object::Ptr pArgs = new Poco::JSON::Object;
			pArgs->set("message", "hi");
			const std::string echoed = firstText(client.callTool("echo", pArgs));
			ok = (echoed == "hi") && tools.size() == 2 && description.serverInfo.name == "mcp-http-sample";
			std::cerr << "selftest http: server=" << description.serverInfo.name << " tools=" << tools.size()
				<< " echo='" << echoed << "' -> " << (ok ? "OK" : "FAIL") << std::endl;
		}
		catch (const Poco::Exception& exc)
		{
			std::cerr << "selftest http FAILED: " << exc.displayText() << std::endl;
			ok = false;
		}
		return ok ? Application::EXIT_OK : Application::EXIT_SOFTWARE;
	}

	std::string _host;
	Poco::UInt16 _port;
	std::vector<std::string> _origins;
	bool _selftest;
	bool _helpRequested;
};


POCO_SERVER_MAIN(MCPHTTPSampleServer)
