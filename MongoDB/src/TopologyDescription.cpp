//
// TopologyDescription.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  TopologyDescription
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/TopologyDescription.h"
#include "Poco/MongoDB/Document.h"
#include <utility>


namespace Poco {
namespace MongoDB {


TopologyDescription::TopologyDescription() = default;


TopologyDescription::TopologyDescription(const std::string& setName):
	_setName(setName)
{
}


TopologyDescription::TopologyDescription(const TopologyDescription& other)
{
	std::lock_guard<std::mutex> lock(other._mutex);
	_type = other._type;
	_setName = other._setName;
	_servers = other._servers;
}


TopologyDescription::TopologyDescription(TopologyDescription&& other) noexcept
{
	std::lock_guard<std::mutex> lock(other._mutex);
	_type = other._type;
	_setName = std::move(other._setName);
	_servers = std::move(other._servers);
}


TopologyDescription::~TopologyDescription() = default;


TopologyDescription& TopologyDescription::operator=(const TopologyDescription& other)
{
	if (this != &other)
	{
		// Lock both mutexes to avoid deadlock (lock in consistent order)
		std::scoped_lock lock(_mutex, other._mutex);

		_type = other._type;
		_setName = other._setName;
		_servers = other._servers;
	}
	return *this;
}


TopologyDescription& TopologyDescription::operator=(TopologyDescription&& other) noexcept
{
	if (this != &other)
	{
		// Lock both mutexes to avoid deadlock (lock in consistent order)
		std::scoped_lock lock(_mutex, other._mutex);

		_type = other._type;
		_setName = std::move(other._setName);
		_servers = std::move(other._servers);
	}
	return *this;
}


bool TopologyDescription::operator==(const TopologyDescription& other) const
{
	std::scoped_lock lock(_mutex, other._mutex);

	// Compare topology type
	if (_type != other._type)
		return false;

	// Compare set name
	if (_setName != other._setName)
		return false;

	// Compare servers map
	if (_servers.size() != other._servers.size())
		return false;

	// Compare each server
	for (const auto& [address, server] : _servers)
	{
		auto it = other._servers.find(address);
		if (it == other._servers.end())
			return false;

		if (server != it->second)
			return false;
	}

	return true;
}


bool TopologyDescription::operator!=(const TopologyDescription& other) const
{
	return !(*this == other);
}


TopologyDescription::TopologyType TopologyDescription::type() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _type;
}


std::string TopologyDescription::setName() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _setName;
}


void TopologyDescription::setName(const std::string& name)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_setName = name;
}


std::vector<ServerDescription> TopologyDescription::servers() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	std::vector<ServerDescription> result;
	result.reserve(_servers.size());
	for (const auto& [address, server] : _servers)
	{
		result.emplace_back(server);
	}
	return result;
}


ServerDescription TopologyDescription::findPrimary() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	for (const auto& [address, server] : _servers)
	{
		if (server.isPrimary())
		{
			return server;
		}
	}
	return ServerDescription();
}


std::vector<ServerDescription> TopologyDescription::findSecondaries() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	std::vector<ServerDescription> result;
	result.reserve(_servers.size());
	for (const auto& [address, server] : _servers)
	{
		if (server.isSecondary())
		{
			result.emplace_back(server);
		}
	}
	return result;
}


bool TopologyDescription::hasPrimary() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	for (const auto& [address, server] : _servers)
	{
		if (server.isPrimary())
		{
			return true;
		}
	}
	return false;
}


bool TopologyDescription::hasServer(const Net::SocketAddress& address) const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _servers.find(address) != _servers.end();
}


ServerDescription TopologyDescription::getServer(const Net::SocketAddress& address) const
{
	std::lock_guard<std::mutex> lock(_mutex);
	auto it = _servers.find(address);
	if (it != _servers.end())
	{
		return it->second;
	}
	return ServerDescription(address);
}


const ServerDescription& TopologyDescription::updateServer(const Net::SocketAddress& address, const Document& helloResponse, Poco::Int64 rttMicros)
{
	std::lock_guard<std::mutex> lock(_mutex);

	// Find or create server description
	auto it = _servers.find(address);
	if (it == _servers.end())
	{
		it = _servers.try_emplace(address, address).first;
	}

	// Update from hello response and get discovered hosts
	auto hosts = it->second.updateFromHelloResponse(helloResponse, rttMicros);

	// Validate replica set name matches
	// Per MongoDB SDAM specification: servers with mismatching replica set names
	// should be marked as Unknown and their discovered hosts should be ignored
	const std::string& serverSetName = it->second.setName();

	if (!serverSetName.empty() && !_setName.empty() && serverSetName != _setName)
	{
		// Replica set name mismatch - mark server as Unknown with error
		it->second.markError("Replica set name mismatch: expected '" + _setName +
		                     "', but server reports '" + serverSetName + "'");

		// Do NOT add discovered hosts from this server (they belong to a different replica set)
		// Do NOT update topology type yet - wait for next refresh
		updateTopologyType();
		return it->second;
	}

	// Update replica set name if not set (only if server reports a set name)
	if (_setName.empty() && !serverSetName.empty())
	{
		_setName = serverSetName;
	}

	// Add newly discovered hosts to the topology (only if set name matches)
	// This prevents cross-contamination between different replica sets
	// Note: We add ALL discovered hosts even if types might be incompatible
	// The updateTopologyType() will detect incompatibility and set topology to Unknown
	// This approach preserves all discovered servers for diagnostic purposes
	for (const auto& host : hosts)
	{
		_servers.try_emplace(host, host);
	}

	// Update topology type
	// This will detect any incompatible server type combinations and set topology to Unknown if needed
	updateTopologyType();

	// Return reference to the updated server
	return it->second;
}


