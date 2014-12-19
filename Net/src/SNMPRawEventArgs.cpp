//
// SNMPRawEventArgs.cpp
//
// $Id: //poco/1.4/Net/src/SNMPRawEventArgs.cpp#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPRawEventArgs
//
// Implementation of SNMPRawEventArgs
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SNMPRawEventArgs.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DNS.h"
#include "Poco/Exception.h"
#include "Poco/Net/NetException.h"
#include <numeric>


using Poco::IOException;
using Poco::InvalidArgumentException;


namespace Poco {
namespace Net {


SNMPRawEventArgs::SNMPRawEventArgs(const SocketAddress& address):
	_address(address), 
	_response()
{
}


SNMPRawEventArgs::~SNMPRawEventArgs()
{
}


std::string SNMPRawEventArgs::hostName() const
{
	try
	{
		return DNS::resolve(_address.host().toString()).name();
	}
	catch (HostNotFoundException&) 
	{
	}
	catch (NoAddressFoundException&) 
	{
	}
	catch (DNSException&)
	{
	}
	catch (IOException&)
	{
	}
	return _address.host().toString();
}


std::string SNMPRawEventArgs::hostAddress() const
{
	return _address.host().toString();
}


ASN1::Ptr SNMPRawEventArgs::response() const
{
	return _response;
}


const std::string &SNMPRawEventArgs::error() const
{
	return _error;
}


void SNMPRawEventArgs::setResponse(ASN1::Ptr response)
{
	_response = response;
}


void SNMPRawEventArgs::setError(const std::string &error)
{
	_error = error;
}


} } // namespace Poco::Net
