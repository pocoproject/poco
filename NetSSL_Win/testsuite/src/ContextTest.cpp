//
// ContextTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ContextTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/X509Certificate.h"
#include "Poco/Util/Application.h"
#include <string>


using Poco::Net::Context;
using Poco::Net::SSLManager;
using Poco::Util::Application;


ContextTest::ContextTest(const std::string& name): CppUnit::TestCase(name)
{
}


ContextTest::~ContextTest() = default;


void ContextTest::testImportPKCS12FileWithChain()
{
	// The file holds the key and certificate of the server with the intermediate
	// and root CA certificates, so it is larger than a single certificate with its key.
	const std::string path = Application::instance().config().getString("application.dir") + "chain.pfx";

	// The handler of the server configuration supplies the password of the file.
	const SSLManager::PrivateKeyPassphraseHandlerPtr pPassphraseHandler = SSLManager::instance().serverPassphraseHandler();
	assertFalse (pPassphraseHandler.isNull());
	Context::Ptr pContext = new Context(Context::SERVER_USE, path, Context::VERIFY_NONE, Context::OPT_LOAD_CERT_FROM_FILE);

	const Poco::Net::X509Certificate cert = pContext->certificate();
	assertEqual ("localhost", cert.commonName());
	assertEqual ("Poco Test Intermediate CA", cert.issuerName(Poco::Net::X509Certificate::NID_COMMON_NAME));
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

	CppUnit_addTest(pSuite, ContextTest, testImportPKCS12FileWithChain);

	return pSuite;
}
