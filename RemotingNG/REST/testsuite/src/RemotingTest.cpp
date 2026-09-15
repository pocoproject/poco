//
// RemotingTest.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "RemotingTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/Transport.h"
#include "Poco/RemotingNG/TransportFactory.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/RemotingNG/Skeleton.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/Context.h"
#include "Poco/RemotingNG/REST/Transport.h"
#include "Poco/RemotingNG/REST/TransportFactory.h"
#include "Poco/RemotingNG/REST/Listener.h"
#include "Poco/RemotingNG/REST/RESTRequestHandler.h"
#include "Poco/RemotingNG/REST/RESTServerException.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Tester.h"
#include "TesterServerHelper.h"
#include "TesterClientHelper.h"
#include "TesterRemoteObject.h"
#include "TesterProxy.h"
#include "AuthTesterServerHelper.h"
#include "AuthTesterClientHelper.h"
#include "AuthTesterRemoteObject.h"
#include "AuthTesterProxy.h"


namespace
{
	class NoAuthRequestHandler: public Poco::Net::HTTPRequestHandler
	{
	public:
		void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
		{
			Poco::NullOutputStream nullStream;
			Poco::StreamCopier::copyStream(request.stream(), nullStream);
			response.requireAuthentication("REST");
			response.setContentLength(0);
			response.send();
		}
	};


	class RESTAuthRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		RESTAuthRequestHandlerFactory(Poco::RemotingNG::REST::Listener& listener):
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
					return new Poco::RemotingNG::REST::RESTRequestHandler(_listener);
				}
			}
			return new NoAuthRequestHandler;
		}

	private:
		Poco::RemotingNG::REST::Listener& _listener;
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


	class MockOAuthenticator: public Poco::RemotingNG::Authenticator
	{
	public:
		typedef Poco::AutoPtr<MockOAuthenticator> Ptr;

		Poco::RemotingNG::AuthenticateResult authenticate(const Poco::RemotingNG::Credentials& creds, Poco::UInt32)
		{
			std::string token = creds.getAttribute(Poco::RemotingNG::Credentials::ATTR_TOKEN);
			if (token == "t0k3n")
				return Poco::RemotingNG::AuthenticateResult(Poco::RemotingNG::AuthenticateResult::AUTH_DONE, creds);
			else
				return Poco::RemotingNG::AuthenticateResult(Poco::RemotingNG::AuthenticateResult::AUTH_FAILED);
		}
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


RemotingTest::RemotingTest(const std::string& name): CppUnit::TestCase(name)
{
}


RemotingTest::~RemotingTest()
{
}


void RemotingTest::setUp()
{
	_publicURI = "http://127.0.0.1:2280/tester/{id}";
	_pListener = new Poco::RemotingNG::REST::Listener("127.0.0.1:2280");
	_pListener->enableChunkedTransferEncoding(false);
	_listener = Poco::RemotingNG::ORB::instance().registerListener(_pListener);
	Poco::RemotingNG::REST::TransportFactory::registerFactory();
	Poco::SharedPtr<Tester> pTester = new Tester;
	Poco::AutoPtr<TesterRemoteObject> pTesterRemoteObject = TesterServerHelper::createRemoteObject(pTester, "TheTester");
	pTesterRemoteObject->remoting__setURI(Poco::URI(_publicURI));
	_objectURI = TesterServerHelper::registerRemoteObject(pTesterRemoteObject, _listener);
}


void RemotingTest::tearDown()
{
	TesterServerHelper::unregisterObject(_objectURI);
	Poco::RemotingNG::REST::TransportFactory::unregisterFactory();
	Poco::RemotingNG::ORB::instance().unregisterListener(_listener);
	_pListener = 0;
}


void RemotingTest::testRegistration()
{
	assert (_objectURI == "http://127.0.0.1:2280/rest/Tester/TheTester");
}


void RemotingTest::testPost()
{
	ITester::Ptr pTester = createProxy(_publicURI);

	Object object;
	object.data = "Object 1";
	Object result = pTester->post(1, object);
	assert (result.data == object.data);
}


void RemotingTest::testPut()
{
	ITester::Ptr pTester = createProxy(_publicURI);

	Object object;
	object.data = "Object 1";
	Object result = pTester->put(1, object);
	assert (result.data == object.data);
}


void RemotingTest::testGet()
{
	ITester::Ptr pTester = createProxy(_publicURI);

	Object object;
	object.data = "Object 1";
	Object result = pTester->put(1, object);
	assert (result.data == object.data);

	Object result2 = pTester->get(1);
	assert (result2.data == object.data);
}


void RemotingTest::testGetNotFound()
{
	ITester::Ptr pTester = createProxy(_publicURI);

	try
	{
		Object result2 = pTester->get(1);
		fail("object not found - must throw");
	}
	catch (Poco::RemotingNG::REST::RESTServerException& exc)
	{
		assert (exc.displayText() == "REST Server Error: Not Found: Not found");
		assert (exc.code() == 404);

		assert (exc.reason() == "Not Found");
		assert (exc.status() == 404);
		assert (exc.error() == "Not found");
		assert (exc.detail() == "");
	}
	catch (Poco::RemotingNG::RemoteException&)
	{
		fail("must throw RESTServerException");
	}
}


void RemotingTest::testCookie()
{
	ITester::Ptr pTester = createProxy(_publicURI);

	Object object;
	pTester->put(Tester::ID_SET_COOKIE, object);

	Object result = pTester->put(Tester::ID_VERIFY_COOKIE, object);
	assert (result.data == "OK");

	Poco::Thread::sleep(6000);
	result = pTester->put(Tester::ID_VERIFY_COOKIE, object);
	assert (result.data == "FAIL");
}


void RemotingTest::testFault()
{
	ITester::Ptr pTester = createProxy(_publicURI);

	try
	{
		Object object;
		pTester->put(Tester::ID_FAULT, object);
		fail("must throw");
	}
	catch (Poco::RemotingNG::REST::RESTServerException& exc)
	{
		assert (exc.displayText() == "REST Server Error: Internal Server Error: Application exception: Something went wrong");

		assert (exc.reason() == "Internal Server Error");
		assert (exc.status() == 500);
		assert (exc.error() == "Application exception");
		assert (exc.detail() == "Something went wrong");
	}
	catch (Poco::RemotingNG::RemoteException&)
	{
		fail("must throw RESTException");
	}
}


ITester::Ptr RemotingTest::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("rest", uri);
	static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport()).enableCompression(false);
	return pProxy;
}


