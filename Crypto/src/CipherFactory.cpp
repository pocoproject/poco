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
	OSSL_PROVIDER_load(nullptr, "default");
	OSSL_PROVIDER_load(nullptr, "legacy");
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
