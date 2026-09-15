//
// RemotingTest.cpp
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "RemotingTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/Transport.h"
#include "Poco/RemotingNG/TransportFactory.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/RemotingNG/Skeleton.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/TCP/Transport.h"
#include "Poco/RemotingNG/TCP/TransportFactory.h"
#include "Poco/RemotingNG/TCP/Listener.h"
#include "Poco/RemotingNG/TCP/PlainClientAuthenticator.h"
#include "Poco/RemotingNG/TCP/SCRAMClientAuthenticator.h"
#include "Poco/RemotingNG/TCP/SCRAMAuthenticator.h"
#include "Poco/RemotingNG/Context.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/MD5Engine.h"
#include "Poco/SHA1Engine.h"
#include "Poco/HMACEngine.h"
#include "Poco/PBKDF2Engine.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Delegate.h"
#include "Tester.h"
#include "TesterServerHelper.h"
#include "TesterClientHelper.h"
#include "TesterRemoteObject.h"
#include "TesterProxy.h"
#include <sstream>


class MockAuthenticator: public Poco::RemotingNG::Authenticator
{
public:
	typedef Poco::AutoPtr<MockAuthenticator> Ptr;

	Poco::RemotingNG::AuthenticateResult authenticate(const Poco::RemotingNG::Credentials& creds, Poco::UInt32 /*conversationID*/)
	{
		std::string username = creds.getAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "");
		std::string password = creds.getAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "");

		_lastUsername = username;

		if ((username == "user" && password == "pass") || (username == "admin" && password == "s3cr3t"))
			return Poco::RemotingNG::AuthenticateResult(Poco::RemotingNG::AuthenticateResult::AUTH_DONE, creds);
		else
			return Poco::RemotingNG::AuthenticateResult(Poco::RemotingNG::AuthenticateResult::AUTH_FAILED);
	}

	const std::string& lastUsername() const
	{
		return _lastUsername;
	}

private:
	std::string _lastUsername;
};


class MockSCRAMAuthenticator: public Poco::RemotingNG::TCP::SCRAMAuthenticator
{
public:
	typedef Poco::AutoPtr<MockSCRAMAuthenticator> Ptr;

	const std::string& lastUsername() const
	{
		return _lastUsername;
	}

	// SCRAMAuthenticator
	std::string saltForUser(const std::string& username, int& iterations)
	{
		if (username == "user" || username == "admin")
		{
			_lastUsername = username;
			iterations = 1000;
			return "salt";
		}
		else return "";
	}

	std::string hashForUser(const std::string& username)
	{
		std::string password;
		if (username == "user")
		{
			password = "pass";
		}
		else if (username == "admin")
		{
			password = "s3cr3t";
		}
		else return "";

		Poco::MD5Engine md5;
		md5.update(username);
		md5.update(std::string(":poco:"));
		md5.update(password);
		std::string hashedPassword = digestToHexString(md5);

		int iterations;
		std::string salt = saltForUser(username, iterations);
		Poco::PBKDF2Engine<Poco::HMACEngine<Poco::SHA1Engine> > pbkdf2(salt, iterations, 20);
		pbkdf2.update(hashedPassword);
		return digestToBinaryString(pbkdf2);
	}

private:
	std::string _lastUsername;
};


class MockAuthorizer: public Poco::RemotingNG::Authorizer
{
public:
	bool authorize(const std::string& method, const std::string& permission)
	{
		Poco::RemotingNG::Context::Ptr pContext = Poco::RemotingNG::Context::get();

		const Poco::RemotingNG::Credentials& creds = pContext->getCredentials();

		std::string username = creds.getAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "");
		std::string password = creds.getAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "");

		return username == "admin";
	}
};


bool operator == (const Struct1& s1, const Struct1& s2)
{
	return s1.aString == s2.aString
	    && s1.anInt   == s2.anInt
	    && s1.aDouble == s2.aDouble
	    && s1.anEnum  == s2.anEnum
	    && s1.anEnum2 == s2.anEnum2;
}


