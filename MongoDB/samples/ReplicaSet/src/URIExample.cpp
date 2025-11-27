//
// URIExample.cpp
//
// Demonstrates MongoDB replica set URI parsing
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/ReplicaSet.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/Exception.h"
#include <iostream>


using namespace Poco::MongoDB;
using namespace Poco;


void printUsage()
{
	std::cout << "MongoDB Replica Set URI Example" << std::endl;
	std::cout << std::endl;
	std::cout << "Usage: URIExample <uri>" << std::endl;
	std::cout << std::endl;
	std::cout << "URI Format:" << std::endl;
	std::cout << "  mongodb://host1:port1,host2:port2,.../?options" << std::endl;
	std::cout << std::endl;
	std::cout << "Supported Options:" << std::endl;
	std::cout << "  replicaSet=name              Replica set name" << std::endl;
	std::cout << "  readPreference=mode          primary|primaryPreferred|secondary|secondaryPreferred|nearest" << std::endl;
	std::cout << "  connectTimeoutMS=ms          Connection timeout in milliseconds" << std::endl;
	std::cout << "  socketTimeoutMS=ms           Socket timeout in milliseconds" << std::endl;
	std::cout << "  heartbeatFrequencyMS=ms      Heartbeat frequency in milliseconds" << std::endl;
	std::cout << std::endl;
	std::cout << "Examples:" << std::endl;
	std::cout << "  URIExample 'mongodb://localhost:27017,localhost:27018,localhost:27019/?replicaSet=rs0'" << std::endl;
	std::cout << "  URIExample 'mongodb://mongo1:27017,mongo2:27017/?replicaSet=rs0&readPreference=primaryPreferred'" << std::endl;
	std::cout << "  URIExample 'mongodb://host1:27017,host2:27017/?replicaSet=rs0&heartbeatFrequencyMS=5000'" << std::endl;
	std::cout << std::endl;
}


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printUsage();
		return 1;
	}

	std::string uri = argv[1];

	std::cout << "Parsing MongoDB Replica Set URI" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	std::cout << "URI: " << uri << std::endl;
	std::cout << std::endl;

	try
	{
		// Create replica set from URI
		ReplicaSet rs(uri);

		std::cout << "✓ URI parsed successfully!" << std::endl;
		std::cout << std::endl;

		// Display configuration
		std::cout << "Configuration:" << std::endl;
		std::cout << std::string(80, '-') << std::endl;

		std::cout << "Replica Set Name: "
		          << (rs.setName().empty() ? "(auto-discover)" : rs.setName())
		          << std::endl;

		std::cout << "Read Preference:  " << rs.readPreference().toString() << std::endl;

		std::cout << "Seed Servers:     ";
		TopologyDescription topology = rs.topology();
		std::vector<ServerDescription> servers = topology.servers();
		for (size_t i = 0; i < servers.size(); ++i)
		{
			if (i > 0) std::cout << ", ";
			std::cout << servers[i].address().toString();
		}
		std::cout << std::endl;

		std::cout << "Monitoring:       " << (rs.hasPrimary() ? "Active" : "Starting...") << std::endl;

		std::cout << std::endl;

		// Try to connect to the replica set
		std::cout << "Connecting to replica set..." << std::endl;

		Connection::Ptr conn = rs.getPrimaryConnection();
		if (conn.isNull())
		{
			std::cerr << "✗ Failed: No primary server available" << std::endl;
			return 1;
		}

		std::cout << "✓ Connected to primary: " << conn->address().toString() << std::endl;
		std::cout << std::endl;

		// Query server information
		Database db("admin");
		Document::Ptr buildInfo = db.queryBuildInfo(*conn);

		std::cout << "Server Information:" << std::endl;
		std::cout << std::string(80, '-') << std::endl;
		std::cout << "MongoDB Version: " << buildInfo->get<std::string>("version") << std::endl;

		if (buildInfo->exists("gitVersion"))
		{
			std::cout << "Git Version:     " << buildInfo->get<std::string>("gitVersion") << std::endl;
		}

		std::cout << std::endl;

		// Display full topology
		std::cout << "Replica Set Topology:" << std::endl;
		std::cout << std::string(80, '-') << std::endl;
		std::cout << "Set Name:    " << topology.setName() << std::endl;
		std::cout << "Has Primary: " << (topology.hasPrimary() ? "Yes" : "No") << std::endl;
		std::cout << "Servers:     " << topology.serverCount() << std::endl;
		std::cout << std::endl;

		for (const auto& server : servers)
		{
			std::cout << "  " << server.address().toString();

			if (server.isPrimary())
			{
				std::cout << " [PRIMARY]";
			}
			else if (server.isSecondary())
			{
				std::cout << " [SECONDARY]";
			}
			else
			{
				std::cout << " [" << (server.hasError() ? "ERROR" : "UNKNOWN") << "]";
			}

			std::cout << " RTT: " << (server.roundTripTime() / 1000.0) << " ms";

			if (server.hasError())
			{
				std::cout << " (" << server.error() << ")";
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
		std::cout << "✓ Success!" << std::endl;

		return 0;
	}
	catch (const Poco::SyntaxException& e)
	{
		std::cerr << "✗ URI Syntax Error: " << e.displayText() << std::endl;
		std::cerr << std::endl;
		std::cerr << "Please check your URI format." << std::endl;
		return 1;
	}
	catch (const Poco::UnknownURISchemeException& e)
	{
		std::cerr << "✗ Invalid URI Scheme: " << e.displayText() << std::endl;
		std::cerr << std::endl;
		std::cerr << "URI must start with 'mongodb://'" << std::endl;
		return 1;
	}
	catch (const Exception& e)
	{
		std::cerr << "✗ Error: " << e.displayText() << std::endl;
		return 1;
	}
}
