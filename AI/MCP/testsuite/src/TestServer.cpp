//
// TestServer.cpp
//
// The MCP server program the ProcessClient tests launch. It serves its
// standard input and output with these tools:
//
//   echo   returns its message argument
//   sleep  waits for ms milliseconds, then returns "slept"
//   exit   ends the process with the given code, without a reply
//   noise  writes a line that is not JSON to standard output, then
//          returns "after noise"
//
// With --stubborn it ignores the end of its input and SIGINT, so that only
// a kill ends it.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Server.h"
#include "Poco/Thread.h"
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string>


using Poco::AI::MCP::Server;
using Poco::AI::MCP::ToolResult;
using Poco::JSON::Object;


int main(int argc, char** argv)
{
	const bool stubborn = argc > 1 && std::string(argv[1]) == "--stubborn";
	if (stubborn)
	{
		std::signal(SIGINT, SIG_IGN);
	}

	Server server;
	Poco::AI::MCP::Implementation info;
	info.name = "test-server";
	info.version = "1.0";
	server.setServerInfo(info);
	server.registerTool("echo", "Returns its message.", nullptr, [](const Object::Ptr& pArgs)
	{
		ToolResult result;
		result.addText(pArgs->optValue<std::string>("message", ""));
		return result;
	});
	server.registerTool("sleep", "Waits, then returns.", nullptr, [](const Object::Ptr& pArgs)
	{
		Poco::Thread::sleep(pArgs->optValue<long>("ms", 0));
		ToolResult result;
		result.addText("slept");
		return result;
	});
	server.registerTool("exit", "Ends the process without a reply.", nullptr, [](const Object::Ptr& pArgs) -> ToolResult
	{
		std::cout.flush();
		std::_Exit(pArgs->optValue<int>("code", 3));
	});
	server.registerTool("noise", "Writes a line that is not JSON first.", nullptr, [](const Object::Ptr&)
	{
		std::cout << "this line is not JSON" << std::endl;
		ToolResult result;
		result.addText("after noise");
		return result;
	});

	try
	{
		server.serve(std::cin, std::cout);
	}
	catch (...)
	{
	}
	while (stubborn)
	{
		Poco::Thread::sleep(1000);
	}
	return 0;
}