bool operator == (const Struct1::Ptr s1, const Struct1::Ptr s2)
{
	return *s1 == *s2;
}


bool operator == (const Struct2& s1, const Struct2& s2)
{
	bool equal = s1.anURI == s2.anURI
	    && s1.aDateTime == s2.aDateTime
	    && s1.aLocalDateTime == s2.aLocalDateTime
	    && s1.aTimestamp == s2.aTimestamp
	    && s1.anUUID == s2.anUUID;

	if (!equal) return false;
	if (s1.aVector.size() != s2.aVector.size()) return false;
	for (int i = 0; i < s1.aVector.size(); i++)
	{
		if (!(s1.aVector[i] == s2.aVector[i])) return false;
	}
	if (s1.anotherVector.size() != s2.anotherVector.size()) return false;
	for (int i = 0; i < s1.anotherVector.size(); i++)
	{
		if (!s1.anotherVector[i].isNull() && !s2.anotherVector[i].isNull())
		{
			if (!(*s1.anotherVector[i] == *s2.anotherVector[i])) return false;
		}
	}
	return true;
}


bool operator == (const Struct3& s1, const Struct3& s2)
{
	bool equal = s1.aCharVector == s2.aCharVector
	          && s1.aSet == s2.aSet
	          && s1.aMultiSet == s2.aMultiSet;
	if (!equal) return false;
	if (s1.aComplexVector.size() != s2.aComplexVector.size()) return false;
	for (int i = 0; i < s1.aComplexVector.size(); i++)
	{
		if (!(s1.aComplexVector[i] == s2.aComplexVector[i])) return false;
	}
	return true;
}


bool operator == (const Struct4& s1, const Struct4& s2)
{
	bool equal = s1.value == s2.value;
	if (!equal) return false;
	if (s1.vec.size() != s2.vec.size()) return false;
	for (int i = 0; i < s1.vec.size(); i++)
	{
		if (!(s1.vec[i] == s2.vec[i])) return false;
	}
	if (s1.ptr.isNull() != s2.ptr.isNull()) return false;
	if (s1.ptr && s2.ptr && !(*s1.ptr == *s2.ptr)) return false;
	return true;
}


RemotingTest::RemotingTest(const std::string& name): CppUnit::TestCase(name)
{
}


RemotingTest::~RemotingTest()
{
}


void RemotingTest::setUp()
{
	_pListener = new Poco::RemotingNG::TCP::Listener("localhost:2345");
	_listener = Poco::RemotingNG::ORB::instance().registerListener(_pListener);
	Poco::RemotingNG::TCP::TransportFactory::registerFactory();
	_objectURI = TesterServerHelper::registerObject(new Tester, "TheTester", _listener);
}


void RemotingTest::tearDown()
{
	TesterServerHelper::unregisterObject(_objectURI);
	Poco::RemotingNG::TCP::TransportFactory::unregisterFactory();
	Poco::RemotingNG::ORB::instance().unregisterListener(_listener);
	_pListener->connectionManager().shutdown();
	_pListener = 0;
	Poco::ThreadPool::defaultPool().joinAll();
}


void RemotingTest::testRegistration()
{
	assert (_objectURI == "remoting.tcp://localhost:2345/tcp/Tester/TheTester");
}


void RemotingTest::testInt()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testInt(pTester);
}


void RemotingTest::testEnum1()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testEnum1(pTester);
}


void RemotingTest::testEnum2()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testEnum2(pTester);
}


void RemotingTest::testNested()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testNested(pTester);
}


void RemotingTest::testStruct1()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testStruct1(pTester);
}


void RemotingTest::testStruct2()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testStruct2(pTester);
}


void RemotingTest::testStruct3()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testStruct3(pTester);
}


void RemotingTest::testStruct4()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testStruct4(pTester);
}


void RemotingTest::testStruct5()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testStruct4(pTester);
}


