//
// X509CertificateTest.h
//
// Definition of the X509CertificateTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef X509CertificateTest_INCLUDED
#define X509CertificateTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class X509CertificateTest: public CppUnit::TestCase
{
public:
	X509CertificateTest(const std::string& name);
	~X509CertificateTest();

	void testVerifyWildcard();
	void testVerifyPartialWildcard();
	void testVerifyIPAddress();
	void testVerifyIPAddressAgainstDNSName();
	void testVerifyIPAddressAgainstCommonName();
	void testVerifyCommonName();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};


#endif // X509CertificateTest_INCLUDED
