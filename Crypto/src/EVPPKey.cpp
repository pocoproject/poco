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
#include "Poco/Crypto/ECKey.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/NumberFormatter.h"


namespace Poco {
namespace Crypto {


EVPPKey::EVPPKey(const std::string& ecCurveName): _pEVPPKey(0)
{
	newECKey(ecCurveName.c_str());
}


EVPPKey::EVPPKey(const char* ecCurveName): _pEVPPKey(0)
{
	newECKey(ecCurveName);
}


EVPPKey::EVPPKey(EVP_PKEY* pEVPPKey): _pEVPPKey(0)
{
	duplicate(pEVPPKey);
}


EVPPKey::EVPPKey(const EVPPKey& other)
{
	duplicate(other._pEVPPKey);
}


EVPPKey& EVPPKey::operator=(const EVPPKey& other)
{
	duplicate(other._pEVPPKey);
	return *this;
}


#ifdef POCO_ENABLE_CPP11

EVPPKey::EVPPKey(EVPPKey&& other): _pEVPPKey(other._pEVPPKey)
{
	other._pEVPPKey = nullptr;
}


EVPPKey& EVPPKey::operator=(EVPPKey&& other)
{
	_pEVPPKey = other._pEVPPKey;
	other._pEVPPKey = nullptr;
	return *this;
}

#endif // POCO_ENABLE_CPP11

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


void EVPPKey::newECKey(const char* ecCurveName)
{
	int curveID = OBJ_txt2nid(ecCurveName);
	EC_KEY* pEC = EC_KEY_new_by_curve_name(curveID);
	if (!pEC) goto err;
	if (!EC_KEY_generate_key(pEC)) goto err;
	_pEVPPKey = EVP_PKEY_new();
	if (!_pEVPPKey) goto err;
	if (!EVP_PKEY_set1_EC_KEY(_pEVPPKey, pEC)) goto err;
	EC_KEY_free(pEC);
	return;
err:
	throw OpenSSLException();
}


void EVPPKey::setKey(ECKey* pKey)
{
	poco_check_ptr(pKey);
	poco_check_ptr(pKey->impl());
	setKey(pKey->impl()->getECKey());
}


void EVPPKey::setKey(RSAKey* pKey)
{
	poco_check_ptr(pKey);
	poco_check_ptr(pKey->impl());
	setKey(pKey->impl()->getRSA());
}

} } // namespace Poco::Crypto
