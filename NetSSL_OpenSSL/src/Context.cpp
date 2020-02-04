//
// Context.cpp
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  Context
//
// Copyright (c) 2006-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/Context.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/Utility.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/Timestamp.h"
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>


namespace Poco {
namespace Net {


Context::Params::Params():
	verificationMode(VERIFY_RELAXED),
	verificationDepth(9),
	loadDefaultCAs(false),
	cipherList("ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"),
	dhUse2048Bits(false)
{
}


Context::Context(Usage usage, const Params& params):
	_usage(usage),
	_mode(params.verificationMode),
	_pSSLContext(0),
	_extendedCertificateVerification(true)
{
	init(params);
}


Context::Context(
	Usage usage,
	const std::string& privateKeyFile,
	const std::string& certificateFile,
	const std::string& caLocation,
	VerificationMode verificationMode,
	int verificationDepth,
	bool loadDefaultCAs,
	const std::string& cipherList):
	_usage(usage),
	_mode(verificationMode),
	_pSSLContext(0),
	_extendedCertificateVerification(true)
{
	Params params;
	params.privateKeyFile = privateKeyFile;
	params.certificateFile = certificateFile;
	params.caLocation = caLocation;
	params.verificationMode = verificationMode;
	params.verificationDepth = verificationDepth;
	params.loadDefaultCAs = loadDefaultCAs;
	params.cipherList = cipherList;
	init(params);
}


Context::Context(
	Usage usage,
	const std::string& caLocation,
	VerificationMode verificationMode,
	int verificationDepth,
	bool loadDefaultCAs,
	const std::string& cipherList):
	_usage(usage),
	_mode(verificationMode),
	_pSSLContext(0),
	_extendedCertificateVerification(true)
{
	Params params;
	params.caLocation = caLocation;
	params.verificationMode = verificationMode;
	params.verificationDepth = verificationDepth;
	params.loadDefaultCAs = loadDefaultCAs;
	params.cipherList = cipherList;
	init(params);
}


Context::~Context()
{
	try
	{
		SSL_CTX_free(_pSSLContext);
		Poco::Crypto::OpenSSLInitializer::uninitialize();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void Context::init(const Params& params)
{
	Poco::Crypto::OpenSSLInitializer::initialize();

	createSSLContext();

	try
	{
		int errCode = 0;
		if (!params.caLocation.empty())
		{
			Poco::File aFile(params.caLocation);
			if (aFile.isDirectory())
				errCode = SSL_CTX_load_verify_locations(_pSSLContext, 0, Poco::Path::transcode(params.caLocation).c_str());
			else
				errCode = SSL_CTX_load_verify_locations(_pSSLContext, Poco::Path::transcode(params.caLocation).c_str(), 0);
			if (errCode != 1)
			{
				std::string msg = Utility::getLastError();
				throw SSLContextException(std::string("Cannot load CA file/directory at ") + params.caLocation, msg);
			}
		}

		if (params.loadDefaultCAs)
		{
			errCode = SSL_CTX_set_default_verify_paths(_pSSLContext);
			if (errCode != 1)
			{
				std::string msg = Utility::getLastError();
				throw SSLContextException("Cannot load default CA certificates", msg);
			}
		}

		if (!params.privateKeyFile.empty())
		{
			errCode = SSL_CTX_use_PrivateKey_file(_pSSLContext, Poco::Path::transcode(params.privateKeyFile).c_str(), SSL_FILETYPE_PEM);
			if (errCode != 1)
			{
				std::string msg = Utility::getLastError();
				throw SSLContextException(std::string("Error loading private key from file ") + params.privateKeyFile, msg);
			}
		}

		if (!params.certificateFile.empty())
		{
			errCode = SSL_CTX_use_certificate_chain_file(_pSSLContext, Poco::Path::transcode(params.certificateFile).c_str());
			if (errCode != 1)
			{
				std::string errMsg = Utility::getLastError();
				throw SSLContextException(std::string("Error loading certificate from file ") + params.certificateFile, errMsg);
			}
		}

		if (isForServerUse())
			SSL_CTX_set_verify(_pSSLContext, params.verificationMode, &SSLManager::verifyServerCallback);
		else
			SSL_CTX_set_verify(_pSSLContext, params.verificationMode, &SSLManager::verifyClientCallback);

		SSL_CTX_set_cipher_list(_pSSLContext, params.cipherList.c_str());
		SSL_CTX_set_verify_depth(_pSSLContext, params.verificationDepth);
		SSL_CTX_set_mode(_pSSLContext, SSL_MODE_AUTO_RETRY);
		SSL_CTX_set_session_cache_mode(_pSSLContext, SSL_SESS_CACHE_OFF);

		initDH(params.dhUse2048Bits, params.dhParamsFile);
		initECDH(params.ecdhCurve);
	}
	catch (...)
	{
		SSL_CTX_free(_pSSLContext);
		throw;
	}
}


void Context::useCertificate(const Poco::Crypto::X509Certificate& certificate)
{
	int errCode = SSL_CTX_use_certificate(_pSSLContext, const_cast<X509*>(certificate.certificate()));
	if (errCode != 1)
	{
		std::string msg = Utility::getLastError();
		throw SSLContextException("Cannot set certificate for Context", msg);
	}
}


void Context::addChainCertificate(const Poco::Crypto::X509Certificate& certificate)
{
	X509* pCert = certificate.dup();
	int errCode = SSL_CTX_add_extra_chain_cert(_pSSLContext, pCert);
	if (errCode != 1)
	{
		X509_free(pCert);
		std::string msg = Utility::getLastError();
		throw SSLContextException("Cannot add chain certificate to Context", msg);
	}
}


void Context::addCertificateAuthority(const Crypto::X509Certificate &certificate)
{
	if (X509_STORE* store = SSL_CTX_get_cert_store(_pSSLContext))
	{
		int errCode = X509_STORE_add_cert(store, const_cast<X509*>(certificate.certificate()));
		if (errCode != 1)
		{
			std::string msg = Utility::getLastError();
			throw SSLContextException("Cannot add certificate authority to Context", msg);
		}
	}
	else
	{
		std::string msg = Utility::getLastError();
		throw SSLContextException("Cannot add certificate authority to Context", msg);
	}
}


void Context::usePrivateKey(const Poco::Crypto::RSAKey& key)
{
	int errCode = SSL_CTX_use_RSAPrivateKey(_pSSLContext, key.impl()->getRSA());
	if (errCode != 1)
	{
		std::string msg = Utility::getLastError();
		throw SSLContextException("Cannot set private key for Context", msg);
	}
}


void Context::usePrivateKey(const Poco::Crypto::EVPPKey& pkey)
{
	int errCode = SSL_CTX_use_PrivateKey(_pSSLContext, const_cast<EVP_PKEY*>(static_cast<const EVP_PKEY*>(pkey)));
	if (errCode != 1)
	{
		std::string msg = Utility::getLastError();
		throw SSLContextException("Cannot set private key for Context", msg);
	}
}


void Context::enableSessionCache(bool flag)
{
	if (flag)
	{
		SSL_CTX_set_session_cache_mode(_pSSLContext, isForServerUse() ? SSL_SESS_CACHE_SERVER : SSL_SESS_CACHE_CLIENT);
	}
	else
	{
		SSL_CTX_set_session_cache_mode(_pSSLContext, SSL_SESS_CACHE_OFF);
	}
}


void Context::enableSessionCache(bool flag, const std::string& sessionIdContext)
{
	poco_assert (isForServerUse());

	if (flag)
	{
		SSL_CTX_set_session_cache_mode(_pSSLContext, SSL_SESS_CACHE_SERVER);
	}
	else
	{
		SSL_CTX_set_session_cache_mode(_pSSLContext, SSL_SESS_CACHE_OFF);
	}

	unsigned length = static_cast<unsigned>(sessionIdContext.length());
	if (length > SSL_MAX_SSL_SESSION_ID_LENGTH) length = SSL_MAX_SSL_SESSION_ID_LENGTH;
	int rc = SSL_CTX_set_session_id_context(_pSSLContext, reinterpret_cast<const unsigned char*>(sessionIdContext.data()), length);
	if (rc != 1) throw SSLContextException("cannot set session ID context");
}


bool Context::sessionCacheEnabled() const
{
	return SSL_CTX_get_session_cache_mode(_pSSLContext) != SSL_SESS_CACHE_OFF;
}


void Context::setSessionCacheSize(std::size_t size)
{
	poco_assert (isForServerUse());

	SSL_CTX_sess_set_cache_size(_pSSLContext, static_cast<long>(size));
}


std::size_t Context::getSessionCacheSize() const
{
	poco_assert (isForServerUse());

	return static_cast<std::size_t>(SSL_CTX_sess_get_cache_size(_pSSLContext));
}


void Context::setSessionTimeout(long seconds)
{
	poco_assert (isForServerUse());

	SSL_CTX_set_timeout(_pSSLContext, seconds);
}


long Context::getSessionTimeout() const
{
	poco_assert (isForServerUse());

	return SSL_CTX_get_timeout(_pSSLContext);
}


void Context::flushSessionCache()
{
	poco_assert (isForServerUse());

	Poco::Timestamp now;
	SSL_CTX_flush_sessions(_pSSLContext, static_cast<long>(now.epochTime()));
}


void Context::enableExtendedCertificateVerification(bool flag)
{
	_extendedCertificateVerification = flag;
}


void Context::disableStatelessSessionResumption()
{
#if defined(SSL_OP_NO_TICKET)
	SSL_CTX_set_options(_pSSLContext, SSL_OP_NO_TICKET);
#endif
}


void Context::disableProtocols(int protocols)
{
	if (protocols & PROTO_SSLV2)
	{
#if defined(SSL_OP_NO_SSLv2)
		SSL_CTX_set_options(_pSSLContext, SSL_OP_NO_SSLv2);
#endif
	}
	if (protocols & PROTO_SSLV3)
	{
#if defined(SSL_OP_NO_SSLv3)
		SSL_CTX_set_options(_pSSLContext, SSL_OP_NO_SSLv3);
#endif
	}
	if (protocols & PROTO_TLSV1)
	{
#if defined(SSL_OP_NO_TLSv1)
		SSL_CTX_set_options(_pSSLContext, SSL_OP_NO_TLSv1);
#endif
	}
	if (protocols & PROTO_TLSV1_1)
	{
#if defined(SSL_OP_NO_TLSv1_1)
		SSL_CTX_set_options(_pSSLContext, SSL_OP_NO_TLSv1_1);
#endif
	}
	if (protocols & PROTO_TLSV1_2)
	{
#if defined(SSL_OP_NO_TLSv1_2)
		SSL_CTX_set_options(_pSSLContext, SSL_OP_NO_TLSv1_2);
#endif
	}
	if (protocols & PROTO_TLSV1_3)
	{
#if defined(SSL_OP_NO_TLSv1_3)
		SSL_CTX_set_options(_pSSLContext, SSL_OP_NO_TLSv1_3);
#endif
	}
}


void Context::requireMinimumProtocol(Protocols protocol)
{
#if OPENSSL_VERSION_NUMBER >= 0x10101000L
	int version = 0;
	switch (protocol)
	{
	case PROTO_SSLV2:
		throw Poco::InvalidArgumentException("SSLv2 is no longer supported");
	case PROTO_SSLV3:
		version = SSL3_VERSION;
		break;
	case PROTO_TLSV1:
		version = TLS1_VERSION;
		break;
	case PROTO_TLSV1_1:
		version = TLS1_1_VERSION;
		break;
	case PROTO_TLSV1_2:
		version = TLS1_2_VERSION;
		break;
	case PROTO_TLSV1_3:
		version = TLS1_3_VERSION;
		break;
	}
	if (!SSL_CTX_set_min_proto_version(_pSSLContext, version))
	{
		unsigned long err = ERR_get_error();
		throw SSLException("Cannot set minimum supported version on SSL_CTX object", ERR_error_string(err, 0));
	}

#else

	switch (protocol)
	{
	case PROTO_SSLV2:
		throw Poco::InvalidArgumentException("SSLv2 is no longer supported");

	case PROTO_SSLV3:
		disableProtocols(PROTO_SSLV2);
		break;

	case PROTO_TLSV1:
		disableProtocols(PROTO_SSLV2 | PROTO_SSLV3);
		break;

	case PROTO_TLSV1_1:
#if defined(SSL_OP_NO_TLSv1_1) && !defined(OPENSSL_NO_TLS1)
		disableProtocols(PROTO_SSLV2 | PROTO_SSLV3 | PROTO_TLSV1);
#else
		throw Poco::InvalidArgumentException("TLSv1.1 is not supported by the available OpenSSL library");
#endif
		break;

	case PROTO_TLSV1_2:
#if defined(SSL_OP_NO_TLSv1_2) && !defined(OPENSSL_NO_TLS1)
		disableProtocols(PROTO_SSLV2 | PROTO_SSLV3 | PROTO_TLSV1 | PROTO_TLSV1_1);
#else
		throw Poco::InvalidArgumentException("TLSv1.2 is not supported by the available OpenSSL library");
#endif
		break;

	case PROTO_TLSV1_3:
		throw Poco::InvalidArgumentException("TLSv1.3 is not supported by the available OpenSSL library");
		break;
	}
#endif
}


void Context::preferServerCiphers()
{
#if defined(SSL_OP_CIPHER_SERVER_PREFERENCE)
	SSL_CTX_set_options(_pSSLContext, SSL_OP_CIPHER_SERVER_PREFERENCE);
#endif
}


void Context::createSSLContext()
{
	int minTLSVersion = 0;

	if (SSLManager::isFIPSEnabled())
	{
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
		_pSSLContext = SSL_CTX_new(TLS_method());
#else
		_pSSLContext = SSL_CTX_new(TLSv1_method());
#endif
	}
	else
	{
		switch (_usage)
		{
		case CLIENT_USE:
		case TLS_CLIENT_USE:
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
			_pSSLContext = SSL_CTX_new(TLS_client_method());
			minTLSVersion = TLS1_VERSION;
#else
			_pSSLContext = SSL_CTX_new(SSLv23_client_method());
#endif
			break;

		case SERVER_USE:
		case TLS_SERVER_USE:
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
			_pSSLContext = SSL_CTX_new(TLS_server_method());
			minTLSVersion = TLS1_VERSION;
#else
			_pSSLContext = SSL_CTX_new(SSLv23_server_method());
#endif
			break;

		case TLSV1_CLIENT_USE:
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
			_pSSLContext = SSL_CTX_new(TLS_client_method());
			minTLSVersion = TLS1_VERSION;
#else
			_pSSLContext = SSL_CTX_new(TLSv1_client_method());
#endif
			break;

		case TLSV1_SERVER_USE:
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
			_pSSLContext = SSL_CTX_new(TLS_server_method());
			minTLSVersion = TLS1_VERSION;
#else
			_pSSLContext = SSL_CTX_new(TLSv1_server_method());
#endif
			break;

#if defined(SSL_OP_NO_TLSv1_1) && !defined(OPENSSL_NO_TLS1)
/* SSL_OP_NO_TLSv1_1 is defined in ssl.h if the library version supports TLSv1.1.
 * OPENSSL_NO_TLS1 is defined in opensslconf.h or on the compiler command line
 * if TLS1.x was removed at OpenSSL library build time via Configure options.
 */
        case TLSV1_1_CLIENT_USE:
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
			_pSSLContext = SSL_CTX_new(TLS_client_method());
			minTLSVersion = TLS1_1_VERSION;
#else
            _pSSLContext = SSL_CTX_new(TLSv1_1_client_method());
#endif
            break;

        case TLSV1_1_SERVER_USE:
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
            _pSSLContext = SSL_CTX_new(TLS_server_method());
			minTLSVersion = TLS1_1_VERSION;
#else
            _pSSLContext = SSL_CTX_new(TLSv1_1_server_method());
#endif
            break;
#endif

#if defined(SSL_OP_NO_TLSv1_2) && !defined(OPENSSL_NO_TLS1)
        case TLSV1_2_CLIENT_USE:
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
            _pSSLContext = SSL_CTX_new(TLS_client_method());
            minTLSVersion = TLS1_2_VERSION;
#else
            _pSSLContext = SSL_CTX_new(TLSv1_2_client_method());
#endif
            break;

        case TLSV1_2_SERVER_USE:
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
            _pSSLContext = SSL_CTX_new(TLS_server_method());
            minTLSVersion = TLS1_2_VERSION;
#else
            _pSSLContext = SSL_CTX_new(TLSv1_2_server_method());
#endif
            break;
#endif

#if defined(SSL_OP_NO_TLSv1_3) && !defined(OPENSSL_NO_TLS1)
        case TLSV1_3_CLIENT_USE:
#if OPENSSL_VERSION_NUMBER >= 0x10101000L
            _pSSLContext = SSL_CTX_new(TLS_client_method());
            minTLSVersion = TLS1_3_VERSION;
#endif
            break;

        case TLSV1_3_SERVER_USE:
#if OPENSSL_VERSION_NUMBER >= 0x10101000L
            _pSSLContext = SSL_CTX_new(TLS_server_method());
            minTLSVersion = TLS1_3_VERSION;
#endif
            break;
#endif

		default:
			throw Poco::InvalidArgumentException("Invalid or unsupported usage");
		}
	}
	if (!_pSSLContext)
	{
		unsigned long err = ERR_get_error();
		throw SSLException("Cannot create SSL_CTX object", ERR_error_string(err, 0));
	}

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
	if (minTLSVersion)
	{
		if (!SSL_CTX_set_min_proto_version(_pSSLContext, minTLSVersion))
		{
			SSL_CTX_free(_pSSLContext);
			_pSSLContext = 0;
			unsigned long err = ERR_get_error();
			throw SSLException("Cannot set minimum supported version on SSL_CTX object", ERR_error_string(err, 0));
		}
	}
#endif

	SSL_CTX_set_default_passwd_cb(_pSSLContext, &SSLManager::privateKeyPassphraseCallback);
	Utility::clearErrorStack();
	SSL_CTX_set_options(_pSSLContext, SSL_OP_ALL);
}


void Context::initDH(bool use2048Bits, const std::string& dhParamsFile)
{
#ifndef OPENSSL_NO_DH
	static const unsigned char dh1024_p[] =
	{
		0xB1,0x0B,0x8F,0x96,0xA0,0x80,0xE0,0x1D,0xDE,0x92,0xDE,0x5E,
		0xAE,0x5D,0x54,0xEC,0x52,0xC9,0x9F,0xBC,0xFB,0x06,0xA3,0xC6,
		0x9A,0x6A,0x9D,0xCA,0x52,0xD2,0x3B,0x61,0x60,0x73,0xE2,0x86,
		0x75,0xA2,0x3D,0x18,0x98,0x38,0xEF,0x1E,0x2E,0xE6,0x52,0xC0,
		0x13,0xEC,0xB4,0xAE,0xA9,0x06,0x11,0x23,0x24,0x97,0x5C,0x3C,
		0xD4,0x9B,0x83,0xBF,0xAC,0xCB,0xDD,0x7D,0x90,0xC4,0xBD,0x70,
		0x98,0x48,0x8E,0x9C,0x21,0x9A,0x73,0x72,0x4E,0xFF,0xD6,0xFA,
		0xE5,0x64,0x47,0x38,0xFA,0xA3,0x1A,0x4F,0xF5,0x5B,0xCC,0xC0,
		0xA1,0x51,0xAF,0x5F,0x0D,0xC8,0xB4,0xBD,0x45,0xBF,0x37,0xDF,
		0x36,0x5C,0x1A,0x65,0xE6,0x8C,0xFD,0xA7,0x6D,0x4D,0xA7,0x08,
		0xDF,0x1F,0xB2,0xBC,0x2E,0x4A,0x43,0x71,
	};

	static const unsigned char dh1024_g[] =
	{
		0xA4,0xD1,0xCB,0xD5,0xC3,0xFD,0x34,0x12,0x67,0x65,0xA4,0x42,
		0xEF,0xB9,0x99,0x05,0xF8,0x10,0x4D,0xD2,0x58,0xAC,0x50,0x7F,
		0xD6,0x40,0x6C,0xFF,0x14,0x26,0x6D,0x31,0x26,0x6F,0xEA,0x1E,
		0x5C,0x41,0x56,0x4B,0x77,0x7E,0x69,0x0F,0x55,0x04,0xF2,0x13,
		0x16,0x02,0x17,0xB4,0xB0,0x1B,0x88,0x6A,0x5E,0x91,0x54,0x7F,
		0x9E,0x27,0x49,0xF4,0xD7,0xFB,0xD7,0xD3,0xB9,0xA9,0x2E,0xE1,
		0x90,0x9D,0x0D,0x22,0x63,0xF8,0x0A,0x76,0xA6,0xA2,0x4C,0x08,
		0x7A,0x09,0x1F,0x53,0x1D,0xBF,0x0A,0x01,0x69,0xB6,0xA2,0x8A,
		0xD6,0x62,0xA4,0xD1,0x8E,0x73,0xAF,0xA3,0x2D,0x77,0x9D,0x59,
		0x18,0xD0,0x8B,0xC8,0x85,0x8F,0x4D,0xCE,0xF9,0x7C,0x2A,0x24,
		0x85,0x5E,0x6E,0xEB,0x22,0xB3,0xB2,0xE5,
	};

	static const unsigned char dh2048_p[] =
	{
		0x87,0xA8,0xE6,0x1D,0xB4,0xB6,0x66,0x3C,0xFF,0xBB,0xD1,0x9C,
		0x65,0x19,0x59,0x99,0x8C,0xEE,0xF6,0x08,0x66,0x0D,0xD0,0xF2,
		0x5D,0x2C,0xEE,0xD4,0x43,0x5E,0x3B,0x00,0xE0,0x0D,0xF8,0xF1,
		0xD6,0x19,0x57,0xD4,0xFA,0xF7,0xDF,0x45,0x61,0xB2,0xAA,0x30,
		0x16,0xC3,0xD9,0x11,0x34,0x09,0x6F,0xAA,0x3B,0xF4,0x29,0x6D,
		0x83,0x0E,0x9A,0x7C,0x20,0x9E,0x0C,0x64,0x97,0x51,0x7A,0xBD,
		0x5A,0x8A,0x9D,0x30,0x6B,0xCF,0x67,0xED,0x91,0xF9,0xE6,0x72,
		0x5B,0x47,0x58,0xC0,0x22,0xE0,0xB1,0xEF,0x42,0x75,0xBF,0x7B,
		0x6C,0x5B,0xFC,0x11,0xD4,0x5F,0x90,0x88,0xB9,0x41,0xF5,0x4E,
		0xB1,0xE5,0x9B,0xB8,0xBC,0x39,0xA0,0xBF,0x12,0x30,0x7F,0x5C,
		0x4F,0xDB,0x70,0xC5,0x81,0xB2,0x3F,0x76,0xB6,0x3A,0xCA,0xE1,
		0xCA,0xA6,0xB7,0x90,0x2D,0x52,0x52,0x67,0x35,0x48,0x8A,0x0E,
		0xF1,0x3C,0x6D,0x9A,0x51,0xBF,0xA4,0xAB,0x3A,0xD8,0x34,0x77,
		0x96,0x52,0x4D,0x8E,0xF6,0xA1,0x67,0xB5,0xA4,0x18,0x25,0xD9,
		0x67,0xE1,0x44,0xE5,0x14,0x05,0x64,0x25,0x1C,0xCA,0xCB,0x83,
		0xE6,0xB4,0x86,0xF6,0xB3,0xCA,0x3F,0x79,0x71,0x50,0x60,0x26,
		0xC0,0xB8,0x57,0xF6,0x89,0x96,0x28,0x56,0xDE,0xD4,0x01,0x0A,
		0xBD,0x0B,0xE6,0x21,0xC3,0xA3,0x96,0x0A,0x54,0xE7,0x10,0xC3,
		0x75,0xF2,0x63,0x75,0xD7,0x01,0x41,0x03,0xA4,0xB5,0x43,0x30,
		0xC1,0x98,0xAF,0x12,0x61,0x16,0xD2,0x27,0x6E,0x11,0x71,0x5F,
		0x69,0x38,0x77,0xFA,0xD7,0xEF,0x09,0xCA,0xDB,0x09,0x4A,0xE9,
		0x1E,0x1A,0x15,0x97,
	};

	static const unsigned char dh2048_g[] =
	{
		0x3F,0xB3,0x2C,0x9B,0x73,0x13,0x4D,0x0B,0x2E,0x77,0x50,0x66,
		0x60,0xED,0xBD,0x48,0x4C,0xA7,0xB1,0x8F,0x21,0xEF,0x20,0x54,
		0x07,0xF4,0x79,0x3A,0x1A,0x0B,0xA1,0x25,0x10,0xDB,0xC1,0x50,
		0x77,0xBE,0x46,0x3F,0xFF,0x4F,0xED,0x4A,0xAC,0x0B,0xB5,0x55,
		0xBE,0x3A,0x6C,0x1B,0x0C,0x6B,0x47,0xB1,0xBC,0x37,0x73,0xBF,
		0x7E,0x8C,0x6F,0x62,0x90,0x12,0x28,0xF8,0xC2,0x8C,0xBB,0x18,
		0xA5,0x5A,0xE3,0x13,0x41,0x00,0x0A,0x65,0x01,0x96,0xF9,0x31,
		0xC7,0x7A,0x57,0xF2,0xDD,0xF4,0x63,0xE5,0xE9,0xEC,0x14,0x4B,
		0x77,0x7D,0xE6,0x2A,0xAA,0xB8,0xA8,0x62,0x8A,0xC3,0x76,0xD2,
		0x82,0xD6,0xED,0x38,0x64,0xE6,0x79,0x82,0x42,0x8E,0xBC,0x83,
		0x1D,0x14,0x34,0x8F,0x6F,0x2F,0x91,0x93,0xB5,0x04,0x5A,0xF2,
		0x76,0x71,0x64,0xE1,0xDF,0xC9,0x67,0xC1,0xFB,0x3F,0x2E,0x55,
		0xA4,0xBD,0x1B,0xFF,0xE8,0x3B,0x9C,0x80,0xD0,0x52,0xB9,0x85,
		0xD1,0x82,0xEA,0x0A,0xDB,0x2A,0x3B,0x73,0x13,0xD3,0xFE,0x14,
		0xC8,0x48,0x4B,0x1E,0x05,0x25,0x88,0xB9,0xB7,0xD2,0xBB,0xD2,
		0xDF,0x01,0x61,0x99,0xEC,0xD0,0x6E,0x15,0x57,0xCD,0x09,0x15,
		0xB3,0x35,0x3B,0xBB,0x64,0xE0,0xEC,0x37,0x7F,0xD0,0x28,0x37,
		0x0D,0xF9,0x2B,0x52,0xC7,0x89,0x14,0x28,0xCD,0xC6,0x7E,0xB6,
		0x18,0x4B,0x52,0x3D,0x1D,0xB2,0x46,0xC3,0x2F,0x63,0x07,0x84,
		0x90,0xF0,0x0E,0xF8,0xD6,0x47,0xD1,0x48,0xD4,0x79,0x54,0x51,
		0x5E,0x23,0x27,0xCF,0xEF,0x98,0xC5,0x82,0x66,0x4B,0x4C,0x0F,
		0x6C,0xC4,0x16,0x59,
	};

	DH* dh = 0;
	if (!dhParamsFile.empty())
	{
		BIO* bio = BIO_new_file(dhParamsFile.c_str(), "r");
		if (!bio)
		{
			std::string msg = Utility::getLastError();
			throw SSLContextException(std::string("Error opening Diffie-Hellman parameters file ") + dhParamsFile, msg);
		}
		dh = PEM_read_bio_DHparams(bio, 0, 0, 0);
		BIO_free(bio);
		if (!dh)
		{
			std::string msg = Utility::getLastError();
			throw SSLContextException(std::string("Error reading Diffie-Hellman parameters from file ") + dhParamsFile, msg);
		}
	}
	else
	{
		dh = DH_new();
		if (!dh)
		{
			std::string msg = Utility::getLastError();
			throw SSLContextException("Error creating Diffie-Hellman parameters", msg);
		}
#if OPENSSL_VERSION_NUMBER >= 0x10100000L && !defined(LIBRESSL_VERSION_NUMBER)
		if (use2048Bits)
		{
			BIGNUM* p = BN_bin2bn(dh2048_p, sizeof(dh2048_p), 0);
			BIGNUM* g = BN_bin2bn(dh2048_g, sizeof(dh2048_g), 0);
			DH_set0_pqg(dh, p, 0, g);
			DH_set_length(dh, 256);
		}
		else
		{
			BIGNUM* p = BN_bin2bn(dh1024_p, sizeof(dh1024_p), 0);
			BIGNUM* g = BN_bin2bn(dh1024_g, sizeof(dh1024_g), 0);
			DH_set0_pqg(dh, p, 0, g);
			DH_set_length(dh, 160);
		}
		if (!p || !g)
		{
			DH_free(dh);
			throw SSLContextException("Error creating Diffie-Hellman parameters");
		}
#else
		if (use2048Bits)
		{
			dh->p = BN_bin2bn(dh2048_p, sizeof(dh2048_p), 0);
			dh->g = BN_bin2bn(dh2048_g, sizeof(dh2048_g), 0);
			dh->length = 256;
		}
		else
		{
			dh->p = BN_bin2bn(dh1024_p, sizeof(dh1024_p), 0);
			dh->g = BN_bin2bn(dh1024_g, sizeof(dh1024_g), 0);
			dh->length = 160;
		}
		if ((!dh->p) || (!dh->g))
		{
			DH_free(dh);
			throw SSLContextException("Error creating Diffie-Hellman parameters");
		}
#endif
	}
	SSL_CTX_set_tmp_dh(_pSSLContext, dh);
	SSL_CTX_set_options(_pSSLContext, SSL_OP_SINGLE_DH_USE);
	DH_free(dh);
#else
	if (!dhParamsFile.empty())
		throw SSLContextException("OpenSSL does not support DH");
#endif
}


void Context::initECDH(const std::string& curve)
{
#ifndef OPENSSL_NO_ECDH
#if OPENSSL_VERSION_NUMBER >= 0x1000200fL
 	const std::string groups(curve.empty() ?
 #if   OPENSSL_VERSION_NUMBER >= 0x1010100fL
 				   "X448:X25519:ffdhe4096:ffdhe3072:ffdhe2048:ffdhe6144:ffdhe8192:P-521:P-384:P-256"
 #elif OPENSSL_VERSION_NUMBER >= 0x1010000fL
 	// while OpenSSL 1.1.0 didn't support Ed25519 (EdDSA using Curve25519),
 	// it did support X25519 (ECDH using Curve25516).
 				   "X25519:P-521:P-384:P-256"
 #else
 				   "P-521:P-384:P-256"
 #endif
 				   : curve);
 	if (SSL_CTX_set1_curves_list(_pSSLContext, groups.c_str()) == 0)
 	{
 		throw SSLContextException("Cannot set ECDH groups", groups);
 	}
 	SSL_CTX_set_options(_pSSLContext, SSL_OP_SINGLE_ECDH_USE);
 #elif OPENSSL_VERSION_NUMBER >= 0x0090800fL
	int nid = 0;
	if (!curve.empty())
	{
		nid = OBJ_sn2nid(curve.c_str());
	}
	else
	{
		nid = OBJ_sn2nid("prime256v1");
	}
	if (nid == 0)
	{
		throw SSLContextException("Unknown ECDH curve name", curve);
	}

	EC_KEY* ecdh = EC_KEY_new_by_curve_name(nid);
	if (!ecdh)
	{
		throw SSLContextException("Cannot create ECDH curve");
	}
	SSL_CTX_set_tmp_ecdh(_pSSLContext, ecdh);
	SSL_CTX_set_options(_pSSLContext, SSL_OP_SINGLE_ECDH_USE);
	EC_KEY_free(ecdh);
#endif
#endif
}


} } // namespace Poco::Net
