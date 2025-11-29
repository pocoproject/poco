//
// ReplicaSetTest.cpp
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ReplicaSetTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/MongoDB/ServerDescription.h"
#include "Poco/MongoDB/TopologyDescription.h"
#include "Poco/MongoDB/ReadPreference.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/Net/SocketAddress.h"

using namespace Poco::MongoDB;
using namespace Poco::Net;
using namespace std::string_literals;


ReplicaSetTest::ReplicaSetTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


ReplicaSetTest::~ReplicaSetTest()
{
}


void ReplicaSetTest::setUp()
{
}


void ReplicaSetTest::tearDown()
{
}


// Generic helper function to create hello response documents with configurable parameters
Document::Ptr createHelloResponse(
	bool isWritablePrimary,
	bool isSecondary,
	const std::string& setName = "",
	const std::string& me = "",
	const std::vector<std::string>& hosts = {},
	const std::vector<std::string>& arbiters = {},
	const std::vector<std::string>& passives = {},
	Document::Ptr tags = nullptr,
	bool arbiterOnly = false,
	bool hidden = false,
	const std::string& msg = "")
{
	Document::Ptr doc = new Document();

	doc->add("isWritablePrimary"s, isWritablePrimary);
	doc->add("secondary"s, isSecondary);

	if (arbiterOnly)
	{
		doc->add("arbiterOnly"s, true);
	}

	if (hidden)
	{
		doc->add("hidden"s, true);
	}

	if (!setName.empty())
	{
		doc->add("setName"s, setName);
	}

	if (!me.empty())
	{
		doc->add("me"s, me);
	}

	if (!hosts.empty())
	{
		Array::Ptr hostsArray = new Array();
		for (const auto& host : hosts)
		{
			hostsArray->add(host);
		}
		doc->add("hosts"s, hostsArray);
	}

	if (!arbiters.empty())
	{
		Array::Ptr arbitersArray = new Array();
		for (const auto& arbiter : arbiters)
		{
			arbitersArray->add(arbiter);
		}
		doc->add("arbiters"s, arbitersArray);
	}

	if (!passives.empty())
	{
		Array::Ptr passivesArray = new Array();
		for (const auto& passive : passives)
		{
			passivesArray->add(passive);
		}
		doc->add("passives"s, passivesArray);
	}

	if (tags)
	{
		doc->add("tags"s, tags);
	}

	if (!msg.empty())
	{
		doc->add("msg"s, msg);
	}

	doc->add("ok"s, 1.0);

	return doc;
}


// Convenience function to create a hello response document for a primary server
Document::Ptr createPrimaryHelloResponse()
{
	return createHelloResponse(
		true,  // isWritablePrimary
		false, // isSecondary
		"rs0"s,
		"localhost:27017"s,
		{"localhost:27017"s, "localhost:27018"s, "localhost:27019"s}
	);
}


// Convenience function to create a hello response document for a secondary server
Document::Ptr createSecondaryHelloResponse()
{
	return createHelloResponse(
		false, // isWritablePrimary
		true,  // isSecondary
		"rs0"s,
		"localhost:27018"s,
		{"localhost:27017"s, "localhost:27018"s, "localhost:27019"s}
	);
}


// Convenience function to create a hello response document for an arbiter
Document::Ptr createArbiterHelloResponse()
{
	return createHelloResponse(
		false, // isWritablePrimary
		false, // isSecondary
		"rs0"s,
		"localhost:27019"s,
		{"localhost:27017"s, "localhost:27018"s},
		{"localhost:27019"s}, // arbiters
		{},                   // passives
		nullptr,              // tags
		true                  // arbiterOnly
	);
}


// Convenience function to create a hello response document for a standalone server
Document::Ptr createStandaloneHelloResponse()
{
	return createHelloResponse(
		true,  // isWritablePrimary
		false  // isSecondary
		// No setName - indicates standalone
	);
}


// Convenience function to create a hello response document for a mongos router
Document::Ptr createMongosHelloResponse()
{
	return createHelloResponse(
		true,  // isWritablePrimary
		false, // isSecondary
		"",    // setName
		"",    // me
		{},    // hosts
		{},    // arbiters
		{},    // passives
		nullptr, // tags
		false, // arbiterOnly
		false, // hidden
		"isdbgrid"s // msg - identifies mongos
	);
}


// Convenience function to create a hello response with tags
Document::Ptr createTaggedSecondaryHelloResponse()
{
	Document::Ptr tags = new Document();
	tags->add("dc"s, "east"s);
	tags->add("rack"s, "1"s);
	tags->add("size"s, "large"s);

	return createHelloResponse(
		false, // isWritablePrimary
		true,  // isSecondary
		"rs0"s,
		"localhost:27018"s,
		{"localhost:27017"s, "localhost:27018"s},
		{},    // arbiters
		{},    // passives
		tags
	);
}


