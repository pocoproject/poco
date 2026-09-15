//
// RemotingTest.h
//
// Definition of the RemotingTest class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingTest_INCLUDED
#define RemotingTest_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "CppUnit/TestCase.h"
#include "ITester.h"
#include "Poco/RemotingNG/TCP/Listener.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/SharedPtr.h"
#include "Poco/Event.h"


class RemotingTest: public CppUnit::TestCase
{
public:
	RemotingTest(const std::string& name);
	~RemotingTest();
	
	void testRegistration();
	void testInt();
	void testEnum1();
	void testEnum2();
	void testNested();
	void testStruct1();
	void testStruct2();
	void testStruct3();
	void testStruct4();
	void testStruct5();
	void testClass1();
	void testPtr();
	void testStruct1Vec();
	void testOneWay();
	void testFault();
	void testAuthenticatedGoodCredentials();
	void testAuthenticatedNoCredentials();
	void testAuthenticatedBadCredentials();
	void testAuthenticatedUpdatedCredentials();
	void testAuthenticatedGoodSCRAMCredentials();
	void testAuthenticatedNoSCRAMCredentials();
	void testAuthenticatedBadSCRAMCredentials();
	void testAuthenticatedUpdatedSCRAMCredentials();
	void testAuthenticatedMultipleSCRAMCredentials();
	void testPermission();
	void testNoPermission();
	void testTimeout();
	void testEvent();
	void testOneWayEvent();
	void testVoidEvent();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

protected:
	void testInt(ITester::Ptr pTester);
	void testEnum1(ITester::Ptr pTester);
	void testEnum2(ITester::Ptr pTester);
	void testNested(ITester::Ptr pTester);
	void testStruct1(ITester::Ptr pTester);
	void testStruct2(ITester::Ptr pTester);
	void testStruct3(ITester::Ptr pTester);
	void testStruct4(ITester::Ptr pTester);
	void testStruct5(ITester::Ptr pTester);
	void testClass1(ITester::Ptr pTester);
	void testPtr(ITester::Ptr pTester);
	void testStruct1Vec(ITester::Ptr pTester);

	virtual ITester::Ptr createProxy(const std::string& uri);

	void onEvent(const void* pSender, std::string& arg);
	
	void onVoidEvent(const void* pSender);

protected:
	std::string _listener;
	std::string _objectURI;
	Poco::RemotingNG::TCP::Listener::Ptr _pListener;
	std::string _eventArg;
	Poco::Event _eventReceived;
};


class RemotingTestCompressed: public RemotingTest
{
public:
	RemotingTestCompressed(const std::string& name);
	~RemotingTestCompressed();

	ITester::Ptr createProxy(const std::string& uri);

	static CppUnit::Test* suite();
};


#endif // RemotingTest_INCLUDED
