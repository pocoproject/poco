//
// EVPPKey.h
//
// $Id: //poco/1.4/Crypto/include/Poco/Crypto/EVPPKey.h#3 $
//
// Library: Crypto
// Package: EC
// Module:  EVPPKey
//
// Definition of the EVPPKey class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_EVPPKeyImpl_INCLUDED
#define Crypto_EVPPKeyImpl_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include <openssl/objects.h>
#include <openssl/ec.h>


namespace Poco {
namespace Crypto {


template<typename K>
class EVPPKey
	/// Utility class for conversion between EVP and native keys.
{
public:
	EVPPKey() = delete;

	explicit EVPPKey(EVP_PKEY* pEVPPKey):
		_ownKey(true),
		_pEVPPKey(pEVPPKey),
		_ownEVP(false)
	{
		getKey(&_pKey);
	}

	explicit EVPPKey(K* pKey):
		_pKey(pKey),
		_ownKey(false),
		_pEVPPKey(EVP_PKEY_new()),
		_ownEVP(true)
	{
		
		if (!_pEVPPKey) throw OpenSSLException();
		setKey(_pKey);
	}

	~EVPPKey()
	{
		if (_ownEVP && _pEVPPKey)
			EVP_PKEY_free(_pEVPPKey);
		if (_ownKey && _pKey) freeKey(&_pKey);
	}

	operator K*()
	{
		return _pKey;
	}

	operator EVP_PKEY*()
	{
		return _pEVPPKey;
	}

private:
	void setKey(EC_KEY* pKey)
	{
		if (!EVP_PKEY_set1_EC_KEY(_pEVPPKey, pKey))
			throw OpenSSLException();
	}

	void setKey(RSA* pKey)
	{
		if (!EVP_PKEY_set1_RSA(_pEVPPKey, pKey))
			throw OpenSSLException();
	}
	
	void getKey(EC_KEY** pKey)
	{
		*pKey = EVP_PKEY_get1_EC_KEY(_pEVPPKey);
		if (!*pKey) throw OpenSSLException();
	}

	void getKey(RSA** pKey)
	{
		*pKey = EVP_PKEY_get1_RSA(_pEVPPKey);
		if (!*pKey) throw OpenSSLException();
	}

	void freeKey(RSA** pKey)
	{
		if (_ownKey && pKey && *pKey)
		{
			RSA_free(*pKey);
			*pKey = 0;
		}
	}

	void freeKey(EC_KEY** pKey)
	{
		if (_ownKey && pKey && *pKey)
		{
			EC_KEY_free(*pKey);
			*pKey = 0;
		}
	}

	K*        _pKey = nullptr;
	bool      _ownKey = false;
	EVP_PKEY* _pEVPPKey = nullptr;
	bool      _ownEVP = false;
};


} } // namespace Poco::Crypto


#endif // Crypto_EVPPKeyImpl_INCLUDED