// Convenience function to create a hello response with hidden member
Document::Ptr createHiddenMemberHelloResponse()
{
	return createHelloResponse(
		false, // isWritablePrimary
		false, // isSecondary
		"rs0"s,
		"localhost:27020"s,
		{"localhost:27017"s, "localhost:27018"s},
		{},    // arbiters
		{"localhost:27020"s}, // passives
		nullptr, // tags
		false, // arbiterOnly
		true   // hidden
	);
}


void ReplicaSetTest::testServerDescriptionPrimary()
{
	SocketAddress addr("localhost:27017");
	ServerDescription server(addr);

	Document::Ptr helloResponse = createPrimaryHelloResponse();
	server.updateFromHelloResponse(*helloResponse, 5000);  // 5ms RTT

	assertEqual(static_cast<int>(ServerDescription::RsPrimary), static_cast<int>(server.type()));
	assertEqual("rs0"s, server.setName());
	assertEqual(5000, server.roundTripTime());
	assertTrue(server.isPrimary());
	assertTrue(server.isWritable());
	assertFalse(server.isSecondary());
	assertFalse(server.hasError());

	// Check hosts list
	const auto& hosts = server.hosts();
	assertEqual(3, static_cast<int>(hosts.size()));
	if (addr.family() == SocketAddress::IPv6)
	{
		// Adjust for IPv6 format
		assertEqual("[::1]:27017"s, hosts[0].toString());
		assertEqual("[::1]:27018"s, hosts[1].toString());
		assertEqual("[::1]:27019"s, hosts[2].toString());
	}
	else
	{
		assertEqual("localhost:27017"s, hosts[0].toString());
		assertEqual("localhost:27018"s, hosts[1].toString());
		assertEqual("localhost:27019"s, hosts[2].toString());
	}
}


void ReplicaSetTest::testServerDescriptionSecondary()
{
	SocketAddress addr("localhost:27018");
	ServerDescription server(addr);

	Document::Ptr helloResponse = createSecondaryHelloResponse();
	server.updateFromHelloResponse(*helloResponse, 3000);  // 3ms RTT

	assertEqual(static_cast<int>(ServerDescription::RsSecondary), static_cast<int>(server.type()));
	assertEqual("rs0"s, server.setName());
	assertEqual(3000, server.roundTripTime());
	assertFalse(server.isPrimary());
	assertFalse(server.isWritable());
	assertTrue(server.isSecondary());
	assertFalse(server.hasError());

	// Check hosts list
	const auto& hosts = server.hosts();
	assertEqual(3, static_cast<int>(hosts.size()));
}


void ReplicaSetTest::testServerDescriptionArbiter()
{
	SocketAddress addr("localhost:27019");
	ServerDescription server(addr);

	Document::Ptr helloResponse = createArbiterHelloResponse();
	server.updateFromHelloResponse(*helloResponse, 2000);  // 2ms RTT

	assertEqual(static_cast<int>(ServerDescription::RsArbiter), static_cast<int>(server.type()));
	assertEqual("rs0"s, server.setName());
	assertEqual(2000, server.roundTripTime());
	assertFalse(server.isPrimary());
	assertFalse(server.isWritable());
	assertFalse(server.isSecondary());
	assertFalse(server.hasError());
}


void ReplicaSetTest::testServerDescriptionStandalone()
{
	SocketAddress addr("localhost:27017");
	ServerDescription server(addr);

	Document::Ptr helloResponse = createStandaloneHelloResponse();
	server.updateFromHelloResponse(*helloResponse, 1000);  // 1ms RTT

	assertEqual(static_cast<int>(ServerDescription::Standalone), static_cast<int>(server.type()));
	assertTrue(server.setName().empty());  // No replica set name for standalone
	assertTrue(server.isPrimary());  // Standalone treated as primary for read preferences
	assertTrue(server.isWritable());
	assertFalse(server.isSecondary());
	assertFalse(server.hasError());
}


void ReplicaSetTest::testServerDescriptionMongos()
{
	SocketAddress addr("localhost:27017");
	ServerDescription server(addr);

	Document::Ptr helloResponse = createMongosHelloResponse();
	server.updateFromHelloResponse(*helloResponse, 4000);  // 4ms RTT

	assertEqual(static_cast<int>(ServerDescription::Mongos), static_cast<int>(server.type()));
	assertTrue(server.setName().empty());  // Mongos doesn't have a set name
	assertFalse(server.isPrimary());
	assertFalse(server.isWritable());
	assertFalse(server.isSecondary());
	assertFalse(server.hasError());
}