RemotingTestREST::RemotingTestREST(const std::string& name): RemotingTest(name)
{
}


RemotingTestREST::~RemotingTestREST()
{
}


void RemotingTestREST::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(false);
}


CppUnit::Test* RemotingTestREST::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestREST");

	CppUnit_addTest(pSuite, RemotingTestREST, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestREST, testPost);
	CppUnit_addTest(pSuite, RemotingTestREST, testPut);
	CppUnit_addTest(pSuite, RemotingTestREST, testGet);
	CppUnit_addTest(pSuite, RemotingTestREST, testGetNotFound);
	CppUnit_addTest(pSuite, RemotingTestREST, testCookie);
	CppUnit_addTest(pSuite, RemotingTestREST, testFault);

	return pSuite;
}


RemotingTestRESTCompressed::RemotingTestRESTCompressed(const std::string& name): RemotingTest(name)
{
}


RemotingTestRESTCompressed::~RemotingTestRESTCompressed()
{
}


void RemotingTestRESTCompressed::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(true);
	_pListener->enableChunkedTransferEncoding(false);
}


ITester::Ptr RemotingTestRESTCompressed::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("rest", uri);
	static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport()).enableCompression(true);
	return pProxy;
}


CppUnit::Test* RemotingTestRESTCompressed::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestRESTCompressed");

	CppUnit_addTest(pSuite, RemotingTestRESTCompressed, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestRESTCompressed, testPost);
	CppUnit_addTest(pSuite, RemotingTestRESTCompressed, testPut);
	CppUnit_addTest(pSuite, RemotingTestRESTCompressed, testGet);
	CppUnit_addTest(pSuite, RemotingTestRESTCompressed, testGetNotFound);
	CppUnit_addTest(pSuite, RemotingTestRESTCompressed, testCookie);
	CppUnit_addTest(pSuite, RemotingTestRESTCompressed, testFault);

	return pSuite;
}


