//
// X509CertificateTestSuite.cpp
//
// $Id: //poco/1.4/NetSSL_OpenSSL/testsuite/src/X509CertificateTestSuite.cpp#1 $
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "X509CertificateTestSuite.h"
#include "X509CertificateTest.h"


CppUnit::Test* X509CertificateTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("X509CertificateTestSuite");

	pSuite->addTest(X509CertificateTest::suite());

	return pSuite;
}
