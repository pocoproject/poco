//
// StdIOServer.cpp
//
// Library: AIMCP
// Package: Transport
// Module:  StdIOServer
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/StdIOServer.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/Logger.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/AutoPtr.h"
#include <iostream>


namespace Poco {
namespace AI {
namespace MCP {


StdIOServer::StdIOServer(Server& server):
	_server(server)
{
}


StdIOServer::~StdIOServer()
{
}


void StdIOServer::run()
{
	// In stdio mode stdout carries protocol bytes only - route all logging to
	// stderr so a stray log line cannot corrupt the message stream.
	Poco::AutoPtr<Poco::ConsoleChannel> stderrChannel(new Poco::ConsoleChannel(std::cerr));
	Poco::Logger::root().setChannel(stderrChannel);

	run(std::cin, std::cout);
}


void StdIOServer::run(std::istream& in, std::ostream& out)
{
	_server.serve(in, out);
}


} } } // namespace Poco::AI::MCP