void RemotingTest::testClass1()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testClass1(pTester);
}


void RemotingTest::testPtr()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testPtr(pTester);
}


void RemotingTest::testStruct1Vec()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	testStruct1Vec(pTester);
}


void RemotingTest::testOneWay()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	pTester->testOneWay("s3cr3t");

	Poco::Thread::sleep(200);

	std::string result = pTester->testOneWayResult();
	assert (result == "s3cr3t");
}


void RemotingTest::testFault()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	try
	{
		pTester->testFault();
		fail("must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.message() == "Application exception: Something went wrong");
	}
}


void RemotingTest::testAuthenticatedGoodCredentials()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "pass");
	trans.setCredentials(creds);
	trans.setAuthenticator(new Poco::RemotingNG::TCP::PlainClientAuthenticator);

	_pListener->setAuthenticator(new MockAuthenticator);

	pTester->testAuthenticated();
}


void RemotingTest::testAuthenticatedBadCredentials()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "bad");
	trans.setCredentials(creds);
	trans.setAuthenticator(new Poco::RemotingNG::TCP::PlainClientAuthenticator);

	_pListener->setAuthenticator(new MockAuthenticator);

	try
	{
		pTester->testAuthenticated();
		fail("bad credentials - must throw");
	}
	catch (Poco::RemotingNG::AuthenticationFailedException& exc)
	{
		assert (exc.message() == "The server refused the provided credentials");
	}
}


void RemotingTest::testAuthenticatedNoCredentials()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	_pListener->setAuthenticator(new MockAuthenticator);

	try
	{
		pTester->testAuthenticated();
		fail("no credentials - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.message() == "Authentication failed");
	}
}


void RemotingTest::testAuthenticatedUpdatedCredentials()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "pass");
	trans.setCredentials(creds);
	trans.setAuthenticator(new Poco::RemotingNG::TCP::PlainClientAuthenticator);

	MockAuthenticator::Ptr pAuth = new MockAuthenticator;
	_pListener->setAuthenticator(pAuth);

	pTester->testAuthenticated();

	assert (pAuth->lastUsername() == "user");

	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "admin");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "s3cr3t");
	trans.setCredentials(creds);

	pTester->testAuthenticated();

	assert (pAuth->lastUsername() == "admin");

	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "h@ck3r");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "dummy");
	trans.setCredentials(creds);

	try
	{
		pTester->testAuthenticated();
		fail("bad credentials - must throw");
	}
	catch (Poco::RemotingNG::AuthenticationFailedException& exc)
	{
		assert (exc.message() == "The server refused the provided credentials");
	}
}


void RemotingTest::testAuthenticatedGoodSCRAMCredentials()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "pass");
	trans.setCredentials(creds);
	trans.setAuthenticator(new Poco::RemotingNG::TCP::SCRAMClientAuthenticator);

	_pListener->setAuthenticator(new MockSCRAMAuthenticator);

	pTester->testAuthenticated();
}


void RemotingTest::testAuthenticatedBadSCRAMCredentials()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "bad");
	trans.setCredentials(creds);
	trans.setAuthenticator(new Poco::RemotingNG::TCP::SCRAMClientAuthenticator);

	_pListener->setAuthenticator(new MockSCRAMAuthenticator);

	try
	{
		pTester->testAuthenticated();
		fail("bad credentials - must throw");
	}
	catch (Poco::RemotingNG::AuthenticationFailedException& exc)
	{
		assert (exc.message() == "The server refused the provided credentials");
	}
}


void RemotingTest::testAuthenticatedNoSCRAMCredentials()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	_pListener->setAuthenticator(new MockSCRAMAuthenticator);

	try
	{
		pTester->testAuthenticated();
		fail("no credentials - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.message() == "Authentication failed");
	}
}