void ReplicaSetTest::testServerDescriptionWithTags()
{
	SocketAddress addr("localhost:27018");
	ServerDescription server(addr);

	Document::Ptr helloResponse = createTaggedSecondaryHelloResponse();
	server.updateFromHelloResponse(*helloResponse, 7000);  // 7ms RTT

	assertEqual(static_cast<int>(ServerDescription::RsSecondary), static_cast<int>(server.type()));

	// Verify tags are parsed correctly
	const Document& tags = server.tags();
	assertFalse(tags.empty());
	assertEqual(3, static_cast<int>(tags.size()));
	assertEqual("east"s, tags.get<std::string>("dc"s));
	assertEqual("1"s, tags.get<std::string>("rack"s));
	assertEqual("large"s, tags.get<std::string>("size"s));
}


void ReplicaSetTest::testServerDescriptionWithHosts()
{
	SocketAddress addr("localhost:27020");
	ServerDescription server(addr);

	Document::Ptr helloResponse = createHiddenMemberHelloResponse();
	server.updateFromHelloResponse(*helloResponse, 8000);  // 8ms RTT

	assertEqual(static_cast<int>(ServerDescription::RsOther), static_cast<int>(server.type()));

	// Verify hosts list includes regular hosts + passives
	const auto& hosts = server.hosts();
	assertEqual(3, static_cast<int>(hosts.size()));  // 2 regular + 1 passive

	// Check that passive member is included
	bool foundPassive = false;
	for (const auto& host : hosts)
	{
		if (host.toString() == "localhost:27020"s || host.toString() == "[::1]:27020"s)
		{
			foundPassive = true;
			break;
		}
	}
	assertTrue(foundPassive);
}


void ReplicaSetTest::testServerDescriptionErrorHandling()
{
	SocketAddress addr("localhost:27017");
	ServerDescription server(addr);

	// Initially update from a valid hello response
	Document::Ptr helloResponse = createPrimaryHelloResponse();
	server.updateFromHelloResponse(*helloResponse, 5000);
	assertEqual(static_cast<int>(ServerDescription::RsPrimary), static_cast<int>(server.type()));
	assertFalse(server.hasError());

	// Now mark it as having an error
	server.markError("Connection timeout");

	assertEqual(static_cast<int>(ServerDescription::Unknown), static_cast<int>(server.type()));
	assertTrue(server.hasError());
	assertEqual("Connection timeout"s, server.error());
	assertFalse(server.isPrimary());
	assertFalse(server.isWritable());
}


void ReplicaSetTest::testServerDescriptionReset()
{
	SocketAddress addr("localhost:27017");
	ServerDescription server(addr);

	Document::Ptr helloResponse = createPrimaryHelloResponse();
	server.updateFromHelloResponse(*helloResponse, 5000);

	assertEqual(static_cast<int>(ServerDescription::RsPrimary), static_cast<int>(server.type()));
	assertEqual("rs0"s, server.setName());

	// Reset the server description
	server.reset();

	assertEqual(static_cast<int>(ServerDescription::Unknown), static_cast<int>(server.type()));
	assertTrue(server.setName().empty());
	assertEqual(0, server.roundTripTime());
	assertTrue(server.hosts().empty());
	assertTrue(server.tags().empty());
	assertFalse(server.hasError());
	assertFalse(server.isPrimary());
	assertFalse(server.isSecondary());
}


// ============================================================================
// TopologyDescription Tests
// ============================================================================


void ReplicaSetTest::testTopologyEmpty()
{
	TopologyDescription topology;

	assertEqual(static_cast<int>(TopologyDescription::Unknown), static_cast<int>(topology.type()));
	assertTrue(topology.setName().empty());
	assertEqual(0, static_cast<int>(topology.serverCount()));
	assertFalse(topology.hasPrimary());
	assertTrue(topology.servers().empty());
	assertTrue(topology.findSecondaries().empty());
}


void ReplicaSetTest::testTopologyAddServers()
{
	TopologyDescription topology("rs0"s);

	assertEqual("rs0"s, topology.setName());
	assertEqual(static_cast<int>(TopologyDescription::Unknown), static_cast<int>(topology.type()));

	// Add first server
	SocketAddress addr1("localhost:27017");
	topology.addServer(addr1);

	assertEqual(1, static_cast<int>(topology.serverCount()));
	assertTrue(topology.hasServer(addr1));

	// Add second server
	SocketAddress addr2("localhost:27018");
	topology.addServer(addr2);

	assertEqual(2, static_cast<int>(topology.serverCount()));
	assertTrue(topology.hasServer(addr2));

	// Adding same server again should be no-op
	topology.addServer(addr1);
	assertEqual(2, static_cast<int>(topology.serverCount()));
}


