//
// DatagramSocket.cpp
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


DatagramSocket::DatagramSocket(const SocketAddress& address, bool reuseAddress, bool reusePort, bool ipV6Only):
	Socket(new DatagramSocketImpl(address.family()))
{
	if (address.family() == SocketAddress::IPv6)
		bind6(address, reuseAddress, reusePort, ipV6Only);
	else bind(address, reuseAddress, reusePort);
}


DatagramSocket::DatagramSocket(const Socket& socket): Socket(socket)
{
	if (!dynamic_cast<DatagramSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


DatagramSocket::DatagramSocket(const DatagramSocket& socket): Socket(socket)
{
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

#if POCO_NEW_STATE_ON_MOVE

DatagramSocket::DatagramSocket(DatagramSocket&& socket): Socket(std::move(socket))
{
}


DatagramSocket::DatagramSocket(Socket&& socket): Socket(std::move(socket))
{
	if (!dynamic_cast<DatagramSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


DatagramSocket& DatagramSocket::operator = (Socket&& socket)
{
	if (dynamic_cast<DatagramSocketImpl*>(socket.impl()))
		Socket::operator = (std::move(socket));
	else
		throw InvalidArgumentException("Cannot assign incompatible socket");
	return *this;
}


DatagramSocket& DatagramSocket::operator = (DatagramSocket&& socket)
{
	Socket::operator = (std::move(socket));
	return *this;
}

#endif // POCO_NEW_STATE_ON_MOVE

DatagramSocket& DatagramSocket::operator = (const DatagramSocket& socket)
{
	Socket::operator = (socket);
	return *this;
}


void DatagramSocket::connect(const SocketAddress& address)
{
	impl()->connect(address);
}


void DatagramSocket::bind(const SocketAddress& address, bool reuseAddress)
{
	impl()->bind(address, reuseAddress);
}


void DatagramSocket::bind(const SocketAddress& address, bool reuseAddress, bool reusePort)
{
	impl()->bind(address, reuseAddress, reusePort);
}


void DatagramSocket::bind6(const SocketAddress& address, bool reuseAddress, bool reusePort, bool ipV6Only)
{
	impl()->bind6(address, reuseAddress, reusePort, ipV6Only);
}


int DatagramSocket::sendBytes(const void* buffer, int length, int flags)
{
	return impl()->sendBytes(buffer, length, flags);
}


int DatagramSocket::sendBytes(const SocketBufVec& buffers, int flags)
{
	return impl()->sendBytes(buffers, flags);
}


int DatagramSocket::receiveBytes(void* buffer, int length, int flags)
{
	return impl()->receiveBytes(buffer, length, flags);
}


int DatagramSocket::receiveBytes(SocketBufVec& buffers, int flags)
{
	return impl()->receiveBytes(buffers, flags);
}


int DatagramSocket::receiveBytes(Poco::Buffer<char>& buffer, int flags, const Poco::Timespan& timeout)
{
	return impl()->receiveBytes(buffer, flags, timeout);
}


int DatagramSocket::sendTo(const void* buffer, int length, const SocketAddress& address, int flags)
{
	return impl()->sendTo(buffer, length, address, flags);
}


int DatagramSocket::sendTo(const SocketBufVec& buffers, const SocketAddress& address, int flags)
{
	return impl()->sendTo(buffers, address, flags);
}


int DatagramSocket::receiveFrom(void* buffer, int length, SocketAddress& address, int flags)
{
	return impl()->receiveFrom(buffer, length, address, flags);
}


int DatagramSocket::receiveFrom(void* buffer, int length, struct sockaddr** ppSA, poco_socklen_t** saLen, int flags)
{
	return impl()->receiveFrom(buffer, length, ppSA, saLen, flags);
}


int DatagramSocket::receiveFrom(SocketBufVec& buffers, SocketAddress& address, int flags)
{
	return impl()->receiveFrom(buffers, address, flags);
}


int DatagramSocket::receiveFrom(SocketBufVec& buffers, struct sockaddr** ppSA, poco_socklen_t** ppSALen, int flags)
{
	return impl()->receiveFrom(buffers, ppSA, ppSALen, flags);
}


} } // namespace Poco::Net
