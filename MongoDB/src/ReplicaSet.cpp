//
// ReplicaSet.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  ReplicaSet
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ReplicaSet.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/MongoDB/ReplicaSetURI.h"
#include "Poco/MongoDB/TopologyChangeNotification.h"
#include "Poco/Dynamic/Struct.h"
#include "Poco/Exception.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Random.h"
#include <exception>

using namespace std::string_literals;


namespace Poco {
namespace MongoDB {


//
// ReplicaSet
//


ReplicaSet::ReplicaSet(const Config& config):
	_config(config),
	_topology(config.setName)
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
	updateTopologyFromAllServers();

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
	updateTopologyFromAllServers();

	// Start monitoring if enabled
	if (_config.enableMonitoring)
	{
		startMonitoring();
	}
}


ReplicaSet::ReplicaSet(const std::string& uri)
{
	// Parse URI using ReplicaSetURI
	ReplicaSetURI parsedURI(uri);

	// Extract configuration from parsed URI
	// Resolve server strings to SocketAddress objects here
	_config.seeds.clear();
	for (const auto& serverStr : parsedURI.servers())
	{
		try
		{
			_config.seeds.emplace_back(serverStr);
		}
		catch (const std::exception& e)
		{
			// Skip servers that cannot be resolved via DNS
			// Note: URI parsing already succeeded - ReplicaSetURI stores servers as strings.
			// Servers that fail DNS resolution are not added to the seed list.
			// Only resolvable servers will be used for topology discovery.
		}
	}

	_config.setName = parsedURI.replicaSet();
	_config.readPreference = parsedURI.readPreference();
	_config.connectTimeoutSeconds = (parsedURI.connectTimeoutMS() + 999) / 1000;  // Convert ms to seconds (round up)
	_config.socketTimeoutSeconds = (parsedURI.socketTimeoutMS() + 999) / 1000;    // Convert ms to seconds (round up)
	_config.heartbeatFrequencySeconds = (parsedURI.heartbeatFrequencyMS() + 999) / 1000;  // Convert ms to seconds (round up)
	_config.serverReconnectRetries = parsedURI.reconnectRetries();
	_config.serverReconnectDelaySeconds = parsedURI.reconnectDelay();

	if (_config.seeds.empty())
	{
		throw Poco::InvalidArgumentException("Replica set URI must contain at least one host");
	}

	// Update topology with set name from config
	_topology.setName(_config.setName);

	// Add seed servers to topology
	for (const auto& seed : _config.seeds)
	{
		_topology.addServer(seed);
	}

	// Perform initial discovery
	updateTopologyFromAllServers();

	// Start monitoring if enabled
	if (_config.enableMonitoring)
	{
		startMonitoring();
	}
}


ReplicaSet::ReplicaSet(const ReplicaSetURI& uri)
{
	// Extract configuration from ReplicaSetURI object
	// Resolve server strings to SocketAddress objects here
	_config.seeds.clear();
	for (const auto& serverStr : uri.servers())
	{
		try
		{
			_config.seeds.emplace_back(serverStr);
		}
		catch (const std::exception& e)
		{
			// Skip servers that cannot be resolved via DNS
			// Note: URI parsing already succeeded - ReplicaSetURI stores servers as strings.
			// Servers that fail DNS resolution are not added to the seed list.
			// Only resolvable servers will be used for topology discovery.
		}
	}

	_config.setName = uri.replicaSet();
	_config.readPreference = uri.readPreference();
	_config.connectTimeoutSeconds = (uri.connectTimeoutMS() + 999) / 1000;  // Convert ms to seconds (round up)
	_config.socketTimeoutSeconds = (uri.socketTimeoutMS() + 999) / 1000;    // Convert ms to seconds (round up)
	_config.heartbeatFrequencySeconds = (uri.heartbeatFrequencyMS() + 999) / 1000;  // Convert ms to seconds (round up)
	_config.serverReconnectRetries = uri.reconnectRetries();
	_config.serverReconnectDelaySeconds = uri.reconnectDelay();

	if (_config.seeds.empty())
	{
		throw Poco::InvalidArgumentException("Replica set URI must contain at least one host");
	}

	// Update topology with set name from config
	_topology.setName(_config.setName);

	// Add seed servers to topology
	for (const auto& seed : _config.seeds)
	{
		_topology.addServer(seed);
	}

	// Perform initial discovery
	updateTopologyFromAllServers();

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


Connection::Ptr ReplicaSet::waitForServerAvailability(const ReadPreference& readPref)
{
	// Timeopoint when this thread started to wait for the server to become available
	const auto waitStartTime = std::chrono::steady_clock::now();

	// Coordinate waiting between threads to avoid redundant refresh attempts
	std::lock_guard<std::mutex> lock(_serverAvailabilityRetryMutex);

	if (waitStartTime <= _topologyRefreshTime)
	{
		// Another thread recently refreshed, try getting connection without waiting
		return getConnection(readPref);
	}

	// Retry up to serverReconnectRetries times to wait for servers to become available
	for (std::size_t i = 0; i < _config.serverReconnectRetries; ++i)
	{
		// Sleep before refreshing topology
		std::this_thread::sleep_for(std::chrono::seconds(_config.serverReconnectDelaySeconds));

		// Refresh topology to discover available servers
		refreshTopology();

		// Try to get a connection after refresh
		Connection::Ptr conn = getConnection(readPref);
		if (!conn.isNull())
		{
			return conn;
		}
	}

	// All retries exhausted, no servers available
	return nullptr;
}


TopologyDescription ReplicaSet::topology() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _topology;
}


ReplicaSet::Config ReplicaSet::configuration() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _config;
}

