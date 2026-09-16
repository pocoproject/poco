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
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/Context.h"
#include "Poco/RemotingNG/Credentials.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/SOAP/Transport.h"
#include "Poco/RemotingNG/SOAP/TransportFactory.h"
#include "Poco/RemotingNG/SOAP/Listener.h"
#include "Poco/RemotingNG/SOAP/SOAPRequestHandler.h"
#include "Poco/RemotingNG/SOAP/Serializer.h"
#include "Poco/RemotingNG/SOAP/SOAPFaultException.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/SHA1Engine.h"
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
			response.requireAuthentication("SOAP");
			response.setContentLength(0);
			response.send();
		}
	};


	class SOAPAuthRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		SOAPAuthRequestHandlerFactory(Poco::RemotingNG::SOAP::Listener& listener):
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
					return new Poco::RemotingNG::SOAP::SOAPRequestHandler(_listener);
				}
			}
			return new NoAuthRequestHandler;
		}

	private:
		Poco::RemotingNG::SOAP::Listener& _listener;
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

	class MockWSSEAuthenticator: public Poco::RemotingNG::Authenticator
	{
	public:
		typedef Poco::AutoPtr<MockAuthenticator> Ptr;

		Poco::RemotingNG::AuthenticateResult authenticate(const Poco::RemotingNG::Credentials& creds, Poco::UInt32)
		{
			std::string username = creds.getAttribute(Poco::RemotingNG::Credentials::ATTR_USERNAME, "");
			std::string password = creds.getAttribute(Poco::RemotingNG::Credentials::ATTR_PASSWORD, "");
			std::string nonce64 = creds.getAttribute(Poco::RemotingNG::Credentials::ATTR_NONCE, "");
			std::string created = creds.getAttribute(Poco::RemotingNG::Credentials::ATTR_CREATED, "");
			std::string type = creds.getAttribute(Poco::RemotingNG::Credentials::ATTR_MECHANISM, "");

			_lastUsername = username;

			if (type == "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest")
			{
				std::stringstream nonceStream(nonce64);
				Poco::Base64Decoder dec(nonceStream);
				std::string nonce;
				Poco::StreamCopier::copyToString(dec, nonce);

				Poco::SHA1Engine sha1;
				sha1.update(nonce);
				sha1.update(created);
				sha1.update("pass");
				auto digest = sha1.digest();

				std::ostringstream digestStream;
				Poco::Base64Encoder enc(digestStream);
				enc.write(reinterpret_cast<const char*>(digest.data()), digest.size());
				enc.close();
				std::string digest64 = digestStream.str();

				if (username == "user" && digest64 == password)
					return Poco::RemotingNG::AuthenticateResult(Poco::RemotingNG::AuthenticateResult::AUTH_DONE, creds);
				else
					return Poco::RemotingNG::AuthenticateResult(Poco::RemotingNG::AuthenticateResult::AUTH_FAILED);
			}
			else
			{
				if (username == "user" && password == "pass")
					return Poco::RemotingNG::AuthenticateResult(Poco::RemotingNG::AuthenticateResult::AUTH_DONE, creds);
				else
					return Poco::RemotingNG::AuthenticateResult(Poco::RemotingNG::AuthenticateResult::AUTH_FAILED);
			}
		}

		const std::string& lastUsername() const
		{
			return _lastUsername;
		}

	private:
		std::string _lastUsername;
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
	_pListener = new Poco::RemotingNG::SOAP::Listener("127.0.0.1:2283", "");
	_listener = Poco::RemotingNG::ORB::instance().registerListener(_pListener);
	Poco::RemotingNG::SOAP::TransportFactory::registerFactory();
	_objectURI = TesterServerHelper::registerObject(new Tester, "TheTester", _listener);
}


void RemotingTest::tearDown()
{
	TesterServerHelper::unregisterObject(_objectURI);
	Poco::RemotingNG::SOAP::TransportFactory::unregisterFactory();
	Poco::RemotingNG::ORB::instance().unregisterListener(_listener);
	_pListener = 0;
}


