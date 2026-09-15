//
// RemotingTest.cpp
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "RemotingTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/Transport.h"
#include "Poco/RemotingNG/TransportFactory.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/RemotingNG/Skeleton.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/Credentials.h"
#include "Poco/RemotingNG/Context.h"
#include "Poco/RemotingNG/HTTP/Transport.h"
#include "Poco/RemotingNG/HTTP/TransportFactory.h"
#include "Poco/RemotingNG/HTTP/Listener.h"
#include "Poco/RemotingNG/HTTP/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Tester.h"
#include "TesterServerHelper.h"
#include "TesterClientHelper.h"
#include "TesterRemoteObject.h"
#include "TesterProxy.h"
#include <sstream>


namespace
{
	class NoAuthRequestHandler: public Poco::Net::HTTPRequestHandler
	{
	public:
		void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
		{
			Poco::NullOutputStream nullStream;
			Poco::StreamCopier::copyStream(request.stream(), nullStream);
			response.requireAuthentication("RemotingNG");
			response.setContentLength(0);
			response.send();
		}
	};


	class HTTPAuthRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		HTTPAuthRequestHandlerFactory(Poco::RemotingNG::HTTP::Listener& listener):
			_listener(listener)
		{
		}
		
		Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request)
		{
			if (request.hasCredentials())
			{
				Poco::Net::HTTPBasicCredentials creds(request);
				if (creds.getUsername() == "user" && creds.getPassword() == "s3cr3t")
				{
					return new Poco::RemotingNG::HTTP::HTTPRequestHandler(_listener);
				}
			}
			return new NoAuthRequestHandler;
		}
		
	private:
		Poco::RemotingNG::HTTP::Listener& _listener;
	};
	
	
	class MockAuthenticator: public Poco::RemotingNG::Authenticator
	{
	public:
		typedef Poco::AutoPtr<MockAuthenticator> Ptr;

		Poco::RemotingNG::AuthenticateResult authenticate(const Poco::RemotingNG::Credentials& creds, Poco::UInt32)
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
}


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
	_pListener = new Poco::RemotingNG::HTTP::Listener("127.0.0.1:2280");
	_listener = Poco::RemotingNG::ORB::instance().registerListener(_pListener);
	Poco::RemotingNG::HTTP::TransportFactory::registerFactory();
	_objectURI = TesterServerHelper::registerObject(new Tester, "TheTester", _listener);
}


void RemotingTest::tearDown()
{
	TesterServerHelper::unregisterObject(_objectURI);
	Poco::RemotingNG::HTTP::TransportFactory::unregisterFactory();
	Poco::RemotingNG::ORB::instance().unregisterListener(_listener);
	_pListener = 0;
}


void RemotingTest::testRegistration()
{
	assert (_objectURI == "http://127.0.0.1:2280/http/Tester/TheTester");
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
	Poco::RemotingNG::HTTP::Transport& trans = static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "pass");
	trans.setCredentials(creds);
	trans.setAuthentication(Poco::RemotingNG::HTTP::Transport::AUTH_BASIC);

	_pListener->setAuthenticator(new MockAuthenticator);

	pTester->testAuthenticated();
}


void RemotingTest::testAuthenticatedBadCredentials()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::HTTP::Transport& trans = static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "bad!");
	trans.setCredentials(creds);
	trans.setAuthentication(Poco::RemotingNG::HTTP::Transport::AUTH_BASIC);

	_pListener->setAuthenticator(new MockAuthenticator);

	try
	{
		pTester->testAuthenticated();
		fail("bad credentials - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.message() == "Authentication failed");
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


void RemotingTest::testPermission()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::HTTP::Transport& trans = static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "admin");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "s3cr3t");
	trans.setCredentials(creds);
	trans.setAuthentication(Poco::RemotingNG::HTTP::Transport::AUTH_BASIC);

	_pListener->setAuthenticator(new MockAuthenticator);
	_pListener->setAuthorizer(new MockAuthorizer);

	pTester->testPermission();
}


void RemotingTest::testNoPermission()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::HTTP::Transport& trans = static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "pass");
	trans.setCredentials(creds);
	trans.setAuthentication(Poco::RemotingNG::HTTP::Transport::AUTH_BASIC);

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
	pProxy->remoting__connect("http", uri);
	static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport()).enableCompression(false);
	return pProxy;
}


RemotingTestHTTP::RemotingTestHTTP(const std::string& name): RemotingTest(name)
{
}


RemotingTestHTTP::~RemotingTestHTTP()
{
}


void RemotingTestHTTP::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(false);
}


CppUnit::Test* RemotingTestHTTP::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestHTTP");

	CppUnit_addTest(pSuite, RemotingTestHTTP, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testInt);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testNested);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testClass1);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testPtr);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testOneWay);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testFault);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testAuthenticatedGoodCredentials);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testAuthenticatedBadCredentials);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testAuthenticatedNoCredentials);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testPermission);
	CppUnit_addTest(pSuite, RemotingTestHTTP, testNoPermission);

	return pSuite;
}


