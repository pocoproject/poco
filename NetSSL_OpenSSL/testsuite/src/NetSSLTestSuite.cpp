//
// OpenSSLTestSuite.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "NetSSLTestSuite.h"

#include "HTTPSClientTestSuite.h"
#include "TCPServerTestSuite.h"
#include "HTTPSServerTestSuite.h"
#include "WebSocketTestSuite.h"
#include "FTPSClientTestSuite.h"
#include "X509CertificateTestSuite.h"


CppUnit::Test* NetSSLTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("OpenSSLTestSuite");

	
	pSuite->addTest(HTTPSClientTestSuite::suite());
	pSuite->addTest(TCPServerTestSuite::suite());
	pSuite->addTest(HTTPSServerTestSuite::suite());
	pSuite->addTest(WebSocketTestSuite::suite());
	pSuite->addTest(FTPSClientTestSuite::suite());
	pSuite->addTest(X509CertificateTestSuite::suite());

	return pSuite;
}
