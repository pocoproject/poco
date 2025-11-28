//
// ReplicaSetMonitor.cpp
//
// MongoDB Replica Set Monitoring and Health Check Tool
//
// This tool continuously monitors a MongoDB replica set by performing
// regular read/write operations and displaying the topology status.
// Useful for deployment verification and replica set health monitoring.
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
#include "Poco/MongoDB/ReadPreference.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/ObjectId.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/SharedPtr.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Poco/Stopwatch.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>


using namespace Poco::MongoDB;
using namespace Poco;


struct MonitorConfig
{
	std::string uri;
	std::string setName;
	std::vector<Net::SocketAddress> seeds;
	int intervalSeconds;
	std::string database;
	std::string collection;
	bool verbose;
	int maxIterations;

	MonitorConfig():
		uri(),
		setName("rs0"),
		intervalSeconds(5),
		database("test"),
		collection("poco_monitor"),
		verbose(false),
		maxIterations(-1)  // -1 means run forever
	{
	}
};


std::vector<Net::SocketAddress> parseHosts(const std::string& hostsStr)
{
	std::vector<Net::SocketAddress> hosts;
	std::size_t start = 0;
	std::size_t end = hostsStr.find(',');

	while (end != std::string::npos)
	{
		std::string hostStr = hostsStr.substr(start, end - start);
		try
		{
			hosts.push_back(Net::SocketAddress(hostStr));
		}
		catch (const Poco::Exception& e)
		{
			throw Poco::InvalidArgumentException("Invalid host address '"s + hostStr + "': "s + e.displayText() );
		}
		start = end + 1;
		end = hostsStr.find(',', start);
	}

	std::string lastHost = hostsStr.substr(start);
	try
	{
		hosts.push_back(Net::SocketAddress(lastHost));
	}
	catch (const Poco::Exception& e)
	{
		throw Poco::InvalidArgumentException("Invalid host address '"s + lastHost + "': "s + e.displayText() );
	}

	return hosts;
}


void printUsage()
{
	std::cout << "MongoDB Replica Set Monitor and Health Check Tool" << std::endl;
	std::cout << std::endl;
	std::cout << "Usage: ReplicaSetMonitor [options]" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "  -h, --help              Show this help message" << std::endl;
	std::cout << "  -u, --uri URI           MongoDB connection URI" << std::endl;
	std::cout << "                          (e.g., mongodb://host1:27017,host2:27017/?replicaSet=rs0)" << std::endl;
	std::cout << "  -s, --set NAME          Replica set name (default: rs0)" << std::endl;
	std::cout << "  -H, --hosts HOSTS       Comma-separated host:port list" << std::endl;
	std::cout << "                          (default: localhost:27017,localhost:27018,localhost:27019)" << std::endl;
	std::cout << "  -i, --interval SECONDS  Check interval in seconds (default: 5)" << std::endl;
	std::cout << "  -d, --database NAME     Database name (default: test)" << std::endl;
	std::cout << "  -c, --collection NAME   Collection name (default: poco_monitor)" << std::endl;
	std::cout << "  -v, --verbose           Verbose output" << std::endl;
	std::cout << "  -n, --iterations N      Number of iterations (default: unlimited)" << std::endl;
	std::cout << std::endl;
	std::cout << "Note: --uri takes precedence over --set and --hosts options." << std::endl;
	std::cout << std::endl;
	std::cout << "Environment variables:" << std::endl;
	std::cout << "  MONGODB_URI             MongoDB connection URI" << std::endl;
	std::cout << "  MONGODB_REPLICA_SET     Replica set name" << std::endl;
	std::cout << "  MONGODB_HOSTS           Comma-separated host:port list" << std::endl;
	std::cout << std::endl;
	std::cout << "Examples:" << std::endl;
	std::cout << "  # Using URI" << std::endl;
	std::cout << "  ReplicaSetMonitor -u 'mongodb://mongo1:27017,mongo2:27017/?replicaSet=rs0'" << std::endl;
	std::cout << std::endl;
	std::cout << "  # Using separate options" << std::endl;
	std::cout << "  ReplicaSetMonitor -s rs0 -H mongo1:27017,mongo2:27017,mongo3:27017 -i 10" << std::endl;
	std::cout << std::endl;
}


