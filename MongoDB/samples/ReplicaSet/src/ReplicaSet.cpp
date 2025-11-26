//
// ReplicaSet.cpp
//
// This sample demonstrates how to use MongoDB replica set features in Poco::MongoDB.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/ReplicaSet.h"
#include "Poco/MongoDB/ReplicaSetConnection.h"
#include "Poco/MongoDB/ReplicaSetPoolableConnectionFactory.h"
#include "Poco/MongoDB/ReadPreference.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/ObjectId.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/SharedPtr.h"
#include "Poco/ObjectPool.h"
#include "Poco/Exception.h"
#include <iostream>


using namespace Poco::MongoDB;
using namespace Poco;


void printUsage()
{
	std::cout << "Usage: ReplicaSet <command> [options]" << std::endl;
	std::cout << std::endl;
	std::cout << "Commands:" << std::endl;
	std::cout << "  basic        - Basic replica set connection example" << std::endl;
	std::cout << "  readpref     - Read preference examples" << std::endl;
	std::cout << "  failover     - Automatic failover demonstration" << std::endl;
	std::cout << "  pool         - Connection pooling example" << std::endl;
	std::cout << "  topology     - Topology discovery and monitoring" << std::endl;
	std::cout << std::endl;
	std::cout << "Environment variables:" << std::endl;
	std::cout << "  MONGODB_REPLICA_SET - Replica set name (default: rs0)" << std::endl;
	std::cout << "  MONGODB_HOSTS       - Comma-separated host:port list (default: localhost:27017,localhost:27018,localhost:27019)" << std::endl;
	std::cout << std::endl;
}


std::vector<Net::SocketAddress> parseHosts(const std::string& hostsStr)
{
	std::vector<Net::SocketAddress> hosts;
	std::size_t start = 0;
	std::size_t end = hostsStr.find(',');

	while (end != std::string::npos)
	{
		hosts.push_back(Net::SocketAddress(hostsStr.substr(start, end - start)));
		start = end + 1;
		end = hostsStr.find(',', start);
	}
	hosts.push_back(Net::SocketAddress(hostsStr.substr(start)));

	return hosts;
}


void basicExample()
{
	std::cout << "=== Basic Replica Set Connection ===" << std::endl;
	std::cout << std::endl;

	try
	{
		// Get replica set configuration from environment
		std::string setName = std::getenv("MONGODB_REPLICA_SET") ? std::getenv("MONGODB_REPLICA_SET") : "rs0";
		std::string hostsStr = std::getenv("MONGODB_HOSTS") ? std::getenv("MONGODB_HOSTS") : "localhost:27017,localhost:27018,localhost:27019";

		std::cout << "Connecting to replica set: " << setName << std::endl;
		std::cout << "Seed hosts: " << hostsStr << std::endl;
		std::cout << std::endl;

		// Configure replica set
		ReplicaSet::Config config;
		config.setName = setName;
		config.seeds = parseHosts(hostsStr);
		config.readPreference = ReadPreference::Primary;
		config.enableMonitoring = true;

		// Create replica set
		ReplicaSet rs(config);

		std::cout << "Replica set discovered successfully!" << std::endl;
		std::cout << "Replica set name: " << rs.setName() << std::endl;
		std::cout << "Has primary: " << (rs.hasPrimary() ? "Yes" : "No") << std::endl;
		std::cout << std::endl;

		// Get primary connection
		Connection::Ptr conn = rs.getPrimaryConnection();
		if (conn.isNull())
		{
			std::cerr << "ERROR: No primary server available" << std::endl;
			return;
		}

		std::cout << "Connected to primary: " << conn->address().toString() << std::endl;
		std::cout << std::endl;

		// Perform a simple operation
		Database db("test");
		Document::Ptr buildInfo = db.queryBuildInfo(*conn);
		std::cout << "MongoDB version: " << buildInfo->get<std::string>("version") << std::endl;
		std::cout << std::endl;

		// Insert a test document
		std::cout << "Inserting test document..." << std::endl;
		Document::Ptr doc = new Document();
		doc->add("name", "Replica Set Example");
		doc->add("timestamp", static_cast<Poco::Int64>(time(nullptr)));
		doc->add("message", "Hello from Poco::MongoDB replica set!");

		OpMsgMessage insertRequest("test", "poco_samples");
		insertRequest.setCommandName(OpMsgMessage::CMD_INSERT);
		insertRequest.documents().push_back(doc);

		OpMsgMessage response;
		conn->sendRequest(insertRequest, response);

		if (response.responseOk())
		{
			std::cout << "Document inserted successfully!" << std::endl;
			std::cout << "Response: " << response.body().toString() << std::endl;
		}
		else
		{
			std::cerr << "ERROR: Insert failed: " << response.body().toString() << std::endl;
		}
	}
	catch (const Exception& e)
	{
		std::cerr << "ERROR: " << e.displayText() << std::endl;
	}

	std::cout << std::endl;
}


