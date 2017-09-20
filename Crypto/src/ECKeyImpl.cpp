//
// ECKeyImpl.cpp
//
//
// Library: Crypto
// Package: EC
// Module:  ECKeyImpl
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/ECKeyImpl.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/PKCS12Container.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include <sstream>
#include <openssl/evp.h>
#if OPENSSL_VERSION_NUMBER >= 0x00908000L
#include <openssl/bn.h>
#endif


namespace Poco {
namespace Crypto {


ECKeyImpl::ECKeyImpl(const EVPPKey& key):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEC(EVP_PKEY_get1_EC_KEY(const_cast<EVP_PKEY*>((const EVP_PKEY*)key)))
{
	if (!_pEC) throw OpenSSLException("ECKeyImpl(const EVPPKey&)");
}


ECKeyImpl::ECKeyImpl(const X509Certificate& cert):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEC(0)
{
	const X509* pCert = cert.certificate();
	if (pCert)
	{
		EVP_PKEY* pKey = X509_get_pubkey(const_cast<X509*>(pCert));
		if (pKey)
		{
			_pEC = EVP_PKEY_get1_EC_KEY(pKey);
			EVP_PKEY_free(pKey);
			return;
		}
	}
	throw OpenSSLException("ECKeyImpl(const X509Certificate&)");
}


ECKeyImpl::ECKeyImpl(const PKCS12Container& cont):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEC(EVP_PKEY_get1_EC_KEY(cont.getKey()))
{
	if (!_pEC) throw OpenSSLException();
}


ECKeyImpl::ECKeyImpl(int curve):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEC(EC_KEY_new_by_curve_name(curve))
{
	if (!EC_KEY_generate_key(_pEC))
		throw OpenSSLException("ECKeyImpl(int)");
}


ECKeyImpl::ECKeyImpl(const std::string& publicKeyFile, 
	const std::string& privateKeyFile, 
	const std::string& privateKeyPassphrase): KeyPairImpl("ec", KT_EC_IMPL), _pEC(0)
{
	if (EVPPKey::loadKey(&_pEC, PEM_read_PrivateKey, EVP_PKEY_get1_EC_KEY, privateKeyFile, privateKeyPassphrase))
		return; // private key is enough

	// no private key, this must be public key only, otherwise throw
	if (!EVPPKey::loadKey(&_pEC, PEM_read_PUBKEY, EVP_PKEY_get1_EC_KEY, publicKeyFile))
	{
		throw OpenSSLException("ECKeyImpl(const string&, const string&, const string&");
	}
}


ECKeyImpl::ECKeyImpl(std::istream* pPublicKeyStream,
	std::istream* pPrivateKeyStream,
	const std::string& privateKeyPassphrase): KeyPairImpl("ec", KT_EC_IMPL), _pEC(0)
{
	if (EVPPKey::loadKey(&_pEC, PEM_read_bio_PrivateKey, EVP_PKEY_get1_EC_KEY, pPrivateKeyStream, privateKeyPassphrase))
		return; // private key is enough

	// no private key, this must be public key only, otherwise throw
	if (!EVPPKey::loadKey(&_pEC, PEM_read_bio_PUBKEY, EVP_PKEY_get1_EC_KEY, pPublicKeyStream))
	{
		throw OpenSSLException("ECKeyImpl(istream*, istream*, const string&");
	}
}


ECKeyImpl::~ECKeyImpl()
{
	freeEC();
}


void ECKeyImpl::freeEC()
{
	if (_pEC)
	{
		EC_KEY_free(_pEC);
		_pEC = 0;
	}
}


int ECKeyImpl::size() const
{
	int sz = -1;
	EVP_PKEY* pKey = EVP_PKEY_new();
	if (pKey && EVP_PKEY_set1_EC_KEY(pKey, _pEC))
	{
		sz = EVP_PKEY_bits(pKey);
		EVP_PKEY_free(pKey);
		return sz;
	}
	throw OpenSSLException("ECKeyImpl::size()");
}


int ECKeyImpl::groupId() const
{
	if (_pEC)
	{
		const EC_GROUP* ecGroup = EC_KEY_get0_group(_pEC);
		if (ecGroup)
		{
			return EC_GROUP_get_curve_name(ecGroup);
		}
		else
		{
			throw OpenSSLException("ECKeyImpl::groupName()");
		}
	}
	throw NullPointerException("ECKeyImpl::groupName() => _pEC");
}


} } // namespace Poco::Crypto
