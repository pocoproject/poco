//
// OpenSSLInitializer.h
//
// Library: Crypto
// Package: CryptoCore
// Module:  OpenSSLInitializer
//
// Definition of the OpenSSLInitializer class.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_OpenSSLInitializer_INCLUDED
#define Crypto_OpenSSLInitializer_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/AtomicCounter.h"
#include <openssl/crypto.h>
#include <openssl/evp.h>
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
#include <openssl/provider.h>
#include <atomic>
#endif


namespace Poco::Crypto {


class Crypto_API OpenSSLInitializer
	/// Initalizes the OpenSSL library.
	///
	/// The class ensures the earliest initialization and the
	/// latest shutdown of the OpenSSL library.
{
public:
	OpenSSLInitializer();
		/// Automatically initialize OpenSSL on startup.

	~OpenSSLInitializer();
		/// Automatically shut down OpenSSL on exit.

	static void initialize();
		/// Initializes the OpenSSL machinery.

	static void uninitialize();
		/// Shuts down the OpenSSL machinery.

	static bool isFIPSEnabled();
		/// Returns true if FIPS mode is enabled, false otherwise.

	static void enableFIPSMode(bool enabled);
		/// Enable or disable FIPS mode. If FIPS is not available, this method doesn't do anything.

	static bool haveLegacyProvider();
		/// Returns true if the OpenSSL legacy provider is available, otherwise false.

private:
	static Poco::AtomicCounter _rc;

#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	static OSSL_PROVIDER* _defaultProvider;
	static OSSL_PROVIDER* _legacyProvider;
#endif
};


//
// inlines
//
inline bool OpenSSLInitializer::isFIPSEnabled()
{
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	return EVP_default_properties_is_fips_enabled(NULL);
#else
	return false;
#endif
}


#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
inline void OpenSSLInitializer::enableFIPSMode(bool enabled)
{
	OSSL_LIB_CTX* libctx = OSSL_LIB_CTX_get0_global_default();

	if(libctx != NULL)
		EVP_default_properties_enable_fips(libctx,enabled);
}
#else
inline void OpenSSLInitializer::enableFIPSMode(bool /*enabled*/)
{
}
#endif


inline bool OpenSSLInitializer::haveLegacyProvider()
{
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	return _legacyProvider != nullptr;
#else
	return false;
#endif
}


} // namespace Poco::Crypto


#endif // Crypto_OpenSSLInitializer_INCLUDED
