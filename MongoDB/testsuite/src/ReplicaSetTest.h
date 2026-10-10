//
// ReplicaSetTest.h
//
// Definition of the ReplicaSetTest class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ReplicaSetTest_INCLUDED
#define ReplicaSetTest_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "CppUnit/TestCase.h"


class ReplicaSetTest: public CppUnit::TestCase
{
public:
	ReplicaSetTest(const std::string& name);
	~ReplicaSetTest() override;

	void testServerDescriptionPrimary();
	void testServerDescriptionSecondary();
	void testServerDescriptionArbiter();
	void testServerDescriptionStandalone();
	void testServerDescriptionMongos();
	void testServerDescriptionWithTags();
	void testServerDescriptionWithHosts();
	void testServerDescriptionErrorHandling();
	void testServerDescriptionReset();

	void testTopologyEmpty();
	void testTopologyAddServers();
	void testTopologyUpdateToPrimary();
	void testTopologyUpdateToSecondary();
	void testTopologyReplicaSetWithPrimary();
	void testTopologyReplicaSetNoPrimary();
	void testTopologyStandalone();
	void testTopologySharded();
	void testTopologyFindPrimary();
	void testTopologyFindSecondaries();
	void testTopologyMarkServerUnknown();
	void testTopologyRemoveServer();
	void testTopologyDiscoverNewHosts();
	void testTopologySetNameMismatch();
	void testTopologyMixedUnknownAndKnown();
	void testTopologyAllUnknown();
	void testTopologyMultipleStandalone();
	void testTopologyMixedMongosAndPrimary();
	void testTopologyMixedStandaloneAndPrimary();
	void testTopologyMultipleStandaloneWithSetName();
	void testTopologyMixedMongosAndSecondary();
	void testTopologyMixedStandaloneAndSecondary();
	void testTopologyTransitions();
	void testTopologyReplicaSetNoPrimaryWithSetName();

	void testReadPreferencePrimary();
	void testReadPreferencePrimaryPreferred();
	void testReadPreferenceSecondary();
	void testReadPreferenceSecondaryPreferred();
	void testReadPreferenceNearest();
	void testReadPreferenceWithTags();

	void testReplicaSetURIParsing();
	void testReplicaSetURIClass();
	void testReplicaSetURIToString();
	void testReplicaSetURIModification();
	void testReplicaSetWithURIObject();

	void testConnectionTimeoutWithUnreachableServer();
	void testReplicaSetConstructionTimeout();
	void testReplicaSetMonitoringStopWithUnreachableServers();
	void testConnectionPoolTimeoutWithUnreachableServers();
	void testPlainConnectionPoolTimeoutWithUnreachableServer();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();
};


#endif // ReplicaSetTest_INCLUDED
