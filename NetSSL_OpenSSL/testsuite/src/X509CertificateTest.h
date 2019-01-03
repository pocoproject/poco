//
// X509CertificateTest.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/testsuite/src/X509CertificateTest.h#1 $
//
// Definition of the X509CertificateTest class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef X509CertificateTest_INCLUDED
#define X509CertificateTest_INCLUDED


#include "Poco/CppUnit/TestCase.h"


class X509CertificateTest: public CppUnit::TestCase
{
public:
	X509CertificateTest(const std::string& name);
	~X509CertificateTest();

	void testVerify();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // X509CertificateTest_INCLUDED
