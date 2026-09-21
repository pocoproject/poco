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
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
#include <openssl/provider.h>
#include <openssl/evp.h>
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

	[[nodiscard]] static bool isFIPSEnabled();
		/// Returns true if FIPS mode is enabled, false otherwise.
		/// Always false with OpenSSL versions before 3.0.

	static void enableFIPSMode(bool enabled);
		/// Enables or disables FIPS mode by setting the default property
		/// query "fips=yes"; the other providers stay loaded.
		///
		/// Throws a CryptoException if FIPS mode cannot be enabled: the
		/// FIPS provider must be activated in the OpenSSL configuration
		/// file (openssl.cnf), and OpenSSL 3.0 or newer is required.
		/// Disabling does nothing with older versions.
		///
		/// This method is not thread safe, because the underlying OpenSSL
		/// function is not. Call it during startup, before other threads
		/// use OpenSSL and before objects that select algorithms when they
		/// are created, such as Poco::Net::Context, are created.

	[[nodiscard]] static bool haveLegacyProvider();
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
	return EVP_default_properties_is_fips_enabled(nullptr) ? true : false;
#else
	return false;
#endif
}


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
