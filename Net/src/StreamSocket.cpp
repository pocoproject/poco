//
// StreamSocket.cpp
//
// Library: Net
// Package: Sockets
// Module:  StreamSocket
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/StreamSocketImpl.h"
#include "Poco/FIFOBuffer.h"
#include "Poco/Mutex.h"
#include "Poco/Exception.h"


using Poco::InvalidArgumentException;
using Poco::Mutex;
using Poco::ScopedLock;


namespace Poco {
namespace Net {


StreamSocket::StreamSocket(): Socket(new StreamSocketImpl)
{
}


StreamSocket::StreamSocket(const SocketAddress& address): Socket(new StreamSocketImpl(address.family()))
{
	connect(address);
}


StreamSocket::StreamSocket(SocketAddress::Family family): Socket(new StreamSocketImpl(family))
{
}


StreamSocket::StreamSocket(const Socket& socket): Socket(socket)
{
	if (!dynamic_cast<StreamSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


StreamSocket::StreamSocket(const StreamSocket& socket): Socket(socket)
{
}


StreamSocket::StreamSocket(SocketImpl* pImpl): Socket(pImpl)
{
	if (!dynamic_cast<StreamSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


StreamSocket::~StreamSocket()
{
}


StreamSocket& StreamSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<StreamSocketImpl*>(socket.impl()))
		Socket::operator = (socket);
	else
		throw InvalidArgumentException("Cannot assign incompatible socket");
	return *this;
}


StreamSocket& StreamSocket::operator = (const StreamSocket& socket)
{
	Socket::operator = (socket);
	return *this;
}

#if POCO_NEW_STATE_ON_MOVE

StreamSocket::StreamSocket(Socket&& socket): Socket(std::move(socket))
{
	if (!dynamic_cast<StreamSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


StreamSocket::StreamSocket(StreamSocket&& socket): Socket(std::move(socket))
{
}

StreamSocket& StreamSocket::operator = (Socket&& socket)
{
	Socket::operator = (std::move(socket));
	return *this;
}


StreamSocket& StreamSocket::operator = (StreamSocket&& socket)
{
	Socket::operator = (std::move(socket));
	return *this;
}

#endif // POCO_NEW_STATE_ON_MOVE


void StreamSocket::bind(const SocketAddress& address, bool reuseAddress, bool ipV6Only)
{
	if (address.family() == IPAddress::IPv4)
		impl()->bind(address, reuseAddress);
	else
		impl()->bind6(address, reuseAddress, ipV6Only);
}


void StreamSocket::connect(const SocketAddress& address)
{
	impl()->connect(address);
}


void StreamSocket::connect(const SocketAddress& address, const Poco::Timespan& timeout)
{
	impl()->connect(address, timeout);
}


void StreamSocket::connectNB(const SocketAddress& address)
{
	impl()->connectNB(address);
}


void StreamSocket::shutdownReceive()
{
	impl()->shutdownReceive();
}


void StreamSocket::shutdownSend()
{
	impl()->shutdownSend();
}


void StreamSocket::shutdown()
{
	impl()->shutdown();
}


int StreamSocket::sendBytes(const void* buffer, int length, int flags)
{
	return impl()->sendBytes(buffer, length, flags);
}


int StreamSocket::sendBytes(const SocketBufVec& buffers, int flags)
{
	return impl()->sendBytes(buffers, flags);
}


int StreamSocket::sendBytes(FIFOBuffer& fifoBuf)
{
	ScopedLock<Mutex> l(fifoBuf.mutex());

	int ret = impl()->sendBytes(fifoBuf.begin(), (int) fifoBuf.used());
	if (ret > 0) fifoBuf.drain(ret);
	return ret;
}


int StreamSocket::receiveBytes(void* buffer, int length, int flags)
{
	return impl()->receiveBytes(buffer, length, flags);
}


int StreamSocket::receiveBytes(SocketBufVec& buffers, int flags)
{
	return impl()->receiveBytes(buffers, flags);
}


int StreamSocket::receiveBytes(Poco::Buffer<char>& buffer, int flags, const Poco::Timespan& timeout)
{
	return impl()->receiveBytes(buffer, flags, timeout);
}


int StreamSocket::receiveBytes(FIFOBuffer& fifoBuf)
{
	ScopedLock<Mutex> l(fifoBuf.mutex());

	int ret = impl()->receiveBytes(fifoBuf.next(), (int)fifoBuf.available());
	if (ret > 0) fifoBuf.advance(ret);
	return ret;
}


void StreamSocket::sendUrgent(unsigned char data)
{
	impl()->sendUrgent(data);
}

Poco::Int64 StreamSocket::sendFile(FileInputStream &fileInputStream, Poco::UInt64 offset)
{
	return impl()->sendFile(fileInputStream, offset);
}

} } // namespace Poco::Net