RemotingTestRESTChunked::RemotingTestRESTChunked(const std::string& name): RemotingTest(name)
{
}


RemotingTestRESTChunked::~RemotingTestRESTChunked()
{
}


void RemotingTestRESTChunked::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(false);
	_pListener->enableChunkedTransferEncoding(true);
}


ITester::Ptr RemotingTestRESTChunked::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("rest", uri);
	static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport()).enableCompression(true);
	return pProxy;
}


CppUnit::Test* RemotingTestRESTChunked::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestRESTChunked");

	CppUnit_addTest(pSuite, RemotingTestRESTChunked, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestRESTChunked, testPost);
	CppUnit_addTest(pSuite, RemotingTestRESTChunked, testPut);
	CppUnit_addTest(pSuite, RemotingTestRESTChunked, testGet);
	CppUnit_addTest(pSuite, RemotingTestRESTChunked, testGetNotFound);
	CppUnit_addTest(pSuite, RemotingTestRESTChunked, testCookie);
	CppUnit_addTest(pSuite, RemotingTestRESTChunked, testFault);

	return pSuite;
}


RemotingTestRESTChunkedCompressed::RemotingTestRESTChunkedCompressed(const std::string& name): RemotingTest(name)
{
}


RemotingTestRESTChunkedCompressed::~RemotingTestRESTChunkedCompressed()
{
}


void RemotingTestRESTChunkedCompressed::setUp()
{
	RemotingTest::setUp();
	_pListener->enableCompression(true);
	_pListener->enableChunkedTransferEncoding(true);
}


ITester::Ptr RemotingTestRESTChunkedCompressed::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("rest", uri);
	static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport()).enableCompression(true);
	return pProxy;
}


CppUnit::Test* RemotingTestRESTChunkedCompressed::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestRESTChunkedCompressed");

	CppUnit_addTest(pSuite, RemotingTestRESTChunkedCompressed, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestRESTChunkedCompressed, testPost);
	CppUnit_addTest(pSuite, RemotingTestRESTChunkedCompressed, testPut);
	CppUnit_addTest(pSuite, RemotingTestRESTChunkedCompressed, testGet);
	CppUnit_addTest(pSuite, RemotingTestRESTChunkedCompressed, testGetNotFound);
	CppUnit_addTest(pSuite, RemotingTestRESTChunkedCompressed, testCookie);
	CppUnit_addTest(pSuite, RemotingTestRESTChunkedCompressed, testFault);

	return pSuite;
}


RemotingTestRESTAuth::RemotingTestRESTAuth(const std::string& name): RemotingTest(name)
{
}


RemotingTestRESTAuth::~RemotingTestRESTAuth()
{
}


void RemotingTestRESTAuth::setUp()
{
	_publicURI = "http://127.0.0.1:2280/tester/{id}";
	_pListener = new Poco::RemotingNG::REST::Listener("127.0.0.1:2280", Poco::RemotingNG::REST::Listener::PROTO_HTTP);
	_listener = Poco::RemotingNG::ORB::instance().registerListener(_pListener);
	Poco::RemotingNG::REST::TransportFactory::registerFactory();
	Poco::SharedPtr<Tester> pTester = new Tester;
	Poco::AutoPtr<TesterRemoteObject> pTesterRemoteObject = TesterServerHelper::createRemoteObject(pTester, "TheTester");
	pTesterRemoteObject->remoting__setURI(Poco::URI(_publicURI));
	_objectURI = TesterServerHelper::registerRemoteObject(pTesterRemoteObject, _listener);

	Poco::Net::ServerSocket ss(2280);
	_pHTTPServer = new Poco::Net::HTTPServer(new RESTAuthRequestHandlerFactory(*_pListener), ss, new Poco::Net::HTTPServerParams);
	_pHTTPServer->start();
}


void RemotingTestRESTAuth::tearDown()
{
	RemotingTest::tearDown();
	_pHTTPServer->stop();
	_pHTTPServer = 0;
}


void RemotingTestRESTAuth::testUnauthorized()
{
	ITester::Ptr pTester = createUnauthorizedProxy(_publicURI);

	try
	{
		pTester->get(0);
		fail("Unauthorized - must throw");
	}
	catch (Poco::Exception& exc)
	{
		assert (exc.message() == "HTTP Error: Unauthorized");
		assert (exc.code() == 401);
	}
}


