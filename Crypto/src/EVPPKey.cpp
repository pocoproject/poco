//
// EVPPKey.cpp
//
//
// Library: Crypto
// Package: CryptoCore
// Module:  EVPPKey
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/EVPPKey.h"
#include "Poco/NumberFormatter.h"


namespace Poco {
namespace Crypto {


EVPPKey::EVPPKey(EVP_PKEY* pEVPPKey)
{
	duplicate(pEVPPKey);
}


EVPPKey::EVPPKey(const EVPPKey& other)
{
	duplicate(other._pEVPPKey);
}


EVPPKey::EVPPKey(EVPPKey&& other): _pEVPPKey(other._pEVPPKey)
{
	other._pEVPPKey = nullptr;
}


EVPPKey& EVPPKey::operator=(const EVPPKey& other)
{
	duplicate(other._pEVPPKey);
	return *this;
}


EVPPKey& EVPPKey::operator=(EVPPKey&& other)
{
	_pEVPPKey = other._pEVPPKey;
	other._pEVPPKey = nullptr;
	return *this;
}


EVPPKey::~EVPPKey()
{
	if (_pEVPPKey) EVP_PKEY_free(_pEVPPKey);
}


void EVPPKey::duplicate(EVP_PKEY* pEVPPKey)
{
	if (!pEVPPKey) throw NullPointerException("EVPPKey::duplicate(): "
		"provided key pointer is null.");

	_pEVPPKey = EVP_PKEY_new();
	if (!_pEVPPKey) throw NullPointerException("EVPPKey::duplicate(): "
		"EVP_PKEY_new() returned null.");

	switch (pEVPPKey->type)
	{
		case EVP_PKEY_RSA:
		{
			RSA* pRSA = EVP_PKEY_get1_RSA(pEVPPKey);
			if (pRSA)
			{
				EVP_PKEY_set1_RSA(_pEVPPKey, pRSA);
				RSA_free(pRSA);
			}
			else throw OpenSSLException();
			break;
		}
		case EVP_PKEY_EC:
		{
			EC_KEY* pEC = EVP_PKEY_get1_EC_KEY(pEVPPKey);
			if (pEC)
			{
				EVP_PKEY_set1_EC_KEY(_pEVPPKey, pEC);
				EC_KEY_free(pEC);
			}
			else throw OpenSSLException();
			break;
		}
		default:
			throw NotImplementedException("EVPPKey:duplicate(); Key type: " +
				NumberFormatter::format(pEVPPKey->type));
	}
}


} } // namespace Poco::Crypto
