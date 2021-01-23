//
// RSAKey.cpp
//
// Library: Crypto
// Package: RSA
// Module:  RSAKey
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/RSAKey.h"
#include <openssl/rsa.h>


namespace Poco {
namespace Crypto {


RSAKey::RSAKey(const EVPPKey& key):
	KeyPair(new RSAKeyImpl(key))
{
}


RSAKey::RSAKey(const X509Certificate& cert):
	KeyPair(new RSAKeyImpl(cert))
{
}


RSAKey::RSAKey(const PKCS12Container& cont):
	KeyPair(new RSAKeyImpl(cont))
{
}


RSAKey::RSAKey(KeyLength keyLength, Exponent exp):
	KeyPair(new RSAKeyImpl(keyLength, (exp == EXP_LARGE) ? RSA_F4 : RSA_3))
{
}


RSAKey::RSAKey(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase):
	KeyPair(new RSAKeyImpl(publicKeyFile, privateKeyFile, privateKeyPassphrase))
{
}


RSAKey::RSAKey(std::istream* pPublicKeyStream, std::istream* pPrivateKeyStream, const std::string& privateKeyPassphrase):
	KeyPair(new RSAKeyImpl(pPublicKeyStream, pPrivateKeyStream, privateKeyPassphrase))
{
}


RSAKey::RSAKey(const RSAKey& other):
	KeyPair(other)
{
}


RSAKey::RSAKey(RSAKey&& other) noexcept:
	KeyPair(std::move(other))
{
}


RSAKey::~RSAKey()
{
}


RSAKey& RSAKey::operator = (const RSAKey& other)
{
	KeyPair::operator = (other);
	return *this;
}


RSAKey& RSAKey::operator = (RSAKey&& other) noexcept
{
	KeyPair::operator = (std::move(other));
	return *this;
}


RSAKeyImpl::ByteVec RSAKey::modulus() const
{
	return impl()->modulus();
}


RSAKeyImpl::ByteVec RSAKey::encryptionExponent() const
{
	return impl()->encryptionExponent();
}


RSAKeyImpl::ByteVec RSAKey::decryptionExponent() const
{
	return impl()->decryptionExponent();
}


} } // namespace Poco::Crypto
