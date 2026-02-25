//
// ServerDescription.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  ServerDescription
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ServerDescription.h"
#include "Poco/MongoDB/Array.h"
#include <limits>

using namespace std::string_literals;


namespace Poco {
namespace MongoDB {


ServerDescription::ServerDescription() = default;


ServerDescription::ServerDescription(const Net::SocketAddress& address):
	_address(address)
{
}


ServerDescription::ServerDescription(const ServerDescription& other) = default;


ServerDescription::ServerDescription(ServerDescription&& other) noexcept = default;


ServerDescription::~ServerDescription() = default;


ServerDescription& ServerDescription::operator=(const ServerDescription& other) = default;


ServerDescription& ServerDescription::operator=(ServerDescription&& other) noexcept = default;


bool ServerDescription::operator==(const ServerDescription& other) const
{
	return _type == other._type &&
	       _address == other._address &&
	       _setName == other._setName &&
	       _hasError == other._hasError &&
	       _error == other._error &&
	       _roundTripTime == other._roundTripTime;
}


bool ServerDescription::operator!=(const ServerDescription& other) const
{
	return !(*this == other);
}


std::vector<Net::SocketAddress> ServerDescription::updateFromHelloResponse(const Document& helloResponse, Poco::Int64 rttMicros)
{
	_lastUpdateTime.update();
	_roundTripTime = rttMicros;
	_hasError = false;
	_error.clear();

	// Parse server type
	parseServerType(helloResponse);

	// Get replica set name
	if (helloResponse.exists("setName"s))
	{
		_setName = helloResponse.get<std::string>("setName"s);
	}

	// Parse lastWrite.lastWriteDate for staleness calculation
	// See: https://www.mongodb.com/docs/manual/reference/command/hello/#std-label-hello-lastWrite
	// The lastWriteDate is a BSON datetime (ms since epoch) indicating when
	// the server last received a write operation.
	_lastWriteDate = 0;
	if (helloResponse.exists("lastWrite"s))
	{
		Document::Ptr lastWrite = helloResponse.get<Document::Ptr>("lastWrite"s);
		if (!lastWrite.isNull() && lastWrite->exists("lastWriteDate"s))
		{
			Timestamp lwDate = lastWrite->get<Timestamp>("lastWriteDate"s);
			_lastWriteDate = lwDate.epochMicroseconds();
		}
	}

	// Parse and return hosts list
	auto hosts = parseHosts(helloResponse);

	// Parse tags
	parseTags(helloResponse);

	return hosts;
}


void ServerDescription::markError(const std::string& errorMessage)
{
	_type = Unknown;
	_error = errorMessage;
	_hasError = true;
	_lastUpdateTime.update();
}


void ServerDescription::reset()
{
	_type = Unknown;
	_lastUpdateTime = 0;
	_lastWriteDate = 0;
	_roundTripTime = std::numeric_limits<Poco::Int64>::max();
	_setName.clear();
	_tags.clear();
	_error.clear();
	_hasError = false;
}


void ServerDescription::parseServerType(const Document& doc)
{
	// Check for standalone
	if (!doc.exists("setName"s))
	{
		// Check if it's a mongos
		if (doc.get<std::string>("msg"s, ""s) == "isdbgrid")
		{
			_type = Mongos;
			return;
		}
		_type = Standalone;
		return;
	}

	// It's part of a replica set - determine the role
	if (doc.get<bool>("isWritablePrimary"s, false))
	{
		_type = RsPrimary;
	}
	else if (doc.get<bool>("secondary"s, false))
	{
		_type = RsSecondary;
	}
	else if (doc.get<bool>("arbiterOnly"s, false))
	{
		_type = RsArbiter;
	}
	else if (doc.get<bool>("hidden"s, false) || doc.get<bool>("passive"s, false))
	{
		_type = RsOther;
	}
	else
	{
		// Server is in replica set but role unclear (might be initializing)
		_type = RsGhost;
	}
}


std::vector<Net::SocketAddress> ServerDescription::parseHosts(const Document& doc)
{
	std::vector<Net::SocketAddress> hosts;

	// Parse hosts array
	if (doc.exists("hosts"s))
	{
		Array::Ptr hostsArray = doc.get<Array::Ptr>("hosts"s);
		hosts.reserve(hostsArray->size());
		for (std::size_t i = 0; i < hostsArray->size(); ++i)
		{
			try
			{
				std::string hostStr = hostsArray->get<std::string>(i);
				hosts.emplace_back(hostStr);
			}
			catch (...)
			{
				// Skip invalid host addresses
			}
		}
	}

	// Parse passives array (hidden/passive members)
	if (doc.exists("passives"s))
	{
		Array::Ptr passivesArray = doc.get<Array::Ptr>("passives"s);
		for (std::size_t i = 0; i < passivesArray->size(); ++i)
		{
			try
			{
				std::string hostStr = passivesArray->get<std::string>(i);
				hosts.emplace_back(hostStr);
			}
			catch (...)
			{
				// Skip invalid host addresses
			}
		}
	}

	// Parse arbiters array
	if (doc.exists("arbiters"s))
	{
		Array::Ptr arbitersArray = doc.get<Array::Ptr>("arbiters"s);
		for (std::size_t i = 0; i < arbitersArray->size(); ++i)
		{
			try
			{
				std::string hostStr = arbitersArray->get<std::string>(i);
				hosts.emplace_back(hostStr);
			}
			catch (...)
			{
				// Skip invalid host addresses
			}
		}
	}

	return hosts;
}


void ServerDescription::parseTags(const Document& doc)
{
	_tags.clear();

	if (doc.exists("tags"s))
	{
		Document::Ptr tagsDoc = doc.get<Document::Ptr>("tags"s);
		if (!tagsDoc.isNull())
		{
			_tags = *tagsDoc;
		}
	}
}


std::string ServerDescription::typeToString(ServerType type)
{
	switch (type)
	{
	case RsPrimary:
		return "PRIMARY"s;
	case RsSecondary:
		return "SECONDARY"s;
	case RsArbiter:
		return "ARBITER"s;
	case Standalone:
		return "STANDALONE"s;
	case Mongos:
		return "MONGOS"s;
	case RsOther:
		return "OTHER"s;
	case RsGhost:
		return "GHOST"s;
	case Unknown:
	default:
		return "UNKNOWN"s;
	}
}


} } // namespace Poco::MongoDB
