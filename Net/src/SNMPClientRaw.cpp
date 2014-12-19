//
// SNMPClientRaw.cpp
//
// $Id: //poco/1.4/Net/src/SNMPClientRaw.cpp#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPClientRaw
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SNMPClientRaw.h"
#include "Poco/Net/NetException.h"
#include "Poco/ASN1Codec.h"
#include "Poco/Net/SNMPClientRawFactory.h"
#include "Poco/Exception.h"
#include <Poco/Delegate.h>

#include <sstream>


using Poco::InvalidArgumentException;
using Poco::NotImplementedException;
using Poco::TimeoutException;
using Poco::Exception;


namespace Poco {
namespace Net {


SNMPClientRaw::SNMPClientRaw() : _socket()
{
	Poco::Net::SocketAddress sa;
	_socket.setSendTimeout(1 * 1000 * 1000);
	_socket.setReceiveTimeout(1 * 1000 * 1000);
	_socket.bind(sa);
}


SNMPClientRaw::SNMPClientRaw(const SocketAddress& listenAddr) :
	_socket()
{
	_socket.setSendTimeout(1 * 1000 * 1000);
	_socket.setReceiveTimeout(1 * 1000 * 1000);
	_socket.bind(listenAddr);
}


SNMPClientRaw::~SNMPClientRaw()
{
}


ASN1::Ptr SNMPClientRaw::query(const std::string& address, ASN1::Ptr query)
{
	SocketAddress addr(address, 161);
	return this->query(addr, query);
}


ASN1::Ptr SNMPClientRaw::query(SocketAddress& address, ASN1::Ptr query)
{
	std::stringstream data;
	Poco::SharedPtr<ASN1Factory> factory(new SNMPClientRawFactory);
	ASN1Codec codec(factory);
	codec.encode(query, data);

	int s = _socket.sendTo(data.str().data(), data.str().size(), address);
	if (s != data.str().size())
		throw Poco::Exception("Not sent all data!");

	char buffer[2048];
	Poco::Net::SocketAddress sender;
	int n = _socket.receiveFrom(buffer, sizeof(buffer)-1, sender);

	std::stringstream srec(std::string(buffer, n));
	srec.exceptions(std::istream::failbit | std::istream::badbit | std::istream::eofbit);

	ASN1::Ptr response = codec.decode(srec);

	return response;
}


void SNMPClientRaw::send(const std::string& address, ASN1::Ptr query)
{
	SocketAddress addr(address, 161);
	send(addr, query);
}


void SNMPClientRaw::send(SocketAddress& address, ASN1::Ptr query)
{
	SNMPRawEventArgs eventArgs(address);
	try
	{
		ASN1::Ptr response = this->query(address, query);
		eventArgs.setResponse(response);
		snmpReply.notify(this, eventArgs);
	}
	catch (TimeoutException&)
	{
		std::ostringstream os;
		os << address.host().toString() << ": Request timed out.";
		eventArgs.setError(os.str());
		snmpError.notify(this, eventArgs);
	}
	catch (Exception& ex)
	{
		std::ostringstream os;
		os << ex.displayText();
		eventArgs.setError(os.str());
		snmpError.notify(this, eventArgs);
	}
	catch (std::exception& ex)
	{
		std::ostringstream os;
		os << ex.what();
		eventArgs.setError(os.str());
		snmpError.notify(this, eventArgs);
	}
}


} } // namespace Poco::Net
