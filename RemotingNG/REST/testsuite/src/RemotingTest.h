//
// RemotingTest.h
//
// Definition of the RemotingTest class.
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingTest_INCLUDED
#define RemotingTest_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "CppUnit/TestCase.h"
#include "ITester.h"
#include "IAuthTester.h"
#include "Poco/RemotingNG/REST/Listener.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/SharedPtr.h"


class RemotingTest: public CppUnit::TestCase
{
public:
	RemotingTest(const std::string& name);
	~RemotingTest();
	
	void testRegistration();
	void testPost();
	void testPut();
	void testGet();
	void testGetNotFound();
	void testCookie();
	void testFault();

	void setUp();
	void tearDown();

protected:
	virtual ITester::Ptr createProxy(const std::string& uri);

protected:
	std::string _listener;
	std::string _objectURI;
	std::string _publicURI;
	Poco::RemotingNG::REST::Listener::Ptr _pListener;
};


class RemotingTestREST: public RemotingTest
{
public:
	RemotingTestREST(const std::string& name);
	~RemotingTestREST();

	void setUp();

	static CppUnit::Test* suite();
};


class RemotingTestRESTCompressed: public RemotingTest
{
public:
	RemotingTestRESTCompressed(const std::string& name);
	~RemotingTestRESTCompressed();

	void setUp();
	
	static CppUnit::Test* suite();

protected:
	virtual ITester::Ptr createProxy(const std::string& uri);
};


class RemotingTestRESTChunked: public RemotingTest
{
public:
	RemotingTestRESTChunked(const std::string& name);
	~RemotingTestRESTChunked();

	void setUp();
	
	static CppUnit::Test* suite();

protected:
	virtual ITester::Ptr createProxy(const std::string& uri);
};


class RemotingTestRESTChunkedCompressed: public RemotingTest
{
public:
	RemotingTestRESTChunkedCompressed(const std::string& name);
	~RemotingTestRESTChunkedCompressed();

	void setUp();
	
	static CppUnit::Test* suite();

protected:
	virtual ITester::Ptr createProxy(const std::string& uri);
};


class RemotingTestRESTAuth: public RemotingTest
{
public:
	RemotingTestRESTAuth(const std::string& name);
	~RemotingTestRESTAuth();

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


class RemotingTestAuth: public CppUnit::TestCase
{
public:
	RemotingTestAuth(const std::string& name);
	~RemotingTestAuth();
	
	void testAuthenticatedGoodCredentials();
	void testAuthenticatedNoCredentials();
	void testAuthenticatedBadCredentials();
	void testAuthenticatedGoodOAuthCredentials();
	void testAuthenticatedBadOAuthCredentials();
	void testPermission();
	void testNoPermission();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

protected:
	IAuthTester::Ptr createProxy(const std::string& uri);

protected:
	std::string _listener;
	std::string _objectURI;
	std::string _publicURI;
	Poco::RemotingNG::REST::Listener::Ptr _pListener;
};


#endif // RemotingTest_INCLUDED
