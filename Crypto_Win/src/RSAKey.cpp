//
// RSAKey.cpp
//
// $Id$
//
// Library: Crypto_Win
// Package: RSA
// Module:  RSAKey
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/RSAKey.h"


// TODO: this is still OpenSSL code


namespace Poco {
namespace Crypto {


RSAKey::RSAKey(const X509Certificate& cert):
	_pImpl(new RSAKeyImpl(cert))
{
}


RSAKey::RSAKey(KeyLength keyLength, Exponent):
	_pImpl(0)
{
	// Exponent is not supported and ignored
	_pImpl = new RSAKeyImpl(keyLength, 0);
}


RSAKey::RSAKey(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase):
	_pImpl(new RSAKeyImpl(publicKeyFile, privateKeyFile, privateKeyPassphrase))
{
}


RSAKey::RSAKey(std::istream* pPublicKeyStream, std::istream* pPrivateKeyStream, const std::string& privateKeyPassphrase):
	_pImpl(new RSAKeyImpl(pPublicKeyStream, pPrivateKeyStream, privateKeyPassphrase))
{
}


RSAKey::~RSAKey()
{
}


int RSAKey::size() const
{
	return _pImpl->size();
}


void RSAKey::save(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase)
{
	_pImpl->save(publicKeyFile, privateKeyFile, privateKeyPassphrase);
}


void RSAKey::save(std::ostream* pPublicKeyStream, std::ostream* pPrivateKeyStream, const std::string& privateKeyPassphrase)
{
	_pImpl->save(pPublicKeyStream, pPrivateKeyStream, privateKeyPassphrase);
}


namespace
{
	static const std::string RSA("rsa");
}


const std::string& RSAKey::name() const
{
	return RSA;
}


} } // namespace Poco::Crypto
