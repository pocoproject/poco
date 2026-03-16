//
// OpenSSLInitializer.cpp
//
// Library: Crypto
// Package: CryptoCore
// Module:  OpenSSLInitializer
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/Crypto/CryptoException.h"
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/conf.h>


#if defined(POCO_OS_FAMILY_WINDOWS)
	#define POCO_STR_HELPER(x) #x
	#define POCO_STR(x) POCO_STR_HELPER(x)
	#if defined POCO_INTERNAL_OPENSSL_MSVC_VER
		#define POCO_INTERNAL_OPENSSL_BUILD          \
				" (POCO internal build, MSVC version " \
				POCO_STR(POCO_INTERNAL_OPENSSL_MSVC_VER) ")"
	#else
		#define POCO_INTERNAL_OPENSSL_BUILD ""
	#endif
	#pragma message (OPENSSL_VERSION_TEXT POCO_INTERNAL_OPENSSL_BUILD)
#endif


#if defined(_MSC_VER) && !defined(_DLL) && defined(POCO_INTERNAL_OPENSSL_MSVC_VER)

	#if (POCO_MSVS_VERSION >= 2015)
		FILE _iob[] = { *stdin, *stdout, *stderr };
		extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
	#endif // (POCO_MSVS_VERSION >= 2015)

	#if (POCO_MSVS_VERSION < 2012)
		extern "C" __declspec(noreturn) void __cdecl __report_rangecheckfailure(void) { ::ExitProcess(1); }
	#endif // (POCO_MSVS_VERSION < 2012)

#endif // _MSC_VER && _MT && !POCO_EXTERNAL_OPENSSL && (POCO_MSVS_VERSION < 2013)


namespace Poco::Crypto {


Poco::AtomicCounter OpenSSLInitializer::_rc;

#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
OSSL_PROVIDER* OpenSSLInitializer::_defaultProvider(nullptr);
OSSL_PROVIDER* OpenSSLInitializer::_legacyProvider(nullptr);
#endif


OpenSSLInitializer::OpenSSLInitializer()
{
	initialize();
}


OpenSSLInitializer::~OpenSSLInitializer()
{
	try
	{
		uninitialize();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void OpenSSLInitializer::initialize()
{
	if (++_rc == 1)
	{
		CONF_modules_load(nullptr, nullptr, 0);

#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
		if (_defaultProvider == nullptr)
		{
			_defaultProvider = OSSL_PROVIDER_load(nullptr, "default");
			if (_defaultProvider == nullptr) throw CryptoException("Failed to load OpenSSL default provider");
		}
		if (_legacyProvider == nullptr)
		{
			_legacyProvider = OSSL_PROVIDER_load(nullptr, "legacy");
			// Note: use haveLegacyProvider() to check if legacy provider has been loaded
		}
#endif
	}
}


void OpenSSLInitializer::uninitialize()
{
	if (--_rc == 0)
	{
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
		// Unload providers in reverse order of loading.
		// Note: OpenSSL internally leaks OSSL_LIB_CTX state if providers
		// are unloaded and then reloaded in the same process. Applications
		// should treat initialize/uninitialize as a one-time lifecycle and
		// avoid cycling them repeatedly.
		if (_legacyProvider != nullptr)
		{
			OSSL_PROVIDER_unload(_legacyProvider);
			_legacyProvider = nullptr;
		}
		if (_defaultProvider != nullptr)
		{
			OSSL_PROVIDER_unload(_defaultProvider);
			_defaultProvider = nullptr;
		}
		CONF_modules_unload(1);
#endif
	}
}


void initializeCrypto()
{
	OpenSSLInitializer::initialize();
}


void uninitializeCrypto()
{
	OpenSSLInitializer::uninitialize();
}


} // namespace Poco::Crypto