void RemotingTest::testRegistration()
{
	assert (_objectURI == "http://127.0.0.1:2283/soap/Tester/TheTester");
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
	catch (Poco::RemotingNG::SOAP::SOAPFaultException& exc)
	{
		using Poco::RemotingNG::SOAP::XMLElement;

		assertMessageEqual ("Application exception: Something went wrong", exc.message());

		assertMessageEqual ("Application exception: Something went wrong", exc.faultReason());

		if (exc.namespaceURI() == "http://schemas.xmlsoap.org/soap/envelope/")
		{
			assert (exc.faultCode() == "soap:Server");

			const XMLElement* pFaultCode = exc.fault().findChild("faultcode", exc.fault().namespaceURI());
			assertNotNull (pFaultCode);
			assert (pFaultCode->value() == "soap:Server");
			
			const XMLElement* pFaultString = exc.fault().findChild("faultstring", exc.fault().namespaceURI());
			assertNotNull (pFaultString);
			assertMessageEqual ("Application exception: Something went wrong", pFaultString->value());
			
			const XMLElement* pDetail = exc.faultDetail();
			assertNotNull (pDetail);

			const XMLElement* pExc = pDetail->findChild("Exception", "http://www.appinf.com/remoting");
			assertNotNull (pExc);

			const XMLElement* pType = pExc->findChild("Type", pExc->namespaceURI());
			assertNotNull (pType);
			assert (pType->value() == "Application exception");

			const XMLElement* pMessage = pExc->findChild("Message", pExc->namespaceURI());
			assertNotNull (pMessage);
			assertMessageEqual ("Something went wrong", pMessage->value());

			const XMLElement* pCode = pExc->findChild("Code", pExc->namespaceURI());
			assertNotNull (pCode);
			assert (pCode->value() == "0");
		}
		else if (exc.namespaceURI() == "http://www.w3.org/2003/05/soap-envelope")
		{
			assert (exc.faultCode() == "soap:Receiver");
	
			const XMLElement* pCode = exc.fault().findChild("Code", exc.fault().namespaceURI());
			assertNotNull (pCode);
			
			const XMLElement* pValue = pCode->findChild("Value", pCode->namespaceURI());
			assertNotNull (pValue);
			assert (pValue->value() == "soap:Receiver");

			const XMLElement* pReason = exc.fault().findChild("Reason", exc.fault().namespaceURI());
			assertNotNull (pReason);
			
			const XMLElement* pText = pReason->findChild("Text", pReason->namespaceURI());
			assertNotNull (pText);
			assertMessageEqual ("Application exception: Something went wrong", pText->value());

			const XMLElement* pDetail = exc.faultDetail();
			assertNotNull (pDetail);
			
			const XMLElement* pExc = pDetail->findChild("Exception", "http://www.appinf.com/remoting");
			assertNotNull (pExc);

			const XMLElement* pType = pExc->findChild("Type", pExc->namespaceURI());
			assertNotNull (pType);
			assert (pType->value() == "Application exception");

			const XMLElement* pMessage = pExc->findChild("Message", pExc->namespaceURI());
			assertNotNull (pMessage);
			assertMessageEqual ("Something went wrong", pMessage->value());

			const XMLElement* pExCode = pExc->findChild("Code", pExc->namespaceURI());
			assertNotNull (pExCode);
			assert (pExCode->value() == "0");
		}
		else
		{
			fail("unexpected namespace in fault element");
		}
	}
	catch (Poco::RemotingNG::RemoteException&)
	{
		fail("must throw SOAPFaultException");
	}
}


void RemotingTest::testCookie()
{
	ITester::Ptr pTester = createProxy(_objectURI);
	pTester->setCookie();
	assert (pTester->verifyCookie());

	Poco::Thread::sleep(6000);
	assert (!pTester->verifyCookie());
}


void RemotingTest::testAuthenticatedGoodCredentials()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::SOAP::Transport& trans = static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport());

	trans.setUsername("user");
	trans.setPassword("pass");
	trans.setAuthentication(Poco::RemotingNG::SOAP::Transport::AUTH_BASIC);

	_pListener->setAuthenticator(new MockAuthenticator);

	pTester->testAuthenticated();
}


void RemotingTest::testAuthenticatedBadCredentials()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::SOAP::Transport& trans = static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport());

	trans.setUsername("user");
	trans.setPassword("bad!");
	trans.setAuthentication(Poco::RemotingNG::SOAP::Transport::AUTH_BASIC);

	_pListener->setAuthenticator(new MockAuthenticator);

	try
	{
		pTester->testAuthenticated();
		fail("bad credentials - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assertMessageEqual ("Authentication failed", exc.message());
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
		assertMessageEqual ("Authentication failed", exc.message());
	}
}