void RemotingTest::testAuthenticatedUpdatedSCRAMCredentials()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "pass");
	trans.setCredentials(creds);
	trans.setAuthenticator(new Poco::RemotingNG::TCP::SCRAMClientAuthenticator);

	MockSCRAMAuthenticator::Ptr pAuth = new MockSCRAMAuthenticator;
	_pListener->setAuthenticator(pAuth);

	pTester->testAuthenticated();

	assert (pAuth->lastUsername() == "user");

	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "admin");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "s3cr3t");
	trans.setCredentials(creds);

	pTester->testAuthenticated();

	assert (pAuth->lastUsername() == "admin");

	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "h@ck3r");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "dummy");
	trans.setCredentials(creds);

	try
	{
		pTester->testAuthenticated();
		fail("bad credentials - must throw");
	}
	catch (Poco::RemotingNG::AuthenticationFailedException& exc)
	{
		assert (exc.message() == "The server refused the provided credentials");
	}
}


void RemotingTest::testAuthenticatedMultipleSCRAMCredentials()
{
	ITester::Ptr pTester1 = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy1 = pTester1.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans1 = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy1->remoting__transport());

	Poco::RemotingNG::Credentials creds1;
	creds1.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds1.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "pass");
	trans1.setCredentials(creds1);
	trans1.setAuthenticator(new Poco::RemotingNG::TCP::SCRAMClientAuthenticator);

	ITester::Ptr pTester2 = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy2 = pTester2.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans2 = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy2->remoting__transport());

	Poco::RemotingNG::Credentials creds2;
	creds2.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "admin");
	creds2.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "s3cr3t");
	trans2.setCredentials(creds2);
	trans2.setAuthenticator(new Poco::RemotingNG::TCP::SCRAMClientAuthenticator);

	ITester::Ptr pTester3 = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy3 = pTester3.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans3 = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy3->remoting__transport());

	Poco::RemotingNG::Credentials creds3;
	creds3.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "h@ck3r");
	creds3.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "dummy");
	trans3.setCredentials(creds3);
	trans3.setAuthenticator(new Poco::RemotingNG::TCP::SCRAMClientAuthenticator);

	MockSCRAMAuthenticator::Ptr pAuth = new MockSCRAMAuthenticator;
	_pListener->setAuthenticator(pAuth);

	pTester1->testAuthenticated();

	assert (pAuth->lastUsername() == "user");

	pTester2->testAuthenticated();

	assert (pAuth->lastUsername() == "admin");

	try
	{
		pTester3->testAuthenticated();
		fail("bad credentials - must throw");
	}
	catch (Poco::RemotingNG::AuthenticationFailedException& exc)
	{
		assert (exc.message() == "The server refused the provided credentials");
	}

	pTester1->testAuthenticated();
	pTester2->testAuthenticated();
}


void RemotingTest::testPermission()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "admin");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "s3cr3t");
	trans.setCredentials(creds);
	trans.setAuthenticator(new Poco::RemotingNG::TCP::PlainClientAuthenticator);

	_pListener->setAuthenticator(new MockAuthenticator);
	_pListener->setAuthorizer(new MockAuthorizer);

	pTester->testPermission();
}


void RemotingTest::testNoPermission()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "pass");
	trans.setCredentials(creds);
	trans.setAuthenticator(new Poco::RemotingNG::TCP::PlainClientAuthenticator);

	_pListener->setAuthenticator(new MockAuthenticator);
	_pListener->setAuthorizer(new MockAuthorizer);

	try
	{
		pTester->testPermission();
		fail("no permission - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.message() == "No permission: perm1");
	}
}


void RemotingTest::testTimeout()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::TCP::Transport& trans = static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy->remoting__transport());

	trans.setTimeout(Poco::Timespan(1, 0));

	try
	{
		pTester->testTimeout();
		fail("timeout - must throw");
	}
	catch (Poco::TimeoutException&)
	{
	}
}


