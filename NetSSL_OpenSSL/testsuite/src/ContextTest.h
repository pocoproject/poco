//
// ContextTest.h
//
// Definition of the ContextTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ContextTest_INCLUDED
#define ContextTest_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/Context.h"
#include "CppUnit/TestCase.h"


class ContextTest: public CppUnit::TestCase
{
public:
	ContextTest(const std::string& name);
	~ContextTest();

	void testValidCipherConfiguration();
	void testInvalidCipherList();
	void testCipherListWithoutMatch();
	void testInvalidCipherSuites();
	void testInvalidECDHGroups();
	void testDHParametersRejectedBySecurityLevel();
	void testAddChainCertificateWithoutX509();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void assertRejected(const Poco::Net::Context::Params& params);
};


#endif // ContextTest_INCLUDED