std::string getServerTypeString(ServerDescription::ServerType type)
{
	switch (type)
	{
	case ServerDescription::RsPrimary:
		return "PRIMARY";
	case ServerDescription::RsSecondary:
		return "SECONDARY";
	case ServerDescription::RsArbiter:
		return "ARBITER";
	case ServerDescription::Standalone:
		return "STANDALONE";
	case ServerDescription::Mongos:
		return "MONGOS";
	case ServerDescription::RsOther:
		return "OTHER";
	case ServerDescription::RsGhost:
		return "GHOST";
	case ServerDescription::Unknown:
	default:
		return "UNKNOWN";
	}
}


std::string getTopologyTypeString(TopologyDescription::TopologyType type)
{
	switch (type)
	{
	case TopologyDescription::Single:
		return "Single Server";
	case TopologyDescription::ReplicaSetWithPrimary:
		return "Replica Set (with Primary)";
	case TopologyDescription::ReplicaSetNoPrimary:
		return "Replica Set (no Primary)";
	case TopologyDescription::Sharded:
		return "Sharded Cluster";
	case TopologyDescription::Unknown:
	default:
		return "Unknown";
	}
}


void printTopology(const TopologyDescription& topology, bool detailed = false)
{
	std::cout << std::string(80, '=') << std::endl;
	std::cout << "TOPOLOGY STATUS" << std::endl;
	std::cout << std::string(80, '=') << std::endl;

	std::cout << "Replica Set: " << (topology.setName().empty() ? "(not set)" : topology.setName()) << std::endl;
	std::cout << "Type:        " << getTopologyTypeString(topology.type()) << std::endl;
	std::cout << "Has Primary: " << (topology.hasPrimary() ? "Yes" : "No") << std::endl;
	std::cout << std::endl;

	std::vector<ServerDescription> servers = topology.servers();
	std::cout << "Servers: " << servers.size() << std::endl;
	std::cout << std::string(80, '-') << std::endl;

	// Print header
	std::cout << std::left
	          << std::setw(30) << "Address"
	          << std::setw(12) << "Type"
	          << std::setw(10) << "RTT (ms)"
	          << std::setw(20) << "Status"
	          << std::endl;
	std::cout << std::string(80, '-') << std::endl;

	// Sort servers: primary first, then secondaries, then others
	std::vector<ServerDescription> primaries, secondaries, others;
	for (const auto& server : servers)
	{
		if (server.isPrimary())
			primaries.push_back(server);
		else if (server.isSecondary())
			secondaries.push_back(server);
		else
			others.push_back(server);
	}

	auto printServer = [&](const ServerDescription& server) {
		std::cout << std::left
		          << std::setw(30) << server.address().toString()
		          << std::setw(12) << getServerTypeString(server.type())
		          << std::setw(10) << std::fixed << std::setprecision(2) << (server.roundTripTime() / 1000.0);

		if (server.hasError())
		{
			std::cout << std::setw(20) << ("ERROR: " + server.error().substr(0, 30));
		}
		else
		{
			std::cout << std::setw(20) << "OK";
		}
		std::cout << std::endl;

		if (detailed && !server.tags().empty())
		{
			std::cout << "  Tags: " << server.tags().toString() << std::endl;
		}
	};

	for (const auto& server : primaries) printServer(server);
	for (const auto& server : secondaries) printServer(server);
	for (const auto& server : others) printServer(server);

	std::cout << std::string(80, '=') << std::endl;
	std::cout << std::endl;
}


bool performWrite(ReplicaSetConnection& conn, const std::string& database,
                  const std::string& collection, int iteration,
                  Stopwatch& timer, bool verbose)
{
	try
	{
		timer.restart();

		Document::Ptr doc = new Document();
		doc->add("iteration", iteration);
		doc->add("timestamp", static_cast<Poco::Int64>(std::time(nullptr)));
		doc->add("hostname", Poco::Environment::nodeName());
		doc->add("message", "Health check from Poco::MongoDB monitor");

		OpMsgMessage request(database, collection);
		request.setCommandName(OpMsgMessage::CMD_INSERT);
		request.documents().push_back(doc);

		OpMsgMessage response;
		conn.sendRequest(request, response);

		timer.stop();

		if (response.responseOk())
		{
			if (verbose)
			{
				std::cout << "  ✓ Write successful to " << conn.address().toString()
				          << " (" << timer.elapsed() / 1000 << " ms)" << std::endl;
			}
			return true;
		}
		else
		{
			std::cerr << "  ✗ Write failed: " << response.body().toString() << std::endl;
			return false;
		}
	}
	catch (const Exception& e)
	{
		std::cerr << "  ✗ Write exception: " << e.displayText() << std::endl;
		return false;
	}
}