void RemotingTest::testPermission()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::SOAP::Transport& trans = static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport());

	trans.setUsername("admin");
	trans.setPassword("s3cr3t");
	trans.setAuthentication(Poco::RemotingNG::SOAP::Transport::AUTH_BASIC);

	_pListener->setAuthenticator(new MockAuthenticator);
	_pListener->setAuthorizer(new MockAuthorizer);

	pTester->testPermission();
}


void RemotingTest::testNoPermission()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::SOAP::Transport& trans = static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport());

	trans.setUsername("user");
	trans.setPassword("pass");
	trans.setAuthentication(Poco::RemotingNG::SOAP::Transport::AUTH_BASIC);

	_pListener->setAuthenticator(new MockAuthenticator);
	_pListener->setAuthorizer(new MockAuthorizer);

	try
	{
		pTester->testPermission();
		fail("no permission - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assertMessageEqual ("No permission: perm1", exc.message());
	}
}


void RemotingTest::testWSSEInterceptor()
{
	using namespace std::string_literals;
	using Poco::RemotingNG::SOAP::Serializer;

	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::SOAP::Transport& trans = static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport());

	trans.setUsername("user");
	trans.setPassword("pass");
	trans.setAuthentication(Poco::RemotingNG::SOAP::Transport::AUTH_WSSE_DIGEST);

	class WSSEServerInterceptor: public Poco::RemotingNG::SOAP::Listener::RequestHeaderInterceptor
	{
	public:
		void interceptHeader(const Poco::RemotingNG::SOAP::XMLElement& header)
		{
			const Poco::RemotingNG::SOAP::XMLElement* pSecurityElem = header.findChild("Security"s, Serializer::WSSENAMESPACE);
			if (pSecurityElem)
			{
				_securityElem = *pSecurityElem;
			}
		}

		const Poco::RemotingNG::SOAP::XMLElement& securityElem() const
		{
			return _securityElem;
		}

	private:
		Poco::RemotingNG::SOAP::XMLElement _securityElem;
	};

	Poco::SharedPtr<WSSEServerInterceptor> pInterceptor = new WSSEServerInterceptor;
	_pListener->addRequestHeaderInterceptor(pInterceptor);

	pTester->testInt1(42);

	const Poco::RemotingNG::SOAP::XMLElement& securityElem = pInterceptor->securityElem();
	assert (securityElem.localName() == "Security"s);
	assert (securityElem.namespaceURI() == Serializer::WSSENAMESPACE);
	
	const Poco::RemotingNG::SOAP::XMLElement* pUsernameTokenElem = securityElem.findChild("UsernameToken", Serializer::WSSENAMESPACE);
	assertNotNull(pUsernameTokenElem);
	
	const Poco::RemotingNG::SOAP::XMLElement* pUsernameElem = pUsernameTokenElem->findChild("Username"s, Serializer::WSSENAMESPACE);
	assertNotNull(pUsernameElem);
	assert (pUsernameElem->value() == "user"s);
	
	const Poco::RemotingNG::SOAP::XMLElement* pPasswordElem = pUsernameTokenElem->findChild("Password"s, Serializer::WSSENAMESPACE);
	assertNotNull(pPasswordElem);

	const Poco::RemotingNG::SOAP::XMLElement* pNonceElem = pUsernameTokenElem->findChild("Nonce"s, Serializer::WSSENAMESPACE);
	assertNotNull(pNonceElem);

	const Poco::RemotingNG::SOAP::XMLElement* pCreatedElem = pUsernameTokenElem->findChild("Created"s, Serializer::WSUNAMESPACE);
	assertNotNull(pCreatedElem);
}


void RemotingTest::testWSSEAuthenticatorPlain()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::SOAP::Transport& trans = static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport());

	trans.setUsername("user");
	trans.setPassword("pass");
	trans.setAuthentication(Poco::RemotingNG::SOAP::Transport::AUTH_WSSE_TEXT);

	Poco::AutoPtr<MockWSSEAuthenticator> pAuth = new MockWSSEAuthenticator;
	_pListener->setAuthenticator(pAuth);
	_pListener->enableWSSE(true);

	pTester->testAuthenticated();

	assert (pAuth->lastUsername() == "user");
}


