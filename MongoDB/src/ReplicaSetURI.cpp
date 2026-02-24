//
// ReplicaSetURI.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReplicaSetURI
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ReplicaSetURI.h"
#include "Poco/Exception.h"
#include "Poco/NumberParser.h"
#include "Poco/String.h"
#include <sstream>

using namespace std::string_literals;


namespace Poco {
namespace MongoDB {


ReplicaSetURI::ReplicaSetURI()
{
}


ReplicaSetURI::ReplicaSetURI(const std::string& uri)
{
	parse(uri);
}


ReplicaSetURI::~ReplicaSetURI()
{
}


const std::vector<std::string>& ReplicaSetURI::servers() const
{
	return _servers;
}


void ReplicaSetURI::setServers(const std::vector<std::string>& servers)
{
	_servers = servers;
}


void ReplicaSetURI::addServer(const std::string& server)
{
	_servers.push_back(server);
}


void ReplicaSetURI::clearServers()
{
	_servers.clear();
}


std::string ReplicaSetURI::replicaSet() const
{
	return _replicaSet;
}


void ReplicaSetURI::setReplicaSet(const std::string& name)
{
	_replicaSet = name;
}


ReadPreference ReplicaSetURI::readPreference() const
{
	return _readPreference;
}


void ReplicaSetURI::setReadPreference(const ReadPreference& pref)
{
	_readPreference = pref;
}


void ReplicaSetURI::setReadPreference(const std::string& mode)
{
	std::string lowerMode = Poco::toLower(mode);

	if (lowerMode == "primary"s)
	{
		_readPreference = ReadPreference(ReadPreference::Primary);
	}
	else if (lowerMode == "primarypreferred"s)
	{
		_readPreference = ReadPreference(ReadPreference::PrimaryPreferred);
	}
	else if (lowerMode == "secondary"s)
	{
		_readPreference = ReadPreference(ReadPreference::Secondary);
	}
	else if (lowerMode == "secondarypreferred"s)
	{
		_readPreference = ReadPreference(ReadPreference::SecondaryPreferred);
	}
	else if (lowerMode == "nearest"s)
	{
		_readPreference = ReadPreference(ReadPreference::Nearest);
	}
	else
	{
		throw Poco::InvalidArgumentException("Invalid read preference mode: " + mode);
	}
}


unsigned int ReplicaSetURI::connectTimeoutMS() const
{
	return _connectTimeoutMS;
}


void ReplicaSetURI::setConnectTimeoutMS(unsigned int timeoutMS)
{
	_connectTimeoutMS = timeoutMS;
}


unsigned int ReplicaSetURI::socketTimeoutMS() const
{
	return _socketTimeoutMS;
}


void ReplicaSetURI::setSocketTimeoutMS(unsigned int timeoutMS)
{
	_socketTimeoutMS = timeoutMS;
}


unsigned int ReplicaSetURI::heartbeatFrequencyMS() const
{
	return _heartbeatFrequencyMS;
}


void ReplicaSetURI::setHeartbeatFrequencyMS(unsigned int milliseconds)
{
	if (milliseconds < MIN_HEARTBEAT_FREQUENCY_MS)
	{
		throw Poco::InvalidArgumentException(
			"heartbeatFrequencyMS must be at least " +
			std::to_string(MIN_HEARTBEAT_FREQUENCY_MS) +
			" milliseconds per MongoDB SDAM specification");
	}
	_heartbeatFrequencyMS = milliseconds;
}


unsigned int ReplicaSetURI::reconnectRetries() const
{
	return _reconnectRetries;
}


void ReplicaSetURI::setReconnectRetries(unsigned int retries)
{
	_reconnectRetries = retries;
}


unsigned int ReplicaSetURI::reconnectDelay() const
{
	return _reconnectDelay;
}


void ReplicaSetURI::setReconnectDelay(unsigned int seconds)
{
	_reconnectDelay = seconds;
}


std::string ReplicaSetURI::database() const
{
	return _database;
}


void ReplicaSetURI::setDatabase(const std::string& database)
{
	_database = database;
}


std::string ReplicaSetURI::username() const
{
	return _username;
}


void ReplicaSetURI::setUsername(const std::string& username)
{
	_username = username;
}


std::string ReplicaSetURI::password() const
{
	return _password;
}


void ReplicaSetURI::setPassword(const std::string& password)
{
	_password = password;
}


std::string ReplicaSetURI::toString() const
{
	if (_servers.empty())
	{
		throw Poco::InvalidArgumentException("Cannot generate URI: no servers configured");
	}

	std::ostringstream uri;

	// Scheme
	uri << "mongodb://";

	// User info
	if (!_username.empty())
	{
		uri << _username;
		if (!_password.empty())
		{
			uri << ":" << _password;
		}
		uri << "@";
	}

	// Hosts
	for (std::size_t i = 0; i < _servers.size(); ++i)
	{
		if (i > 0)
		{
			uri << ",";
		}
		uri << _servers[i];
	}

	// Database
	if (!_database.empty())
	{
		uri << "/" << _database;
	}

	// Query parameters
	std::string queryString = buildQueryString();
	if (!queryString.empty())
	{
		// Add leading '/' if we don't have a database
		if (_database.empty())
		{
			uri << "/";
		}
		uri << "?" << queryString;
	}

	return uri.str();
}


void ReplicaSetURI::parse(const std::string& uri)
{
	// MongoDB URIs can contain comma-separated hosts which Poco::URI doesn't handle correctly.
	// We need to extract the host list manually first, then create a simplified URI for Poco::URI
	// to parse the scheme, path, and query parameters.

	// Find the scheme delimiter
	auto schemeEnd = uri.find("://");
	if (schemeEnd == std::string::npos)
	{
		throw Poco::SyntaxException("Invalid URI: missing scheme delimiter");
	}

	std::string scheme = uri.substr(0, schemeEnd);
	if (scheme != "mongodb"s)
	{
		throw Poco::UnknownURISchemeException("Replica set URI must use 'mongodb' scheme");
	}

	// Find where the authority (hosts) section ends
	// It ends at either '/' (path) or '?' (query)
	std::string::size_type authorityStart = schemeEnd + 3;  // Skip "://"
	std::string::size_type authorityEnd = uri.find_first_of("/?", authorityStart);

	// Extract authority and the rest of the URI
	std::string authority;
	std::string pathAndQuery;

	if (authorityEnd != std::string::npos)
	{
		authority = uri.substr(authorityStart, authorityEnd - authorityStart);
		pathAndQuery = uri.substr(authorityEnd);
	}
	else
	{
		authority = uri.substr(authorityStart);
		pathAndQuery = "";
	}

	// Parse user info if present (username:password@)
	const auto atPos = authority.find('@');
	std::string hostsStr;

	if (atPos != std::string::npos)
	{
		std::string userInfo = authority.substr(0, atPos);
		hostsStr = authority.substr(atPos + 1);

		// Parse username and password
		auto colonPos = userInfo.find(':');
		if (colonPos != std::string::npos)
		{
			_username = userInfo.substr(0, colonPos);
			_password = userInfo.substr(colonPos + 1);
		}
		else
		{
			_username = userInfo;
			_password = "";
		}
	}
	else
	{
		hostsStr = authority;
		_username = "";
		_password = "";
	}

	// Parse comma-separated hosts
	// Store as strings WITHOUT resolving - resolution happens in ReplicaSet
	_servers.clear();
	std::string::size_type start = 0;
	std::string::size_type end;

	while ((end = hostsStr.find(',', start)) != std::string::npos)
	{
		const auto hostPort = hostsStr.substr(start, end - start);
		if (!hostPort.empty())
		{
			_servers.push_back(hostPort);
		}
		start = end + 1;
	}

	// Parse last host
	const auto lastHost = hostsStr.substr(start);
	if (!lastHost.empty())
	{
		_servers.push_back(lastHost);
	}

	if (_servers.empty())
	{
		throw Poco::SyntaxException("No valid hosts found in replica set URI");
	}

	// Parse path and query using Poco::URI
	// Create a simplified URI with just the scheme and path/query for Poco::URI to parse
	std::string simplifiedURI = scheme + "://localhost" + pathAndQuery;
	Poco::URI theURI(simplifiedURI);

	// Extract database from path
	std::string path = theURI.getPath();
	if (!path.empty() && path[0] == '/')
	{
		_database = path.substr(1);  // Remove leading '/'
	}
	else
	{
		_database = path;
	}

	// Parse query parameters
	Poco::URI::QueryParameters params = theURI.getQueryParameters();
	parseOptions(params);
}


void ReplicaSetURI::parseOptions(const Poco::URI::QueryParameters& params)
{
	for (const auto& param : params)
	{
		if (param.first == "replicaSet"s)
		{
			setReplicaSet(param.second);
		}
		else if (param.first == "readPreference"s)
		{
			setReadPreference(param.second);
		}
		else if (param.first == "connectTimeoutMS"s)
		{
			setConnectTimeoutMS(Poco::NumberParser::parseUnsigned(param.second));
		}
		else if (param.first == "socketTimeoutMS"s)
		{
			setSocketTimeoutMS(Poco::NumberParser::parseUnsigned(param.second));
		}
		else if (param.first == "heartbeatFrequencyMS"s)
		{
			setHeartbeatFrequencyMS(Poco::NumberParser::parseUnsigned(param.second));
		}
		else if (param.first == "reconnectRetries"s)
		{
			setReconnectRetries(Poco::NumberParser::parseUnsigned(param.second));
		}
		else if (param.first == "reconnectDelay"s)
		{
			setReconnectDelay(Poco::NumberParser::parseUnsigned(param.second));
		}
		// Add other options as needed
	}
}


std::string ReplicaSetURI::buildQueryString() const
{
	std::vector<std::string> params;

	if (!_replicaSet.empty())
	{
		params.push_back("replicaSet=" + _replicaSet);
	}

	if (_readPreference.mode() != ReadPreference::Primary)
	{
		params.push_back("readPreference=" + _readPreference.toString());
	}

	if (_connectTimeoutMS != DEFAULT_CONNECT_TIMEOUT_MS)  // Only add if non-default
	{
		params.push_back("connectTimeoutMS=" + std::to_string(_connectTimeoutMS));
	}

	if (_socketTimeoutMS != DEFAULT_SOCKET_TIMEOUT_MS)  // Only add if non-default
	{
		params.push_back("socketTimeoutMS=" + std::to_string(_socketTimeoutMS));
	}

	if (_heartbeatFrequencyMS != DEFAULT_HEARTBEAT_FREQUENCY_MS)  // Only add if non-default
	{
		params.push_back("heartbeatFrequencyMS=" + std::to_string(_heartbeatFrequencyMS));
	}

	if (_reconnectRetries != DEFAULT_RECONNECT_RETRIES)  // Only add if non-default
	{
		params.push_back("reconnectRetries=" + std::to_string(_reconnectRetries));
	}

	if (_reconnectDelay != DEFAULT_RECONNECT_DELAY)  // Only add if non-default
	{
		params.push_back("reconnectDelay=" + std::to_string(_reconnectDelay));
	}

	if (params.empty())
	{
		return "";
	}

	std::ostringstream queryString;
	for (std::size_t i = 0; i < params.size(); ++i)
	{
		if (i > 0)
		{
			queryString << "&";
		}
		queryString << params[i];
	}

	return queryString.str();
}


} } // namespace Poco::MongoDB