bool performRead(ReplicaSetConnection& conn, const std::string& database,
                 const std::string& collection, Stopwatch& timer, bool verbose)
{
	try
	{
		timer.restart();

		OpMsgMessage request(database, collection);
		request.setCommandName(OpMsgMessage::CMD_FIND);
		request.body().add("limit", 1);

		// Sort by timestamp descending to get latest document
		Document::Ptr sort = new Document();
		sort->add("timestamp", -1);
		request.body().add("sort", sort);

		OpMsgMessage response;
		conn.sendRequest(request, response);

		timer.stop();

		if (response.responseOk())
		{
			if (verbose)
			{
				int docCount = response.documents().size();
				std::cout << "  ✓ Read successful from " << conn.address().toString()
				          << " (" << timer.elapsed() / 1000 << " ms, "
				          << docCount << " doc" << (docCount != 1 ? "s" : "") << ")" << std::endl;
			}
			return true;
		}
		else
		{
			std::cerr << "  ✗ Read failed: " << response.body().toString() << std::endl;
			return false;
		}
	}
	catch (const Exception& e)
	{
		std::cerr << "  ✗ Read exception: " << e.displayText() << std::endl;
		return false;
	}
}


void runMonitor(const MonitorConfig& config)
{
	try
	{
		// Create replica set - use URI if provided, otherwise use Config
		SharedPtr<ReplicaSet> rs;

		if (!config.uri.empty())
		{
			// Use URI constructor
			std::cout << "Connecting to replica set via URI" << std::endl;
			std::cout << "URI: " << config.uri << std::endl;
			std::cout << "Check interval: " << config.intervalSeconds << " seconds" << std::endl;
			std::cout << std::endl;

			rs = new ReplicaSet(config.uri);
		}
		else
		{
			// Use Config constructor
			ReplicaSet::Config rsConfig;
			rsConfig.setName = config.setName;
			rsConfig.seeds = config.seeds;
			rsConfig.readPreference = ReadPreference(ReadPreference::PrimaryPreferred);
			rsConfig.enableMonitoring = true;
			rsConfig.heartbeatFrequency = Poco::Timespan(5, 0);  // 5 seconds

			std::cout << "Connecting to replica set: " << config.setName << std::endl;
			std::cout << "Seed servers: ";
			for (size_t i = 0; i < config.seeds.size(); ++i)
			{
				if (i > 0) std::cout << ", ";
				std::cout << config.seeds[i].toString();
			}
			std::cout << std::endl;
			std::cout << "Check interval: " << config.intervalSeconds << " seconds" << std::endl;
			std::cout << std::endl;

			rs = new ReplicaSet(rsConfig);
		}

		std::cout << "Replica set connected successfully!" << std::endl;
		std::cout << "Background monitoring active." << std::endl;
		std::cout << std::endl;

		// Print initial topology
		printTopology(rs->topology(), config.verbose);

		// Create replica set connections for reads and writes
		ReplicaSetConnection::Ptr writeConn = new ReplicaSetConnection(*rs, ReadPreference(ReadPreference::Primary));
		ReplicaSetConnection::Ptr readConn = new ReplicaSetConnection(*rs, ReadPreference(ReadPreference::PrimaryPreferred));

		// Monitoring loop
		int iteration = 0;
		Stopwatch writeTimer, readTimer;
		int successfulWrites = 0;
		int successfulReads = 0;
		int failedWrites = 0;
		int failedReads = 0;

		while (config.maxIterations == -1 || iteration < config.maxIterations)
		{
			iteration++;

			std::string timestamp = DateTimeFormatter::format(
				DateTime(),
				DateTimeFormat::ISO8601_FORMAT
			);

			std::cout << "[" << timestamp << "] Check #" << iteration << std::endl;

			// Perform write to primary
			std::cout << "Write (Primary): ";
			if (performWrite(*writeConn, config.database, config.collection, iteration, writeTimer, config.verbose))
			{
				successfulWrites++;
				if (!config.verbose)
				{
					std::cout << "✓ OK (" << writeTimer.elapsed() / 1000 << " ms)" << std::endl;
				}
			}
			else
			{
				failedWrites++;
			}

			// Perform read
			std::cout << "Read (PrimaryPreferred): ";
			if (performRead(*readConn, config.database, config.collection, readTimer, config.verbose))
			{
				successfulReads++;
				if (!config.verbose)
				{
					std::cout << "✓ OK (" << readTimer.elapsed() / 1000 << " ms)" << std::endl;
				}
			}
			else
			{
				failedReads++;
			}

			// Print statistics
			double writeSuccessRate = successfulWrites * 100.0 / (successfulWrites + failedWrites);
			double readSuccessRate = successfulReads * 100.0 / (successfulReads + failedReads);

			std::cout << "Statistics: "
			          << "Writes: " << successfulWrites << "/" << (successfulWrites + failedWrites)
			          << " (" << std::fixed << std::setprecision(1) << writeSuccessRate << "%), "
			          << "Reads: " << successfulReads << "/" << (successfulReads + failedReads)
			          << " (" << std::fixed << std::setprecision(1) << readSuccessRate << "%)"
			          << std::endl;

			// Print topology every 10 iterations or on first iteration
			if (iteration % 10 == 0 || iteration == 1)
			{
				std::cout << std::endl;
				printTopology(rs->topology(), config.verbose);
			}
			else
			{
				std::cout << std::string(80, '-') << std::endl;
			}

			// Sleep before next iteration
			if (config.maxIterations == -1 || iteration < config.maxIterations)
			{
				Thread::sleep(config.intervalSeconds * 1000);
			}
		}

		// Final summary
		std::cout << std::endl;
		std::cout << std::string(80, '=') << std::endl;
		std::cout << "FINAL SUMMARY" << std::endl;
		std::cout << std::string(80, '=') << std::endl;
		std::cout << "Total iterations: " << iteration << std::endl;
		std::cout << "Successful writes: " << successfulWrites << " / " << (successfulWrites + failedWrites) << std::endl;
		std::cout << "Successful reads:  " << successfulReads << " / " << (successfulReads + failedReads) << std::endl;
		std::cout << std::string(80, '=') << std::endl;
	}
	catch (const Exception& e)
	{
		std::cerr << "ERROR: " << e.displayText() << std::endl;
		throw;
	}
}


