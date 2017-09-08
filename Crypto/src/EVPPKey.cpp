//
// EVPPKey.cpp
//
// $Id: //poco/1.4/Crypto/src/EVPPKey.cpp#2 $
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
	if (!_pEVPPKey) poco_unexpected();
	EVP_PKEY_free(_pEVPPKey);
}


void EVPPKey::duplicate(EVP_PKEY* pEVPPKey)
{
	if (!pEVPPKey) throw NullPointerException("EVPPKey:duplicate()");
	EVP_PKEY* pDupKey = EVP_PKEY_new();
	switch (pEVPPKey->type)
	{
		case EVP_PKEY_RSA:
		{
			RSA* pRSA = EVP_PKEY_get1_RSA(pEVPPKey);
			EVP_PKEY_set1_RSA(pDupKey, pRSA);
			RSA_free(pRSA);
			break;
		}
		case EVP_PKEY_EC:
		{
			EC_KEY* pEC = EVP_PKEY_get1_EC_KEY(pEVPPKey);
			EVP_PKEY_set1_EC_KEY(pDupKey, pEC);
			EC_KEY_free(pEC);
			break;
		}
		default:
			EVP_PKEY_free(pDupKey);
			throw NotImplementedException("EVPPKey:duplicate(); Key type: " +
				NumberFormatter::format(pEVPPKey->type));
	}
	poco_assert_dbg(pDupKey);
	_pEVPPKey = pDupKey;
}


} } // namespace Poco::Crypto
