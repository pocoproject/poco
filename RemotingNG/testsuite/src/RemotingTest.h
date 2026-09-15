//
// RemotingTest.h
//
// Definition of the RemotingTest class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingTest_INCLUDED
#define RemotingTest_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "CppUnit/TestCase.h"
#include "ITester.h"
#include "Poco/RemotingNG/Listener.h"


class RemotingTest: public CppUnit::TestCase
{
public:
	RemotingTest(const std::string& name);
	~RemotingTest();

	void testListeners();
	void testRegistration();
	void testFindObject();
	void testInt();
	void testEnum1();
	void testEnum2();
	void testNested();
	void testStruct1();
	void testStruct2();
	void testStruct3();
	void testStruct4();
	void testStruct5();
	void testStruct6();
	void testArray();
	void testStruct7();
	void testStdArray();
	void testOptional();
	void testStruct8();
	void testClass1();
	void testPtr();
	void testStruct1Vec();
	void testOneWay();
	void testEvent();
	void testOneWayEvent();
	void testVoidEvent();
	void testFilteredEvent();
	void testPermissions();

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
	void testStruct6(ITester::Ptr pTester);
	void testArray(ITester::Ptr pTester);
	void testStruct7(ITester::Ptr pTester);
	void testStdArray(ITester::Ptr pTester);
	void testOptional(ITester::Ptr pTester);
	void testStruct8(ITester::Ptr pTester);
	void testClass1(ITester::Ptr pTester);
	void testPtr(ITester::Ptr pTester);
	void testStruct1Vec(ITester::Ptr pTester);
	void testPermissions(ITester::Ptr pTester);
	void onEvent(const void* pSender, std::string& arg);
	void onVoidEvent(const void* pSender);
	void onFilteredEvent(const int& arg);

private:
	std::string _listener;
	std::string _objectURI;
	Poco::RemotingNG::Listener::Ptr _pListener;
	std::string _eventArg;
	int _filteredArg;
};


#endif // RemotingTest_INCLUDED
