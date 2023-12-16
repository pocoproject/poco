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


#include <utility>

#include "Poco/Net/SocketNotification.h"


namespace Poco {
namespace Net {


SocketNotification::SocketNotification(SocketReactor* pReactor):
	_pReactor(pReactor)
{
}


SocketNotification::~SocketNotification()
= default;


void SocketNotification::setSocket(const Socket& socket)
{
	_socket = socket;
}


ReadableNotification::ReadableNotification(SocketReactor* pReactor):
	SocketNotification(pReactor)
{
}


ReadableNotification::~ReadableNotification()
= default;


WritableNotification::WritableNotification(SocketReactor* pReactor):
	SocketNotification(pReactor)
{
}


WritableNotification::~WritableNotification()
= default;


ErrorNotification::ErrorNotification(SocketReactor* pReactor, int code, std::string  description):
	SocketNotification(pReactor),
	_code(code),
	_description(std::move(description))
{
}


ErrorNotification::ErrorNotification(SocketReactor* pReactor, const Socket& socket,
	int code, std::string  description):
	SocketNotification(pReactor),
	_code(code),
	_description(std::move(description))
{
	setSocket(socket);
}


ErrorNotification::~ErrorNotification()
= default;


TimeoutNotification::TimeoutNotification(SocketReactor* pReactor):
	SocketNotification(pReactor)
{
}


TimeoutNotification::~TimeoutNotification()
= default;


IdleNotification::IdleNotification(SocketReactor* pReactor):
	SocketNotification(pReactor)
{
}


IdleNotification::~IdleNotification()
= default;


ShutdownNotification::ShutdownNotification(SocketReactor* pReactor):
	SocketNotification(pReactor)
{
}


ShutdownNotification::~ShutdownNotification()
= default;


} } // namespace Poco::Net
