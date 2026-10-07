//
// MCPSampleServer.cpp
//
// A sample MCP server exposing two trivial tools (echo, ping) over stdio.
// Also provides a --selftest mode that drives an in-process client through
// the full handshake and a tool call.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Client.h"
#include "Poco/AI/MCP/Content.h"
#include "Poco/AI/MCP/StdIOServer.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#if defined(POCO_OS_FAMILY_UNIX)
#include <unistd.h>
#endif


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;


namespace
{


std::string firstText(const Poco::AI::MCP::ToolResult& result)
{
	Poco::JSON::Array::Ptr content = result.content();
	if (content && content->size() > 0)
	{
		Poco::JSON::Object::Ptr block = content->getObject(0);
		if (block && block->has("text"))
		{
			return block->getValue<std::string>("text");
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
		[](Poco::JSON::Object::Ptr args)
		{
			Poco::AI::MCP::ToolResult result;
			const std::string message = (args && args->has("message")) ? args->getValue<std::string>("message") : std::string();
			result.addText(message);
			return result;
		});

	// ping: returns "pong".
	Poco::JSON::Object::Ptr pingSchema = new Poco::JSON::Object;
	pingSchema->set("type", "object");
	pingSchema->set("properties", Poco::JSON::Object::Ptr(new Poco::JSON::Object));

	server.registerTool("ping", "Return the string 'pong'.", pingSchema,
		[](Poco::JSON::Object::Ptr)
		{
			Poco::AI::MCP::ToolResult result;
			result.addText("pong");
			return result;
		});
}


class ServeRunnable: public Poco::Runnable
	/// Runs Server::serve over a pipe pair on a background thread.
{
public:
	ServeRunnable(Poco::AI::MCP::Server& server, Poco::Pipe& in, Poco::Pipe& out):
		_server(server),
		_in(in),
		_out(out)
	{
	}

	void run() override
	{
		Poco::PipeInputStream input(_in);
		Poco::PipeOutputStream output(_out);
		_server.serve(input, output);
	}

private:
	Poco::AI::MCP::Server& _server;
	Poco::Pipe& _in;
	Poco::Pipe& _out;
};


} // anonymous namespace


class MCPSampleServer: public Poco::Util::ServerApplication
{
public:
	MCPSampleServer():
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
			.callback(Poco::Util::OptionCallback<MCPSampleServer>(this, &MCPSampleServer::handleHelp)));
		options.addOption(Option("selftest", "", "Run an in-process client smoke test and exit.")
			.required(false).repeatable(false)
			.callback(Poco::Util::OptionCallback<MCPSampleServer>(this, &MCPSampleServer::handleFlag)));
	}

	void handleHelp(const std::string&, const std::string&)
	{
		_helpRequested = true;
		HelpFormatter formatter(options());
		formatter.setCommand(commandName());
		formatter.setUsage("OPTIONS");
		formatter.setHeader("A sample MCP server exposing echo and ping tools.");
		formatter.format(std::cout);
		printExamples(std::cout);
		stopOptionsProcessing();
	}

	void printExamples(std::ostream& out)
	{
		const std::string cmd = commandName();
		out << "\n"
			"This is a stdio MCP server, not a REPL. It reads newline-delimited\n"
			"JSON-RPC 2.0 from stdin and writes one JSON reply per line to stdout\n"
			"(logs go to stderr). Plain text is a parse error.\n"
			"\n"
			"Quick start:\n"
			"  " << cmd << " --selftest   # in-process smoke test, no client needed\n"
			"\n"
			"For stdio JSON-RPC examples and how to register this server with an MCP\n"
			"client, see the README next to this sample.\n";
	}

	void handleFlag(const std::string& name, const std::string&)
	{
		if (name == "selftest") _selftest = true;
	}

	int main(const std::vector<std::string>&) override
	{
		if (_helpRequested)
		{
			return Application::EXIT_OK;
		}

		if (_selftest)
		{
			return runSelfTest();
		}

		Poco::AI::MCP::Server server;
		server.setServerInfo("mcp-sample", "1.0.0");
		server.setInstructions("A sample MCP server with echo and ping tools.");
		registerTools(server);
		return runStdIO(server);
	}

private:
	int runStdIO(Poco::AI::MCP::Server& server)
	{
#if defined(POCO_OS_FAMILY_UNIX)
		// A naive interactive run (stdin is a TTY) looks like a hang. Nudge the
		// user - to stderr, so stdout stays protocol-clean for real clients (which
		// pipe stdin and never see this).
		if (isatty(STDIN_FILENO))
		{
			std::cerr <<
				"[" << commandName() << "] stdio MCP server: reading newline-delimited JSON-RPC 2.0 from stdin.\n"
				"[" << commandName() << "] This is a server, not a REPL - plain text is a parse error.\n"
				"[" << commandName() << "] Run --help or see the README next to this sample; --selftest to self-check. Ctrl-D to exit.\n";
		}
#endif
		// Routes logging to stderr; stdout carries protocol bytes only.
		Poco::AI::MCP::StdIOServer transport(server);
		transport.run();
		return Application::EXIT_OK;
	}

	int runSelfTest()
	{
		Poco::AI::MCP::Server server;
		server.setServerInfo("mcp-sample", "1.0.0");
		registerTools(server);

		Poco::Pipe toServer;
		Poco::Pipe toClient;
		ServeRunnable runnable(server, toServer, toClient);
		Poco::Thread thread;
		thread.start(runnable);

		Poco::PipeOutputStream clientOut(toServer);
		Poco::PipeInputStream clientIn(toClient);
		Poco::AI::MCP::Client client(clientIn, clientOut);

		bool ok = true;
		try
		{
			client.initialize("selftest", "1.0.0");
			Poco::JSON::Array::Ptr tools = client.listTools();
			Poco::JSON::Object::Ptr args = new Poco::JSON::Object;
			args->set("message", "hi");
			const std::string echoed = firstText(client.callTool("echo", args));
			client.ping();
			ok = (echoed == "hi") && tools && tools->size() == 2;
			std::cerr << "selftest stdio: tools=" << (tools ? tools->size() : 0)
				<< " echo='" << echoed << "' -> " << (ok ? "OK" : "FAIL") << std::endl;
		}
		catch (Poco::Exception& exc)
		{
			std::cerr << "selftest stdio FAILED: " << exc.displayText() << std::endl;
			ok = false;
		}

		toServer.close(Poco::Pipe::CLOSE_WRITE);
		thread.join();
		return ok ? Application::EXIT_OK : Application::EXIT_SOFTWARE;
	}

	bool _selftest;
	bool _helpRequested;
};


POCO_SERVER_MAIN(MCPSampleServer)