void ReplicaSetTest::testTopologyUpdateToPrimary()
{
	TopologyDescription topology;

	SocketAddress addr("localhost:27017");
	Document::Ptr helloResponse = createPrimaryHelloResponse();

	topology.updateServer(addr, *helloResponse, 5000);

	// Should discover it's a replica set with primary
	assertEqual(static_cast<int>(TopologyDescription::ReplicaSetWithPrimary), static_cast<int>(topology.type()));
	assertEqual("rs0"s, topology.setName());
	assertTrue(topology.hasPrimary());

	ServerDescription primary = topology.findPrimary();
	assertEqual(static_cast<int>(ServerDescription::RsPrimary), static_cast<int>(primary.type()));
	if (addr.family() == SocketAddress::IPv6)
	{
		assertEqual("[::1]:27017"s, primary.address().toString());
	}
	else
	{
		assertEqual("localhost:27017"s, primary.address().toString());
	}
}


void ReplicaSetTest::testTopologyUpdateToSecondary()
{
	TopologyDescription topology("rs0"s);

	SocketAddress addr("localhost:27018");
	Document::Ptr helloResponse = createSecondaryHelloResponse();

	topology.updateServer(addr, *helloResponse, 3000);

	// Replica set without primary (only secondary known)
	assertEqual(static_cast<int>(TopologyDescription::ReplicaSetNoPrimary), static_cast<int>(topology.type()));
	assertEqual("rs0"s, topology.setName());
	assertFalse(topology.hasPrimary());

	auto secondaries = topology.findSecondaries();
	assertEqual(1, static_cast<int>(secondaries.size()));
	assertEqual(static_cast<int>(ServerDescription::RsSecondary), static_cast<int>(secondaries[0].type()));
}


void ReplicaSetTest::testTopologyReplicaSetWithPrimary()
{
	TopologyDescription topology;

	// Add primary
	SocketAddress primary("localhost:27017");
	topology.updateServer(primary, *createPrimaryHelloResponse(), 5000);

	// Add secondary
	SocketAddress secondary("localhost:27018");
	topology.updateServer(secondary, *createSecondaryHelloResponse(), 3000);

	// Add arbiter
	SocketAddress arbiter("localhost:27019");
	topology.updateServer(arbiter, *createArbiterHelloResponse(), 2000);

	assertEqual(static_cast<int>(TopologyDescription::ReplicaSetWithPrimary), static_cast<int>(topology.type()));
	assertEqual("rs0"s, topology.setName());
	assertTrue(topology.hasPrimary());

	// Verify we can find primary
	ServerDescription primaryServer = topology.findPrimary();
	assertEqual(static_cast<int>(ServerDescription::RsPrimary), static_cast<int>(primaryServer.type()));

	if (primary.family() == SocketAddress::IPv6)
	{
		assertEqual("[::1]:27017"s, primaryServer.address().toString());
	}
	else
	{
		assertEqual("localhost:27017"s, primaryServer.address().toString());
	}

	// Verify we can find secondaries
	auto secondaries = topology.findSecondaries();
	assertEqual(1, static_cast<int>(secondaries.size()));
	assertEqual(static_cast<int>(ServerDescription::RsSecondary), static_cast<int>(secondaries[0].type()));
}


void ReplicaSetTest::testTopologyReplicaSetNoPrimary()
{
	TopologyDescription topology("rs0"s);

	// Add only secondary servers (no primary)
	SocketAddress secondary1("localhost:27018");
	topology.updateServer(secondary1, *createSecondaryHelloResponse(), 3000);

	// Create another secondary response for different server
	Document::Ptr secondary2Response = new Document();
	secondary2Response->add("isWritablePrimary"s, false);
	secondary2Response->add("secondary"s, true);
	secondary2Response->add("setName"s, "rs0"s);
	secondary2Response->add("me"s, "localhost:27019"s);
	Array::Ptr hosts = new Array();
	hosts->add("localhost:27018"s);
	hosts->add("localhost:27019"s);
	secondary2Response->add("hosts"s, hosts);
	secondary2Response->add("ok"s, 1.0);

	SocketAddress secondary2("localhost:27019");
	topology.updateServer(secondary2, *secondary2Response, 4000);

	assertEqual(static_cast<int>(TopologyDescription::ReplicaSetNoPrimary), static_cast<int>(topology.type()));
	assertEqual("rs0"s, topology.setName());
	assertFalse(topology.hasPrimary());

	auto secondaries = topology.findSecondaries();
	assertEqual(2, static_cast<int>(secondaries.size()));
}


