//
// CipherKey.cpp
//
// Library: Crypto
// Package: Cipher
// Module:  CipherKey
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/CipherKey.h"


namespace Poco {
namespace Crypto {


CipherKey::CipherKey(const std::string& name,
	const std::string& passphrase,
	const std::string& salt,
	int iterationCount,
	const std::string &digest):
		_pImpl(new CipherKeyImpl(name, passphrase, salt, iterationCount, digest))
{
}


CipherKey::CipherKey(const std::string& name, const ByteVec& key, const ByteVec& iv):
	_pImpl(new CipherKeyImpl(name, key, iv))
{
}


CipherKey::CipherKey(const std::string& name):
	_pImpl(new CipherKeyImpl(name))
{
}


CipherKey::CipherKey(const CipherKey& other):
	_pImpl(other._pImpl)
{
}


CipherKey::CipherKey(CipherKey&& other) noexcept:
	_pImpl(std::move(other._pImpl))
{
}


CipherKey::~CipherKey()
{
}


CipherKey& CipherKey::operator = (const CipherKey& other)
{
	if (&other != this)
	{
		_pImpl = other._pImpl;
	}
	return *this;
}


CipherKey& CipherKey::operator = (CipherKey&& other) noexcept
{
	_pImpl = std::move(other._pImpl);
	return *this;
}


} } // namespace Poco::Crypto
