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
	// Capture current topology before refresh
	TopologyDescription oldTopology;
	{
		std::lock_guard<std::mutex> lock(_mutex);
		oldTopology = _topology;
	}

	// Update topology from all servers
	updateTopologyFromAllServers();

	// Check if logger is registered before building log messages
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_config.logger == nullptr)
		{
			// No logger registered, skip message preparation
			return;
		}
	}

	// Get new topology and compare using comparison operator
	TopologyDescription newTopology;
	{
		std::lock_guard<std::mutex> lock(_mutex);
		newTopology = _topology;
	}

	// Check if topology changed using comparison operator
	if (oldTopology == newTopology)
	{
		// No change detected, nothing to log
		return;
	}

	// Topology changed and logger is registered - build detailed change description for logging

	std::lock_guard<std::mutex> lock(_mutex);

	_config.logger->information("MongoDB replica set: ** Topology changed: ");

	// Compare topology type
	if (oldTopology.type() != newTopology.type())
	{
		std::string changeDescription =
			" Type changed from " + TopologyDescription::typeToString(oldTopology.type()) +
			" to " + TopologyDescription::typeToString(newTopology.type());

		_config.logger->information("MongoDB replica set: " + changeDescription);
	}

	// Compare primary server
	auto oldPrimary = oldTopology.findPrimary();
	auto newPrimary = newTopology.findPrimary();
	bool oldHadPrimary = oldPrimary.type() != ServerDescription::Unknown;
	bool newHasPrimary = newPrimary.type() != ServerDescription::Unknown;

	if (oldHadPrimary != newHasPrimary)
	{
		std::string changeDescription;
		if (newHasPrimary)
			changeDescription += " Primary elected: " + newPrimary.address().toString();
		else
			changeDescription += " Primary lost: " + oldPrimary.address().toString();

		_config.logger->information("MongoDB replica set: " + changeDescription);
	}
	else if (oldHadPrimary && newHasPrimary && oldPrimary != newPrimary)
	{
		std::string changeDescription =
			" Primary changed from " + oldPrimary.address().toString() +
			" to " + newPrimary.address().toString();

		_config.logger->information("MongoDB replica set: " + changeDescription);
	}

	// Compare server count
	if (oldTopology.serverCount() != newTopology.serverCount())
	{
		std::string changeDescription =
			" Server count changed from " + std::to_string(oldTopology.serverCount()) +
			" to " + std::to_string(newTopology.serverCount()) + "; ";

		_config.logger->information("MongoDB replica set: " + changeDescription);
	}

	// Compare server states using comparison operator
	auto oldServers = oldTopology.servers();
	auto newServers = newTopology.servers();
	for (const auto& newServer : newServers)
	{
		for (const auto& oldServer : oldServers)
		{
			if (newServer.address() == oldServer.address())
			{
				if (newServer != oldServer)
				{
					std::string changeDescription =
						" Server " + newServer.address().toString() +
						" changed from " + ServerDescription::typeToString(oldServer.type()) +
						" to " + ServerDescription::typeToString(newServer.type());

					_config.logger->information("MongoDB replica set: " + changeDescription);
				}
				break;
			}
		}
	}

	_config.logger->information("MongoDB replica set: Current topology: total servers: " + std::to_string(newTopology.serverCount()) );
	if (newTopology.hasPrimary())
		_config.logger->information("MongoDB replica set:  PRIMARY: " + newPrimary.address().toString());

	auto secondaries = newTopology.findSecondaries();
	if (!secondaries.empty())
	{
		std::string logMessage = " SECONDARIES=[";
		for (size_t i = 0; i < secondaries.size(); ++i)
		{
			if (i > 0) logMessage += ", ";
			logMessage += secondaries[i].address().toString();
		}
		logMessage += "]";
		_config.logger->information("MongoDB replica set: " + logMessage);
	}
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
			_config.connectTimeoutSeconds = static_cast<unsigned int>((timeoutMs + 999) / 1000);  // Convert ms to seconds (round up)
		}
		else if (param.first == "socketTimeoutMS"s)
		{
			Poco::Int64 timeoutMs = Poco::NumberParser::parse64(param.second);
			_config.socketTimeoutSeconds = static_cast<unsigned int>((timeoutMs + 999) / 1000);  // Convert ms to seconds (round up)
		}
		else if (param.first == "heartbeatFrequency"s)
		{
			_config.heartbeatFrequencySeconds = Poco::NumberParser::parseUnsigned(param.second);
		}
		else if (param.first == "reconnectRetries"s)
		{
			_config.serverReconnectRetries = Poco::NumberParser::parseUnsigned(param.second);
		}
		else if (param.first == "reconnectDelay"s)
		{
			_config.serverReconnectDelaySeconds = Poco::NumberParser::parseUnsigned(param.second);
		}
		// Note: readPreferenceTags and maxStalenessSeconds would require more complex parsing
		// and are not commonly used, so we skip them for now
	}
}


} } // namespace Poco::MongoDB