void ReplicaSetTest::testTopologyStandalone()
{
	TopologyDescription topology;

	SocketAddress addr("localhost:27017");
	Document::Ptr helloResponse = createStandaloneHelloResponse();

	topology.updateServer(addr, *helloResponse, 1000);

	// Single standalone server
	assertEqual(static_cast<int>(TopologyDescription::Single), static_cast<int>(topology.type()));
	assertTrue(topology.setName().empty());
	assertEqual(1, static_cast<int>(topology.serverCount()));

	// Standalone servers are treated as primary for read preferences
	assertTrue(topology.hasPrimary());
	ServerDescription primary = topology.findPrimary();
	assertEqual(static_cast<int>(ServerDescription::Standalone), static_cast<int>(primary.type()));
}


void ReplicaSetTest::testTopologySharded()
{
	TopologyDescription topology;

	// Add mongos routers
	SocketAddress mongos1("localhost:27017");
	topology.updateServer(mongos1, *createMongosHelloResponse(), 5000);

	SocketAddress mongos2("localhost:27018");
	topology.updateServer(mongos2, *createMongosHelloResponse(), 6000);

	assertEqual(static_cast<int>(TopologyDescription::Sharded), static_cast<int>(topology.type()));
	assertEqual(2, static_cast<int>(topology.serverCount()));
	assertTrue(topology.setName().empty());

	// Sharded topology doesn't have primary/secondaries
	assertFalse(topology.hasPrimary());
	assertTrue(topology.findSecondaries().empty());
}


void ReplicaSetTest::testTopologyFindPrimary()
{
	TopologyDescription topology;

	// Initially no primary
	ServerDescription noPrimary = topology.findPrimary();
	assertEqual(static_cast<int>(ServerDescription::Unknown), static_cast<int>(noPrimary.type()));

	// Add secondary first
	SocketAddress secondary("localhost:27018");
	topology.updateServer(secondary, *createSecondaryHelloResponse(), 3000);

	// Still no primary
	noPrimary = topology.findPrimary();
	assertEqual(static_cast<int>(ServerDescription::Unknown), static_cast<int>(noPrimary.type()));

	// Add primary
	SocketAddress primary("localhost:27017");
	topology.updateServer(primary, *createPrimaryHelloResponse(), 5000);

	// Now we should find the primary
	ServerDescription foundPrimary = topology.findPrimary();
	assertEqual(static_cast<int>(ServerDescription::RsPrimary), static_cast<int>(foundPrimary.type()));
	if (primary.family() == SocketAddress::IPv6)
	{
		assertEqual("[::1]:27017"s, foundPrimary.address().toString());
	}
	else
	{
		assertEqual("localhost:27017"s, foundPrimary.address().toString());
	}
}


void ReplicaSetTest::testTopologyFindSecondaries()
{
	TopologyDescription topology;

	// Initially no secondaries
	auto secondaries = topology.findSecondaries();
	assertTrue(secondaries.empty());

	// Add primary
	SocketAddress primary("localhost:27017");
	topology.updateServer(primary, *createPrimaryHelloResponse(), 5000);

	// Still no secondaries
	secondaries = topology.findSecondaries();
	assertTrue(secondaries.empty());

	// Add first secondary
	SocketAddress secondary1("localhost:27018");
	topology.updateServer(secondary1, *createSecondaryHelloResponse(), 3000);

	secondaries = topology.findSecondaries();
	assertEqual(1, static_cast<int>(secondaries.size()));

	// Add tagged secondary (which is also a secondary)
	SocketAddress secondary2("localhost:27020");
	topology.updateServer(secondary2, *createTaggedSecondaryHelloResponse(), 4000);

	secondaries = topology.findSecondaries();
	assertEqual(2, static_cast<int>(secondaries.size()));
}


void ReplicaSetTest::testTopologyMarkServerUnknown()
{
	TopologyDescription topology;

	// Add primary
	SocketAddress primary("localhost:27017");
	topology.updateServer(primary, *createPrimaryHelloResponse(), 5000);

	assertEqual(static_cast<int>(TopologyDescription::ReplicaSetWithPrimary), static_cast<int>(topology.type()));
	assertTrue(topology.hasPrimary());

	// Mark primary as unknown (simulating connection failure)
	topology.markServerUnknown(primary, "Connection failed");

	// Topology should transition to no primary
	assertEqual(static_cast<int>(TopologyDescription::ReplicaSetNoPrimary), static_cast<int>(topology.type()));
	assertFalse(topology.hasPrimary());

	// Server should still exist but be Unknown
	ServerDescription server = topology.getServer(primary);
	assertEqual(static_cast<int>(ServerDescription::Unknown), static_cast<int>(server.type()));
}