int main(int argc, char** argv)
{
	MonitorConfig config;

	// Parse environment variables
	const char* envUri = std::getenv("MONGODB_URI");
	if (envUri)
	{
		config.uri = envUri;
	}

	const char* envSet = std::getenv("MONGODB_REPLICA_SET");
	if (envSet)
	{
		config.setName = envSet;
	}

	const char* envHosts = std::getenv("MONGODB_HOSTS");
	if (envHosts)
	{
		config.seeds = parseHosts(envHosts);
	}

	// Parse command line arguments
	for (int i = 1; i < argc; ++i)
	{
		std::string arg(argv[i]);

		if (arg == "-h" || arg == "--help")
		{
			printUsage();
			return 0;
		}
		else if ((arg == "-u" || arg == "--uri") && i + 1 < argc)
		{
			config.uri = argv[++i];
		}
		else if ((arg == "-s" || arg == "--set") && i + 1 < argc)
		{
			config.setName = argv[++i];
		}
		else if ((arg == "-H" || arg == "--hosts") && i + 1 < argc)
		{
			config.seeds = parseHosts(argv[++i]);
		}
		else if ((arg == "-i" || arg == "--interval") && i + 1 < argc)
		{
			config.intervalSeconds = std::stoi(argv[++i]);
		}
		else if ((arg == "-d" || arg == "--database") && i + 1 < argc)
		{
			config.database = argv[++i];
		}
		else if ((arg == "-c" || arg == "--collection") && i + 1 < argc)
		{
			config.collection = argv[++i];
		}
		else if (arg == "-v" || arg == "--verbose")
		{
			config.verbose = true;
		}
		else if ((arg == "-n" || arg == "--iterations") && i + 1 < argc)
		{
			config.maxIterations = std::stoi(argv[++i]);
		}
		else
		{
			std::cerr << "Unknown option: " << arg << std::endl;
			std::cout << std::endl;
			printUsage();
			return 1;
		}
	}

	// Use defaults if not configured and no URI provided
	if (config.uri.empty() && config.seeds.empty())
	{
		config.seeds = parseHosts("localhost:27017,localhost:27018,localhost:27019");
	}

	try
	{
		runMonitor(config);
		return 0;
	}
	catch (const Poco::Exception& e)
	{
		std::cerr << "Error: " << e.displayText() << std::endl;
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "Unknown error occurred" << std::endl;
		return 1;
	}
}
