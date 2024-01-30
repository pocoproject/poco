//
// SSLManager.cpp
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  SSLManager
//
// Copyright (c) 2006-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#if defined(_MSC_VER)
#pragma warning(disable:4996) // deprecation warnings
#endif


#include "Poco/Net/SSLManager.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/Utility.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/RejectCertificateHandler.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Delegate.h"
#include "Poco/StringTokenizer.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/OptionException.h"
#if OPENSSL_VERSION_NUMBER >= 0x10001000L
#include <openssl/ocsp.h>
#include <openssl/tls1.h>
#endif


namespace Poco {
namespace Net {


const std::string SSLManager::CFG_PRIV_KEY_FILE("privateKeyFile");
const std::string SSLManager::CFG_CERTIFICATE_FILE("certificateFile");
const std::string SSLManager::CFG_CA_LOCATION("caConfig");
const std::string SSLManager::CFG_VER_MODE("verificationMode");
const Context::VerificationMode SSLManager::VAL_VER_MODE(Context::VERIFY_RELAXED);
const std::string SSLManager::CFG_VER_DEPTH("verificationDepth");
const int         SSLManager::VAL_VER_DEPTH(9);
const std::string SSLManager::CFG_ENABLE_DEFAULT_CA("loadDefaultCAFile");
const bool        SSLManager::VAL_ENABLE_DEFAULT_CA(true);
const std::string SSLManager::CFG_CIPHER_LIST("cipherList");
const std::string SSLManager::CFG_CYPHER_LIST("cypherList");
const std::string SSLManager::VAL_CIPHER_LIST("ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
const std::string SSLManager::CFG_PREFER_SERVER_CIPHERS("preferServerCiphers");
const std::string SSLManager::CFG_DELEGATE_HANDLER("privateKeyPassphraseHandler.name");
const std::string SSLManager::VAL_DELEGATE_HANDLER("KeyConsoleHandler");
const std::string SSLManager::CFG_CERTIFICATE_HANDLER("invalidCertificateHandler.name");
const std::string SSLManager::VAL_CERTIFICATE_HANDLER("ConsoleCertificateHandler");
const std::string SSLManager::CFG_SERVER_PREFIX("openSSL.server.");
const std::string SSLManager::CFG_CLIENT_PREFIX("openSSL.client.");
const std::string SSLManager::CFG_CACHE_SESSIONS("cacheSessions");
const std::string SSLManager::CFG_SESSION_ID_CONTEXT("sessionIdContext");
const std::string SSLManager::CFG_SESSION_CACHE_SIZE("sessionCacheSize");
const std::string SSLManager::CFG_SESSION_TIMEOUT("sessionTimeout");
const std::string SSLManager::CFG_EXTENDED_VERIFICATION("extendedVerification");
const std::string SSLManager::CFG_REQUIRE_TLSV1("requireTLSv1");
const std::string SSLManager::CFG_REQUIRE_TLSV1_1("requireTLSv1_1");
const std::string SSLManager::CFG_REQUIRE_TLSV1_2("requireTLSv1_2");
const std::string SSLManager::CFG_REQUIRE_TLSV1_3("requireTLSv1_3");
const std::string SSLManager::CFG_DISABLE_PROTOCOLS("disableProtocols");
const std::string SSLManager::CFG_DH_PARAMS_FILE("dhParamsFile");
const std::string SSLManager::CFG_ECDH_CURVE("ecdhCurve");
#ifdef OPENSSL_FIPS
const std::string SSLManager::CFG_FIPS_MODE("openSSL.fips");
const bool        SSLManager::VAL_FIPS_MODE(false);
#endif


SSLManager::SSLManager():
	_contextIndex(SSL_CTX_get_ex_new_index(0, nullptr, nullptr, nullptr, nullptr)),
	_socketIndex(SSL_get_ex_new_index(0, nullptr, nullptr, nullptr, nullptr))
{
}


SSLManager::~SSLManager()
{
	try
	{
		shutdown();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void SSLManager::shutdown()
{
	PrivateKeyPassphraseRequired.clear();
	ClientVerificationError.clear();
	ServerVerificationError.clear();
	_ptrDefaultServerContext = nullptr;
	_ptrDefaultClientContext = nullptr;
}


SSLManager& SSLManager::instance()
{
	static SSLManager sm;
	return sm;
}


void SSLManager::initializeServer(PrivateKeyPassphraseHandlerPtr ptrPassphraseHandler, InvalidCertificateHandlerPtr ptrHandler, Context::Ptr ptrContext)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_ptrServerPassphraseHandler  = ptrPassphraseHandler;
	_ptrServerCertificateHandler = ptrHandler;
	_ptrDefaultServerContext     = ptrContext;
}


void SSLManager::initializeClient(PrivateKeyPassphraseHandlerPtr ptrPassphraseHandler, InvalidCertificateHandlerPtr ptrHandler, Context::Ptr ptrContext)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_ptrClientPassphraseHandler  = ptrPassphraseHandler;
	_ptrClientCertificateHandler = ptrHandler;
	_ptrDefaultClientContext     = ptrContext;
}


Context::Ptr SSLManager::defaultServerContext()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (!_ptrDefaultServerContext)
		initDefaultContext(true);

	return _ptrDefaultServerContext;
}


Context::Ptr SSLManager::defaultClientContext()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (!_ptrDefaultClientContext)
	{
		try
		{
			initDefaultContext(false);
		}
		catch (Poco::IllegalStateException&)
		{
			_ptrClientCertificateHandler = new RejectCertificateHandler(false);
			_ptrDefaultClientContext = new Context(Context::CLIENT_USE, "", Context::VERIFY_RELAXED, 9, true);
			_ptrDefaultClientContext->disableProtocols(Context::PROTO_SSLV2 | Context::PROTO_SSLV3);
		}
	}