void RemotingTest::testEvent()
{
	Poco::RemotingNG::TCP::Listener::Ptr pEventListener = new Poco::RemotingNG::TCP::Listener;
	TesterServerHelper::enableEvents(_objectURI, "tcp");

	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	pProxy->remoting__enableEvents(pEventListener);

	pTester->testEvent += Poco::delegate(this, &RemotingTest::onEvent);
	_eventArg.clear();
	pTester->fireTestEvent("s3cr3t");
	_eventReceived.wait(2000);
	assert (_eventArg == "s3cr3t");

	pTester->testEvent -= Poco::delegate(this, &RemotingTest::onEvent);
	pProxy->remoting__enableEvents(pEventListener, false);
	pTester = 0;
	pProxy = 0;
	Poco::Thread::sleep(1000); // wait for unsubscribe message
}


void RemotingTest::testOneWayEvent()
{
	Poco::RemotingNG::TCP::Listener::Ptr pEventListener = new Poco::RemotingNG::TCP::Listener;
	TesterServerHelper::enableEvents(_objectURI, "tcp");

	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	pProxy->remoting__enableEvents(pEventListener);

	pTester->testOneWayEvent += Poco::delegate(this, &RemotingTest::onEvent);
	_eventArg.clear();
	pTester->fireTestOneWayEvent("s3cr3t");
	_eventReceived.wait(2000);
	assert (_eventArg == "s3cr3t");

	pTester->testOneWayEvent -= Poco::delegate(this, &RemotingTest::onEvent);
	pProxy->remoting__enableEvents(pEventListener, false);
	pTester = 0;
	pProxy = 0;
	Poco::Thread::sleep(1000); // wait for unsubscribe message
}


void RemotingTest::testVoidEvent()
{
	Poco::RemotingNG::TCP::Listener::Ptr pEventListener = new Poco::RemotingNG::TCP::Listener;
	TesterServerHelper::enableEvents(_objectURI, "tcp");

	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	pProxy->remoting__enableEvents(pEventListener);

	pTester->testVoidEvent += Poco::delegate(this, &RemotingTest::onVoidEvent);
	_eventArg.clear();
	pTester->fireTestVoidEvent();
	_eventReceived.wait(2000);
	assert (_eventArg == "FIRED");

	pTester->testVoidEvent -= Poco::delegate(this, &RemotingTest::onVoidEvent);
	pProxy->remoting__enableEvents(pEventListener, false);
	pTester = 0;
	pProxy = 0;
	Poco::Thread::sleep(1000); // wait for unsubscribe message
}


void RemotingTest::onEvent(const void* pSender, std::string& arg)
{
	_eventArg = arg;
	_eventReceived.set();
}


void RemotingTest::onVoidEvent(const void* pSender)
{
	_eventArg = "FIRED";
	_eventReceived.set();
}


void RemotingTest::testInt(ITester::Ptr pTester)
{
	int i = pTester->testInt1(42);
	assert (i == 42);

	pTester->testInt2(i);
	assert (i == 2112);

	int i2(0);
	pTester->testInt3(55, i2);
	assert (i2 == 55);
}


void RemotingTest::testEnum1(ITester::Ptr pTester)
{
	Enum1 e = pTester->testEnum11(VALUE_1);
	assert (e == VALUE_1);

	pTester->testEnum12(e);
	assert (e == VALUE_2);

	pTester->testEnum13(VALUE_3, e);
	assert (e == VALUE_3);
}


void RemotingTest::testEnum2(ITester::Ptr pTester)
{
	Struct1::Enum2 e = pTester->testEnum21(Struct1::VALUE_1);
	assert (e == Struct1::VALUE_1);

	pTester->testEnum22(e);
	assert (e == Struct1::VALUE_2);

	pTester->testEnum23(Struct1::VALUE_3, e);
	assert (e == Struct1::VALUE_3);
}


void RemotingTest::testNested(ITester::Ptr pTester)
{
	Poco::Timestamp::TimeVal tv1 = 12345678;
	Poco::Timestamp::TimeVal tv2 = pTester->testNestedType(tv1);
	assert (tv1 == tv2);
}