void TopologyDescription::markServerUnknown(const Net::SocketAddress& address, const std::string& error)
{
	std::lock_guard<std::mutex> lock(_mutex);

	auto it = _servers.find(address);
	if (it != _servers.end())
	{
		it->second.markError(error);
		updateTopologyType();
	}
}


void TopologyDescription::addServer(const Net::SocketAddress& address)
{
	std::lock_guard<std::mutex> lock(_mutex);

	auto [_, inserted] = _servers.try_emplace(address, address);
	if (inserted)
	{
		updateTopologyType();
	}
}


void TopologyDescription::removeServer(const Net::SocketAddress& address)
{
	std::lock_guard<std::mutex> lock(_mutex);

	_servers.erase(address);
	updateTopologyType();
}


void TopologyDescription::clear()
{
	std::lock_guard<std::mutex> lock(_mutex);

	_servers.clear();
	_type = Unknown;
}


std::size_t TopologyDescription::serverCount() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _servers.size();
}


void TopologyDescription::updateTopologyType()
{
	// This method must be called while holding the mutex

	if (_servers.empty())
	{
		_type = Unknown;
		return;
	}

	// Count server types for topology classification
	// Note: These counters are used to determine the overall topology type
	// based on the types of servers that have been discovered and updated
	int primaries = 0;
	int otherRsMembers = 0;  // Non-primary replica set members (secondaries, arbiters, etc.)
	int mongosCount = 0;
	int standaloneCount = 0;
	[[maybe_unused]] int unknownCount = 0;

	for (const auto& [address, server] : _servers)
	{
		switch (server.type())
		{
		case ServerDescription::RsPrimary:
			primaries++;
			break;
		case ServerDescription::RsSecondary:
		case ServerDescription::RsArbiter:
		case ServerDescription::RsOther:
		case ServerDescription::RsGhost:
			// Count all non-primary replica set members together
			// for topology determination (they all indicate replica set membership)
			otherRsMembers++;
			break;
		case ServerDescription::Mongos:
			mongosCount++;
			break;
		case ServerDescription::Standalone:
			standaloneCount++;
			break;
		case ServerDescription::Unknown:
			// Unknown servers don't affect topology classification
			// Count them to help with diagnostics
			unknownCount++;
			break;
		}
	}

	// STEP 1: Validate server type compatibility (per MongoDB SDAM specification)
	// Incompatible combinations indicate misconfiguration and must result in Unknown topology
	// This prevents incorrect routing and security issues

	// Cannot mix mongos with replica set members
	if (mongosCount > 0 && (primaries > 0 || otherRsMembers > 0))
	{
		_type = Unknown;
		return;
	}

	// Cannot mix standalone with any other server type
	if (standaloneCount > 0 && (primaries > 0 || otherRsMembers > 0 || mongosCount > 0))
	{
		_type = Unknown;
		return;
	}

	// Multiple standalone servers is invalid (each standalone is independent)
	if (standaloneCount > 1)
	{
		_type = Unknown;
		return;
	}

	// STEP 2: Classify valid topologies
	// At this point, server types are compatible, so we can safely determine topology type

	if (mongosCount > 0)
	{
		// Sharded cluster: one or more mongos routers detected
		_type = Sharded;
	}
	else if (standaloneCount == 1 && _servers.size() == 1)
	{
		// Single standalone server - treat as Single topology
		// Standalone servers behave like a single primary for read preferences
		_type = Single;
	}
	else if (primaries > 0)
	{
		// Replica set with at least one primary
		_type = ReplicaSetWithPrimary;
	}
	else if (otherRsMembers > 0 || !_setName.empty())
	{
		// Replica set without primary: either we have non-primary replica set members,
		// or we have a configured setName (indicating this is intended to be a replica set)
		_type = ReplicaSetNoPrimary;
	}
	else
	{
		// Unable to determine topology (all servers are unknown or no clear pattern)
		_type = Unknown;
	}
}


std::string TopologyDescription::typeToString(TopologyType type)
{
	switch (type)
	{
	case Single:
		return "Single Server"s;
	case ReplicaSetWithPrimary:
		return "Replica Set (with Primary)"s;
	case ReplicaSetNoPrimary:
		return "Replica Set (no Primary)"s;
	case Sharded:
		return "Sharded Cluster"s;
	case Unknown:
	default:
		return "Unknown"s;
	}
}


} } // namespace Poco::MongoDB
