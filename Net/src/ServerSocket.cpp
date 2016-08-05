//
// ServerSocket.cpp
//
// $Id: //poco/1.4/Net/src/ServerSocket.cpp#2 $
//
// Library: Net
// Package: Sockets
// Module:  ServerSocket
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/ServerSocketImpl.h"
#include "Poco/Exception.h"


using Poco::InvalidArgumentException;


namespace Poco {
namespace Net {


ServerSocket::ServerSocket(): Socket(new ServerSocketImpl)
{
}


ServerSocket::ServerSocket(const Socket& socket): Socket(socket)
{
	if (!dynamic_cast<ServerSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


ServerSocket::ServerSocket(const SocketAddress& rAddress, int backlog): Socket(new ServerSocketImpl)
{
	impl()->bind(rAddress, true);
	impl()->listen(backlog);
}


ServerSocket::ServerSocket(Poco::UInt16 port, int backlog): Socket(new ServerSocketImpl)
{
	IPAddress wildcardAddr;
	SocketAddress socketAddress(wildcardAddr, port);
	impl()->bind(socketAddress, true);
	impl()->listen(backlog);
}


ServerSocket::ServerSocket(SocketImpl* pImpl, bool ignore): Socket(pImpl)
{
}


ServerSocket::~ServerSocket()
{
}


ServerSocket& ServerSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<ServerSocketImpl*>(socket.impl()))
		Socket::operator = (socket);
	else
		throw InvalidArgumentException("Cannot assign incompatible socket");
	return *this;
}


void ServerSocket::bind(const SocketAddress& rAddress, bool reuseAddress)
{
	impl()->bind(rAddress, reuseAddress);
}


void ServerSocket::bind(Poco::UInt16 port, bool reuseAddress)
{
	IPAddress wildcardAddr;
	SocketAddress socketAddress(wildcardAddr, port);
	impl()->bind(socketAddress, reuseAddress);
}


void ServerSocket::bind6(const SocketAddress& rAddress, bool reuseAddress, bool ipV6Only)
{
	impl()->bind6(rAddress, reuseAddress, ipV6Only);
}


void ServerSocket::bind6(Poco::UInt16 port, bool reuseAddress, bool ipV6Only)
{
#if defined(POCO_HAVE_IPv6)
	IPAddress wildcardAddr(IPAddress::IPv6);
	SocketAddress socketAddress(wildcardAddr, port);
	impl()->bind6(socketAddress, reuseAddress, ipV6Only);
#else
	throw Poco::NotImplementedException("No IPv6 support available");
#endif // POCO_HAVE_IPv6
}

	
void ServerSocket::listen(int backlog)
{
	impl()->listen(backlog);
}


StreamSocket ServerSocket::acceptConnection(SocketAddress& clientAddr)
{
	return StreamSocket(impl()->acceptConnection(clientAddr));
}


StreamSocket ServerSocket::acceptConnection()
{
	SocketAddress clientAddr;
	return StreamSocket(impl()->acceptConnection(clientAddr));
}


} } // namespace Poco::Net
