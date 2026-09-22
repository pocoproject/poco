//
// ContextTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ContextTest.h"
#include "ErrorQueueCleaner.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Util/Application.h"
#include "Poco/Exception.h"
#include <openssl/err.h>
#include <string>
#include <utility>


using Poco::Crypto::X509Certificate;
using Poco::Net::Context;
using Poco::Net::SSLContextException;
using Poco::Net::SSLManager;
using Poco::Util::Application;


ContextTest::ContextTest(const std::string& name): CppUnit::TestCase(name)
{
}


ContextTest::~ContextTest()
{
}


void ContextTest::testValidCipherConfiguration()
{
	Context::Params params;	// default cipher list
	Context::Ptr pDefault = new Context(Context::TLS_SERVER_USE, params);

	params.cipherList = "ALL";
	params.cipherSuites = "TLS_AES_256_GCM_SHA384";
	Context::Ptr pClient = new Context(Context::TLS_CLIENT_USE, params);

	params.cipherList = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH";
	params.cipherSuites.clear();
	params.securityLevel = Context::SECURITY_LEVEL_112_BITS;
	Context::Ptr pLevel2 = new Context(Context::TLS_SERVER_USE, params);
}


void ContextTest::testInvalidCipherList()
{
	Context::Params params;
	params.cipherList = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGHT";	// unknown command
	assertRejected(params);
	params.cipherList = "\"ALL\"";	// invalid character
	assertRejected(params);
}


void ContextTest::testCipherListWithoutMatch()
{
	ErrorQueueCleaner cleaner;

	// accepted: such a list leaves the TLS 1.3 cipher suites only
	const std::string lists[] = {"", "TLS_AES_256_GCM_SHA384", "NOSUCHCIPHER"};
	for (const auto& list: lists)
	{
		Context::Params params;
		params.cipherList = list;
		ERR_clear_error();

		Context::Ptr pContext = new Context(Context::TLS_SERVER_USE, params);
		assertTrue (ERR_peek_error() == 0);
	}
}


void ContextTest::testInvalidCipherSuites()
{
	Context::Params params;
	params.cipherSuites = "TLS_AES_256_GCM_SHA385";
	assertRejected(params);
}


void ContextTest::testInvalidECDHGroups()
{
	Context::Params params;
	params.ecdhCurve = "P-256//X25519";
	assertRejected(params);
}


void ContextTest::testDHParametersRejectedBySecurityLevel()
{
	// in FIPS mode named ffdhe groups are used instead of the built-in parameters
	if (SSLManager::isFIPSEnabled()) return;

	Context::Params params(Context::KEY_DH_GROUP_1024);
	params.securityLevel = Context::SECURITY_LEVEL_112_BITS;
	assertRejected(params);
}


void ContextTest::testAddChainCertificateWithoutX509()
{
	ErrorQueueCleaner cleaner;

	X509Certificate certificate(Application::instance().config().getString("openSSL.server.privateKeyFile"));
	X509Certificate owner(std::move(certificate));

	Context::Params params;
	Context::Ptr pContext = new Context(Context::TLS_SERVER_USE, params);
	try
	{
		// the moved-from certificate has no X509 object
		pContext->addChainCertificate(certificate);
		fail("chain certificate without an X509 object - must throw");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}
}


void ContextTest::assertRejected(const Context::Params& params)
{
	ErrorQueueCleaner cleaner;

	ERR_clear_error();
	try
	{
		Context::Ptr pContext = new Context(Context::TLS_SERVER_USE, params);
		fail("rejected by OpenSSL - must throw");
	}
	catch (SSLContextException&)
	{
	}
	// the exception has consumed the OpenSSL errors
	assertTrue (ERR_peek_error() == 0);
}


void ContextTest::setUp()
{
}


void ContextTest::tearDown()
{
}


CppUnit::Test* ContextTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ContextTest");

	CppUnit_addTest(pSuite, ContextTest, testValidCipherConfiguration);
	CppUnit_addTest(pSuite, ContextTest, testInvalidCipherList);
	CppUnit_addTest(pSuite, ContextTest, testCipherListWithoutMatch);
	CppUnit_addTest(pSuite, ContextTest, testInvalidCipherSuites);
	CppUnit_addTest(pSuite, ContextTest, testInvalidECDHGroups);
	CppUnit_addTest(pSuite, ContextTest, testDHParametersRejectedBySecurityLevel);
	CppUnit_addTest(pSuite, ContextTest, testAddChainCertificateWithoutX509);

	return pSuite;
}
