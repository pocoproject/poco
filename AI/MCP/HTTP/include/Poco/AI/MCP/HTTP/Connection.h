//
// Connection.h
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  Connection
//
// Functions for the connection of a client session.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCPHTTP_Connection_INCLUDED
#define AIMCPHTTP_Connection_INCLUDED


#include "Poco/AI/MCP/HTTP/HTTP.h"
#include "Poco/Net/SocketDefs.h"


namespace Poco::AI::MCP::HTTP {


namespace Impl {
	// Internal to the AI libraries, which share these helpers: they are no
	// part of the public interface and may change.


[[nodiscard]] AIMCPHTTP_API poco_socket_t duplicate(poco_socket_t fd);
	/// Returns another descriptor of the connection of fd, which only its
	/// owner closes, so that another thread may shut the connection down
	/// without meeting a descriptor that a closed one left to some other
	/// connection. Like the sockets of Poco::Net, it is not inherited by
	/// child processes. Returns POCO_INVALID_SOCKET when fd is invalid or no
	/// descriptor can be made.


} // namespace Impl


} // namespace Poco::AI::MCP::HTTP


#endif // AIMCPHTTP_Connection_INCLUDED
