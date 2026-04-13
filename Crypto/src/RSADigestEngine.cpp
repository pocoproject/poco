//
// RSADigestEngine.cpp
//
// Library: Crypto
// Package: RSA
// Module:  RSADigestEngine
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/RSADigestEngine.h"
#include "Poco/Crypto/CryptoException.h"
#include <openssl/rsa.h>
#include <openssl/evp.h>


namespace Poco::Crypto {


RSADigestEngine::RSADigestEngine(const RSAKey& key, DigestType digestType):
	_key(key),
	_engine(digestType == DIGEST_MD5 ? "MD5" : "SHA1")
{
}

RSADigestEngine::RSADigestEngine(const RSAKey& key, const std::string &name):
	_key(key),
	_engine(name)
{
}


RSADigestEngine::~RSADigestEngine()
{
}


std::size_t RSADigestEngine::digestLength() const
{
	return _engine.digestLength();
}


void RSADigestEngine::reset()
{
	_engine.reset();
	_digest.clear();
	_signature.clear();
}


const DigestEngine::Digest& RSADigestEngine::digest()
{
	if (_digest.empty())
	{
		_digest = _engine.digest();
	}
	return _digest;
}


const DigestEngine::Digest& RSADigestEngine::signature()
{
	if (_signature.empty())
	{
		digest();
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
		EVP_PKEY_CTX* pCtx = EVP_PKEY_CTX_new(_key.impl()->getEVPPKey(), nullptr);
		if (pCtx == nullptr)
			throw OpenSSLException("RSADigestEngine::signature(): EVP_PKEY_CTX_new()");
		if (EVP_PKEY_sign_init(pCtx) != 1)
		{
			EVP_PKEY_CTX_free(pCtx);
			throw OpenSSLException("EVP_PKEY_sign_init()");
		}
		if (EVP_PKEY_CTX_set_rsa_padding(pCtx, RSA_PKCS1_PADDING) != 1)
		{
			EVP_PKEY_CTX_free(pCtx);
			throw OpenSSLException("EVP_PKEY_CTX_set_rsa_padding()");
		}
		const EVP_MD* md = EVP_get_digestbyname(_engine.algorithm().c_str());
		if (md == nullptr)
		{
			EVP_PKEY_CTX_free(pCtx);
			throw OpenSSLException("RSADigestEngine: unknown digest: " + _engine.algorithm());
		}
		if (EVP_PKEY_CTX_set_signature_md(pCtx, md) != 1)
		{
			EVP_PKEY_CTX_free(pCtx);
			throw OpenSSLException("EVP_PKEY_CTX_set_signature_md()");
		}
		size_t sigLen = 0;
		if (EVP_PKEY_sign(pCtx, nullptr, &sigLen, _digest.data(), _digest.size()) != 1)
		{
			EVP_PKEY_CTX_free(pCtx);
			throw OpenSSLException("EVP_PKEY_sign()");
		}
		_signature.resize(sigLen);
		if (EVP_PKEY_sign(pCtx, _signature.data(), &sigLen, _digest.data(), _digest.size()) != 1)
		{
			EVP_PKEY_CTX_free(pCtx);
			throw OpenSSLException("EVP_PKEY_sign()");
		}
		_signature.resize(sigLen);
		EVP_PKEY_CTX_free(pCtx);
#else
		_signature.resize(_key.size());
		unsigned sigLen = static_cast<unsigned>(_signature.size());
		if (!RSA_sign(_engine.nid(), &_digest[0], static_cast<unsigned>(_digest.size()), &_signature[0], &sigLen, _key.impl()->getRSA()))
			throw OpenSSLException("RSADigestEngine::signature(): RSA_sign()");
		if (sigLen < _signature.size())
			_signature.resize(sigLen);
#endif
	}
	return _signature;
}


bool RSADigestEngine::verify(const DigestEngine::Digest& sig)
{
	digest();
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	EVP_PKEY_CTX* pCtx = EVP_PKEY_CTX_new(_key.impl()->getEVPPKey(), nullptr);
	if (pCtx == nullptr)
		throw OpenSSLException("RSADigestEngine::verify(): EVP_PKEY_CTX_new()");
	if (EVP_PKEY_verify_init(pCtx) != 1)
	{
		EVP_PKEY_CTX_free(pCtx);
		throw OpenSSLException("EVP_PKEY_verify_init()");
	}
	if (EVP_PKEY_CTX_set_rsa_padding(pCtx, RSA_PKCS1_PADDING) != 1)
	{
		EVP_PKEY_CTX_free(pCtx);
		throw OpenSSLException("EVP_PKEY_CTX_set_rsa_padding()");
	}
	const EVP_MD* md = EVP_get_digestbyname(_engine.algorithm().c_str());
	if (md == nullptr)
	{
		EVP_PKEY_CTX_free(pCtx);
		throw OpenSSLException("RSADigestEngine: unknown digest: " + _engine.algorithm());
	}
	if (EVP_PKEY_CTX_set_signature_md(pCtx, md) != 1)
	{
		EVP_PKEY_CTX_free(pCtx);
		throw OpenSSLException("EVP_PKEY_CTX_set_signature_md()");
	}
	int ret = EVP_PKEY_verify(pCtx, sig.data(), sig.size(), _digest.data(), _digest.size());
	EVP_PKEY_CTX_free(pCtx);
	return ret == 1;
#else
	DigestEngine::Digest sigCpy = sig; // copy because RSA_verify can modify sigCpy
	int ret = RSA_verify(_engine.nid(), &_digest[0], static_cast<unsigned>(_digest.size()), &sigCpy[0], static_cast<unsigned>(sigCpy.size()), _key.impl()->getRSA());
	return ret != 0;
#endif
}


void RSADigestEngine::updateImpl(const void* data, std::size_t length)
{
	_engine.update(data, length);
}


} // namespace Poco::Crypto