void RemotingTest::testStruct1(ITester::Ptr pTester)
{
	Struct1 s1;
	s1.aString = "foobar";
	s1.anInt = 2112;
	s1.aDouble = 0.5;
	s1.anEnum = VALUE_1;
	s1.anEnum2 = Struct1::VALUE_2;

	Struct1 s2 = pTester->testStruct11(s1);
	assert (s2 == s1);

	pTester->testStruct12(s2);
	assert (s2 == s1);

	Struct1 s3;
	pTester->testStruct13(s1, s3);
	assert (s3 == s1);
}


void RemotingTest::testStruct2(ITester::Ptr pTester)
{
	Struct2 s21;
	for (int i = 0; i < 10; i++)
	{
		Struct1 s1;
		s1.aString = "foobar" + Poco::NumberFormatter::format(i);
		s1.anInt = i;
		s1.aDouble = i + 0.5;
		s1.anEnum = VALUE_1;
		s1.anEnum2 = Struct1::VALUE_2;
		s21.aVector.push_back(s1);
	}

	for (int i = 0; i < 9; i++)
	{
		Struct1::Ptr p1 = new Struct1;
		p1->aString = "foobar" + Poco::NumberFormatter::format(i);
		p1->anInt = i;
		p1->aDouble = i + 0.5;
		p1->anEnum = VALUE_1;
		p1->anEnum2 = Struct1::VALUE_2;
		s21.anotherVector.push_back(p1);
	}

	s21.anotherVector.push_back(Struct1::Ptr());
	s21.anURI = "http://www.appinf.com";
	s21.anUUID.parse("01234567-ABCD-EF00-1234-0123456789AB");
	s21.aDateTime.assign(2009, 11, 9, 13, 23, 23, 200);
	s21.aLocalDateTime.assign(7200, 2009, 11, 9, 13, 23, 23, 200, 0);
	s21.aTimestamp = s21.aDateTime.timestamp();

	Struct2 s22 = pTester->testStruct21(s21);
	assert (s22 == s21);

	pTester->testStruct22(s22);
	assert (s22 == s21);

	Struct2 s23;
	assert (!(s23 == s21));
	pTester->testStruct23(s21, s23);
	assert (s23 == s21);
}


void RemotingTest::testStruct3(ITester::Ptr pTester)
{
	Struct3 s31;
	Struct2 s21;
	for (int i = 0; i < 10; i++)
	{
		Struct1 s1;
		s1.aString = "foobar" + Poco::NumberFormatter::format(i);
		s1.anInt = i;
		s1.aDouble = i + 0.5;
		s1.anEnum = VALUE_1;
		s1.anEnum2 = Struct1::VALUE_2;
		s21.aVector.push_back(s1);
	}

	for (int i = 0; i < 9; i++)
	{
		Struct1::Ptr p1 = new Struct1;
		p1->aString = "foobar" + Poco::NumberFormatter::format(i);
		p1->anInt = i;
		p1->aDouble = i + 0.5;
		p1->anEnum = VALUE_1;
		p1->anEnum2 = Struct1::VALUE_2;
		s21.anotherVector.push_back(p1);
	}

	s21.anotherVector.push_back(Struct1::Ptr());
	s21.anURI = "http://www.appinf.com";
	s21.anUUID.parse("01234567-ABCD-EF00-1234-0123456789AB");
	s21.aDateTime.assign(2009, 11, 9, 13, 23, 23, 200);
	s21.aLocalDateTime.assign(7200, 2009, 11, 9, 13, 23, 23, 200, 0);
	s21.aTimestamp = s21.aDateTime.timestamp();

	for (int i = 0; i < 10; i++)
	{
		for (char c = 'A'; c <= 'Z'; c++)
			s31.aCharVector.push_back(c);
	}
	s31.aComplexVector.push_back(s21);
	s31.aComplexVector.push_back(s21);
	s31.aSet.insert(1);
	s31.aSet.insert(2);
	s31.aSet.insert(3);
	s31.aMultiSet.insert(1);
	s31.aMultiSet.insert(2);
	s31.aMultiSet.insert(3);

	Struct3 s32 = pTester->testStruct31(s31);
	assert (s32 == s31);

	pTester->testStruct32(s32);
	assert (s32 == s31);

	Struct3 s33;
	assert (!(s33 == s31));
	pTester->testStruct33(s31, s33);
	assert (s33 == s31);
}


