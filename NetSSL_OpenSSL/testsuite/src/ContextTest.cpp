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
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/x509_vfy.h>
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
#include <openssl/core_names.h>
#endif
#include <memory>
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
	// in FIPS mode the DH parameters are ignored
	if (SSLManager::isFIPSEnabled()) return;

	Context::Params params(Context::KEY_DH_GROUP_1024);
	params.securityLevel = Context::SECURITY_LEVEL_112_BITS;
	assertRejected(params);
}


#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
void ContextTest::testBuiltInDHParameters()
{
	// in FIPS mode the DH parameters are ignored
	if (SSLManager::isFIPSEnabled()) return;

	assertEqual (std::string("dh_2048_256"), negotiatedDHGroup(Context::KEY_DH_GROUP_2048));
	assertEqual (std::string("dh_1024_160"), negotiatedDHGroup(Context::KEY_DH_GROUP_1024));
}
#endif


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


void ContextTest::testClientContextIgnoresDHParameters()
{
	Context::Params params(Context::KEY_DH_GROUP_1024);
	params.securityLevel = Context::SECURITY_LEVEL_112_BITS;
	Context::Ptr pContext = new Context(Context::TLS_CLIENT_USE, params);
}


void ContextTest::testVerifyErrorWithoutCertificate()
{
	ErrorQueueCleaner cleaner;

	// OpenSSL reports some verification errors, for example a missing explicit
	// policy, without a current certificate. The callback must reject them.
	Context::Ptr pContext = new Context(Context::TLS_CLIENT_USE, Context::Params());
	std::unique_ptr<SSL, decltype(&SSL_free)> pSSL(SSL_new(pContext->sslContext()), &SSL_free);
	assertNotNullPtr (pSSL.get());
	std::unique_ptr<X509_STORE_CTX, decltype(&X509_STORE_CTX_free)> pStore(X509_STORE_CTX_new(), &X509_STORE_CTX_free);
	assertNotNullPtr (pStore.get());
	assertTrue (X509_STORE_CTX_init(pStore.get(), SSL_CTX_get_cert_store(pContext->sslContext()), nullptr, nullptr) == 1);
	assertTrue (X509_STORE_CTX_set_ex_data(pStore.get(), SSL_get_ex_data_X509_STORE_CTX_idx(), pSSL.get()) == 1);
	X509_STORE_CTX_set_error(pStore.get(), X509_V_ERR_NO_EXPLICIT_POLICY);
	X509_STORE_CTX_set_current_cert(pStore.get(), nullptr);

	SSL_verify_cb verify = SSL_CTX_get_verify_callback(pContext->sslContext());
	assertTrue (verify != nullptr);
	assertTrue (verify(0, pStore.get()) == 0);
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


#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
std::string ContextTest::negotiatedDHGroup(Context::KeyDHGroup dhGroup)
{
	// neither a failed handshake nor an unnamed group may leave its errors to the following tests
	ErrorQueueCleaner cleaner;

	Context::Params serverParams(dhGroup);
	serverParams.privateKeyFile = Application::instance().config().getString("openSSL.server.privateKeyFile");
	serverParams.verificationMode = Context::VERIFY_NONE;
	Context::Ptr pServerContext = new Context(Context::TLS_SERVER_USE, serverParams);

	Context::Params clientParams;
	clientParams.verificationMode = Context::VERIFY_NONE;
	clientParams.cipherList = "DHE-RSA-AES256-GCM-SHA384";
	// without ffdhe groups the server has to send its own DH parameters (OpenSSL 4.0 prefers RFC 7919 groups)
	clientParams.ecdhCurve = "X25519:P-256";
	Context::Ptr pClientContext = new Context(Context::TLS_CLIENT_USE, clientParams);

	using SSLPtr = std::unique_ptr<SSL, decltype(&SSL_free)>;
	SSLPtr pServer(SSL_new(pServerContext->sslContext()), &SSL_free);
	SSLPtr pClient(SSL_new(pClientContext->sslContext()), &SSL_free);
	assertNotNullPtr (pServer.get());
	assertNotNullPtr (pClient.get());
	// DHE cipher suites exist up to TLS 1.2 only
	assertTrue (SSL_set_max_proto_version(pClient.get(), TLS1_2_VERSION) == 1);

	BIO* pServerBIO = nullptr;
	BIO* pClientBIO = nullptr;
	assertTrue (BIO_new_bio_pair(&pServerBIO, 0, &pClientBIO, 0) == 1);
	SSL_set_bio(pServer.get(), pServerBIO, pServerBIO);
	SSL_set_bio(pClient.get(), pClientBIO, pClientBIO);
	SSL_set_accept_state(pServer.get());
	SSL_set_connect_state(pClient.get());

	bool clientDone = false;
	bool serverDone = false;
	for (int i = 0; i < 10 && !(clientDone && serverDone); ++i)
	{
		if (!clientDone) clientDone = SSL_do_handshake(pClient.get()) == 1;
		if (!serverDone) serverDone = SSL_do_handshake(pServer.get()) == 1;
	}
	assertTrue (clientDone && serverDone);

	EVP_PKEY* pKey = nullptr;
	assertTrue (SSL_get_peer_tmp_key(pClient.get(), &pKey) == 1);
	std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> pServerDHKey(pKey, &EVP_PKEY_free);

	// OpenSSL names the group only if p and g are those of a group it knows
	char groupName[32] = "";
	EVP_PKEY_get_utf8_string_param(pServerDHKey.get(), OSSL_PKEY_PARAM_GROUP_NAME, groupName, sizeof(groupName), nullptr);
	return groupName;
}
#endif


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
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	CppUnit_addTest(pSuite, ContextTest, testBuiltInDHParameters);
#endif
	CppUnit_addTest(pSuite, ContextTest, testAddChainCertificateWithoutX509);
	CppUnit_addTest(pSuite, ContextTest, testClientContextIgnoresDHParameters);
	CppUnit_addTest(pSuite, ContextTest, testVerifyErrorWithoutCertificate);

	return pSuite;
}