void RemotingTest::testWSSEAuthenticatorDigest()
{
	ITester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	Poco::RemotingNG::SOAP::Transport& trans = static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport());

	trans.setUsername("user");
	trans.setPassword("pass");
	trans.setAuthentication(Poco::RemotingNG::SOAP::Transport::AUTH_WSSE_DIGEST);

	Poco::AutoPtr<MockWSSEAuthenticator> pAuth = new MockWSSEAuthenticator;
	_pListener->setAuthenticator(pAuth);
	_pListener->enableWSSE(true);

	pTester->testAuthenticated();

	assert (pAuth->lastUsername() == "user");
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
	pProxy->remoting__connect("soap", uri);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).enableCompression(false);
	return pProxy;
}


RemotingTestSOAP11::RemotingTestSOAP11(const std::string& name): RemotingTest(name)
{
}


RemotingTestSOAP11::~RemotingTestSOAP11()
{
}


void RemotingTestSOAP11::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(false);
}


CppUnit::Test* RemotingTestSOAP11::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestSOAP11");

	CppUnit_addTest(pSuite, RemotingTestSOAP11, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testInt);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testNested);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testClass1);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testPtr);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testOneWay);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testFault);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testCookie);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testAuthenticatedGoodCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testAuthenticatedBadCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testAuthenticatedNoCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testPermission);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testNoPermission);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testWSSEInterceptor);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testWSSEAuthenticatorPlain);
	CppUnit_addTest(pSuite, RemotingTestSOAP11, testWSSEAuthenticatorDigest);

	return pSuite;
}


RemotingTestSOAP12::RemotingTestSOAP12(const std::string& name): RemotingTest(name)
{
}


RemotingTestSOAP12::~RemotingTestSOAP12()
{
}


void RemotingTestSOAP12::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(false);
}


ITester::Ptr RemotingTestSOAP12::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("soap-1.2", uri);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).enableCompression(false);
	return pProxy;
}


CppUnit::Test* RemotingTestSOAP12::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestSOAP12");

	CppUnit_addTest(pSuite, RemotingTestSOAP12, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testInt);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testNested);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testClass1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testPtr);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testOneWay);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testFault);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testCookie);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testAuthenticatedGoodCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testAuthenticatedBadCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testAuthenticatedNoCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testPermission);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testNoPermission);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testWSSEInterceptor);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testWSSEAuthenticatorPlain);
	CppUnit_addTest(pSuite, RemotingTestSOAP12, testWSSEAuthenticatorDigest);

	return pSuite;
}


RemotingTestSOAP12Compressed::RemotingTestSOAP12Compressed(const std::string& name): RemotingTest(name)
{
}


RemotingTestSOAP12Compressed::~RemotingTestSOAP12Compressed()
{
}


void RemotingTestSOAP12Compressed::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(true);
}


ITester::Ptr RemotingTestSOAP12Compressed::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("soap-1.2", uri);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).enableCompression(true);
	return pProxy;
}


CppUnit::Test* RemotingTestSOAP12Compressed::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestSOAP12Compressed");

	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testInt);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testNested);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testClass1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testPtr);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testOneWay);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testFault);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testCookie);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testAuthenticatedGoodCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testAuthenticatedBadCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testAuthenticatedNoCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testPermission);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testNoPermission);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testWSSEInterceptor);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testWSSEAuthenticatorPlain);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Compressed, testWSSEAuthenticatorDigest);

	return pSuite;
}



RemotingTestSOAP11NonChunked::RemotingTestSOAP11NonChunked(const std::string& name): RemotingTest(name)
{
}


RemotingTestSOAP11NonChunked::~RemotingTestSOAP11NonChunked()
{
}


ITester::Ptr RemotingTestSOAP11NonChunked::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("soap-1.1", uri);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).enableChunkedTransferEncoding(false);
	return pProxy;
}


void RemotingTestSOAP11NonChunked::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(false);
	_pListener->enableChunkedTransferEncoding(false);
}


CppUnit::Test* RemotingTestSOAP11NonChunked::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestSOAP11NonChunked");

	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testInt);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testNested);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testClass1);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testPtr);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testOneWay);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testFault);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testCookie);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testAuthenticatedGoodCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testAuthenticatedBadCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testAuthenticatedNoCredentials);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testPermission);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testNoPermission);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testWSSEInterceptor);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testWSSEAuthenticatorPlain);
	CppUnit_addTest(pSuite, RemotingTestSOAP11NonChunked, testWSSEAuthenticatorDigest);

	return pSuite;
}



