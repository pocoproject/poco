//
// RawSocket.cpp
//
// Library: Net
// Package: Sockets
// Module:  RawSocket
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/RawSocket.h"
#include "Poco/Net/RawSocketImpl.h"
#include "Poco/Exception.h"


using Poco::InvalidArgumentException;


namespace Poco {
namespace Net {


RawSocket::RawSocket():
	Socket(new RawSocketImpl)
{
}


RawSocket::RawSocket(SocketAddress::Family family, int proto):
	Socket(new RawSocketImpl(family, proto))
{
}


RawSocket::RawSocket(const SocketAddress& address, bool reuseAddress):
	Socket(new RawSocketImpl(address.family()))
{
	bind(address, reuseAddress);
}


RawSocket::RawSocket(const Socket& socket): Socket(socket)
{
	if (!dynamic_cast<RawSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


RawSocket::RawSocket(const RawSocket& socket): Socket(socket)
{
}

#ifdef POCO_NEW_STATE_ON_MOVE

RawSocket::RawSocket(RawSocket&& socket): Socket(std::move(socket))
{
}

#endif // POCO_NEW_STATE_ON_MOVE

RawSocket::RawSocket(SocketImpl* pImpl): Socket(pImpl)
{
	if (!dynamic_cast<RawSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


RawSocket::~RawSocket()
{
}


RawSocket& RawSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<RawSocketImpl*>(socket.impl()))
		Socket::operator = (socket);
	else
		throw InvalidArgumentException("Cannot assign incompatible socket");
	return *this;
}


#ifdef POCO_NEW_STATE_ON_MOVE

RawSocket& RawSocket::operator = (Socket&& socket)
{
	if (dynamic_cast<RawSocketImpl*>(socket.impl()))
		Socket::operator = (std::move(socket));
	else
		throw InvalidArgumentException("Cannot assign incompatible socket");
	return *this;
}

#endif // POCO_NEW_STATE_ON_MOVE

RawSocket& RawSocket::operator = (const RawSocket& socket)
{
	Socket::operator = (socket);
	return *this;
}


#ifdef POCO_NEW_STATE_ON_MOVE

RawSocket& RawSocket::operator = (RawSocket&& socket)
{
	Socket::operator = (std::move(socket));
	return *this;
}

#endif // POCO_NEW_STATE_ON_MOVE

void RawSocket::connect(const SocketAddress& address)
{
	impl()->connect(address);
}


void RawSocket::bind(const SocketAddress& address, bool reuseAddress)
{
	impl()->bind(address, reuseAddress);
}


void RawSocket::bind(const SocketAddress& address, bool reuseAddress, bool reusePort)
{
	impl()->bind(address, reuseAddress, reusePort);
}


int RawSocket::sendBytes(const void* buffer, int length, int flags)
{
	return impl()->sendBytes(buffer, length, flags);
}


int RawSocket::receiveBytes(void* buffer, int length, int flags)
{
	return impl()->receiveBytes(buffer, length, flags);
}


int RawSocket::sendTo(const void* buffer, int length, const SocketAddress& address, int flags)
{
	return impl()->sendTo(buffer, length, address, flags);
}


int RawSocket::receiveFrom(void* buffer, int length, SocketAddress& address, int flags)
{
	return impl()->receiveFrom(buffer, length, address, flags);
}


} } // namespace Poco::Net