void ReplicaSet::refreshTopology()
{
	// Simply delegate to updateTopologyFromAllServers which handles
	// change detection and notification sending
	updateTopologyFromAllServers();
}


void ReplicaSet::startMonitoring()
{
	std::lock_guard<std::mutex> lock(_mutex);

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


void ReplicaSet::setSocketFactory(Connection::SocketFactory* factory)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_config.socketFactory = factory;
}


void ReplicaSet::setReadPreference(const ReadPreference& pref)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_config.readPreference = pref;
}


ReadPreference ReplicaSet::readPreference() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _config.readPreference;
}


std::string ReplicaSet::setName() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _topology.setName();
}


bool ReplicaSet::hasPrimary() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _topology.hasPrimary();
}


void ReplicaSet::monitor() noexcept
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
		auto sleepUntil = std::chrono::steady_clock::now() +
			std::chrono::seconds(_config.heartbeatFrequencySeconds);

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
		std::lock_guard<std::mutex> lock(_mutex);

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
			// Custom factories can be set via Config or using setSocketFactory().
			// They can access timeout values via configuration().connectTimeoutSeconds
			// and configuration().socketTimeoutSeconds to properly configure sockets.
			conn->connect(address.toString(), *_config.socketFactory);
		}
		else
		{
			conn->connect(address);
		}

		// Note: Connection class doesn't expose socket() accessor, so socket timeouts
		// must be configured during socket creation via custom SocketFactory.

		return conn;
	}
	catch (...)
	{
		// Mark server as unknown on connection failure
		std::lock_guard<std::mutex> lock(_mutex);
		_topology.markServerUnknown(address, "Connection failed");
		throw;
	}
}


