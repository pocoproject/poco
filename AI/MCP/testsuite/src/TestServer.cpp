//
// TestServer.cpp
//
// The MCP server program the ProcessClient tests launch. It serves its
// standard input and output with these tools:
//
//   echo    returns its message argument
//   sleep   waits for ms milliseconds, then returns "slept"
//   exit    ends the process with the given code, without a reply
//   noise   writes a line that is not JSON to standard output, then
//           returns "after noise"
//   getenv  returns the value of the environment variable name
//   cwd     returns the current directory
//   big     returns a text of bytes bytes
//   flood   writes count notifications before its reply, "flooded"
//   spawn   starts this program with --linger, which keeps the standard
//           output it inherits open, and returns its process id
//
// With the environment variable MCP_TEST_INPUT_LOG it copies what it reads
// to the file it names, so that a test can see what reached the server.
//
// With --stubborn it ignores the end of its input and the termination
// request (SIGINT, and SIGBREAK on Windows), so that only a kill ends it.
// With --interruptible it ignores the end of its input, and ends with exit
// code 42 on the termination request. With --linger it serves nothing,
// ignores the termination request and ends after 20 seconds.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Server.h"
#include "Poco/Environment.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Path.h"
#include "Poco/Process.h"
#include "Poco/Thread.h"
#include "Poco/TeeStream.h"
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using Poco::AI::MCP::Server;
using Poco::AI::MCP::ToolResult;
using Poco::JSON::Object;


namespace
{
	void interrupted(int)
	{
		std::_Exit(42);
	}


	void onTerminationRequest(void (*handler)(int))
	{
		std::signal(SIGINT, handler);
#if defined(SIGBREAK)
		std::signal(SIGBREAK, handler);
#endif
	}
}


int main(int argc, char** argv)
{
	const std::string mode = argc > 1 ? std::string(argv[1]) : std::string();
	const bool stubborn = mode == "--stubborn";
	const bool interruptible = mode == "--interruptible";
	if (mode == "--linger")
	{
		onTerminationRequest(SIG_IGN);
		Poco::Thread::sleep(20000);
		return 0;
	}
	if (stubborn)
	{
		onTerminationRequest(SIG_IGN);
	}
	if (interruptible)
	{
		onTerminationRequest(interrupted);
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

	server.registerTool("getenv", "Returns the value of an environment variable.", nullptr, [](const Object::Ptr& pArgs)
	{
		ToolResult result;
		result.addText(Poco::Environment::get(pArgs->optValue<std::string>("name", ""), ""));
		return result;
	});
	server.registerTool("cwd", "Returns the current directory.", nullptr, [](const Object::Ptr&)
	{
		ToolResult result;
		result.addText(Poco::Path::current());
		return result;
	});
	server.registerTool("big", "Returns a text of the given size.", nullptr, [](const Object::Ptr& pArgs)
	{
		ToolResult result;
		result.addText(std::string(pArgs->optValue<std::size_t>("bytes", 0), 'x'));
		return result;
	});
	server.registerTool("flood", "Writes notifications before its reply.", nullptr, [](const Object::Ptr& pArgs)
	{
		const int count = pArgs->optValue<int>("count", 0);
		for (int i = 0; i < count; ++i)
		{
			std::cout << "{\"jsonrpc\":\"2.0\",\"method\":\"notifications/message\",\"params\":{\"level\":\"info\",\"data\":" << i << "}}\n";
		}
		std::cout.flush();
		ToolResult result;
		result.addText("flooded");
		return result;
	});
	server.registerTool("spawn", "Starts a process that keeps the standard output open.", nullptr, [](const Object::Ptr&)
	{
		std::vector<std::string> arguments;
		arguments.push_back("--linger");
		Poco::ProcessHandle handle = Poco::Process::launch(Poco::Path::self(), arguments);
		ToolResult result;
		result.addText(Poco::NumberFormatter::format(handle.id()));
		return result;
	});

	try
	{
		const std::string inputLog = Poco::Environment::get("MCP_TEST_INPUT_LOG", "");
		if (inputLog.empty())
		{
			server.serve(std::cin, std::cout);
		}
		else
		{
			std::ofstream log(inputLog, std::ios::binary);
			Poco::TeeInputStream input(std::cin);
			input.addStream(log);
			server.serve(input, std::cout);
		}
	}
	catch (...)
	{
	}
	while (stubborn || interruptible)
	{
		Poco::Thread::sleep(1000);
	}
	return 0;
}
