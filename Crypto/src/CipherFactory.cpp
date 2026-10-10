//
// CipherFactory.cpp
//
// Library: Crypto
// Package: Cipher
// Module:  CipherFactory
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherKey.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/CipherImpl.h"
#include "Poco/Crypto/RSACipherImpl.h"
#include "Poco/Crypto/EVPCipherImpl.h"
#include "Poco/Crypto/CryptoException.h"
#include "Poco/Exception.h"
#include <openssl/evp.h>
#include <openssl/err.h>
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
#include <openssl/provider.h>
#endif


namespace Poco::Crypto {


CipherFactory::CipherFactory()
{
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	if (OSSL_PROVIDER_load(nullptr, "default") == nullptr)
	{
		std::string msg("Failed to load OpenSSL default provider");
		throw CryptoException(getError(msg));
	}
	// The legacy provider is optional: a failed load must not leave its errors on the queue.
	ERR_set_mark();
	if (OSSL_PROVIDER_load(nullptr, "legacy") == nullptr)
		ERR_pop_to_mark();
	else
		ERR_clear_last_mark();
#endif
}


CipherFactory::~CipherFactory()
{
}


CipherFactory& CipherFactory::defaultFactory()
{
	static CipherFactory cf;
	return cf;
}


Cipher* CipherFactory::createCipher(const CipherKey& key)
{
	return new CipherImpl(key);
}


Cipher* CipherFactory::createCipher(const RSAKey& key, RSAPaddingMode paddingMode)
{
	return new RSACipherImpl(key, paddingMode);
}


Cipher* CipherFactory::createCipher(const EVPPKey& key)
{
	return new EVPCipherImpl(key);
}


} // namespace Poco::Crypto