void ReplicaSet::updateTopologyFromHello(const Net::SocketAddress& address) noexcept
{
	Connection::Ptr conn = new Connection();

	try
	{
		// Measure RTT
		auto startTime = std::chrono::high_resolution_clock::now();

		if (_config.socketFactory != nullptr)
		{
			// Custom factories can be set via Config or using setSocketFactory().
			// They can access timeout values via configuration() to configure sockets.
			conn->connect(address.toString(), *_config.socketFactory);
		}
		else
		{
			conn->connect(address);
		}

		// Note: Connection class doesn't expose socket() accessor, so socket timeouts
		// must be configured during socket creation via custom SocketFactory.

		// Send hello command
		OpMsgMessage request("admin"s, ""s);
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
			std::lock_guard<std::mutex> lock(_mutex);
			const ServerDescription& server = _topology.updateServer(address, doc, rttMicros);

			// Update replica set name if not set
			// Get set name from the updated server instead of re-querying the document
			if (_config.setName.empty() && !server.setName().empty())
			{
				_config.setName = server.setName();
				_topology.setName(_config.setName);
			}
		}
		else
		{
			// Mark server as unknown
			std::lock_guard<std::mutex> lock(_mutex);
			_topology.markServerUnknown(address, "Hello command failed"s);
		}
	}
	catch (const std::exception& e)
	{
		// Mark server as unknown
		std::lock_guard<std::mutex> lock(_mutex);
		_topology.markServerUnknown(address, e.what());
	}
	catch (...)
	{
		// Mark server as unknown
		std::lock_guard<std::mutex> lock(_mutex);
		_topology.markServerUnknown(address, "Unknown error"s);
	}
}


void ReplicaSet::updateTopologyFromAllServers() noexcept
{
	// Capture current topology before refresh
	TopologyDescription oldTopology;
	std::vector<ServerDescription> servers;

	{
		std::lock_guard<std::mutex> lock(_mutex);
		oldTopology = _topology;
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

	// Update timestamp after refreshing topology
	_topologyRefreshTime = std::chrono::steady_clock::now();

	// Get new topology and compare using comparison operator
	TopologyDescription newTopology;
	{
		std::lock_guard<std::mutex> lock(_mutex);
		newTopology = _topology;
	}

	// Check if topology changed using comparison operator
	if (oldTopology == newTopology)
	{
		// No change detected, nothing to notify
		return;
	}

	// Topology changed - build brief change description
	// Note: Build notification data outside mutex to allow handlers to access ReplicaSet methods
	std::string changeDescription;

	// Check topology type change
	if (oldTopology.type() != newTopology.type())
	{
		changeDescription = "Type: " + TopologyDescription::typeToString(oldTopology.type()) +
			" -> " + TopologyDescription::typeToString(newTopology.type());
	}

	// Check primary change
	auto oldPrimary = oldTopology.findPrimary();
	auto newPrimary = newTopology.findPrimary();
	bool oldHadPrimary = oldPrimary.type() != ServerDescription::Unknown;
	bool newHasPrimary = newPrimary.type() != ServerDescription::Unknown;

	if (oldHadPrimary != newHasPrimary)
	{
		if (!changeDescription.empty()) changeDescription += "; ";
		if (newHasPrimary)
			changeDescription += "Primary elected: " + newPrimary.address().toString();
		else
			changeDescription += "Primary lost: " + oldPrimary.address().toString();
	}
	else if (oldHadPrimary && newHasPrimary && oldPrimary != newPrimary)
	{
		if (!changeDescription.empty()) changeDescription += "; ";
		changeDescription += "Primary: " + oldPrimary.address().toString() +
			" -> " + newPrimary.address().toString();
	}

	// Check server count change
	if (oldTopology.serverCount() != newTopology.serverCount())
	{
		if (!changeDescription.empty()) changeDescription += "; ";
		changeDescription += "Servers: " + std::to_string(oldTopology.serverCount()) +
			" -> " + std::to_string(newTopology.serverCount());
	}

	// If no specific changes identified, use generic message
	if (changeDescription.empty())
	{
		changeDescription = "Topology updated";
	}

	// Send notification outside mutex to allow handlers to access ReplicaSet methods
	Poco::DynamicStruct notificationData;
	notificationData["replicaSet"s] = newTopology.setName();
	notificationData["timestamp"s] =
		std::chrono::duration_cast<std::chrono::seconds>( std::chrono::system_clock::now().time_since_epoch() ).count();
	notificationData["topologyType"s] = TopologyDescription::typeToString(newTopology.type());
	notificationData["changeDescription"s] = changeDescription;

	Poco::NotificationCenter::defaultCenter().postNotification(
		new TopologyChangeNotification(notificationData)
	);
}


} } // namespace Poco::MongoDB
