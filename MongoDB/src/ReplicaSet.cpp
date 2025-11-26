//
// ReplicaSet.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReplicaSet
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ReplicaSet.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/Exception.h"
#include "Poco/Random.h"
#include "Poco/Thread.h"
#include <chrono>


namespace Poco {
namespace MongoDB {


//
// ReplicaSet::Config
//


ReplicaSet::Config::Config():
	seeds(),
	setName(),
	readPreference(ReadPreference::Primary),
	connectTimeout(10, 0),       // 10 seconds
	socketTimeout(30, 0),         // 30 seconds
	heartbeatFrequency(10, 0),    // 10 seconds
	enableMonitoring(true),
	socketFactory(nullptr)
{
}


//
// ReplicaSet
//


ReplicaSet::ReplicaSet(const Config& config):
	_config(config),
	_topology(config.setName),
	_mutex(),
	_monitorThread(),
	_stopMonitoring(false),
	_monitoringActive(false)
{
	if (_config.seeds.empty())
	{
		throw Poco::InvalidArgumentException("Replica set configuration must have at least one seed server");
	}

	// Add seed servers to topology
	for (const auto& seed : _config.seeds)
	{
		_topology.addServer(seed);
	}

	// Perform initial discovery
	discover();

	// Start monitoring if enabled
	if (_config.enableMonitoring)
	{
		startMonitoring();
	}
}


ReplicaSet::ReplicaSet(const std::vector<Net::SocketAddress>& seeds):
	ReplicaSet(Config())
{
	_config.seeds = seeds;

	if (_config.seeds.empty())
	{
		throw Poco::InvalidArgumentException("Replica set must have at least one seed server");
	}

	// Add seed servers to topology
	for (const auto& seed : _config.seeds)
	{
		_topology.addServer(seed);
	}

	// Perform initial discovery
	discover();

	// Start monitoring if enabled
	if (_config.enableMonitoring)
	{
		startMonitoring();
	}
}


ReplicaSet::ReplicaSet(const std::string& uri):
	ReplicaSet(Config())
{
	parseURI(uri);

	// Add seed servers to topology
	for (const auto& seed : _config.seeds)
	{
		_topology.addServer(seed);
	}

	// Perform initial discovery
	discover();

	// Start monitoring if enabled
	if (_config.enableMonitoring)
	{
		startMonitoring();
	}
}


ReplicaSet::~ReplicaSet()
{
	stopMonitoring();
}


Connection::Ptr ReplicaSet::getConnection(const ReadPreference& readPref)
{
	return selectServer(readPref);
}


Connection::Ptr ReplicaSet::getPrimaryConnection()
{
	return selectServer(ReadPreference(ReadPreference::Primary));
}


Connection::Ptr ReplicaSet::getSecondaryConnection()
{
	return selectServer(ReadPreference(ReadPreference::Secondary));
}


TopologyDescription ReplicaSet::topology() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _topology;
}


void ReplicaSet::refreshTopology()
{
	updateTopologyFromAllServers();
}


void ReplicaSet::startMonitoring()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (_monitoringActive.load())
	{
		return;  // Already running
	}

	_stopMonitoring.store(false);
	_monitoringActive.store(true);

	_monitorThread = std::thread([this]() {
		monitor();
	});
}


void ReplicaSet::stopMonitoring()
{
	_stopMonitoring.store(true);

	if (_monitorThread.joinable())
	{
		_monitorThread.join();
	}

	_monitoringActive.store(false);
}


void ReplicaSet::setReadPreference(const ReadPreference& pref)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_config.readPreference = pref;
}


ReadPreference ReplicaSet::readPreference() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _config.readPreference;
}


std::string ReplicaSet::setName() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _topology.setName();
}


bool ReplicaSet::hasPrimary() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _topology.hasPrimary();
}


Connection::Ptr ReplicaSet::findMaster()
{
	// Legacy method for backward compatibility
	return getPrimaryConnection();
}


Connection::Ptr ReplicaSet::isMaster(const Net::SocketAddress& address)
{
	// Legacy method for backward compatibility
	Connection::Ptr conn = new Connection();

	try
	{
		conn->connect(address);

		OpMsgMessage request("admin", "");
		request.setCommandName(OpMsgMessage::CMD_HELLO);

		OpMsgMessage response;
		conn->sendRequest(request, response);

		if (response.responseOk())
		{
			const Document& doc = response.body();
			if (doc.get<bool>("isWritablePrimary", false) || doc.get<bool>("ismaster", false))
			{
				return conn;
			}
			else if (doc.exists("primary"))
			{
				return isMaster(Net::SocketAddress(doc.get<std::string>("primary")));
			}
		}
	}
	catch (...)
	{
		conn = nullptr;
	}

	return nullptr;
}