void readPreferenceExample()
{
	std::cout << "=== Read Preference Examples ===" << std::endl;
	std::cout << std::endl;

	try
	{
		std::string setName = std::getenv("MONGODB_REPLICA_SET") ? std::getenv("MONGODB_REPLICA_SET") : "rs0";
		std::string hostsStr = std::getenv("MONGODB_HOSTS") ? std::getenv("MONGODB_HOSTS") : "localhost:27017,localhost:27018,localhost:27019";

		ReplicaSet::Config config;
		config.setName = setName;
		config.seeds = parseHosts(hostsStr);
		ReplicaSet rs(config);

		// Primary read preference
		std::cout << "1. Primary read preference:" << std::endl;
		Connection::Ptr primaryConn = rs.getConnection(ReadPreference::Primary);
		if (!primaryConn.isNull())
		{
			std::cout << "   Connected to: " << primaryConn->address().toString() << std::endl;
		}
		std::cout << std::endl;

		// Secondary read preference
		std::cout << "2. Secondary read preference:" << std::endl;
		Connection::Ptr secondaryConn = rs.getConnection(ReadPreference::Secondary);
		if (!secondaryConn.isNull())
		{
			std::cout << "   Connected to: " << secondaryConn->address().toString() << std::endl;
		}
		else
		{
			std::cout << "   No secondary available" << std::endl;
		}
		std::cout << std::endl;

		// PrimaryPreferred read preference
		std::cout << "3. PrimaryPreferred read preference:" << std::endl;
		Connection::Ptr prefConn = rs.getConnection(ReadPreference::PrimaryPreferred);
		if (!prefConn.isNull())
		{
			std::cout << "   Connected to: " << prefConn->address().toString() << std::endl;
		}
		std::cout << std::endl;

		// Nearest read preference (lowest latency)
		std::cout << "4. Nearest read preference (lowest latency):" << std::endl;
		Connection::Ptr nearestConn = rs.getConnection(ReadPreference::Nearest);
		if (!nearestConn.isNull())
		{
			std::cout << "   Connected to: " << nearestConn->address().toString() << std::endl;
		}
		std::cout << std::endl;
	}
	catch (const Exception& e)
	{
		std::cerr << "ERROR: " << e.displayText() << std::endl;
	}
}


void failoverExample()
{
	std::cout << "=== Automatic Failover Demonstration ===" << std::endl;
	std::cout << std::endl;

	try
	{
		std::string setName = std::getenv("MONGODB_REPLICA_SET") ? std::getenv("MONGODB_REPLICA_SET") : "rs0";
		std::string hostsStr = std::getenv("MONGODB_HOSTS") ? std::getenv("MONGODB_HOSTS") : "localhost:27017,localhost:27018,localhost:27019";

		ReplicaSet::Config config;
		config.setName = setName;
		config.seeds = parseHosts(hostsStr);
		config.readPreference = ReadPreference::PrimaryPreferred;
		ReplicaSet rs(config);

		// Create a replica set connection with automatic failover
		ReplicaSetConnection::Ptr rsConn = new ReplicaSetConnection(rs, ReadPreference::PrimaryPreferred);

		std::cout << "Using ReplicaSetConnection for automatic failover" << std::endl;
		std::cout << "Initial connection: " << rsConn->address().toString() << std::endl;
		std::cout << std::endl;

		// Perform multiple operations
		// If a server fails, operations will automatically retry on another server
		for (int i = 0; i < 5; ++i)
		{
			std::cout << "Operation " << (i + 1) << ": ";

			try
			{
				Document::Ptr doc = new Document();
				doc->add("iteration", i);
				doc->add("timestamp", static_cast<Poco::Int64>(time(nullptr)));

				OpMsgMessage insertRequest("test", "poco_failover");
				insertRequest.setCommandName(OpMsgMessage::CMD_INSERT);
				insertRequest.documents().push_back(doc);

				OpMsgMessage response;
				rsConn->sendRequest(insertRequest, response);

				if (response.responseOk())
				{
					std::cout << "Success (server: " << rsConn->address().toString() << ")" << std::endl;
				}
				else
				{
					std::cout << "Failed: " << response.body().toString() << std::endl;
				}
			}
			catch (const Exception& e)
			{
				std::cout << "Failed: " << e.displayText() << std::endl;
			}

			Poco::Thread::sleep(1000);  // Sleep 1 second between operations
		}

		std::cout << std::endl;
		std::cout << "Note: If a server fails during these operations, the connection" << std::endl;
		std::cout << "will automatically fail over to another replica set member." << std::endl;
	}
	catch (const Exception& e)
	{
		std::cerr << "ERROR: " << e.displayText() << std::endl;
	}

	std::cout << std::endl;
}