RemotingTestSOAP12MTOM::RemotingTestSOAP12MTOM(const std::string& name): RemotingTest(name)
{
}


RemotingTestSOAP12MTOM::~RemotingTestSOAP12MTOM()
{
}


void RemotingTestSOAP12MTOM::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(false);
	_pListener->enableMTOM(true);
}


ITester::Ptr RemotingTestSOAP12MTOM::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("soap-1.2", uri);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).enableCompression(false);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).enableMTOM(true);
	return pProxy;
}


CppUnit::Test* RemotingTestSOAP12MTOM::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestSOAP12MTOM");

	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testInt);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testNested);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testClass1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testPtr);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testOneWay);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testFault);
	CppUnit_addTest(pSuite, RemotingTestSOAP12MTOM, testCookie);

	return pSuite;
}


RemotingTestSOAP12Auth::RemotingTestSOAP12Auth(const std::string& name): RemotingTest(name)
{
}


RemotingTestSOAP12Auth::~RemotingTestSOAP12Auth()
{
}


void RemotingTestSOAP12Auth::setUp()
{
	_pListener = new Poco::RemotingNG::SOAP::Listener("127.0.0.1:2283", Poco::RemotingNG::SOAP::Listener::PROTO_HTTP);
	_listener = Poco::RemotingNG::ORB::instance().registerListener(_pListener);
	Poco::RemotingNG::SOAP::TransportFactory::registerFactory();
	_objectURI = TesterServerHelper::registerObject(new Tester, "TheTester", _listener);

	Poco::Net::ServerSocket ss(2283);
	_pHTTPServer = new Poco::Net::HTTPServer(new SOAPAuthRequestHandlerFactory(*_pListener), ss, new Poco::Net::HTTPServerParams);
	_pHTTPServer->start();
}


void RemotingTestSOAP12Auth::tearDown()
{
	RemotingTest::tearDown();
	_pHTTPServer->stop();
	_pHTTPServer = 0;
}


void RemotingTestSOAP12Auth::testUnauthorized()
{
	ITester::Ptr pTester = createUnauthorizedProxy(_objectURI);

	try
	{
		testInt(pTester);
		fail("Unauthorized - must throw");
	}
	catch (Poco::Exception& exc)
	{
		assertMessageEqual ("HTTP Error: Unauthorized", exc.message());
	}
}


ITester::Ptr RemotingTestSOAP12Auth::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("soap-1.2", uri);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).enableChunkedTransferEncoding(false);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).setAuthentication(Poco::RemotingNG::SOAP::Transport::AUTH_ANY);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).setUsername("user");
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).setPassword("s3cr3t");
	return pProxy;
}


ITester::Ptr RemotingTestSOAP12Auth::createUnauthorizedProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("soap-1.2", uri);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).enableChunkedTransferEncoding(false);

	return pProxy;
}


CppUnit::Test* RemotingTestSOAP12Auth::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestSOAP12Auth");

	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testInt);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testNested);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testClass1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testPtr);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testFault);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testCookie);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testUnauthorized);

	return pSuite;
}


RemotingTestSOAP12AuthChunked::RemotingTestSOAP12AuthChunked(const std::string& name): RemotingTestSOAP12Auth(name)
{
}


RemotingTestSOAP12AuthChunked::~RemotingTestSOAP12AuthChunked()
{
}


ITester::Ptr RemotingTestSOAP12AuthChunked::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("soap-1.2", uri);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).enableChunkedTransferEncoding(true);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).setAuthentication(Poco::RemotingNG::SOAP::Transport::AUTH_BASIC);
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).setUsername("user");
	static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport()).setPassword("s3cr3t");
	return pProxy;
}


CppUnit::Test* RemotingTestSOAP12AuthChunked::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestSOAP12AuthChunked");

	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testInt);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testEnum1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testEnum2);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testNested);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct2);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct3);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct4);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct5);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testClass1);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testPtr);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testFault);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testCookie);
	CppUnit_addTest(pSuite, RemotingTestSOAP12Auth, testUnauthorized);

	return pSuite;
}
