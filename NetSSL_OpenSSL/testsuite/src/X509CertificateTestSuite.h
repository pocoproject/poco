//
// X509CertificateTestSuite.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/testsuite/src/X509CertificateTestSuite.h#1 $
//
// Definition of the X509CertificateTestSuite class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef X509CertificateTestSuite_INCLUDED
#define X509CertificateTestSuite_INCLUDED


#include "Poco/CppUnit/TestSuite.h"


class X509CertificateTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // X509CertificateTestSuite_INCLUDED