	return _ptrDefaultClientContext;
}


SSLManager::PrivateKeyPassphraseHandlerPtr SSLManager::serverPassphraseHandler()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (!_ptrServerPassphraseHandler)
		initPassphraseHandler(true);

	return _ptrServerPassphraseHandler;
}


SSLManager::PrivateKeyPassphraseHandlerPtr SSLManager::clientPassphraseHandler()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (!_ptrClientPassphraseHandler)
		initPassphraseHandler(false);

	return _ptrClientPassphraseHandler;
}


SSLManager::InvalidCertificateHandlerPtr SSLManager::serverCertificateHandler()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (!_ptrServerCertificateHandler)
		initCertificateHandler(true);

	return _ptrServerCertificateHandler;
}


SSLManager::InvalidCertificateHandlerPtr SSLManager::clientCertificateHandler()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (!_ptrClientCertificateHandler)
		initCertificateHandler(false);

	return _ptrClientCertificateHandler;
}


int SSLManager::verifyCallback(bool server, int ok, X509_STORE_CTX* pStore)
{
	if (!ok)
	{
		SSLManager& sslManager = SSLManager::instance();
		SSL* pSSL = reinterpret_cast<SSL*>(X509_STORE_CTX_get_ex_data(pStore, SSL_get_ex_data_X509_STORE_CTX_idx()));
		poco_assert_dbg (pSSL);
		SSL_CTX* pSSLContext = SSL_get_SSL_CTX(pSSL);
		poco_assert_dbg (pSSLContext);

		Context* pContext = reinterpret_cast<Context*>(SSL_CTX_get_ex_data(pSSLContext, sslManager.contextIndex()));
		poco_assert_dbg (pContext);

		X509* pCert = X509_STORE_CTX_get_current_cert(pStore);
		X509Certificate x509(pCert, true);
		int depth = X509_STORE_CTX_get_error_depth(pStore);
		int err = X509_STORE_CTX_get_error(pStore);
		std::string error(X509_verify_cert_error_string(err));
		VerificationErrorArgs args(Context::Ptr(pContext, true), x509, depth, err, error);
		if (server)
		{
			if (pContext->getInvalidCertificateHandler())
			{
				pContext->getInvalidCertificateHandler()->onInvalidCertificate(&sslManager, args);
			}
			else if (sslManager._ptrServerCertificateHandler)
			{
				sslManager._ptrServerCertificateHandler->onInvalidCertificate(&sslManager, args);
			}
			sslManager.ServerVerificationError.notify(&sslManager, args);
		}
		else
		{
			if (pContext->getInvalidCertificateHandler())
			{
				pContext->getInvalidCertificateHandler()->onInvalidCertificate(&sslManager, args);
			}
			else if (sslManager._ptrClientCertificateHandler)
			{
				sslManager._ptrClientCertificateHandler->onInvalidCertificate(&sslManager, args);
			}
			sslManager.ClientVerificationError.notify(&sslManager, args);
		}
		ok = args.getIgnoreError() ? 1 : 0;
	}

	return ok;
}


