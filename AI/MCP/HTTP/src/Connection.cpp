//
// Connection.cpp
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  Connection
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/HTTP/Connection.h"
#if !defined(POCO_OS_FAMILY_WINDOWS)
#include <fcntl.h>
#include <unistd.h>
#endif


namespace Poco::AI::MCP::HTTP {


namespace Impl {


poco_socket_t duplicate(poco_socket_t fd)
{
	if (fd == POCO_INVALID_SOCKET) return POCO_INVALID_SOCKET;
#if defined(POCO_OS_FAMILY_WINDOWS)
	WSAPROTOCOL_INFOW info;
	if (::WSADuplicateSocketW(fd, ::GetCurrentProcessId(), &info) != 0) return POCO_INVALID_SOCKET;
	const poco_socket_t copy = ::WSASocketW(FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, &info, 0, WSA_FLAG_OVERLAPPED | WSA_FLAG_NO_HANDLE_INHERIT);
	if (copy == POCO_INVALID_SOCKET) return POCO_INVALID_SOCKET;
	// A socket made from the protocol information of another one may be
	// inheritable all the same.
	if (!::SetHandleInformation(reinterpret_cast<HANDLE>(copy), HANDLE_FLAG_INHERIT, 0))
	{
		poco_closesocket(copy);
		return POCO_INVALID_SOCKET;
	}
	return copy;
#elif defined(F_DUPFD_CLOEXEC)
	const int copy = ::fcntl(fd, F_DUPFD_CLOEXEC, 0);
	return copy >= 0 ? copy : POCO_INVALID_SOCKET;
#else
	const int copy = ::dup(fd);
	if (copy < 0) return POCO_INVALID_SOCKET;
	if (::fcntl(copy, F_SETFD, FD_CLOEXEC) == -1)
	{
		::close(copy);
		return POCO_INVALID_SOCKET;
	}
	return copy;
#endif
}


} // namespace Impl


} // namespace Poco::AI::MCP::HTTP