RemotingTestHTTPCompressed::RemotingTestHTTPCompressed(const std::string& name): RemotingTest(name)
{
}


RemotingTestHTTPCompressed::~RemotingTestHTTPCompressed()
{
}


void RemotingTestHTTPCompressed::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(true);
}


ITester::Ptr RemotingTestHTTPCompressed::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("http", uri);
	static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport()).enableCompression(true);
	return pProxy;
}


CppUnit::Test* RemotingTestHTTPCompressed::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestHTTPCompressed");

	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testInt);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testNested);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testClass1);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testPtr);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testOneWay);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testFault);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testAuthenticatedGoodCredentials);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testAuthenticatedBadCredentials);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testAuthenticatedNoCredentials);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testPermission);
	CppUnit_addTest(pSuite, RemotingTestHTTPCompressed, testNoPermission);

	return pSuite;
}


RemotingTestHTTPNonChunked::RemotingTestHTTPNonChunked(const std::string& name): RemotingTest(name)
{
}


RemotingTestHTTPNonChunked::~RemotingTestHTTPNonChunked()
{
}


ITester::Ptr RemotingTestHTTPNonChunked::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("http", uri);
	static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport()).enableChunkedTransferEncoding(false);
	return pProxy;
}


void RemotingTestHTTPNonChunked::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(false);
	_pListener->enableChunkedTransferEncoding(false);
}


CppUnit::Test* RemotingTestHTTPNonChunked::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestHTTPNonChunked");

	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testInt);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testNested);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testClass1);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testPtr);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testOneWay);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testFault);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testAuthenticatedGoodCredentials);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testAuthenticatedBadCredentials);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testAuthenticatedNoCredentials);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testPermission);
	CppUnit_addTest(pSuite, RemotingTestHTTPNonChunked, testNoPermission);

	return pSuite;
}


RemotingTestHTTPAuth::RemotingTestHTTPAuth(const std::string& name): RemotingTest(name)
{
}


RemotingTestHTTPAuth::~RemotingTestHTTPAuth()
{
}


void RemotingTestHTTPAuth::setUp()
{
	_pListener = new Poco::RemotingNG::HTTP::Listener("127.0.0.1:2280", Poco::RemotingNG::HTTP::Listener::PROTO_HTTP);
	_listener = Poco::RemotingNG::ORB::instance().registerListener(_pListener);
	Poco::RemotingNG::HTTP::TransportFactory::registerFactory();
	_objectURI = TesterServerHelper::registerObject(new Tester, "TheTester", _listener);

	Poco::Net::ServerSocket ss(2280);
	_pHTTPServer = new Poco::Net::HTTPServer(new HTTPAuthRequestHandlerFactory(*_pListener), ss, new Poco::Net::HTTPServerParams);
	_pHTTPServer->start();
}


void RemotingTestHTTPAuth::tearDown()
{
	RemotingTest::tearDown();
	_pHTTPServer->stop();
	_pHTTPServer = 0;
}


void RemotingTestHTTPAuth::testUnauthorized()
{
	ITester::Ptr pTester = createUnauthorizedProxy(_objectURI);

	try
	{
		testInt(pTester);
		fail("Unauthorized - must throw");
	}
	catch (Poco::Exception& exc)
	{
		assert (exc.message() == "HTTP Error: Unauthorized");
	}
}


ITester::Ptr RemotingTestHTTPAuth::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("http", uri);
	static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport()).enableChunkedTransferEncoding(false);
	static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport()).setAuthentication(Poco::RemotingNG::HTTP::Transport::AUTH_ANY);

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "s3cr3t");
	static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport()).setCredentials(creds);

	return pProxy;
}


ITester::Ptr RemotingTestHTTPAuth::createUnauthorizedProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("http", uri);
	static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport()).enableChunkedTransferEncoding(false);

	return pProxy;
}


CppUnit::Test* RemotingTestHTTPAuth::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestHTTPAuth");

	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testInt);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testNested);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testClass1);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testPtr);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testFault);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuth, testUnauthorized);

	return pSuite;
}


RemotingTestHTTPAuthChunked::RemotingTestHTTPAuthChunked(const std::string& name): RemotingTestHTTPAuth(name)
{
}


RemotingTestHTTPAuthChunked::~RemotingTestHTTPAuthChunked()
{
}


ITester::Ptr RemotingTestHTTPAuthChunked::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("http", uri);
	static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport()).enableChunkedTransferEncoding(true);
	static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport()).setAuthentication(Poco::RemotingNG::HTTP::Transport::AUTH_BASIC);

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "user");
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "s3cr3t");
	static_cast<Poco::RemotingNG::HTTP::Transport&>(pProxy->remoting__transport()).setCredentials(creds);

	return pProxy;
}


CppUnit::Test* RemotingTestHTTPAuthChunked::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestHTTPAuthChunked");

	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testInt);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testNested);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testClass1);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testPtr);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testFault);
	CppUnit_addTest(pSuite, RemotingTestHTTPAuthChunked, testUnauthorized);

	return pSuite;
}
