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
#include "Poco/Delegate.h"
#include <string>


using Poco::Net::Context;
using Poco::Net::SSLManager;
using Poco::Util::Application;


namespace
{
	void supplyNonASCIIPassword(const void*, std::string& password)
		/// Sets the password of nonascii.pfx: 40 times U+00E4, which is
		/// 80 bytes in UTF-8 but 40 UTF-16 code units.
	{
		password.clear();
		for (int i = 0; i < 40; ++i) password += "\xC3\xA4";
	}
}


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


void ContextTest::testImportPKCS12FileWithNonASCIIPassword()
{
	const std::string path = Application::instance().config().getString("application.dir") + "nonascii.pfx";

	// Registered after the handlers of the configuration, so this password is the one used.
	SSLManager::instance().PrivateKeyPassphraseRequired += Poco::delegate(&supplyNonASCIIPassword);
	Context::Ptr pContext;
	try
	{
		pContext = new Context(Context::SERVER_USE, path, Context::VERIFY_NONE, Context::OPT_LOAD_CERT_FROM_FILE);
		const Poco::Net::X509Certificate cert = pContext->certificate();
		assertEqual ("localhost", cert.commonName());
	}
	catch (...)
	{
		SSLManager::instance().PrivateKeyPassphraseRequired -= Poco::delegate(&supplyNonASCIIPassword);
		throw;
	}
	SSLManager::instance().PrivateKeyPassphraseRequired -= Poco::delegate(&supplyNonASCIIPassword);
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
	CppUnit_addTest(pSuite, ContextTest, testImportPKCS12FileWithNonASCIIPassword);

	return pSuite;
}