void RemotingTest::testStruct4(ITester::Ptr pTester)
{
	Struct4 s41;
	s41.value = "baz";
	Struct4 s42;
	s42.value = "foo";
	Struct4 s43;
	s43.value = "bar";
	s41.vec.push_back(s42);
	s41.vec.push_back(s43);

	Struct4 s44 = pTester->testStruct41(s41);
	assert (s44 == s41);

	pTester->testStruct42(s44);
	assert (s44 == s41);

	Struct4 s45;
	assert (!(s45 == s41));
	pTester->testStruct43(s41, s45);
	assert (s41 == s45);

	s41.ptr = new Struct4;

	s44 = pTester->testStruct41(s41);
	assert (s44 == s41);

	pTester->testStruct42(s44);
	assert (s44 == s41);

	Struct4 s46;
	assert (!(s46 == s41));
	pTester->testStruct43(s41, s46);
	assert (s41 == s46);
}


void RemotingTest::testStruct5(ITester::Ptr pTester)
{
	Struct5 s51;
	s51.tv = 12345678;

	Struct5 s52 = pTester->testStruct51(s51);
	assert (s52.tv == s51.tv);

	pTester->testStruct52(s52);
	assert (s52.tv == s51.tv);

	Struct5 s53;
	pTester->testStruct53(s51, s53);
	assert (s51.tv == s53.tv);
}


void RemotingTest::testClass1(ITester::Ptr pTester)
{
	Class1 c1;
	c1.setAString("foobar");
	c1.setAnInt(42);
	c1.setAnEnum(VALUE_2);

	Class1 c2 = pTester->testClass11(c1);
	assert (c2 == c1);

	pTester->testClass12(c2);
	assert (c2 == c1);

	Class1 c3;
	pTester->testClass13(c1, c3);
	assert (c3 == c1);
}


void RemotingTest::testPtr(ITester::Ptr pTester)
{
	Struct1::Ptr p1 = new Struct1;
	p1->aString = "foobar";
	p1->anInt = 2112;
	p1->aDouble = 0.5;
	p1->anEnum = VALUE_1;
	p1->anEnum2 = Struct1::VALUE_2;

	Struct1::Ptr p2 = pTester->testPtr(p1);
	assert (*p2 == *p1);
}


void RemotingTest::testStruct1Vec(ITester::Ptr pTester)
{
	Struct1Vec vec1;
	for (int i = 0; i < 10; i++)
	{
		Struct1 s1;
		s1.aString = "foobar" + Poco::NumberFormatter::format(i);
		s1.anInt = i;
		s1.aDouble = i + 0.5;
		s1.anEnum = VALUE_1;
		s1.anEnum2 = Struct1::VALUE_2;
		vec1.push_back(s1);
	}

	Struct1Vec vec2 = pTester->testStruct1Vec1(vec1);
	assert (vec2.size() == vec1.size());
	for (int i = 0; i < vec1.size(); i++)
	{
		assert (vec2[i] == vec1[i]);
	}

	pTester->testStruct1Vec2(vec2);
	assert (vec2.size() == vec1.size());
	for (int i = 0; i < vec1.size(); i++)
	{
		assert (vec2[i] == vec1[i]);
	}

	Struct1Vec vec3;
	pTester->testStruct1Vec3(vec1, vec3);
	assert (vec2.size() == vec1.size());
	for (int i = 0; i < vec1.size(); i++)
	{
		assert (vec2[i] == vec1[i]);
	}
}


