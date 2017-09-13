//
// ECDSADigestEngine.cpp
//
//
// Library: Crypto
// Package: ECDSA
// Module:  ECDSADigestEngine
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/ECDSADigestEngine.h"
#include <openssl/ecdsa.h>


namespace Poco {
namespace Crypto {


ECDSADigestEngine::ECDSADigestEngine(const ECKey& key, const std::string &name):
	_key(key),
	_engine(name)
{
}


ECDSADigestEngine::~ECDSADigestEngine()
{
}


std::size_t ECDSADigestEngine::digestLength() const
{
	return _engine.digestLength();
}


void ECDSADigestEngine::reset()
{
	_engine.reset();
	_digest.clear();
	_signature.clear();
}

	
const DigestEngine::Digest& ECDSADigestEngine::digest()
{
	if (_digest.empty())
	{
		_digest = _engine.digest();
	}
	return _digest;
}


const DigestEngine::Digest& ECDSADigestEngine::signature()
{
	if (_signature.empty())
	{
		digest();
		_signature.resize(_key.size());
		unsigned sigLen = static_cast<unsigned>(_signature.size());
		if (!ECDSA_do_sign(&_digest[0], static_cast<unsigned>(_digest.size()), _key.impl()->getECKey()))
			throw OpenSSLException();
		if (sigLen < _signature.size()) _signature.resize(sigLen);
	}
	return _signature;
}


bool ECDSADigestEngine::verify(const DigestEngine::Digest& sig)
{
	digest();
	return 0 != ECDSA_do_verify(&_digest[0],
		static_cast<unsigned>(_digest.size()),
		reinterpret_cast<const ECDSA_SIG*>(&sig[0]),
		_key.impl()->getECKey());
}


void ECDSADigestEngine::updateImpl(const void* data, std::size_t length)
{
	_engine.update(data, length);
}


} } // namespace Poco::Crypto
