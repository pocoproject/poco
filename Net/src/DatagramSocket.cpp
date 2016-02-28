//
// DatagramSocket.cpp
//
// $Id: //poco/1.4/Net/src/DatagramSocket.cpp#1 $
//
// Library: Net
// Package: Sockets
// Module:  DatagramSocket
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/DatagramSocketImpl.h"
#include "Poco/Exception.h"


using Poco::InvalidArgumentException;


namespace Poco {
namespace Net {


DatagramSocket::DatagramSocket(): Socket(new DatagramSocketImpl)
{
}


DatagramSocket::DatagramSocket(SocketAddress::Family family): Socket(new DatagramSocketImpl(family))
{
}


DatagramSocket::DatagramSocket(const SocketAddress& rAddress, bool reuseAddress): Socket(new DatagramSocketImpl(rAddress.family()))
{
	bind(rAddress, reuseAddress);
}


DatagramSocket::DatagramSocket(const Socket& socket): Socket(socket)
{
	if (!dynamic_cast<DatagramSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


DatagramSocket::DatagramSocket(SocketImpl* pImpl): Socket(pImpl)
{
	if (!dynamic_cast<DatagramSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


DatagramSocket::~DatagramSocket()
{
}


DatagramSocket& DatagramSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<DatagramSocketImpl*>(socket.impl()))
		Socket::operator = (socket);
	else
		throw InvalidArgumentException("Cannot assign incompatible socket");
	return *this;
}


void DatagramSocket::connect(const SocketAddress& rAddress)
{
	impl()->connect(rAddress);
}


void DatagramSocket::bind(const SocketAddress& rAddress, bool reuseAddress)
{
	impl()->bind(rAddress, reuseAddress);
}


int DatagramSocket::sendBytes(const void* buffer, int length, int flags)
{
	return impl()->sendBytes(buffer, length, flags);
}


int DatagramSocket::receiveBytes(void* buffer, int length, int flags)
{
	return impl()->receiveBytes(buffer, length, flags);
}


int DatagramSocket::sendTo(const void* buffer, int length, const SocketAddress& rAddress, int flags)
{
	return impl()->sendTo(buffer, length, rAddress, flags);
}


int DatagramSocket::receiveFrom(void* buffer, int length, SocketAddress& rAddress, int flags)
{
	return impl()->receiveFrom(buffer, length, rAddress, flags);
}


} } // namespace Poco::Net