ITester::Ptr RemotingTestRESTAuth::createProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("rest", uri);
	static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport()).setAuthentication(Poco::RemotingNG::REST::Transport::AUTH_ANY);
	static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport()).setUsername("user");
	static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport()).setPassword("s3cr3t");
	return pProxy;
}


ITester::Ptr RemotingTestRESTAuth::createUnauthorizedProxy(const std::string& uri)
{
	Poco::AutoPtr<TesterProxy> pProxy = new TesterProxy("TheTester");
	pProxy->remoting__connect("rest", uri);

	return pProxy;
}


CppUnit::Test* RemotingTestRESTAuth::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestRESTAuth");

	CppUnit_addTest(pSuite, RemotingTestRESTAuth, testRegistration);
	CppUnit_addTest(pSuite, RemotingTestRESTAuth, testPost);
	CppUnit_addTest(pSuite, RemotingTestRESTAuth, testPut);
	CppUnit_addTest(pSuite, RemotingTestRESTAuth, testGet);
	CppUnit_addTest(pSuite, RemotingTestRESTAuth, testGetNotFound);
	CppUnit_addTest(pSuite, RemotingTestRESTAuth, testCookie);
	CppUnit_addTest(pSuite, RemotingTestRESTAuth, testFault);
	CppUnit_addTest(pSuite, RemotingTestRESTAuth, testUnauthorized);

	return pSuite;
}


RemotingTestAuth::RemotingTestAuth(const std::string& name): CppUnit::TestCase(name)
{
}


RemotingTestAuth::~RemotingTestAuth()
{
}


void RemotingTestAuth::setUp()
{
	_publicURI = "http://127.0.0.1:2280/authtester/{id}";
	_pListener = new Poco::RemotingNG::REST::Listener("127.0.0.1:2280");
	_pListener->enableChunkedTransferEncoding(false);
	_listener = Poco::RemotingNG::ORB::instance().registerListener(_pListener);
	Poco::RemotingNG::REST::TransportFactory::registerFactory();
	Poco::SharedPtr<AuthTester> pAuthTester = new AuthTester;
	Poco::AutoPtr<AuthTesterRemoteObject> pAuthTesterRemoteObject = AuthTesterServerHelper::createRemoteObject(pAuthTester, "TheAuthTester");
	pAuthTesterRemoteObject->remoting__setURI(Poco::URI(_publicURI));
	_objectURI = AuthTesterServerHelper::registerRemoteObject(pAuthTesterRemoteObject, _listener);
}


void RemotingTestAuth::tearDown()
{
	AuthTesterServerHelper::unregisterObject(_objectURI);
	Poco::RemotingNG::REST::TransportFactory::unregisterFactory();
	Poco::RemotingNG::ORB::instance().unregisterListener(_listener);
	_pListener = 0;
}


IAuthTester::Ptr RemotingTestAuth::createProxy(const std::string& uri)
{
	Poco::AutoPtr<AuthTesterProxy> pProxy = new AuthTesterProxy("TheAuthTester");
	pProxy->remoting__connect("rest", uri);
	return pProxy;
}


void RemotingTestAuth::testAuthenticatedGoodCredentials()
{
	IAuthTester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<AuthTesterProxy> pProxy = pTester.cast<AuthTesterProxy>();
	Poco::RemotingNG::REST::Transport& trans = static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport());

	trans.setUsername("user");
	trans.setPassword("pass");
	trans.setAuthentication(Poco::RemotingNG::REST::Transport::AUTH_BASIC);

	_pListener->setAuthenticator(new MockAuthenticator);

	pTester->get(1);
}