int SSLManager::privateKeyPassphraseCallback(char* pBuf, int size, int flag, void* userData)
{
	std::string pwd;
	SSLManager::instance().PrivateKeyPassphraseRequired.notify(&SSLManager::instance(), pwd);

	strncpy(pBuf, (char *)(pwd.c_str()), size);
	pBuf[size - 1] = '\0';
	if (size > pwd.length())
		size = (int) pwd.length();

	return size;
}


int SSLManager::verifyOCSPResponseCallback(SSL* pSSL, void* arg)
{
#if OPENSSL_VERSION_NUMBER >= 0x10001000L
	const long OCSP_VALIDITY_LEEWAY = 5*60;

	Poco::Net::Context* pContext = static_cast<Poco::Net::Context*>(arg);

	// Fetch the OSCP verify flag
	bool ocspVerifyFlag = pContext->ocspStaplingResponseVerificationEnabled();

	const unsigned char* pResp;
	int len = SSL_get_tlsext_status_ocsp_resp(pSSL, &pResp);
	if (!pResp)
	{
		// OCSP response not received
		return ocspVerifyFlag ? 0 : 1;
	}

	OCSP_RESPONSE* pOcspResp = d2i_OCSP_RESPONSE(nullptr, &pResp, len);
	if (!pOcspResp) return 0;

	if (OCSP_response_status(pOcspResp) != OCSP_RESPONSE_STATUS_SUCCESSFUL)
	{
		OCSP_RESPONSE_free(pOcspResp);
		return 0;
	}

	OCSP_BASICRESP* pBasicResp = OCSP_response_get1_basic(pOcspResp);
	if (!pBasicResp)
	{
		OCSP_RESPONSE_free(pOcspResp);
		return 0;
	}

	X509* pPeerCert = SSL_get_peer_certificate(pSSL);
	if (!pPeerCert)
	{
		OCSP_BASICRESP_free(pBasicResp);
		OCSP_RESPONSE_free(pOcspResp);
		return 0;
	}

	X509* pPeerIssuerCert = nullptr;
	STACK_OF(X509)* pCertChain = SSL_get_peer_cert_chain(pSSL);
	unsigned certChainLen = sk_X509_num(pCertChain);
	for (int i= 0; i < certChainLen ; i++)
	{
		if (!pPeerIssuerCert)
		{
			X509* pIssuerCert = sk_X509_value(pCertChain, i);
			if (X509_check_issued(pIssuerCert, pPeerCert) == X509_V_OK)
			{
				pPeerIssuerCert = pIssuerCert;
				break;
			}
		}
	}
	if (!pPeerIssuerCert)
	{
		X509_free(pPeerCert);
		OCSP_BASICRESP_free(pBasicResp);
		OCSP_RESPONSE_free(pOcspResp);
		return 0;
	}

	STACK_OF(X509)* pCerts = sk_X509_new_null();
	if (pCerts)
	{
		X509* pCert = X509_dup(pPeerIssuerCert);
		if (pCert && !sk_X509_push(pCerts, pCert))
		{
			X509_free(pCert);
			sk_X509_free(pCerts);
			pCerts = nullptr;
		}
	}

	X509_STORE* pStore = SSL_CTX_get_cert_store(SSL_get_SSL_CTX(pSSL));

	int verifyStatus = OCSP_basic_verify(pBasicResp, pCerts, pStore, OCSP_TRUSTOTHER);

	sk_X509_pop_free(pCerts, X509_free);

	if (verifyStatus <= 0)
	{
		X509_free(pPeerCert);
		OCSP_BASICRESP_free(pBasicResp);
		OCSP_RESPONSE_free(pOcspResp);
		return 0;
	}

	OCSP_CERTID* pCertId = OCSP_cert_to_id(nullptr, pPeerCert, pPeerIssuerCert);
	if (!pCertId)
	{
		X509_free(pPeerCert);
		OCSP_BASICRESP_free(pBasicResp);
		OCSP_RESPONSE_free(pOcspResp);
		return 0;
	}

	X509_free(pPeerCert);

	ASN1_GENERALIZEDTIME* pRevTime;
	ASN1_GENERALIZEDTIME* pThisUpdate;
	ASN1_GENERALIZEDTIME* pNextUpdate;
	int certStatus;
	int reason;
	if (!OCSP_resp_find_status(pBasicResp, pCertId, &certStatus, &reason, &pRevTime, &pThisUpdate, &pNextUpdate))
	{
		OCSP_CERTID_free(pCertId);
		OCSP_BASICRESP_free(pBasicResp);
		OCSP_RESPONSE_free(pOcspResp);
		return 0;
	}

	OCSP_CERTID_free(pCertId);

	if (certStatus != V_OCSP_CERTSTATUS_GOOD)
	{
		OCSP_BASICRESP_free(pBasicResp);
		OCSP_RESPONSE_free(pOcspResp);
		return 0;
	}

	if (!OCSP_check_validity(pThisUpdate, pNextUpdate, OCSP_VALIDITY_LEEWAY, -1))
	{
		OCSP_BASICRESP_free(pBasicResp);
		OCSP_RESPONSE_free(pOcspResp);
		return 0;
	}

	OCSP_BASICRESP_free(pBasicResp);
	OCSP_RESPONSE_free(pOcspResp);
#endif

	return 1;
}