ITester::Ptr RemotingTest::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("tcp", uri);
	static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy->remoting__transport()).enableCompression(false);
	return pProxy;
}


CppUnit::Test* RemotingTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTest");

	CppUnit_addTest(pSuite, RemotingTest, testRegistration);
	CppUnit_addTest(pSuite, RemotingTest, testInt);
	CppUnit_addTest(pSuite, RemotingTest, testEnum1);
	CppUnit_addTest(pSuite, RemotingTest, testEnum2);
	CppUnit_addTest(pSuite, RemotingTest, testNested);
	CppUnit_addTest(pSuite, RemotingTest, testStruct1);
	CppUnit_addTest(pSuite, RemotingTest, testStruct2);
	CppUnit_addTest(pSuite, RemotingTest, testStruct3);
	CppUnit_addTest(pSuite, RemotingTest, testStruct4);
	CppUnit_addTest(pSuite, RemotingTest, testStruct5);
	CppUnit_addTest(pSuite, RemotingTest, testClass1);
	CppUnit_addTest(pSuite, RemotingTest, testPtr);
	CppUnit_addTest(pSuite, RemotingTest, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTest, testOneWay);
	CppUnit_addTest(pSuite, RemotingTest, testFault);
	CppUnit_addTest(pSuite, RemotingTest, testAuthenticatedGoodCredentials);
	CppUnit_addTest(pSuite, RemotingTest, testAuthenticatedBadCredentials);
	CppUnit_addTest(pSuite, RemotingTest, testAuthenticatedNoCredentials);
	CppUnit_addTest(pSuite, RemotingTest, testAuthenticatedUpdatedCredentials);
	CppUnit_addTest(pSuite, RemotingTest, testAuthenticatedGoodSCRAMCredentials);
	CppUnit_addTest(pSuite, RemotingTest, testAuthenticatedBadSCRAMCredentials);
	CppUnit_addTest(pSuite, RemotingTest, testAuthenticatedNoSCRAMCredentials);
	CppUnit_addTest(pSuite, RemotingTest, testAuthenticatedUpdatedSCRAMCredentials);
	CppUnit_addTest(pSuite, RemotingTest, testAuthenticatedMultipleSCRAMCredentials);
	CppUnit_addTest(pSuite, RemotingTest, testPermission);
	CppUnit_addTest(pSuite, RemotingTest, testNoPermission);
	CppUnit_addTest(pSuite, RemotingTest, testTimeout);
	CppUnit_addTest(pSuite, RemotingTest, testEvent);
	CppUnit_addTest(pSuite, RemotingTest, testOneWayEvent);

	return pSuite;
}



RemotingTestCompressed::RemotingTestCompressed(const std::string& name): RemotingTest(name)
{
}


RemotingTestCompressed::~RemotingTestCompressed()
{
}


ITester::Ptr RemotingTestCompressed::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("tcp", uri);
	static_cast<Poco::RemotingNG::TCP::Transport&>(pProxy->remoting__transport()).enableCompression(true);
	return pProxy;
}


CppUnit::Test* RemotingTestCompressed::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestCompressed");

	CppUnit_addTest(pSuite, RemotingTestCompressed, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testInt);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testNested);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testClass1);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testPtr);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testOneWay);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testFault);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testAuthenticatedGoodCredentials);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testAuthenticatedBadCredentials);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testAuthenticatedNoCredentials);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testAuthenticatedUpdatedCredentials);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testAuthenticatedGoodSCRAMCredentials);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testAuthenticatedBadSCRAMCredentials);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testAuthenticatedNoSCRAMCredentials);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testAuthenticatedUpdatedSCRAMCredentials);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testAuthenticatedMultipleSCRAMCredentials);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testPermission);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testNoPermission);
	CppUnit_addTest(pSuite, RemotingTest, testTimeout);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testEvent);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testOneWayEvent);
	CppUnit_addTest(pSuite, RemotingTestCompressed, testVoidEvent);

	return pSuite;
}

