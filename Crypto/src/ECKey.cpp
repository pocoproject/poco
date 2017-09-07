//
// ECKey.cpp
//
// $Id: //poco/1.4/Crypto/src/ECKey.cpp#2 $
//
// Library: Crypto
// Package: EC
// Module:  ECKey
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/ECKey.h"
#include <openssl/rsa.h>


namespace Poco {
namespace Crypto {


ECKey::ECKey(const X509Certificate& cert):
	_pImpl(new ECKeyImpl(cert))
{
}


ECKey::ECKey(int eccGroup): _pImpl(0)
{
	_pImpl = new ECKeyImpl(eccGroup);
}


ECKey::ECKey(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase):
	_pImpl(new ECKeyImpl(publicKeyFile, privateKeyFile, privateKeyPassphrase))
{
}


ECKey::ECKey(std::istream* pPublicKeyStream, std::istream* pPrivateKeyStream, const std::string& privateKeyPassphrase):
	_pImpl(new ECKeyImpl(pPublicKeyStream, pPrivateKeyStream, privateKeyPassphrase))
{
}


ECKey::~ECKey()
{
}


int ECKey::size() const
{
	return _pImpl->size();
}

/*
ECKeyImpl::ByteVec ECKey::modulus() const
{
	return _pImpl->modulus();
}


ECKeyImpl::ByteVec ECKey::encryptionExponent() const
{
	return _pImpl->encryptionExponent();
}


ECKeyImpl::ByteVec ECKey::decryptionExponent() const
{
	return _pImpl->decryptionExponent();
}
*/

void ECKey::save(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase)
{
	_pImpl->save(publicKeyFile, privateKeyFile, privateKeyPassphrase);
}


void ECKey::save(std::ostream* pPublicKeyStream, std::ostream* pPrivateKeyStream, const std::string& privateKeyPassphrase)
{
	_pImpl->save(pPublicKeyStream, pPrivateKeyStream, privateKeyPassphrase);
}


const std::string& ECKey::name() const
{
	static const std::string EC("ec");
	return EC;
}


} } // namespace Poco::Crypto
