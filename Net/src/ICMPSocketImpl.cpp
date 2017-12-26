//
// ICMPSocketImpl.cpp
//
// Library: Net
// Package: ICMP
// Module:  ICMPSocketImpl
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/ICMPSocketImpl.h"
#include "Poco/Net/NetException.h"
#include "Poco/Format.h"
#include "Poco/Buffer.h"


using Poco::TimeoutException;
using Poco::Timespan;
using Poco::Exception;


namespace Poco {
namespace Net {


ICMPSocketImpl::ICMPSocketImpl(IPAddress::Family family, int dataSize, int ttl, int timeout):
	RawSocketImpl(family, IPPROTO_ICMP),
	_icmpPacket(family, dataSize),
	_ttl(ttl),
	_timeout(timeout)
{
	setOption(IPPROTO_IP, IP_TTL, ttl);
	setBlocking(true);
	setReceiveTimeout(Timespan(timeout));
}


ICMPSocketImpl::~ICMPSocketImpl()
{
}


int ICMPSocketImpl::sendTo(const void*, int, const SocketAddress& address, int flags)
{
	int n = SocketImpl::sendTo(_icmpPacket.packet(), _icmpPacket.packetSize(), address, flags);
	return n;
}


int ICMPSocketImpl::receiveFrom(void*, int, SocketAddress& address, int flags)
{
	int maxPacketSize = _icmpPacket.maxPacketSize();
	Poco::Buffer<unsigned char> buffer(maxPacketSize);

	try
	{
		Poco::Timestamp ts;
		int rc;
		int expected = _icmpPacket.packetSize();
		do
		{
			// guard against a DoS attack
			if (ts.isElapsed(_timeout)) throw TimeoutException();
			buffer.clear();
			SocketAddress respAddr;
			rc = SocketImpl::receiveFrom(buffer.begin(), expected, respAddr, flags);
			if (rc == 0) break;
			if (respAddr == address)
			{
				expected -= rc;
				if (expected == 0)
				{
					if (_icmpPacket.validReplyID(buffer.begin(), maxPacketSize)) break;
					std::string err = _icmpPacket.errorDescription(buffer.begin(), maxPacketSize);
					if (!err.empty()) throw ICMPException(err);
					throw ICMPException("Invalid ICMP reply");
				}
			}
			else
			{
				throw ICMPException(Poco::format("Reply from an unknown IP address "
												 "(requested %s, received %s).",
												 address.host().toString(), respAddr.host().toString()));
			}
		}
		while (expected && !_icmpPacket.validReplyID(buffer.begin(), maxPacketSize));
	}
	catch (ICMPException&)
	{
		throw;
	}
	catch (Exception&)
	{
		std::string err = _icmpPacket.errorDescription(buffer.begin(), maxPacketSize);
		if (!err.empty())
			throw ICMPException(err);
		else
			throw;
	}

	struct timeval then = _icmpPacket.time(buffer.begin(), maxPacketSize);
	struct timeval now  = _icmpPacket.time();

	int elapsed	= (((now.tv_sec * 1000000) + now.tv_usec) - ((then.tv_sec * 1000000) + then.tv_usec))/1000;

	return elapsed;
}


} } // namespace Poco::Net
