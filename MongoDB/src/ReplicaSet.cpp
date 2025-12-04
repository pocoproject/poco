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
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/Exception.h"
#include "Poco/Random.h"
#include "Poco/URI.h"
#include "Poco/NumberParser.h"
#include <chrono>

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


ReplicaSet::ReplicaSet(const std::string& uri)
{
	// Parse URI first to extract seeds and configuration
	parseURI(uri);

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


void ReplicaSet::setLogger(Logger::Ptr logger)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_config.logger = logger;
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


void ReplicaSet::discover()
{
	// Try to discover topology from seed servers
	bool discovered = false;

	const auto servers = _topology.servers();
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
			std::lock_guard<std::mutex> lock(_mutex);
			_topology.updateServer(address, doc, rttMicros);

			// Update replica set name if not set
			if (_config.setName.empty() && doc.exists("setName"s))
			{
				_config.setName = doc.get<std::string>("setName"s);
				_topology.setName(_config.setName);
			}
		}
		else
		{
			// Mark server as unknown
			std::lock_guard<std::mutex> lock(_mutex);
			_topology.markServerUnknown(address, "Hello command failed");
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
		_topology.markServerUnknown(address, "Unknown error");
	}
}


void ReplicaSet::updateTopologyFromAllServers() noexcept
{
	std::vector<ServerDescription> servers;

	{
		std::lock_guard<std::mutex> lock(_mutex);
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
	// Parse MongoDB URI: mongodb://[user:pass@]host1:port1,host2:port2[,hostN:portN]/[database][?options]
	Poco::URI theURI(uri);

	if (theURI.getScheme() != "mongodb")
	{
		throw Poco::UnknownURISchemeException("Replica set URI must use 'mongodb' scheme");
	}

	// Parse authority to extract multiple hosts
	// The authority in MongoDB URIs can be: host1:port1,host2:port2,host3:port3
	// Poco::URI will give us the full authority string, we need to parse it manually
	const auto& authority = theURI.getAuthority();

	// Remove userinfo if present (username:password@)
	const auto atPos = authority.find('@');
	const auto hostsStr = (atPos != std::string::npos) ? authority.substr(atPos + 1) : authority;

	// Parse comma-separated hosts
	_config.seeds.clear();
	std::string::size_type start = 0;
	std::string::size_type end;

	while ((end = hostsStr.find(',', start)) != std::string::npos)
	{
		const auto hostPort = hostsStr.substr(start, end - start);
		if (!hostPort.empty())
		{
			try
			{
				_config.seeds.emplace_back(hostPort);
			}
			catch (...)
			{
				// Skip invalid host addresses
			}
		}
		start = end + 1;
	}

	// Parse last host
	const auto lastHost = hostsStr.substr(start);
	if (!lastHost.empty())
	{
		try
		{
			_config.seeds.emplace_back(lastHost);
		}
		catch (...)
		{
			// Skip invalid host address
		}
	}

	if (_config.seeds.empty())
	{
		throw Poco::SyntaxException("No valid hosts found in replica set URI");
	}

	// Parse query parameters
	Poco::URI::QueryParameters params = theURI.getQueryParameters();
	for (const auto& param : params)
	{
		if (param.first == "replicaSet"s)
		{
			_config.setName = param.second;
		}
		else if (param.first == "readPreference"s)
		{
			// Parse read preference mode
			if (param.second == "primary"s)
			{
				_config.readPreference = ReadPreference(ReadPreference::Primary);
			}
			else if (param.second == "primaryPreferred"s)
			{
				_config.readPreference = ReadPreference(ReadPreference::PrimaryPreferred);
			}
			else if (param.second == "secondary"s)
			{
				_config.readPreference = ReadPreference(ReadPreference::Secondary);
			}
			else if (param.second == "secondaryPreferred"s)
			{
				_config.readPreference = ReadPreference(ReadPreference::SecondaryPreferred);
			}
			else if (param.second == "nearest"s)
			{
				_config.readPreference = ReadPreference(ReadPreference::Nearest);
			}
		}
		else if (param.first == "connectTimeoutMS"s)
		{
			Poco::Int64 timeoutMs = Poco::NumberParser::parse64(param.second);
			_config.connectTimeout = Poco::Timespan(timeoutMs * 1000);  // Convert ms to microseconds
		}
		else if (param.first == "socketTimeoutMS"s)
		{
			Poco::Int64 timeoutMs = Poco::NumberParser::parse64(param.second);
			_config.socketTimeout = Poco::Timespan(timeoutMs * 1000);  // Convert ms to microseconds
		}
		else if (param.first == "heartbeatFrequencyMS"s)
		{
			Poco::Int64 freqMs = Poco::NumberParser::parse64(param.second);
			_config.heartbeatFrequency = Poco::Timespan(freqMs * 1000);  // Convert ms to microseconds
		}
		// Note: readPreferenceTags and maxStalenessSeconds would require more complex parsing
		// and are not commonly used, so we skip them for now
	}
}


} } // namespace Poco::MongoDB
