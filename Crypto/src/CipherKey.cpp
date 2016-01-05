//
// CipherKey.cpp
//
// $Id: //poco/1.4/Crypto/src/CipherKey.cpp#1 $
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


CipherKey::CipherKey(const std::string& rName, const std::string& passphrase,  const std::string& salt, int iterationCount,
	const std::string & rDigest):
	_pImpl(new CipherKeyImpl(rName, passphrase, salt, iterationCount, rDigest))
{
}


CipherKey::CipherKey(const std::string& rName, const ByteVec& key, const ByteVec& iv):
	_pImpl(new CipherKeyImpl(rName, key, iv))
{
}


CipherKey::CipherKey(const std::string& rName):
	_pImpl(new CipherKeyImpl(rName))
{
}


CipherKey::~CipherKey()
{
}


} } // namespace Poco::Crypto