void RemotingTestAuth::testAuthenticatedNoCredentials()
{
	IAuthTester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<AuthTesterProxy> pProxy = pTester.cast<AuthTesterProxy>();

	_pListener->setAuthenticator(new MockAuthenticator);

	try
	{
		pTester->get(1);
		fail("no credentials - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.message() == "Unauthorized: Authentication failed");
	}
}


void RemotingTestAuth::testAuthenticatedBadCredentials()
{
	IAuthTester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<AuthTesterProxy> pProxy = pTester.cast<AuthTesterProxy>();
	Poco::RemotingNG::REST::Transport& trans = static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport());

	trans.setUsername("user");
	trans.setPassword("bAd!");
	trans.setAuthentication(Poco::RemotingNG::REST::Transport::AUTH_BASIC);

	_pListener->setAuthenticator(new MockAuthenticator);

	try
	{
		pTester->get(1);
		fail("bad credentials - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.message() == "Unauthorized: Authentication failed");
	}
}


void RemotingTestAuth::testAuthenticatedGoodOAuthCredentials()
{
	IAuthTester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<AuthTesterProxy> pProxy = pTester.cast<AuthTesterProxy>();
	Poco::RemotingNG::REST::Transport& trans = static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_TOKEN, "t0k3n");
	trans.setCredentials(creds);
	trans.setAuthentication(Poco::RemotingNG::REST::Transport::AUTH_BEARER);

	_pListener->setAuthenticator(new MockOAuthenticator);

	pTester->get(1);
}


void RemotingTestAuth::testAuthenticatedBadOAuthCredentials()
{
	IAuthTester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<AuthTesterProxy> pProxy = pTester.cast<AuthTesterProxy>();
	Poco::RemotingNG::REST::Transport& trans = static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport());

	Poco::RemotingNG::Credentials creds;
	creds.setAttribute(Poco::RemotingNG::Credentials::ATTR_TOKEN, "b@dt0k3n");
	trans.setCredentials(creds);
	trans.setAuthentication(Poco::RemotingNG::REST::Transport::AUTH_BEARER);

	_pListener->setAuthenticator(new MockOAuthenticator);

	try
	{
		pTester->get(1);
		fail("bad credentials - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.message() == "Unauthorized: Authentication failed");
	}
}


void RemotingTestAuth::testPermission()
{
	IAuthTester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<AuthTesterProxy> pProxy = pTester.cast<AuthTesterProxy>();
	Poco::RemotingNG::REST::Transport& trans = static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport());

	trans.setUsername("admin");
	trans.setPassword("s3cr3t");
	trans.setAuthentication(Poco::RemotingNG::REST::Transport::AUTH_BASIC);

	_pListener->setAuthenticator(new MockAuthenticator);
	_pListener->setAuthorizer(new MockAuthorizer);

	Object object;
	object.data = "Object 1";
	Object result = pTester->post(1, object);
	assert (result.data == object.data);
}


void RemotingTestAuth::testNoPermission()
{
	IAuthTester::Ptr pTester = createProxy(_objectURI);

	Poco::AutoPtr<AuthTesterProxy> pProxy = pTester.cast<AuthTesterProxy>();
	Poco::RemotingNG::REST::Transport& trans = static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport());

	trans.setUsername("user");
	trans.setPassword("pass");
	trans.setAuthentication(Poco::RemotingNG::REST::Transport::AUTH_BASIC);

	_pListener->setAuthenticator(new MockAuthenticator);
	_pListener->setAuthorizer(new MockAuthorizer);

	try
	{
		Object object;
		object.data = "Object 1";
		Object result = pTester->post(1, object);
		fail("no permission - must throw");
	}
	catch (Poco::RemotingNG::RemoteException& exc)
	{
		assert (exc.message() == "Forbidden: No permission: perm1");
	}
}


CppUnit::Test* RemotingTestAuth::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTestREST");

	CppUnit_addTest(pSuite, RemotingTestAuth, testAuthenticatedGoodCredentials);
	CppUnit_addTest(pSuite, RemotingTestAuth, testAuthenticatedNoCredentials);
	CppUnit_addTest(pSuite, RemotingTestAuth, testAuthenticatedBadCredentials);
	CppUnit_addTest(pSuite, RemotingTestAuth, testAuthenticatedGoodOAuthCredentials);
	CppUnit_addTest(pSuite, RemotingTestAuth, testAuthenticatedBadOAuthCredentials);
	CppUnit_addTest(pSuite, RemotingTestAuth, testPermission);
	CppUnit_addTest(pSuite, RemotingTestAuth, testNoPermission);

	return pSuite;
}


