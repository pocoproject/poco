//
// TopologyDescription.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  TopologyDescription
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/TopologyDescription.h"
#include "Poco/Exception.h"


namespace Poco {
namespace MongoDB {


TopologyDescription::TopologyDescription() = default;


TopologyDescription::TopologyDescription(const std::string& setName):
	_setName(setName)
{
}


TopologyDescription::TopologyDescription(const TopologyDescription& other)
{
	Mutex::ScopedLock lock(other._mutex);
	_type = other._type;
	_setName = other._setName;
	_servers = other._servers;
}


TopologyDescription::TopologyDescription(TopologyDescription&& other) noexcept
{
	Mutex::ScopedLock lock(other._mutex);
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
		Mutex::ScopedLock lock1(&_mutex < &other._mutex ? _mutex : other._mutex);
		Mutex::ScopedLock lock2(&_mutex < &other._mutex ? other._mutex : _mutex);

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
		Mutex::ScopedLock lock1(&_mutex < &other._mutex ? _mutex : other._mutex);
		Mutex::ScopedLock lock2(&_mutex < &other._mutex ? other._mutex : _mutex);

		_type = other._type;
		_setName = std::move(other._setName);
		_servers = std::move(other._servers);
	}
	return *this;
}


TopologyDescription::TopologyType TopologyDescription::type() const
{
	Mutex::ScopedLock lock(_mutex);
	return _type;
}


std::string TopologyDescription::setName() const
{
	Mutex::ScopedLock lock(_mutex);
	return _setName;
}


void TopologyDescription::setName(const std::string& name)
{
	Mutex::ScopedLock lock(_mutex);
	_setName = name;
}


std::vector<ServerDescription> TopologyDescription::servers() const
{
	Mutex::ScopedLock lock(_mutex);
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
	Mutex::ScopedLock lock(_mutex);
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
	Mutex::ScopedLock lock(_mutex);
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
	Mutex::ScopedLock lock(_mutex);
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
	Mutex::ScopedLock lock(_mutex);
	return _servers.find(address) != _servers.end();
}


ServerDescription TopologyDescription::getServer(const Net::SocketAddress& address) const
{
	Mutex::ScopedLock lock(_mutex);
	auto it = _servers.find(address);
	if (it != _servers.end())
	{
		return it->second;
	}
	return ServerDescription(address);
}


void TopologyDescription::updateServer(const Net::SocketAddress& address, const Document& helloResponse, Poco::Int64 rttMicros)
{
	Mutex::ScopedLock lock(_mutex);

	// Find or create server description
	auto it = _servers.find(address);
	if (it == _servers.end())
	{
		it = _servers.try_emplace(address, address).first;
	}

	// Update from hello response
	it->second.updateFromHelloResponse(helloResponse, rttMicros);

	// Update replica set name if not set
	if (_setName.empty() && !it->second.setName().empty())
	{
		_setName = it->second.setName();
	}

	// Process newly discovered hosts
	processNewHosts(it->second);

	// Update topology type
	updateTopologyType();
}


void TopologyDescription::markServerUnknown(const Net::SocketAddress& address, const std::string& error)
{
	Mutex::ScopedLock lock(_mutex);

	auto it = _servers.find(address);
	if (it != _servers.end())
	{
		it->second.markError(error);
		updateTopologyType();
	}
}


void TopologyDescription::addServer(const Net::SocketAddress& address)
{
	Mutex::ScopedLock lock(_mutex);

	auto [it, inserted] = _servers.try_emplace(address, address);
	if (inserted)
	{
		updateTopologyType();
	}
}


void TopologyDescription::removeServer(const Net::SocketAddress& address)
{
	Mutex::ScopedLock lock(_mutex);

	_servers.erase(address);
	updateTopologyType();
}


void TopologyDescription::clear()
{
	Mutex::ScopedLock lock(_mutex);

	_servers.clear();
	_type = Unknown;
}


std::size_t TopologyDescription::serverCount() const
{
	Mutex::ScopedLock lock(_mutex);
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

	// Count server types
	int primaries = 0;
	int secondaries = 0;
	int mongosCount = 0;
	int unknownCount = 0;
	int standaloneCount = 0;

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
			secondaries++;
			break;
		case ServerDescription::Mongos:
			mongosCount++;
			break;
		case ServerDescription::Standalone:
			standaloneCount++;
			break;
		case ServerDescription::Unknown:
			unknownCount++;
			break;
		}
	}

	// Determine topology type
	if (mongosCount > 0)
	{
		_type = Sharded;
	}
	else if (standaloneCount > 0 && _servers.size() == 1)
	{
		_type = Single;
	}
	else if (primaries > 0)
	{
		_type = ReplicaSetWithPrimary;
	}
	else if (secondaries > 0 || !_setName.empty())
	{
		_type = ReplicaSetNoPrimary;
	}
	else
	{
		_type = Unknown;
	}
}


void TopologyDescription::processNewHosts(const ServerDescription& serverDesc)
{
	// This method must be called while holding the mutex

	// Add newly discovered hosts to the topology
	const auto& hosts = serverDesc.hosts();
	for (const auto& host : hosts)
	{
		_servers.try_emplace(host, host);
	}
}


} } // namespace Poco::MongoDB
