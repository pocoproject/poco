//
// ModulesTest.h
//
// Definition of the ModulesTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ModulesTest_INCLUDED
#define ModulesTest_INCLUDED

#include <string>

#include "CppUnit/TestCase.h"

class ModulesTest: public CppUnit::TestCase
{
public:
	ModulesTest(const std::string& name);
	~ModulesTest();

	void testFoundation();
	void testEncodings();
	void testXML();
	void testJSON();
	void testUtil();
	void testNet();
	void testNetSSL();
	void testCrypto();
	void testJWT();
	void testData();
	void testSQLite();
	void testODBC();
	void testMongoDB();
	void testRedis();
	void testPrometheus();
	void testZip();
	void testPDF();
	void testDNSSD();
	void testCppParser();
	void testSevenZip();
	void testActiveRecord();
	void testSSH();
	void testCodeGeneration();
	void testRemotingNG();
	void testXSD();
	void testGeo();
	void testSerial();

	static CppUnit::Test* suite();
};


#endif // ModulesTest_INCLUDED