void poolExample()
{
	std::cout << "=== Connection Pooling Example ===" << std::endl;
	std::cout << std::endl;

	try
	{
		std::string setName = std::getenv("MONGODB_REPLICA_SET") ? std::getenv("MONGODB_REPLICA_SET") : "rs0";
		std::string hostsStr = std::getenv("MONGODB_HOSTS") ? std::getenv("MONGODB_HOSTS") : "localhost:27017,localhost:27018,localhost:27019";

		ReplicaSet::Config config;
		config.setName = setName;
		config.seeds = parseHosts(hostsStr);
		SharedPtr<ReplicaSet> rs(new ReplicaSet(config));

		std::cout << "Creating connection pool..." << std::endl;
		std::cout << "  Capacity: 5 connections" << std::endl;
		std::cout << "  Peak reserve: 10 connections" << std::endl;
		std::cout << std::endl;

		// Create connection pool
		PoolableObjectFactory<ReplicaSetConnection, ReplicaSetConnection::Ptr> factory(*rs, ReadPreference::PrimaryPreferred);
		ObjectPool<ReplicaSetConnection, ReplicaSetConnection::Ptr> pool(factory, 5, 10);

		// Use pooled connections
		std::cout << "Performing operations with pooled connections..." << std::endl;
		for (int i = 0; i < 10; ++i)
		{
			// Borrow connection from pool (RAII pattern)
			PooledReplicaSetConnection conn(pool);

			std::cout << "  Operation " << (i + 1) << ": ";

			Document::Ptr doc = new Document();
			doc->add("pool_test", i);
			doc->add("timestamp", static_cast<Poco::Int64>(time(nullptr)));

			OpMsgMessage insertRequest("test", "poco_pool");
			insertRequest.setCommandName(OpMsgMessage::CMD_INSERT);
			insertRequest.documents().push_back(doc);

			OpMsgMessage response;
			conn->sendRequest(insertRequest, response);

			if (response.responseOk())
			{
				std::cout << "Success" << std::endl;
			}
			else
			{
				std::cout << "Failed" << std::endl;
			}

			// Connection is automatically returned to pool when conn goes out of scope
		}

		std::cout << std::endl;
		std::cout << "All operations completed using pooled connections." << std::endl;
	}
	catch (const Exception& e)
	{
		std::cerr << "ERROR: " << e.displayText() << std::endl;
	}

	std::cout << std::endl;
}


void topologyExample()
{
	std::cout << "=== Topology Discovery and Monitoring ===" << std::endl;
	std::cout << std::endl;

	try
	{
		std::string setName = std::getenv("MONGODB_REPLICA_SET") ? std::getenv("MONGODB_REPLICA_SET") : "rs0";
		std::string hostsStr = std::getenv("MONGODB_HOSTS") ? std::getenv("MONGODB_HOSTS") : "localhost:27017,localhost:27018,localhost:27019";

		ReplicaSet::Config config;
		config.setName = setName;
		config.seeds = parseHosts(hostsStr);
		config.heartbeatFrequency = Poco::Timespan(5, 0);  // 5 seconds
		config.enableMonitoring = true;
		ReplicaSet rs(config);

		std::cout << "Replica Set Topology:" << std::endl;
		std::cout << "  Set name: " << rs.setName() << std::endl;
		std::cout << std::endl;

		TopologyDescription topology = rs.topology();
		std::vector<ServerDescription> servers = topology.servers();

		std::cout << "  Servers (" << servers.size() << "):" << std::endl;
		for (const auto& server : servers)
		{
			std::cout << "    - " << server.address().toString() << std::endl;
			std::cout << "      Type: ";
			switch (server.type())
			{
			case ServerDescription::RsPrimary:
				std::cout << "Primary";
				break;
			case ServerDescription::RsSecondary:
				std::cout << "Secondary";
				break;
			case ServerDescription::RsArbiter:
				std::cout << "Arbiter";
				break;
			case ServerDescription::Unknown:
				std::cout << "Unknown";
				break;
			default:
				std::cout << "Other";
			}
			std::cout << std::endl;

			std::cout << "      RTT: " << (server.roundTripTime() / 1000.0) << " ms" << std::endl;

			if (server.hasError())
			{
				std::cout << "      Error: " << server.error() << std::endl;
			}
		}

		std::cout << std::endl;
		std::cout << "Background monitoring is active (heartbeat every " << config.heartbeatFrequency.totalSeconds() << "s)" << std::endl;
		std::cout << "Topology will be automatically updated as servers change state." << std::endl;
	}
	catch (const Exception& e)
	{
		std::cerr << "ERROR: " << e.displayText() << std::endl;
	}

	std::cout << std::endl;
}


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printUsage();
		return 1;
	}

	std::string command(argv[1]);

	if (command == "basic")
	{
		basicExample();
	}
	else if (command == "readpref")
	{
		readPreferenceExample();
	}
	else if (command == "failover")
	{
		failoverExample();
	}
	else if (command == "pool")
	{
		poolExample();
	}
	else if (command == "topology")
	{
		topologyExample();
	}
	else
	{
		std::cerr << "Unknown command: " << command << std::endl;
		std::cout << std::endl;
		printUsage();
		return 1;
	}

	return 0;
}
