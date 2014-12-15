//
// ICMPClient.cpp
//
// $Id: //poco/1.4/Net/src/ICMPClient.cpp#1 $
//
// Library: Net
// Package: ICMP
// Module:  ICMPClient
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/ICMPClient.h"
#include "Poco/Net/ICMPClientRaw.h"
#include "Poco/Net/NetException.h"
#include "Poco/Exception.h"
#include <Poco/Delegate.h>


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/Net/ICMPClientUser_WIN32.h"
#include "ICMPClientUser_WIN32.cpp"
#endif


using Poco::InvalidArgumentException;
using Poco::NotImplementedException;
using Poco::TimeoutException;
using Poco::Exception;


namespace Poco {
namespace Net {


ICMPClient::ICMPClient(IPAddress::Family family, bool useRawSocket)
{
	if (!useRawSocket)
	{
#if defined(POCO_OS_FAMILY_WINDOWS)
		_impl = new ICMPClientUserImpl(family);
#else
		throw Poco::NotImplementedException("Non-raw socket not implemented for this platform");
#endif
	}
	else
		_impl = new ICMPClientRaw(family);

	_impl->pingBegin += Poco::delegate(this, &ICMPClient::tpbegin);
	_impl->pingReply += Poco::delegate(this, &ICMPClient::tpreply);
	_impl->pingError += Poco::delegate(this, &ICMPClient::tperror);
	_impl->pingEnd += Poco::delegate(this, &ICMPClient::tpend);
}


ICMPClient::~ICMPClient()
{
	if (_impl) delete _impl;
}


int ICMPClient::ping(const std::string& address, int repeat) const
{
	if (repeat <= 0) return 0;

	return _impl->ping(address, repeat);
}


int ICMPClient::ping(SocketAddress& address, int repeat) const
{
	if (repeat <= 0) return 0;

	return _impl->ping(address, repeat);
}


int ICMPClient::pingIPv4(const std::string& address, int repeat, bool useRawSocket)
{
	if (repeat <= 0) return 0;

	SocketAddress a(address, 0);
	return ping(a, IPAddress::IPv4, repeat, useRawSocket);
}


int ICMPClient::ping(SocketAddress& address, IPAddress::Family family, int repeat, bool useRawSocket)
{
	if (repeat <= 0) return 0;

	ICMPClient icmpClient(family, useRawSocket);
	return icmpClient.ping(address, repeat);
}


void ICMPClient::tpbegin(const void *sender, Poco::Net::ICMPEventArgs &args)
{
	pingBegin.notify(this, args);
}


void ICMPClient::tpreply(const void *sender, Poco::Net::ICMPEventArgs &args)
{
	pingReply.notify(this, args);
}


void ICMPClient::tperror(const void *sender, Poco::Net::ICMPEventArgs &args)
{
	pingError.notify(this, args);
}


void ICMPClient::tpend(const void *sender, Poco::Net::ICMPEventArgs &args)
{
	pingEnd.notify(this, args);
}


} } // namespace Poco::Net
