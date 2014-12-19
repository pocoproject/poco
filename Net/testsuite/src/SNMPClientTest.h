//
// SNMPClientTest.h
//
// $Id: //poco/1.4/Net/testsuite/src/SNMPClientTest.h#1 $
//
// Definition of the SNMPClientTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SNMPClientTest_INCLUDED
#define SNMPClientTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"
#include "Poco/Net/SNMPClient.h"
#include "Poco/Net/SNMPEventArgs.h"


class SNMPClientTest: public CppUnit::TestCase
{
public:
	SNMPClientTest(const std::string& name);
	~SNMPClientTest();

	void testQuery();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

	void onBegin(const void* pSender, Poco::Net::SNMPEventArgs& args);
	void onReply(const void* pSender, Poco::Net::SNMPEventArgs& args);
	void onError(const void* pSender, Poco::Net::SNMPEventArgs& args);
	void onEnd(const void* pSender, Poco::Net::SNMPEventArgs& args);

private:
	Poco::Net::SNMPClient _snmpClient;
};


#endif // SNMPClientTest_INCLUDED