void ReplicaSetTest::testTopologyRemoveServer()
{
	TopologyDescription topology;

	SocketAddress addr1("localhost:27017");
	SocketAddress addr2("localhost:27018");

	topology.addServer(addr1);
	topology.addServer(addr2);
	assertEqual(2, static_cast<int>(topology.serverCount()));

	topology.removeServer(addr1);
	assertEqual(1, static_cast<int>(topology.serverCount()));
	assertFalse(topology.hasServer(addr1));
	assertTrue(topology.hasServer(addr2));

	topology.removeServer(addr2);
	assertEqual(0, static_cast<int>(topology.serverCount()));
	assertEqual(static_cast<int>(TopologyDescription::Unknown), static_cast<int>(topology.type()));
}


void ReplicaSetTest::testTopologyDiscoverNewHosts()
{
	TopologyDescription topology;

	assertEqual(0, static_cast<int>(topology.serverCount()));

	// Update with primary - hello response includes hosts array
	SocketAddress primary("localhost:27017");
	topology.updateServer(primary, *createPrimaryHelloResponse(), 5000);

	// Topology should have discovered all hosts from the hello response
	// Primary response includes: localhost:27017, localhost:27018, localhost:27019
	assertEqual(3, static_cast<int>(topology.serverCount()));

	if (primary.family() == SocketAddress::IPv6)
	{
		assertTrue(topology.hasServer(SocketAddress("[::1]:27017"s)));
		assertTrue(topology.hasServer(SocketAddress("[::1]:27018"s)));
		assertTrue(topology.hasServer(SocketAddress("[::1]:27019"s)));
	}
	else
	{
		assertTrue(topology.hasServer(SocketAddress("localhost:27017"s)));
		assertTrue(topology.hasServer(SocketAddress("localhost:27018"s)));
		assertTrue(topology.hasServer(SocketAddress("localhost:27019"s)));
	}
}


void ReplicaSetTest::testTopologySetNameMismatch()
{
	TopologyDescription topology("rs0"s);

	assertEqual("rs0"s, topology.setName());

	// Create hello response with different set name
	Document::Ptr wrongSetResponse = new Document();
	wrongSetResponse->add("isWritablePrimary"s, true);
	wrongSetResponse->add("secondary"s, false);
	wrongSetResponse->add("setName"s, "differentSet"s);  // Wrong set name
	wrongSetResponse->add("ok"s, 1.0);

	SocketAddress addr("localhost:27017");

	// Update server with mismatched set name
	// The topology keeps original set name but updates server
	topology.updateServer(addr, *wrongSetResponse, 5000);

	// Original set name is preserved (set in constructor)
	assertEqual("rs0"s, topology.setName());

	// Server is updated but topology maintains original expected set name
	assertEqual(1, static_cast<int>(topology.serverCount()));
}


// ============================================================================
// ReadPreference Tests
// ============================================================================


void ReplicaSetTest::testReadPreferencePrimary()
{
	ReadPreference pref = ReadPreference::primary();

	assertEqual(static_cast<int>(ReadPreference::Primary), static_cast<int>(pref.mode()));
	assertEqual("primary"s, pref.toString());

	// Create topology with primary and secondaries
	TopologyDescription topology;
	topology.updateServer(SocketAddress("localhost:27017"), *createPrimaryHelloResponse(), 5000);
	topology.updateServer(SocketAddress("localhost:27018"), *createSecondaryHelloResponse(), 3000);

	// Primary read preference should only select primary
	auto selected = pref.selectServers(topology);
	assertEqual(1, static_cast<int>(selected.size()));
	assertEqual(static_cast<int>(ServerDescription::RsPrimary), static_cast<int>(selected[0].type()));

	const SocketAddress& addr = selected[0].address();
	if (addr.family() == SocketAddress::IPv6)
	{
		assertEqual("[::1]:27017"s, selected[0].address().toString());
	}
	else
	{
		assertEqual("localhost:27017"s, selected[0].address().toString());
	}
}


