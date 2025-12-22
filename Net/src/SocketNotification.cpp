//
// SocketNotification.cpp
//
// Library: Net
// Package: Reactor
// Module:  SocketNotification
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketNotification.h"


namespace Poco {
namespace Net {


SocketNotification::SocketNotification(SocketReactor* pReactor):
	_pReactor(pReactor)
{
}


SocketNotification::~SocketNotification()
{
}


void SocketNotification::setSocket(const Socket& socket)
{
	_socket = socket;
}


ReadableNotification::ReadableNotification(SocketReactor* pReactor):
	SocketNotification(pReactor)
{
}


ReadableNotification::~ReadableNotification()
{
}


WritableNotification::WritableNotification(SocketReactor* pReactor):
	SocketNotification(pReactor)
{
}


WritableNotification::~WritableNotification()
{
}


ErrorNotification::ErrorNotification(SocketReactor* pReactor, int code, const std::string& description):
	SocketNotification(pReactor),
	_code(code),
	_description(description)
{
}


ErrorNotification::ErrorNotification(SocketReactor* pReactor, const Socket& socket,
	int code, const std::string& description):
	SocketNotification(pReactor),
	_code(code),
	_description(description)
{
	setSocket(socket);
}


ErrorNotification::~ErrorNotification()
{
}


TimeoutNotification::TimeoutNotification(SocketReactor* pReactor):
	SocketNotification(pReactor)
{
}


TimeoutNotification::~TimeoutNotification()
{
}


IdleNotification::IdleNotification(SocketReactor* pReactor):
	SocketNotification(pReactor)
{
}


IdleNotification::~IdleNotification()
{
}


ShutdownNotification::ShutdownNotification(SocketReactor* pReactor):
	SocketNotification(pReactor)
{
}


ShutdownNotification::~ShutdownNotification()
{
}


} } // namespace Poco::Net
