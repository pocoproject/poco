//
// DigestEngine.cpp
//
// Library: Crypto
// Package: Digest
// Module:  DigestEngine
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/DigestEngine.h"
#include "Poco/Crypto/CryptoException.h"
#include "Poco/Exception.h"


namespace Poco::Crypto {


namespace
{
	[[noreturn]] void throwError(const std::string& what, const std::string& name)
	{
		std::string msg(what);
		msg += ": ";
		msg += name;
		throw OpenSSLException(getError(msg));
	}


	EVP_MD_CTX* newContext(const std::string& name)
		/// Creates and initializes in one step: _pContext never refers to
		/// a context that OpenSSL refused to initialize.
	{
		const EVP_MD* md = EVP_get_digestbyname(name.c_str());
		if (md == nullptr) throw Poco::NotFoundException(name);
		EVP_MD_CTX* pContext = EVP_MD_CTX_new();
		if (pContext == nullptr) throwError("Cannot create digest context", name);
		if (EVP_DigestInit_ex(pContext, md, nullptr) != 1)
		{
			EVP_MD_CTX_free(pContext);
			throwError("Cannot initialize digest", name);
		}
		return pContext;
	}
}


DigestEngine::DigestEngine(const std::string& name):
	_name(name),
	_pContext(nullptr)
{
	// Not in the initializer list: _openSSLInitializer must have loaded
	// the providers before the digest is fetched.
	_pContext = newContext(_name);
}


DigestEngine::~DigestEngine()
{
	EVP_MD_CTX_free(_pContext);
}

int DigestEngine::nid() const
{
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	return EVP_MD_nid(EVP_MD_CTX_get0_md(_pContext));
#else
	return EVP_MD_nid(EVP_MD_CTX_md(_pContext));
#endif
}

std::size_t DigestEngine::digestLength() const
{
	const int size = EVP_MD_CTX_size(_pContext);
	if (size <= 0) throwError("Cannot determine digest length", _name);
	return static_cast<std::size_t>(size);
}


void DigestEngine::reset()
{
	// A failure must leave the engine as it was.
	EVP_MD_CTX* pContext = newContext(_name);
	EVP_MD_CTX_free(_pContext);
	_pContext = pContext;
}


const Poco::DigestEngine::Digest& DigestEngine::digest()
{
	_digest.clear();
	_digest.resize(DigestEngine::digestLength());
	unsigned len = 0;
	if (EVP_DigestFinal_ex(_pContext, _digest.data(), &len) != 1)
	{
		_digest.clear();
		throwError("Cannot finalize digest", _name);
	}
	if (len < _digest.size()) _digest.resize(len);
	reset();
	return _digest;
}


void DigestEngine::updateImpl(const void* data, std::size_t length)
{
	if (EVP_DigestUpdate(_pContext, data, length) != 1)
		throwError("Cannot update digest", _name);
}


} // namespace Poco::Crypto
