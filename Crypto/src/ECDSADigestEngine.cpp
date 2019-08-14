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
#include "Poco/Crypto/CryptoException.h"
#include <openssl/ecdsa.h>


namespace Poco {
namespace Crypto {


//
// ECDSADigestEngine
//


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
		if (!ECDSA_sign(0, &_digest[0], static_cast<unsigned>(_digest.size()),
			&_signature[0], &sigLen, _key.impl()->getECKey()))
		{
			throw OpenSSLException();
		}
		if (sigLen < _signature.size()) _signature.resize(sigLen);
	}
	return _signature;
}


bool ECDSADigestEngine::verify(const DigestEngine::Digest& sig)
{
	digest();
	EC_KEY* pKey = _key.impl()->getECKey();
	if (pKey)
	{
		int ret = ECDSA_verify(0, &_digest[0], static_cast<unsigned>(_digest.size()),
			&sig[0], static_cast<unsigned>(sig.size()),
			pKey);
		if (1 == ret) return true;
		else if (0 == ret) return false;
	}
	throw OpenSSLException();
}


void ECDSADigestEngine::updateImpl(const void* data, std::size_t length)
{
	_engine.update(data, length);
}


//
// ECDSASignature
//


ECDSASignature::ECDSASignature(const ByteVec& derSignature)
{
	poco_assert (!derSignature.empty());

	const unsigned char* p = &derSignature[0];
	_pSig = d2i_ECDSA_SIG(0, &p, static_cast<long>(derSignature.size()));
	if (!_pSig)
		throw OpenSSLException();
}


ECDSASignature::ECDSASignature(const ByteVec& rawR, const ByteVec& rawS):
	_pSig(ECDSA_SIG_new())
{
	poco_assert (!rawR.empty() && !rawS.empty());

	if (!_pSig) throw CryptoException("cannot allocate ECDSA signature");

	try
	{
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
		ECDSA_SIG_set0(_pSig,
			BN_bin2bn(&rawR[0], static_cast<long>(rawR.size()), 0),
			BN_bin2bn(&rawS[0], static_cast<long>(rawS.size()), 0));
		const BIGNUM* pR = 0;
		const BIGNUM* pS = 0;
		ECDSA_SIG_get0(_pSig, &pR, &pS);
		if (pR == 0 || pS == 0)
			throw Poco::Crypto::CryptoException("failed to decode R and S values");
#else
		if (!BN_bin2bn(&rawR[0], rawR.size(), _pSig->r))
			 throw Poco::Crypto::OpenSSLException();
		if (!BN_bin2bn(&rawS[0], rawS.size(), _pSig->s))
			 throw Poco::Crypto::OpenSSLException();
#endif
	}
	catch (...)
	{
		ECDSA_SIG_free(_pSig);
		throw;
	}
}


ECDSASignature::~ECDSASignature()
{
	ECDSA_SIG_free(_pSig);
}


ECDSASignature::ByteVec ECDSASignature::toDER() const
{
	int size = i2d_ECDSA_SIG(_pSig, 0);
	if (size > 0)
	{
		ByteVec buffer(size);
		unsigned char* p = &buffer[0];
		i2d_ECDSA_SIG(_pSig, &p);
		return buffer;
	}
	else throw OpenSSLException();
}


ECDSASignature::ByteVec ECDSASignature::rawR() const
{
	ByteVec buffer;
#if OPENSSL_VERSION_NUMBER >= 0x10101000L
	const BIGNUM* pR = ECDSA_SIG_get0_r(_pSig);
#elif OPENSSL_VERSION_NUMBER >= 0x10100000L
	const BIGNUM* pR = 0;
	ECDSA_SIG_get0(_pSig, &pR, 0);
#else
	const BIGNUM* pR = _pSig->r;
#endif
	if (pR)
	{
		buffer.resize(BN_num_bytes(pR));
		BN_bn2bin(pR, &buffer[0]);
	}
	return buffer;
}


ECDSASignature::ByteVec ECDSASignature::rawS() const
{
	ByteVec buffer;
#if OPENSSL_VERSION_NUMBER >= 0x10101000L
	const BIGNUM* pS = ECDSA_SIG_get0_s(_pSig);
#elif OPENSSL_VERSION_NUMBER >= 0x10100000L
	const BIGNUM* pS = 0;
	ECDSA_SIG_get0(_pSig, 0, &pS);
#else
	const BIGNUM* pS = _pSig->s;
#endif
	if (pS)
	{
		buffer.resize(BN_num_bytes(pS));
		BN_bn2bin(pS, &buffer[0]);
	}
	return buffer;
}


} } // namespace Poco::Crypto
