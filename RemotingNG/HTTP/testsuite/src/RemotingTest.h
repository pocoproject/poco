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
#include "Poco/RemotingNG/HTTP/Listener.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/SharedPtr.h"


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
	void testPermission();
	void testNoPermission();

	void setUp();
	void tearDown();

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

protected:
	std::string _listener;
	std::string _objectURI;
	Poco::RemotingNG::HTTP::Listener::Ptr _pListener;
};


class RemotingTestHTTP: public RemotingTest
{
public:
	RemotingTestHTTP(const std::string& name);
	~RemotingTestHTTP();

	void setUp();

	static CppUnit::Test* suite();
};


class RemotingTestHTTPCompressed: public RemotingTest
{
public:
	RemotingTestHTTPCompressed(const std::string& name);
	~RemotingTestHTTPCompressed();

	void setUp();
	
	static CppUnit::Test* suite();

protected:
	virtual ITester::Ptr createProxy(const std::string& uri);
};


class RemotingTestHTTPNonChunked: public RemotingTest
{
public:
	RemotingTestHTTPNonChunked(const std::string& name);
	~RemotingTestHTTPNonChunked();

	void setUp();

	static CppUnit::Test* suite();

protected:
	virtual ITester::Ptr createProxy(const std::string& uri);
};


class RemotingTestHTTPAuth: public RemotingTest
{
public:
	RemotingTestHTTPAuth(const std::string& name);
	~RemotingTestHTTPAuth();

	void setUp();
	void tearDown();

	void testUnauthorized();
	
	static CppUnit::Test* suite();

protected:
	virtual ITester::Ptr createProxy(const std::string& uri);
	ITester::Ptr createUnauthorizedProxy(const std::string& uri);
	
private:
	Poco::SharedPtr<Poco::Net::HTTPServer> _pHTTPServer;
};


class RemotingTestHTTPAuthChunked: public RemotingTestHTTPAuth
{
public:
	RemotingTestHTTPAuthChunked(const std::string& name);
	~RemotingTestHTTPAuthChunked();
	
	static CppUnit::Test* suite();

protected:
	virtual ITester::Ptr createProxy(const std::string& uri);
};


#endif // RemotingTest_INCLUDED