void SSLManager::initDefaultContext(bool server)
{
	if (server && _ptrDefaultServerContext) return;
	if (!server && _ptrDefaultClientContext) return;

	Poco::Crypto::OpenSSLInitializer openSSLInitializer;
	initEvents(server);
	Poco::Util::AbstractConfiguration& config = appConfig();

#ifdef OPENSSL_FIPS
	bool fipsEnabled = config.getBool(CFG_FIPS_MODE, VAL_FIPS_MODE);
	if (fipsEnabled && !Poco::Crypto::OpenSSLInitializer::isFIPSEnabled())
	{
		Poco::Crypto::OpenSSLInitializer::enableFIPSMode(true);
	}
#endif

	std::string prefix = server ? CFG_SERVER_PREFIX : CFG_CLIENT_PREFIX;

	Context::Params params;
	// mandatory options
	params.privateKeyFile = config.getString(prefix + CFG_PRIV_KEY_FILE, "");
	params.certificateFile = config.getString(prefix + CFG_CERTIFICATE_FILE, params.privateKeyFile);
	params.caLocation = config.getString(prefix + CFG_CA_LOCATION, "");

	if (server && params.certificateFile.empty() && params.privateKeyFile.empty())
		throw SSLException("Configuration error: no certificate file has been specified");

	// optional options for which we have defaults defined
	params.verificationMode = VAL_VER_MODE;
	if (config.hasProperty(prefix + CFG_VER_MODE))
	{
		// either: none, relaxed, strict, once
		std::string mode = config.getString(prefix + CFG_VER_MODE);
		params.verificationMode = Utility::convertVerificationMode(mode);
	}

	params.verificationDepth = config.getInt(prefix + CFG_VER_DEPTH, VAL_VER_DEPTH);
	params.loadDefaultCAs = config.getBool(prefix + CFG_ENABLE_DEFAULT_CA, VAL_ENABLE_DEFAULT_CA);
	params.cipherList = config.getString(prefix + CFG_CIPHER_LIST, VAL_CIPHER_LIST);
	params.cipherList = config.getString(prefix + CFG_CYPHER_LIST, params.cipherList); // for backwards compatibility
	bool requireTLSv1 = config.getBool(prefix + CFG_REQUIRE_TLSV1, false);
	bool requireTLSv1_1 = config.getBool(prefix + CFG_REQUIRE_TLSV1_1, false);
	bool requireTLSv1_2 = config.getBool(prefix + CFG_REQUIRE_TLSV1_2, false);
	bool requireTLSv1_3 = config.getBool(prefix + CFG_REQUIRE_TLSV1_3, false);

	params.dhParamsFile = config.getString(prefix + CFG_DH_PARAMS_FILE, "");
	params.ecdhCurve    = config.getString(prefix + CFG_ECDH_CURVE, "");

	Context::Usage usage;

	if (server)
	{
		if (requireTLSv1_3)
			usage = Context::TLSV1_3_SERVER_USE;
		else if (requireTLSv1_2)
			usage = Context::TLSV1_2_SERVER_USE;
		else if (requireTLSv1_1)
			usage = Context::TLSV1_1_SERVER_USE;
		else if (requireTLSv1)
			usage = Context::TLSV1_SERVER_USE;
		else
			usage = Context::SERVER_USE;
		_ptrDefaultServerContext = new Context(usage, params);
	}
	else
	{
		if (requireTLSv1_3)
			usage = Context::TLSV1_3_CLIENT_USE;
		else if (requireTLSv1_2)
			usage = Context::TLSV1_2_CLIENT_USE;
		else if (requireTLSv1_1)
			usage = Context::TLSV1_1_CLIENT_USE;
		else if (requireTLSv1)
			usage = Context::TLSV1_CLIENT_USE;
		else
			usage = Context::CLIENT_USE;
		_ptrDefaultClientContext = new Context(usage, params);
	}

	std::string disabledProtocolsList = config.getString(prefix + CFG_DISABLE_PROTOCOLS, "");
	Poco::StringTokenizer dpTok(disabledProtocolsList, ";,", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
	int disabledProtocols = 0;
	for (Poco::StringTokenizer::Iterator it = dpTok.begin(); it != dpTok.end(); ++it)
	{
		if (*it == "sslv2")
			disabledProtocols |= Context::PROTO_SSLV2;
		else if (*it == "sslv3")
			disabledProtocols |= Context::PROTO_SSLV3;
		else if (*it == "tlsv1")
			disabledProtocols |= Context::PROTO_TLSV1;
		else if (*it == "tlsv1_1")
			disabledProtocols |= Context::PROTO_TLSV1_1;
		else if (*it == "tlsv1_2")
			disabledProtocols |= Context::PROTO_TLSV1_2;
		else if (*it == "tlsv1_3")
			disabledProtocols |= Context::PROTO_TLSV1_3;
	}
	if (server)
		_ptrDefaultServerContext->disableProtocols(disabledProtocols);
	else
		_ptrDefaultClientContext->disableProtocols(disabledProtocols);

	bool cacheSessions = config.getBool(prefix + CFG_CACHE_SESSIONS, false);
	if (server)
	{
		std::string sessionIdContext = config.getString(prefix + CFG_SESSION_ID_CONTEXT, config.getString("application.name", ""));
		_ptrDefaultServerContext->enableSessionCache(cacheSessions, sessionIdContext);
		if (config.hasProperty(prefix + CFG_SESSION_CACHE_SIZE))
		{
			int cacheSize = config.getInt(prefix + CFG_SESSION_CACHE_SIZE);
			_ptrDefaultServerContext->setSessionCacheSize(cacheSize);
		}
		if (config.hasProperty(prefix + CFG_SESSION_TIMEOUT))
		{
			int timeout = config.getInt(prefix + CFG_SESSION_TIMEOUT);
			_ptrDefaultServerContext->setSessionTimeout(timeout);
		}
	}
	else
	{
		_ptrDefaultClientContext->enableSessionCache(cacheSessions);
	}
	bool extendedVerification = config.getBool(prefix + CFG_EXTENDED_VERIFICATION, false);
	if (server)
		_ptrDefaultServerContext->enableExtendedCertificateVerification(extendedVerification);
	else
		_ptrDefaultClientContext->enableExtendedCertificateVerification(extendedVerification);

	bool preferServerCiphers = config.getBool(prefix + CFG_PREFER_SERVER_CIPHERS, false);
	if (preferServerCiphers)
	{
		if (server)
			_ptrDefaultServerContext->preferServerCiphers();
		else
			_ptrDefaultClientContext->preferServerCiphers();
	}
}


void SSLManager::initEvents(bool server)
{
	initPassphraseHandler(server);
	initCertificateHandler(server);
}


void SSLManager::initPassphraseHandler(bool server)
{
	if (server && _ptrServerPassphraseHandler) return;
	if (!server && _ptrClientPassphraseHandler) return;

	std::string prefix = server ? CFG_SERVER_PREFIX : CFG_CLIENT_PREFIX;
	Poco::Util::AbstractConfiguration& config = appConfig();

	std::string className(config.getString(prefix + CFG_DELEGATE_HANDLER, VAL_DELEGATE_HANDLER));

	const PrivateKeyFactory* pFactory = 0;
	if (privateKeyFactoryMgr().hasFactory(className))
	{
		pFactory = privateKeyFactoryMgr().getFactory(className);
	}

	if (pFactory)
	{
		if (server)
			_ptrServerPassphraseHandler = pFactory->create(server);
		else
			_ptrClientPassphraseHandler = pFactory->create(server);
	}
	else throw Poco::Util::UnknownOptionException(std::string("No passphrase handler known with the name ") + className);
}


void SSLManager::initCertificateHandler(bool server)
{
	if (server && _ptrServerCertificateHandler) return;
	if (!server && _ptrClientCertificateHandler) return;

	std::string prefix = server ? CFG_SERVER_PREFIX : CFG_CLIENT_PREFIX;
	Poco::Util::AbstractConfiguration& config = appConfig();

	std::string className(config.getString(prefix+CFG_CERTIFICATE_HANDLER, VAL_CERTIFICATE_HANDLER));

	const CertificateHandlerFactory* pFactory = 0;
	if (certificateHandlerFactoryMgr().hasFactory(className))
	{
		pFactory = certificateHandlerFactoryMgr().getFactory(className);
	}

	if (pFactory)
	{
		if (server)
			_ptrServerCertificateHandler = pFactory->create(true);
		else
			_ptrClientCertificateHandler = pFactory->create(false);
	}
	else throw Poco::Util::UnknownOptionException(std::string("No InvalidCertificate handler known with the name ") + className);
}


Poco::Util::AbstractConfiguration& SSLManager::appConfig()
{
	try
	{
		return Poco::Util::Application::instance().config();
	}
	catch (Poco::NullPointerException&)
	{
		throw Poco::IllegalStateException(
			"An application configuration is required to initialize the Poco::Net::SSLManager, "
			"but no Poco::Util::Application instance is available."
		);
	}
}


void initializeSSL()
{
	Poco::Crypto::initializeCrypto();
}


void uninitializeSSL()
{
	SSLManager::instance().shutdown();
	Poco::Crypto::uninitializeCrypto();
}


} } // namespace Poco::Net
