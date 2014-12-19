//
// SNMPClient.cpp
//
// $Id: //poco/1.4/Net/src/SNMPClient.cpp#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPClient
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SNMPClient.h"
#include "Poco/Net/SNMPCore.h"
#include "Poco/Net/SNMPClientRaw.h"
#include "Poco/Net/SNMPClientRawTypes.h"
#include "Poco/Net/NetException.h"
#include "Poco/Exception.h"
#include <Poco/Delegate.h>


#include <sstream>


using Poco::InvalidArgumentException;
using Poco::NotImplementedException;
using Poco::TimeoutException;
using Poco::Exception;


namespace Poco {
namespace Net {


SNMPClient::SNMPClient()
{
}


SNMPClient::~SNMPClient()
{
}


void SNMPClient::send(const std::string& address, SNMPTypes::SNMPMessage::Ptr message)
{
	SocketAddress addr(address, 161);
	send(addr, message);
}


void SNMPClient::send(SocketAddress& address, SNMPTypes::SNMPMessage::Ptr message)
{
	SNMPClientRaw snmpClient;

	SNMPEventArgs eventArgs(address);
	snmpBegin.notify(this, eventArgs);

	std::string startoid;

	if (message->pdu().type().rawValue() == SNMP_ASN1::GetNextRequestPDU)
	{
		if (message->pdu().varBindList().list().size() != 1)
			throw Poco::IllegalStateException("For SNMP GET NEXT, only one VarBind must be requested");

		startoid = message->pdu().varBindList().list().at(0)->oid();
	}

	try
	{
		std::string lastoid;
		while (true)
		{
			Poco::ASN1::Ptr resp = snmpClient.query(address, message->encode());
			SNMPTypes::SNMPMessage::Ptr sresp(new SNMPTypes::SNMPMessage(resp));

			std::string curoid;

			if (message->pdu().type().rawValue() == SNMP_ASN1::GetNextRequestPDU)
			{
				curoid = sresp->pdu().varBindList().list().at(0)->oid();
				if (curoid == lastoid || curoid.substr(0, startoid.size()) != startoid)
					break;

				message->pdu().varBindList().list()[0]->setOid(curoid);
			}

			lastoid = curoid;
			eventArgs.setMessage(sresp);
			eventArgs.setRawMessage(resp);
			eventArgs.setAbort(false);
			snmpReply.notify(this, eventArgs);

			if (eventArgs.abort())
				break;

			if (message->pdu().type().rawValue() == SNMP_ASN1::GetNextRequestPDU)
				message->pdu().varBindList().list()[0]->setOid(curoid);
			else
				break;
		}
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

	snmpEnd.notify(this, eventArgs);
}


void SNMPClient::get(SocketAddress& address, const std::string &oid, int requestId, 
	const std::string &community, SNMPVersion::Version version)
{
	SNMPTypes::SNMPMessage::Ptr message(new SNMPTypes::SNMPMessage);
	message->setVersion(version);
	message->setCommunity(community);
	message->pdu().setType(SNMP_ASN1::GetRequestPDU);
	message->pdu().setRequestId(requestId);
	message->pdu().varBindList().add(new Poco::Net::SNMPTypes::VarBind(oid, new Poco::ASN1Types::Null()));

	send(address, message);
}


void SNMPClient::get(const std::string& address, const std::string &oid, int requestId, 
	const std::string &community, SNMPVersion::Version version)
{
	SocketAddress addr(address, 161);
	get(addr, oid, requestId, community, version);
}


void SNMPClient::set(SocketAddress& address, const std::string &oid, ASN1::Ptr value, int requestId, 
	const std::string &community, SNMPVersion::Version version)
{
	SNMPTypes::SNMPMessage::Ptr message(new SNMPTypes::SNMPMessage);
	message->setVersion(version);
	message->setCommunity(community);
	message->pdu().setType(SNMP_ASN1::SetRequestPDU);
	message->pdu().setRequestId(requestId);
	message->pdu().varBindList().add(new Poco::Net::SNMPTypes::VarBind(oid, value));

	send(address, message);
}


void SNMPClient::set(const std::string& address, const std::string &oid, ASN1::Ptr value, int requestId, 
	const std::string &community, SNMPVersion::Version version)
{
	SocketAddress addr(address, 161);
	set(addr, oid, value, requestId, community, version);
}


void SNMPClient::walk(SocketAddress& address, const std::string &oid, int requestId, 
	const std::string &community, SNMPVersion::Version version)
{
	SNMPTypes::SNMPMessage::Ptr message(new SNMPTypes::SNMPMessage);
	message->setVersion(version);
	message->setCommunity(community);
	message->pdu().setType(SNMP_ASN1::GetNextRequestPDU);
	message->pdu().setRequestId(requestId);
	message->pdu().varBindList().add(new Poco::Net::SNMPTypes::VarBind(oid, new Poco::ASN1Types::Null()));

	send(address, message);
}


void SNMPClient::walk(const std::string& address, const std::string &oid, int requestId, 
	const std::string &community, SNMPVersion::Version version)
{
	SocketAddress addr(address, 161);
	walk(addr, oid, requestId, community, version);
}


void SNMPClient::getBulk(SocketAddress& address, const std::string &oid, int requestId, 
	int nonRepeaters, int maxRepetitions,
	const std::string &community, SNMPVersion::Version version)
{
	SNMPTypes::SNMPMessage::Ptr message(new SNMPTypes::SNMPMessage);
	message->setVersion(version);
	message->setCommunity(community);
	message->pdu().setType(SNMP_ASN1::GetBulkRequestPDU);
	message->pdu().setRequestId(requestId);
	message->pdu().setNonRepeaters(nonRepeaters);
	message->pdu().setMaxRepetitions(maxRepetitions);
	message->pdu().varBindList().add(new Poco::Net::SNMPTypes::VarBind(oid, new Poco::ASN1Types::Null()));

	send(address, message);
}


void SNMPClient::getBulk(const std::string& address, const std::string &oid, int requestId, 
	int nonRepeaters, int maxRepetitions,
	const std::string &community, SNMPVersion::Version version)
{
	SocketAddress addr(address, 161);
	getBulk(addr, oid, requestId, nonRepeaters, maxRepetitions, community, version);
}


} } // namespace Poco::Net
