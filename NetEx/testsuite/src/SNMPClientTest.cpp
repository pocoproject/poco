//
// SNMPClientTest.cpp
//
// $Id: //poco/1.4/Net/testsuite/src/SNMPClientTest.cpp#1 $
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SNMPClientTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/SNMPClient.h"
#include "Poco/Net/SNMPEventArgs.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/AutoPtr.h"
#include "Poco/Delegate.h"
#include <sstream>
#include <iostream>


using Poco::Net::SNMPClient;
using Poco::Net::SNMPEventArgs;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::Net::HostNotFoundException;
using Poco::Delegate;
using Poco::AutoPtr;


SNMPClientTest::SNMPClientTest(const std::string& name): 
	CppUnit::TestCase(name),
	_snmpClient()
{
}


SNMPClientTest::~SNMPClientTest()
{
}


void SNMPClientTest::testQuery()
{
	_snmpClient.get("localhost", "1.3.6.1.2.1.1.1.0");

	// warning: may fail depending on the existence of the addresses at test site
	// if so, adjust accordingly (i.e. specify non-existent or unreachable IP addresses)
	_snmpClient.get("192.168.243.1", "1.3.6.1.2.1.1.1.0");
	_snmpClient.get("10.11.12.13", "1.3.6.1.2.1.1.1.0");
}

void SNMPClientTest::setUp()
{
	_snmpClient.snmpBegin += Delegate<SNMPClientTest, SNMPEventArgs>(this, &SNMPClientTest::onBegin);
	_snmpClient.snmpReply += Delegate<SNMPClientTest, SNMPEventArgs>(this, &SNMPClientTest::onReply);
	_snmpClient.snmpError += Delegate<SNMPClientTest, SNMPEventArgs>(this, &SNMPClientTest::onError);
	_snmpClient.snmpEnd   += Delegate<SNMPClientTest, SNMPEventArgs>(this, &SNMPClientTest::onEnd);
}


void SNMPClientTest::tearDown()
{
	_snmpClient.snmpBegin -= Delegate<SNMPClientTest, SNMPEventArgs>(this, &SNMPClientTest::onBegin);
	_snmpClient.snmpReply -= Delegate<SNMPClientTest, SNMPEventArgs>(this, &SNMPClientTest::onReply);
	_snmpClient.snmpError -= Delegate<SNMPClientTest, SNMPEventArgs>(this, &SNMPClientTest::onError);
	_snmpClient.snmpEnd   -= Delegate<SNMPClientTest, SNMPEventArgs>(this, &SNMPClientTest::onEnd);
}


void SNMPClientTest::onBegin(const void* pSender, SNMPEventArgs& args)
{
	std::ostringstream os;
	os << std::endl << "Querying " << args.hostName() << " [" << args.hostAddress() << "] " 
		<< std::endl << "-------------------------------------------------------" << std::endl;
	std::cout << os.str() << std::endl;
}


void SNMPClientTest::onReply(const void* pSender, SNMPEventArgs& args)
{
	std::ostringstream os;
	os << "Reply from " << args.hostAddress();
	std::cout << os.str() << std::endl;
}


void SNMPClientTest::onError(const void* pSender, SNMPEventArgs& args)
{
	std::ostringstream os;
	os << args.error();
	std::cerr << os.str() << std::endl;
}


void SNMPClientTest::onEnd(const void* pSender, SNMPEventArgs& args)
{
	std::ostringstream os;
	os << std::endl << "--- SNMP finished for " << args.hostAddress() << " ---"
		<< std::endl << "-----------------------------------------------" << std::endl;
	std::cout << os.str() << std::endl;
}


CppUnit::Test* SNMPClientTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SNMPClientTest");

	CppUnit_addTest(pSuite, SNMPClientTest, testQuery);

	return pSuite;
}
