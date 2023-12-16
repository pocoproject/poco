//
// KeyPair.cpp
//
// Library: Crypto
// Package: CryptoCore
// Module:  KeyPair
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/KeyPair.h"
#include <openssl/rsa.h>

#include <utility>


namespace Poco {
namespace Crypto {


KeyPair::KeyPair(KeyPairImpl::Ptr pKeyPairImpl):
	_pImpl(std::move(pKeyPairImpl))
{
}


KeyPair::KeyPair(const KeyPair& other)
	
= default;


KeyPair::KeyPair(KeyPair&& other) noexcept:
	_pImpl(std::move(other._pImpl))
{
}


KeyPair::~KeyPair()
= default;


KeyPair& KeyPair::operator = (const KeyPair& other)
{
	if (&other != this)
	{
		_pImpl = other._pImpl;
	}
	return *this;
}


KeyPair& KeyPair::operator = (KeyPair&& other) noexcept
{
	_pImpl = std::move(other._pImpl);
	return *this;
}


} } // namespace Poco::Crypto