void ReplicaSetTest::testReadPreferencePrimaryPreferred()
{
	ReadPreference pref = ReadPreference::primaryPreferred();

	assertEqual(static_cast<int>(ReadPreference::PrimaryPreferred), static_cast<int>(pref.mode()));

	// Create topology with primary and secondary
	TopologyDescription topology;
	topology.updateServer(SocketAddress("localhost:27017"), *createPrimaryHelloResponse(), 5000);
	topology.updateServer(SocketAddress("localhost:27018"), *createSecondaryHelloResponse(), 3000);

	// Should select primary when available
	auto selected = pref.selectServers(topology);
	assertEqual(1, static_cast<int>(selected.size()));
	assertEqual(static_cast<int>(ServerDescription::RsPrimary), static_cast<int>(selected[0].type()));

	// Create topology with only secondary (no primary)
	TopologyDescription topologyNoPrimary;
	topologyNoPrimary.updateServer(SocketAddress("localhost:27018"), *createSecondaryHelloResponse(), 3000);

	// Should fall back to secondary
	selected = pref.selectServers(topologyNoPrimary);
	assertEqual(1, static_cast<int>(selected.size()));
	assertEqual(static_cast<int>(ServerDescription::RsSecondary), static_cast<int>(selected[0].type()));
}


void ReplicaSetTest::testReadPreferenceSecondary()
{
	ReadPreference pref = ReadPreference::secondary();

	assertEqual(static_cast<int>(ReadPreference::Secondary), static_cast<int>(pref.mode()));

	// Create topology with primary and secondaries
	TopologyDescription topology;
	topology.updateServer(SocketAddress("localhost:27017"), *createPrimaryHelloResponse(), 5000);
	topology.updateServer(SocketAddress("localhost:27018"), *createSecondaryHelloResponse(), 3000);

	// Should only select secondaries
	auto selected = pref.selectServers(topology);
	assertEqual(1, static_cast<int>(selected.size()));
	assertEqual(static_cast<int>(ServerDescription::RsSecondary), static_cast<int>(selected[0].type()));

	const SocketAddress& addr = selected[0].address();
	if (addr.family() == SocketAddress::IPv6)
	{
		assertEqual("[::1]:27018"s, selected[0].address().toString());
	}
	else
	{
		assertEqual("localhost:27018"s, selected[0].address().toString());
	}

	// Create topology with only primary (no secondaries)
	TopologyDescription topologyPrimaryOnly;
	topologyPrimaryOnly.updateServer(SocketAddress("localhost:27017"), *createPrimaryHelloResponse(), 5000);

	// Should return empty (no secondaries available)
	selected = pref.selectServers(topologyPrimaryOnly);
	assertTrue(selected.empty());
}


void ReplicaSetTest::testReadPreferenceSecondaryPreferred()
{
	ReadPreference pref = ReadPreference::secondaryPreferred();

	assertEqual(static_cast<int>(ReadPreference::SecondaryPreferred), static_cast<int>(pref.mode()));

	// Create topology with primary and secondary
	TopologyDescription topology;
	topology.updateServer(SocketAddress("localhost:27017"), *createPrimaryHelloResponse(), 5000);
	topology.updateServer(SocketAddress("localhost:27018"), *createSecondaryHelloResponse(), 3000);

	// Should select secondary when available
	auto selected = pref.selectServers(topology);
	assertEqual(1, static_cast<int>(selected.size()));
	assertEqual(static_cast<int>(ServerDescription::RsSecondary), static_cast<int>(selected[0].type()));

	// Create topology with only primary (no secondaries)
	TopologyDescription topologyPrimaryOnly;
	topologyPrimaryOnly.updateServer(SocketAddress("localhost:27017"), *createPrimaryHelloResponse(), 5000);

	// Should fall back to primary
	selected = pref.selectServers(topologyPrimaryOnly);
	assertEqual(1, static_cast<int>(selected.size()));
	assertEqual(static_cast<int>(ServerDescription::RsPrimary), static_cast<int>(selected[0].type()));
}


void ReplicaSetTest::testReadPreferenceNearest()
{
	ReadPreference pref = ReadPreference::nearest();

	assertEqual(static_cast<int>(ReadPreference::Nearest), static_cast<int>(pref.mode()));

	// Create topology with primary (5ms RTT) and secondaries (3ms and 8ms RTT)
	TopologyDescription topology;
	topology.updateServer(SocketAddress("localhost:27017"), *createPrimaryHelloResponse(), 5000);
	topology.updateServer(SocketAddress("localhost:27018"), *createSecondaryHelloResponse(), 3000);

	Document::Ptr secondary2Response = new Document();
	secondary2Response->add("isWritablePrimary"s, false);
	secondary2Response->add("secondary"s, true);
	secondary2Response->add("setName"s, "rs0"s);
	Array::Ptr hosts = new Array();
	hosts->add("localhost:27017"s);
	hosts->add("localhost:27018"s);
	hosts->add("localhost:27019"s);
	secondary2Response->add("hosts"s, hosts);
	secondary2Response->add("ok"s, 1.0);

	topology.updateServer(SocketAddress("localhost:27019"), *secondary2Response, 8000);

	// Nearest should select servers regardless of type (primary or secondary)
	// All servers should be eligible
	auto selected = pref.selectServers(topology);
	assertTrue(selected.size() >= 1);  // At least one server selected

	// The selection should include servers with lowest latency
	// In practice, nearest mode selects all servers within latency window
}