void ReplicaSet::discover()
{
	// Try to discover topology from seed servers
	bool discovered = false;

	std::vector<ServerDescription> servers = _topology.servers();
	for (const auto& server : servers)
	{
		try
		{
			updateTopologyFromHello(server.address());
			discovered = true;
			break;  // Successfully discovered from at least one server
		}
		catch (...)
		{
			// Continue to next server
		}
	}

	if (!discovered)
	{
		throw Poco::IOException("Failed to discover replica set topology from any seed server");
	}
}


void ReplicaSet::monitor()
{
	while (!_stopMonitoring.load())
	{
		try
		{
			// Update topology from all known servers
			updateTopologyFromAllServers();
		}
		catch (...)
		{
			// Ignore errors during monitoring
		}

		// Sleep for heartbeat frequency
		auto sleepMs = _config.heartbeatFrequency.totalMilliseconds();
		auto sleepUntil = std::chrono::steady_clock::now() +
			std::chrono::milliseconds(sleepMs);

		// Check stop flag periodically during sleep
		while (std::chrono::steady_clock::now() < sleepUntil)
		{
			if (_stopMonitoring.load())
			{
				return;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}


Connection::Ptr ReplicaSet::selectServer(const ReadPreference& readPref)
{
	Net::SocketAddress selectedAddress;

	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		// Select servers based on read preference
		std::vector<ServerDescription> eligible = readPref.selectServers(_topology);

		if (eligible.empty())
		{
			return nullptr;  // No suitable server found
		}

		// Randomly select from eligible servers for load balancing
		Poco::Random random;
		int index = random.next(static_cast<int>(eligible.size()));
		selectedAddress = eligible[index].address();
	}

	// Create connection outside the lock
	return createConnection(selectedAddress);
}


Connection::Ptr ReplicaSet::createConnection(const Net::SocketAddress& address)
{
	Connection::Ptr conn = new Connection();

	try
	{
		if (_config.socketFactory != nullptr)
		{
			// Use custom socket factory (e.g., for SSL/TLS)
			// Note: Socket timeouts should be configured in the SocketFactory
			conn->connect(address.toString(), *_config.socketFactory);
		}
		else
		{
			conn->connect(address);
		}

		// Note: Socket timeouts are configured via URI or during socket creation
		// Connection class doesn't expose socket() accessor

		return conn;
	}
	catch (...)
	{
		// Mark server as unknown on connection failure
		Poco::FastMutex::ScopedLock lock(_mutex);
		_topology.markServerUnknown(address, "Connection failed");
		throw;
	}
}


void ReplicaSet::updateTopologyFromHello(const Net::SocketAddress& address)
{
	Connection::Ptr conn = new Connection();

	try
	{
		// Measure RTT
		auto startTime = std::chrono::high_resolution_clock::now();

		if (_config.socketFactory != nullptr)
		{
			conn->connect(address.toString(), *_config.socketFactory);
		}
		else
		{
			conn->connect(address);
		}

		// Note: Socket timeouts are configured via URI or during socket creation
		// Connection class doesn't expose socket() accessor

		// Send hello command
		OpMsgMessage request("admin", "");
		request.setCommandName(OpMsgMessage::CMD_HELLO);

		OpMsgMessage response;
		conn->sendRequest(request, response);

		auto endTime = std::chrono::high_resolution_clock::now();
		auto rttMicros = std::chrono::duration_cast<std::chrono::microseconds>(
			endTime - startTime).count();

		if (response.responseOk())
		{
			const Document& doc = response.body();

			// Update topology
			Poco::FastMutex::ScopedLock lock(_mutex);
			_topology.updateServer(address, doc, rttMicros);

			// Update replica set name if not set
			if (_config.setName.empty() && doc.exists("setName"))
			{
				_config.setName = doc.get<std::string>("setName");
				_topology.setName(_config.setName);
			}
		}
		else
		{
			// Mark server as unknown
			Poco::FastMutex::ScopedLock lock(_mutex);
			_topology.markServerUnknown(address, "Hello command failed");
		}
	}
	catch (const std::exception& e)
	{
		// Mark server as unknown
		Poco::FastMutex::ScopedLock lock(_mutex);
		_topology.markServerUnknown(address, e.what());
	}
	catch (...)
	{
		// Mark server as unknown
		Poco::FastMutex::ScopedLock lock(_mutex);
		_topology.markServerUnknown(address, "Unknown error");
	}
}


void ReplicaSet::updateTopologyFromAllServers()
{
	std::vector<ServerDescription> servers;

	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		servers = _topology.servers();
	}

	// Update each server (sequentially for now, could be parallelized)
	for (const auto& server : servers)
	{
		if (_stopMonitoring.load())
		{
			break;  // Stop if monitoring is being shut down
		}

		try
		{
			updateTopologyFromHello(server.address());
		}
		catch (...)
		{
			// Continue to next server
		}
	}
}


void ReplicaSet::parseURI(const std::string& uri)
{
	// Placeholder for URI parsing implementation
	// This would parse mongodb://host1:port1,host2:port2,...?options
	// For now, throw NotImplementedException
	throw Poco::NotImplementedException("URI parsing not yet implemented. Use Config constructor instead.");
}


} } // namespace Poco::MongoDB
