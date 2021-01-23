//
// NTLMCredentialsTest.h
//
// Definition of the NTLMCredentialsTest class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NTLMCredentialsTest_INCLUDED
#define NTLMCredentialsTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class NTLMCredentialsTest: public CppUnit::TestCase
{
public:
	NTLMCredentialsTest(const std::string& name);
	~NTLMCredentialsTest();

	void testNonce();
	void testPasswordHash();
	void testNTLMv2Hash();
	void testLMv2Response();
	void testNTLMv2Response();
	void testFormatNegotiateMessage();
	void testParseChallengeMessage();
	void testParseChallengeMessageWithTargetInfo();
	void testFormatAuthenticateMessage();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // NTLMCredentialsTest_INCLUDED
