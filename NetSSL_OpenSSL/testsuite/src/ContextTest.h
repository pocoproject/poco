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

#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	void testBuiltInDHParameters();
#endif
	void testClientContextIgnoresDHParameters();
	void testVerifyErrorWithoutCertificate();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	std::string negotiatedDHGroup(Poco::Net::Context::KeyDHGroup dhGroup);
		/// Returns the OpenSSL name of the DH group that a server Context with
		/// the given built-in parameters sends in a TLS 1.2 DHE handshake.
#endif
};


#endif // ContextTest_INCLUDED
