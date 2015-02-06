//
// DigestEngine.cpp
//
// $Id$
//
// Library: Crypto_Win
// Package: Digest
// Module:  DigestEngine
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/DigestEngine.h"
#include "Poco/Exception.h"
#include "Poco/String.h"


namespace Poco {
namespace Crypto {


DigestEngine::DigestEngine(const std::string& name):
	_name(name),
	_handle(0)
{
	reset();
}

	
DigestEngine::~DigestEngine()
{
	CryptDestroyHash(_handle);
}


unsigned DigestEngine::digestLength() const
{
	DWORD hashLen;
	DWORD len = sizeof(hashLen);
	if (CryptGetHashParam(_handle, HP_HASHSIZE, reinterpret_cast<BYTE*>(&hashLen), &len, 0))
		return static_cast<unsigned>(hashLen);
	else
		throw Poco::SystemException("Failed to obtain hash size", GetLastError());
}


void DigestEngine::reset()
{
	if (_handle)
	{
		CryptDestroyHash(_handle);
		_handle = 0;
	}
	ALG_ID algo;
	if (icompare(_name, "MD2") == 0)
		algo = CALG_MD2;
	else if (icompare(_name, "MD4") == 0)
		algo = CALG_MD4;
	else if (icompare(_name, "MD5") == 0)
		algo = CALG_MD5;
	else if (icompare(_name, "SHA1") == 0 || icompare(_name, "SHA-1") == 0)
		algo = CALG_SHA1;
	else if (icompare(_name, "SHA256") == 0 || icompare(_name, "SHA-256") == 0)
		algo = CALG_SHA_256;
	else if (icompare(_name, "SHA384") == 0 || icompare(_name, "SHA-384") == 0)
		algo = CALG_SHA_384;
	else if (icompare(_name, "SHA512") == 0 || icompare(_name, "SHA-512") == 0)
		algo = CALG_SHA_512;
	else
		throw Poco::NotFoundException("hash algorithm", _name);
	if (!CryptCreateHash(_sp.handle(),	algo, NULL, 0, &_handle))
		throw Poco::SystemException("Failed to create hash", GetLastError());
}


const Poco::DigestEngine::Digest& DigestEngine::digest()
{
	_digest.clear();
	unsigned hashLen = digestLength();
	_digest.resize(hashLen);
	DWORD len = hashLen;
	if (!CryptGetHashParam(_handle, HP_HASHVAL, &_digest[0], &len, 0))
		throw Poco::SystemException("Failed to obtain hash", GetLastError());
	reset();
	return _digest;
}


void DigestEngine::updateImpl(const void* data, unsigned length)
{
	if (!CryptHashData(_handle, reinterpret_cast<const BYTE*>(data), length, 0))
		throw Poco::SystemException("Failed to hash data", GetLastError());
}


} } // namespace Poco::Crypto
