//
// ICMPSocket.cpp
//
// Library: Net
// Package: ICMP
// Module:  ICMPSocket
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/ICMPSocket.h"
#include "Poco/Net/ICMPSocketImpl.h"
#include "Poco/Exception.h"
#include "Poco/Net/NetException.h"


using Poco::InvalidArgumentException;


namespace Poco {
namespace Net {


ICMPSocket::ICMPSocket(IPAddress::Family family, int dataSize, int ttl, int timeout):
	Socket(new ICMPSocketImpl(family, dataSize, ttl, timeout))
{
}


ICMPSocket::ICMPSocket(const Socket& socket):
	Socket(socket)
{
	if (!dynamic_cast<ICMPSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


ICMPSocket::ICMPSocket(SocketImpl* pImpl):
	Socket(pImpl)
{
	if (!dynamic_cast<ICMPSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


ICMPSocket::~ICMPSocket()
{
}


ICMPSocket& ICMPSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<ICMPSocketImpl*>(socket.impl()))
		Socket::operator = (socket);
	else
		throw InvalidArgumentException("Cannot assign incompatible socket");
	return *this;
}


int ICMPSocket::sendTo(const SocketAddress& address, int flags)
{
	return impl()->sendTo(0, 0, address, flags);
}


int ICMPSocket::receiveFrom(SocketAddress& address, int flags)
{
	return impl()->receiveFrom(0, 0, address, flags);
}


int ICMPSocket::dataSize() const
{
	return static_cast<ICMPSocketImpl*>(impl())->dataSize();
}


int ICMPSocket::packetSize() const
{
	return static_cast<ICMPSocketImpl*>(impl())->packetSize();
}


int ICMPSocket::ttl() const
{
	return static_cast<ICMPSocketImpl*>(impl())->ttl();
}


int ICMPSocket::timeout() const
{
	return static_cast<ICMPSocketImpl*>(impl())->timeout();
}


Poco::UInt16 ICMPSocket::mtu(const SocketAddress& address, Poco::UInt16 sz)
{
	if (address.family() != IPAddress::IPv4) return 0;

	SocketAddress returnAddress(address);
	for (; sz >= 68 /*RFC791*/; --sz)
	{
		ICMPSocket icmpSocket(address.family(), sz);

#ifdef IP_DONTFRAGMENT
		icmpSocket.setOption(IPPROTO_IP, IP_DONTFRAGMENT, 1);
#elif defined(IP_MTU_DISCOVER)
		icmpSocket.setOption(IPPROTO_IP, IP_MTU_DISCOVER, IP_PMTUDISC_DO);
#elif defined(IP_DONTFRAG)
		icmpSocket.setOption(IPPROTO_IP, IP_DONTFRAG, 1);
#else
		throw NotImplementedException("ICMPSocket::mtu()");
#endif

		try
		{
			icmpSocket.sendTo(address);
			icmpSocket.receiveFrom(returnAddress);
			poco_assert_dbg (returnAddress == address);
			return sz;
		}
		catch (ICMPFragmentationException&)
		{
			// PMTU fragmentation, continue discovery
			continue;
		}
		catch (NetException& ex)
		{
			// local MTU limit, continue discovery
			if (ex.code() == POCO_EMSGSIZE)
				continue;
		}
		catch (Exception&)
		{
			return 0;
		}
	}
	return 0;
}


} } // namespace Poco::Net
