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
#include "Poco/RemotingNG/SOAP/Listener.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/SharedPtr.h"


class RemotingTest: public CppUnit::TestCase
{
public:
	RemotingTest(const std::string& name);
	~RemotingTest() override;
	
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
	void testCookie();
	void testAuthenticatedGoodCredentials();
	void testAuthenticatedNoCredentials();
	void testAuthenticatedBadCredentials();
	void testPermission();
	void testNoPermission();
	void testWSSEInterceptor();
	void testWSSEAuthenticatorPlain();
	void testWSSEAuthenticatorDigest();

	void setUp() override;
	void tearDown() override;

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
	Poco::RemotingNG::SOAP::Listener::Ptr _pListener;
};


class RemotingTestSOAP11: public RemotingTest
{
public:
	RemotingTestSOAP11(const std::string& name);
	~RemotingTestSOAP11() override;

	void setUp() override;

	static CppUnit::Test* suite();
};


class RemotingTestSOAP12: public RemotingTest
{
public:
	RemotingTestSOAP12(const std::string& name);
	~RemotingTestSOAP12() override;

	void setUp() override;
	
	static CppUnit::Test* suite();

protected:
	ITester::Ptr createProxy(const std::string& uri) override;
};


class RemotingTestSOAP12Compressed: public RemotingTest
{
public:
	RemotingTestSOAP12Compressed(const std::string& name);
	~RemotingTestSOAP12Compressed() override;

	void setUp() override;
	
	static CppUnit::Test* suite();

protected:
	ITester::Ptr createProxy(const std::string& uri) override;
};


class RemotingTestSOAP11NonChunked: public RemotingTest
{
public:
	RemotingTestSOAP11NonChunked(const std::string& name);
	~RemotingTestSOAP11NonChunked() override;

	void setUp() override;

	static CppUnit::Test* suite();

protected:
	ITester::Ptr createProxy(const std::string& uri) override;
};


class RemotingTestSOAP12MTOM: public RemotingTest
{
public:
	RemotingTestSOAP12MTOM(const std::string& name);
	~RemotingTestSOAP12MTOM() override;

	void setUp() override;
	
	static CppUnit::Test* suite();

protected:
	ITester::Ptr createProxy(const std::string& uri) override;
};


class RemotingTestSOAP12Auth: public RemotingTest
{
public:
	RemotingTestSOAP12Auth(const std::string& name);
	~RemotingTestSOAP12Auth() override;

	void setUp() override;
	void tearDown() override;

	void testUnauthorized();
	
	static CppUnit::Test* suite();

protected:
	ITester::Ptr createProxy(const std::string& uri) override;
	ITester::Ptr createUnauthorizedProxy(const std::string& uri);
	
private:
	Poco::SharedPtr<Poco::Net::HTTPServer> _pHTTPServer;
};


class RemotingTestSOAP12AuthChunked: public RemotingTestSOAP12Auth
{
public:
	RemotingTestSOAP12AuthChunked(const std::string& name);
	~RemotingTestSOAP12AuthChunked() override;
	
	static CppUnit::Test* suite();

protected:
	ITester::Ptr createProxy(const std::string& uri) override;
};


#endif // RemotingTest_INCLUDED