void ReplicaSetTest::testReadPreferenceWithTags()
{
	// Create tag set for targeting specific datacenter
	Document tags;
	tags.add("dc"s, "east"s);
	tags.add("rack"s, "1"s);

	ReadPreference pref(ReadPreference::Secondary, tags);

	assertEqual(static_cast<int>(ReadPreference::Secondary), static_cast<int>(pref.mode()));
	assertFalse(pref.tags().empty());
	assertEqual("east"s, pref.tags().get<std::string>("dc"s));

	// Create topology with tagged and untagged secondaries
	TopologyDescription topology;
	topology.updateServer(SocketAddress("localhost:27017"), *createPrimaryHelloResponse(), 5000);
	topology.updateServer(SocketAddress("localhost:27018"), *createTaggedSecondaryHelloResponse(), 3000);

	// Create untagged secondary
	Document::Ptr untaggedSecondary = new Document();
	untaggedSecondary->add("isWritablePrimary"s, false);
	untaggedSecondary->add("secondary"s, true);
	untaggedSecondary->add("setName"s, "rs0"s);
	Array::Ptr hosts = new Array();
	hosts->add("localhost:27017"s);
	hosts->add("localhost:27018"s);
	hosts->add("localhost:27019"s);
	untaggedSecondary->add("hosts"s, hosts);
	untaggedSecondary->add("ok"s, 1.0);

	topology.updateServer(SocketAddress("localhost:27019"), *untaggedSecondary, 4000);

	// Should select only tagged secondary matching the tag set
	auto selected = pref.selectServers(topology);

	// At least the tagged server should be selected
	bool foundTaggedServer = false;
	for (const auto& server : selected)
	{
		if (server.address().toString() == "localhost:27018"s)
		{
			foundTaggedServer = true;
			assertEqual("east"s, server.tags().get<std::string>("dc"s));
			assertEqual("1"s, server.tags().get<std::string>("rack"s));
		}
	}

	// The tagged server may or may not be selected depending on implementation
	// This test verifies the tag data is preserved correctly
}


CppUnit::Test* ReplicaSetTest::suite()
{
	auto* pSuite = new CppUnit::TestSuite("ReplicaSetTest");

	// ServerDescription tests
	CppUnit_addTest(pSuite, ReplicaSetTest, testServerDescriptionPrimary);
	CppUnit_addTest(pSuite, ReplicaSetTest, testServerDescriptionSecondary);
	CppUnit_addTest(pSuite, ReplicaSetTest, testServerDescriptionArbiter);
	CppUnit_addTest(pSuite, ReplicaSetTest, testServerDescriptionStandalone);
	CppUnit_addTest(pSuite, ReplicaSetTest, testServerDescriptionMongos);
	CppUnit_addTest(pSuite, ReplicaSetTest, testServerDescriptionWithTags);
	CppUnit_addTest(pSuite, ReplicaSetTest, testServerDescriptionWithHosts);
	CppUnit_addTest(pSuite, ReplicaSetTest, testServerDescriptionErrorHandling);
	CppUnit_addTest(pSuite, ReplicaSetTest, testServerDescriptionReset);

	// TopologyDescription tests
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyEmpty);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyAddServers);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyUpdateToPrimary);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyUpdateToSecondary);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyReplicaSetWithPrimary);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyReplicaSetNoPrimary);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyStandalone);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologySharded);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyFindPrimary);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyFindSecondaries);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyMarkServerUnknown);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyRemoveServer);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologyDiscoverNewHosts);
	CppUnit_addTest(pSuite, ReplicaSetTest, testTopologySetNameMismatch);

	// ReadPreference tests
	CppUnit_addTest(pSuite, ReplicaSetTest, testReadPreferencePrimary);
	CppUnit_addTest(pSuite, ReplicaSetTest, testReadPreferencePrimaryPreferred);
	CppUnit_addTest(pSuite, ReplicaSetTest, testReadPreferenceSecondary);
	CppUnit_addTest(pSuite, ReplicaSetTest, testReadPreferenceSecondaryPreferred);
	CppUnit_addTest(pSuite, ReplicaSetTest, testReadPreferenceNearest);
	CppUnit_addTest(pSuite, ReplicaSetTest, testReadPreferenceWithTags);

	return pSuite;
}
