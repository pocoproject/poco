//
// ICMPClientRaw.cpp
//
// $Id: //poco/1.4/Net/src/ICMPClientRaw.cpp#1 $
//
// Library: Net
// Package: ICMP
// Module:  ICMPClientRaw
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/ICMPClientRaw.h"
#include "Poco/Net/ICMPSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Exception.h"
#include <sstream>


using Poco::InvalidArgumentException;
using Poco::NotImplementedException;
using Poco::TimeoutException;
using Poco::Exception;


namespace Poco {
namespace Net {


ICMPClientRaw::ICMPClientRaw(IPAddress::Family family) :
	_family(family)
{
}


ICMPClientRaw::~ICMPClientRaw()
{
}


int ICMPClientRaw::ping(const std::string& address, int repeat) const
{
	if (repeat <= 0) return 0;

	SocketAddress addr(address, 0);
	return ping(addr, repeat);
}


int ICMPClientRaw::ping(SocketAddress& address, int repeat) const
{
	if (repeat <= 0) return 0;

	ICMPSocket icmpSocket(_family);
	SocketAddress returnAddress;

	ICMPEventArgs eventArgs(address, repeat, icmpSocket.dataSize(), icmpSocket.ttl());
	pingBegin.notify(this, eventArgs);

	for (int i = 0; i < repeat; ++i)
	{
		icmpSocket.sendTo(address);
		++eventArgs;

		try
		{
			int t = icmpSocket.receiveFrom(returnAddress);
			eventArgs.setReplyTime(i, t);
			pingReply.notify(this, eventArgs);
		}
		catch (TimeoutException&)
		{
			std::ostringstream os;
			os << address.host().toString() << ": Request timed out.";
			eventArgs.setError(i, os.str());
			pingError.notify(this, eventArgs);
			continue;
		}
		catch (ICMPException& ex)
		{
			std::ostringstream os;
			os << address.host().toString() << ": " << ex.what();
			eventArgs.setError(i, os.str());
			pingError.notify(this, eventArgs);
			continue;
		}
		catch (Exception& ex)
		{
			std::ostringstream os;
			os << ex.displayText();
			eventArgs.setError(i, os.str());
			pingError.notify(this, eventArgs);
			continue;
		}
	}
	pingEnd.notify(this, eventArgs);
	return eventArgs.received();
}


} } // namespace Poco::Net
