//
// ServerDescription.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  ServerDescription
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ServerDescription.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/Exception.h"
#include "Poco/StringTokenizer.h"


namespace Poco {
namespace MongoDB {


ServerDescription::ServerDescription():
	_address(),
	_type(Unknown),
	_lastUpdateTime(),
	_roundTripTime(0),
	_setName(),
	_hosts(),
	_tags(),
	_error(),
	_hasError(false)
{
}


ServerDescription::ServerDescription(const Net::SocketAddress& address):
	_address(address),
	_type(Unknown),
	_lastUpdateTime(),
	_roundTripTime(0),
	_setName(),
	_hosts(),
	_tags(),
	_error(),
	_hasError(false)
{
}


ServerDescription::ServerDescription(const ServerDescription& other):
	_address(other._address),
	_type(other._type),
	_lastUpdateTime(other._lastUpdateTime),
	_roundTripTime(other._roundTripTime),
	_setName(other._setName),
	_hosts(other._hosts),
	_tags(other._tags),
	_error(other._error),
	_hasError(other._hasError)
{
}


ServerDescription::ServerDescription(ServerDescription&& other) noexcept:
	_address(std::move(other._address)),
	_type(other._type),
	_lastUpdateTime(other._lastUpdateTime),
	_roundTripTime(other._roundTripTime),
	_setName(std::move(other._setName)),
	_hosts(std::move(other._hosts)),
	_tags(std::move(other._tags)),
	_error(std::move(other._error)),
	_hasError(other._hasError)
{
	other._type = Unknown;
	other._roundTripTime = 0;
	other._hasError = false;
}


ServerDescription::~ServerDescription()
{
}


ServerDescription& ServerDescription::operator=(const ServerDescription& other)
{
	if (this != &other)
	{
		_address = other._address;
		_type = other._type;
		_lastUpdateTime = other._lastUpdateTime;
		_roundTripTime = other._roundTripTime;
		_setName = other._setName;
		_hosts = other._hosts;
		_tags = other._tags;
		_error = other._error;
		_hasError = other._hasError;
	}
	return *this;
}


ServerDescription& ServerDescription::operator=(ServerDescription&& other) noexcept
{
	if (this != &other)
	{
		_address = std::move(other._address);
		_type = other._type;
		_lastUpdateTime = other._lastUpdateTime;
		_roundTripTime = other._roundTripTime;
		_setName = std::move(other._setName);
		_hosts = std::move(other._hosts);
		_tags = std::move(other._tags);
		_error = std::move(other._error);
		_hasError = other._hasError;

		other._type = Unknown;
		other._roundTripTime = 0;
		other._hasError = false;
	}
	return *this;
}


void ServerDescription::updateFromHelloResponse(const Document& helloResponse, Poco::Int64 rttMicros)
{
	_lastUpdateTime.update();
	_roundTripTime = rttMicros;
	_hasError = false;
	_error.clear();

	// Parse server type
	parseServerType(helloResponse);

	// Get replica set name
	if (helloResponse.exists("setName"))
	{
		_setName = helloResponse.get<std::string>("setName");
	}

	// Parse hosts list
	parseHosts(helloResponse);

	// Parse tags
	parseTags(helloResponse);
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
	_roundTripTime = 0;
	_setName.clear();
	_hosts.clear();
	_tags.clear();
	_error.clear();
	_hasError = false;
}


void ServerDescription::parseServerType(const Document& doc)
{
	// Check for standalone
	if (!doc.exists("setName"))
	{
		// Check if it's a mongos
		if (doc.get<std::string>("msg", "") == "isdbgrid")
		{
			_type = Mongos;
			return;
		}
		_type = Standalone;
		return;
	}

	// It's part of a replica set - determine the role
	if (doc.get<bool>("isWritablePrimary", false) || doc.get<bool>("ismaster", false))
	{
		_type = RsPrimary;
	}
	else if (doc.get<bool>("secondary", false))
	{
		_type = RsSecondary;
	}
	else if (doc.get<bool>("arbiterOnly", false))
	{
		_type = RsArbiter;
	}
	else if (doc.get<bool>("hidden", false) || doc.get<bool>("passive", false))
	{
		_type = RsOther;
	}
	else
	{
		// Server is in replica set but role unclear (might be initializing)
		_type = RsGhost;
	}
}


void ServerDescription::parseHosts(const Document& doc)
{
	_hosts.clear();

	// Parse hosts array
	if (doc.exists("hosts"))
	{
		Array::Ptr hostsArray = doc.get<Array::Ptr>("hosts");
		for (int i = 0; i < hostsArray->size(); ++i)
		{
			try
			{
				std::string hostStr = hostsArray->get<std::string>(i);
				_hosts.push_back(Net::SocketAddress(hostStr));
			}
			catch (...)
			{
				// Skip invalid host addresses
			}
		}
	}

	// Parse passives array (hidden/passive members)
	if (doc.exists("passives"))
	{
		Array::Ptr passivesArray = doc.get<Array::Ptr>("passives");
		for (int i = 0; i < passivesArray->size(); ++i)
		{
			try
			{
				std::string hostStr = passivesArray->get<std::string>(i);
				_hosts.push_back(Net::SocketAddress(hostStr));
			}
			catch (...)
			{
				// Skip invalid host addresses
			}
		}
	}

	// Parse arbiters array
	if (doc.exists("arbiters"))
	{
		Array::Ptr arbitersArray = doc.get<Array::Ptr>("arbiters");
		for (int i = 0; i < arbitersArray->size(); ++i)
		{
			try
			{
				std::string hostStr = arbitersArray->get<std::string>(i);
				_hosts.push_back(Net::SocketAddress(hostStr));
			}
			catch (...)
			{
				// Skip invalid host addresses
			}
		}
	}
}


void ServerDescription::parseTags(const Document& doc)
{
	_tags.clear();

	if (doc.exists("tags"))
	{
		Document::Ptr tagsDoc = doc.get<Document::Ptr>("tags");
		if (!tagsDoc.isNull())
		{
			_tags = *tagsDoc;
		}
	}
}


} } // namespace Poco::MongoDB
