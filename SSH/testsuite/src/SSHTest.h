//
// SSHTest.h
//
// Definition of the SSHTest class.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSHTest_INCLUDED
#define SSHTest_INCLUDED


#include "Poco/SSH/SSH.h"
#include "CppUnit/TestCase.h"


class SSHTest: public CppUnit::TestCase
{
public:
	SSHTest(const std::string& name);
	~SSHTest();

	void testHostKeyGeneration();
	void testHostKeyReuse();
	void testServerStartStop();
	void testClientConnect();
	void testPasswordAuth();
	void testPasswordAuthFail();
	void testSessionHandling();
	void testServerShutdownWithActiveSession();
	void testClientDisconnect();
	void testMaxConnections();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	std::string _testDir;
};


#endif // SSHTest_INCLUDED
