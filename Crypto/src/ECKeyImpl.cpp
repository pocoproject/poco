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
#include "Poco/Format.h"
#include "Poco/StreamCopier.h"
#include <algorithm>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/bn.h>

#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
#include <openssl/core_names.h>
#endif


namespace Poco {
namespace Crypto {


#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)


// OpenSSL 3.0+ implementation using EVP_PKEY


ECKeyImpl::ECKeyImpl(const EVPPKey& key):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEVPPKey(nullptr)
{
	EVPPKey::duplicate(const_cast<EVP_PKEY*>((const EVP_PKEY*)key), &_pEVPPKey);
	safeCheckEC("ECKeyImpl(const EVPPKey&)", "EVP_PKEY_dup()");
}


ECKeyImpl::ECKeyImpl(const X509Certificate& cert):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEVPPKey(nullptr)
{
	const X509* pCert = cert.certificate();
	if (pCert != nullptr)
	{
		_pEVPPKey = X509_get_pubkey(const_cast<X509*>(pCert));
		if (_pEVPPKey != nullptr)
		{
			safeCheckEC("ECKeyImpl(const X509Certificate&)", "X509_get_pubkey()");
			return;
		}
	}
	throw OpenSSLException("ECKeyImpl(const X509Certificate&)");
}


ECKeyImpl::ECKeyImpl(const PKCS12Container& cont):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEVPPKey(nullptr)
{
	EVPPKey key = cont.getKey();
	EVPPKey::duplicate(static_cast<EVP_PKEY*>(key), &_pEVPPKey);
	safeCheckEC("ECKeyImpl(const PKCS12Container&)", "EVP_PKEY_dup()");
}


ECKeyImpl::ECKeyImpl(int curve):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEVPPKey(nullptr)
{
	EVP_PKEY_CTX* pCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, nullptr);
	if (pCtx == nullptr)
		throw OpenSSLException("ECKeyImpl: EVP_PKEY_CTX_new_id()");
	if (EVP_PKEY_keygen_init(pCtx) != 1)
	{
		EVP_PKEY_CTX_free(pCtx);
		throw OpenSSLException("ECKeyImpl: EVP_PKEY_keygen_init()");
	}
	if (EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pCtx, curve) != 1)
	{
		EVP_PKEY_CTX_free(pCtx);
		throw OpenSSLException("ECKeyImpl: EVP_PKEY_CTX_set_ec_paramgen_curve_nid()");
	}
	if (EVP_PKEY_generate(pCtx, &_pEVPPKey) != 1)
	{
		EVP_PKEY_CTX_free(pCtx);
		throw OpenSSLException("ECKeyImpl(int curve): EVP_PKEY_generate()");
	}
	EVP_PKEY_CTX_free(pCtx);
	safeCheckEC("ECKeyImpl(int curve)", "EVP_PKEY_generate()");
}


ECKeyImpl::ECKeyImpl(const std::string& publicKeyFile,
	const std::string& privateKeyFile,
	const std::string& privateKeyPassphrase): KeyPairImpl("ec", KT_EC_IMPL), _pEVPPKey(nullptr)
{
	EVP_PKEY* pKey = nullptr;
	if (EVPPKey::loadKey(&pKey, PEM_read_PrivateKey, (EVPPKey::EVP_PKEY_get_Key_fn) nullptr, privateKeyFile, privateKeyPassphrase))
	{
		_pEVPPKey = pKey;
		safeCheckEC(Poco::format("ECKeyImpl(%s, %s, %s)",
			publicKeyFile, privateKeyFile, privateKeyPassphrase.empty() ? privateKeyPassphrase : std::string("***")),
			"PEM_read_PrivateKey()");
		return;
	}

	if (!EVPPKey::loadKey(&pKey, PEM_read_PUBKEY, (EVPPKey::EVP_PKEY_get_Key_fn) nullptr, publicKeyFile))
	{
		throw OpenSSLException("ECKeyImpl(const string&, const string&, const string&");
	}
	_pEVPPKey = pKey;
	safeCheckEC(Poco::format("ECKeyImpl(%s, %s, %s)",
		publicKeyFile, privateKeyFile, privateKeyPassphrase.empty() ? privateKeyPassphrase : std::string("***")),
		"PEM_read_PUBKEY()");
}


ECKeyImpl::ECKeyImpl(std::istream* pPublicKeyStream,
	std::istream* pPrivateKeyStream,
	const std::string& privateKeyPassphrase): KeyPairImpl("ec", KT_EC_IMPL), _pEVPPKey(nullptr)
{
	EVP_PKEY* pKey = nullptr;
	if (EVPPKey::loadKey(&pKey, PEM_read_bio_PrivateKey, (EVPPKey::EVP_PKEY_get_Key_fn) nullptr, pPrivateKeyStream, privateKeyPassphrase))
	{
		_pEVPPKey = pKey;
		safeCheckEC(Poco::format("ECKeyImpl(stream, stream, %s)",
			privateKeyPassphrase.empty() ? privateKeyPassphrase : std::string("***")),
			"PEM_read_bio_PrivateKey()");
		return;
	}

	if (!EVPPKey::loadKey(&pKey, PEM_read_bio_PUBKEY, (EVPPKey::EVP_PKEY_get_Key_fn) nullptr, pPublicKeyStream))
	{
		throw OpenSSLException("ECKeyImpl(istream*, istream*, const string&");
	}
	_pEVPPKey = pKey;
	safeCheckEC(Poco::format("ECKeyImpl(stream, stream, %s)",
		privateKeyPassphrase.empty() ? privateKeyPassphrase : std::string("***")),
		"PEM_read_bio_PUBKEY()");
}


ECKeyImpl::~ECKeyImpl()
{
	freeEC();
}


void ECKeyImpl::checkEC(const std::string& method, const std::string& func) const
{
	if (_pEVPPKey == nullptr) throw OpenSSLException(Poco::format("%s: %s", method, func));
	EVP_PKEY_CTX* pCtx = EVP_PKEY_CTX_new(_pEVPPKey, nullptr);
	if (pCtx == nullptr) throw OpenSSLException(Poco::format("%s: EVP_PKEY_CTX_new()", method));
	int rc = EVP_PKEY_check(pCtx);
	if (rc != 1)
	{
		// public-key-only: EVP_PKEY_check may fail, try EVP_PKEY_public_check
		rc = EVP_PKEY_public_check(pCtx);
	}
	EVP_PKEY_CTX_free(pCtx);
	if (rc != 1)
		throw OpenSSLException(Poco::format("%s: EVP_PKEY_check()", method));
}


void ECKeyImpl::safeCheckEC(const std::string& method, const std::string& func)
{
	try
	{
		checkEC(method, func);
	}
	catch (...)
	{
		freeEC();
		throw;
	}
}


void ECKeyImpl::freeEC()
{
	if (_pEVPPKey != nullptr)
	{
		EVP_PKEY_free(_pEVPPKey);
		_pEVPPKey = nullptr;
	}
}


int ECKeyImpl::size() const
{
	return EVP_PKEY_bits(_pEVPPKey);
}


int ECKeyImpl::groupId() const
{
	if (_pEVPPKey != nullptr)
	{
		char groupName[80];
		size_t len = 0;
		if (EVP_PKEY_get_utf8_string_param(_pEVPPKey, OSSL_PKEY_PARAM_GROUP_NAME, groupName, sizeof(groupName), &len))
		{
			return OBJ_sn2nid(groupName);
		}
		throw OpenSSLException("ECKeyImpl::groupId(): EVP_PKEY_get_utf8_string_param()");
	}
	throw NullPointerException("ECKeyImpl::groupId() => _pEVPPKey");
}


#ifndef OPENSSL_NO_DEPRECATED_3_0

#if defined(__GNUC__) || defined(__clang__)
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable:4996)
#endif

EC_KEY* ECKeyImpl::getECKey()
{
	return const_cast<EC_KEY*>(EVP_PKEY_get0_EC_KEY(_pEVPPKey));
}


const EC_KEY* ECKeyImpl::getECKey() const
{
	return EVP_PKEY_get0_EC_KEY(_pEVPPKey);
}

#if defined(__GNUC__) || defined(__clang__)
#	pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif // !OPENSSL_NO_DEPRECATED_3_0


#else // !POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)


// OpenSSL 1.1.x implementation using EC_KEY


ECKeyImpl::ECKeyImpl(const EVPPKey& key):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEC(EVP_PKEY_get1_EC_KEY(const_cast<EVP_PKEY*>((const EVP_PKEY*)key)))
{
	checkEC("ECKeyImpl(const EVPPKey&)", "EVP_PKEY_get1_EC_KEY()");
}


ECKeyImpl::ECKeyImpl(const X509Certificate& cert):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEC(nullptr)
{
	const X509* pCert = cert.certificate();
	if (pCert)
	{
		EVP_PKEY* pKey = X509_get_pubkey(const_cast<X509*>(pCert));
		if (pKey)
		{
			_pEC = EVP_PKEY_get1_EC_KEY(pKey);
			EVP_PKEY_free(pKey);
			checkEC("ECKeyImpl(const const X509Certificate&)", "EVP_PKEY_get1_EC_KEY()");
			return;
		}
	}
	throw OpenSSLException("ECKeyImpl(const X509Certificate&)");
}


ECKeyImpl::ECKeyImpl(const PKCS12Container& cont):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEC(EVP_PKEY_get1_EC_KEY(cont.getKey()))
{
	checkEC("ECKeyImpl(const PKCS12Container&)", "EVP_PKEY_get1_EC_KEY()");
}


ECKeyImpl::ECKeyImpl(int curve):
	KeyPairImpl("ec", KT_EC_IMPL),
	_pEC(EC_KEY_new_by_curve_name(curve))
{
	poco_check_ptr(_pEC);
	EC_KEY_set_asn1_flag(_pEC, OPENSSL_EC_NAMED_CURVE);
	if (!(EC_KEY_generate_key(_pEC)))
		throw OpenSSLException("ECKeyImpl(int curve): EC_KEY_generate_key()");
	checkEC("ECKeyImpl(int curve)", "EC_KEY_generate_key()");
}


ECKeyImpl::ECKeyImpl(const std::string& publicKeyFile,
	const std::string& privateKeyFile,
	const std::string& privateKeyPassphrase): KeyPairImpl("ec", KT_EC_IMPL), _pEC(nullptr)
{
	if (EVPPKey::loadKey(&_pEC, PEM_read_PrivateKey, EVP_PKEY_get1_EC_KEY, privateKeyFile, privateKeyPassphrase))
	{
		checkEC(Poco::format("ECKeyImpl(%s, %s, %s)",
			publicKeyFile, privateKeyFile, privateKeyPassphrase.empty() ? privateKeyPassphrase : std::string("***")),
			"PEM_read_PrivateKey() or EVP_PKEY_get1_EC_KEY()");
		return; // private key is enough
	}

	// no private key, this must be public key only, otherwise throw
	if (!EVPPKey::loadKey(&_pEC, PEM_read_PUBKEY, EVP_PKEY_get1_EC_KEY, publicKeyFile))
	{
		throw OpenSSLException("ECKeyImpl(const string&, const string&, const string&");
	}
	checkEC(Poco::format("ECKeyImpl(%s, %s, %s)",
		publicKeyFile, privateKeyFile, privateKeyPassphrase.empty() ? privateKeyPassphrase : std::string("***")),
		"PEM_read_PUBKEY() or EVP_PKEY_get1_EC_KEY()");
}


ECKeyImpl::ECKeyImpl(std::istream* pPublicKeyStream,
	std::istream* pPrivateKeyStream,
	const std::string& privateKeyPassphrase): KeyPairImpl("ec", KT_EC_IMPL), _pEC(nullptr)
{
	if (EVPPKey::loadKey(&_pEC, PEM_read_bio_PrivateKey, EVP_PKEY_get1_EC_KEY, pPrivateKeyStream, privateKeyPassphrase))
	{
		checkEC(Poco::format("ECKeyImpl(stream, stream, %s)",
			privateKeyPassphrase.empty() ? privateKeyPassphrase : std::string("***")),
			"PEM_read_bio_PrivateKey() or EVP_PKEY_get1_EC_KEY()");
		return; // private key is enough
	}

	// no private key, this must be public key only, otherwise throw
	if (!EVPPKey::loadKey(&_pEC, PEM_read_bio_PUBKEY, EVP_PKEY_get1_EC_KEY, pPublicKeyStream))
	{
		throw OpenSSLException("ECKeyImpl(istream*, istream*, const string&");
	}
	checkEC(Poco::format("ECKeyImpl(stream, stream, %s)",
		privateKeyPassphrase.empty() ? privateKeyPassphrase : std::string("***")),
		"PEM_read_bio_PUBKEY() or EVP_PKEY_get1_EC_KEY()");
}


ECKeyImpl::~ECKeyImpl()
{
	freeEC();
}


void ECKeyImpl::checkEC(const std::string& method, const std::string& func) const
{
	if (!_pEC) throw OpenSSLException(Poco::format("%s: %s", method, func));
	if (!EC_KEY_check_key(_pEC))
		throw OpenSSLException(Poco::format("%s: EC_KEY_check_key()", method));
}


void ECKeyImpl::freeEC()
{
	if (_pEC)
	{
		EC_KEY_free(_pEC);
		_pEC = nullptr;
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


#endif // POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)


// Static methods (shared between versions -- EC_get_builtin_curves is not deprecated)


std::string ECKeyImpl::getCurveName(int nid)
{
	std::string curveName;
	size_t len = EC_get_builtin_curves(nullptr, 0);
	EC_builtin_curve* pCurves =
			(EC_builtin_curve*) OPENSSL_malloc(sizeof(EC_builtin_curve) * len);
	if (!pCurves) return curveName;

	if (!EC_get_builtin_curves(pCurves, len))
	{
		OPENSSL_free(pCurves);
		return curveName;
	}

	if (-1 == nid) nid = pCurves[0].nid;
	const int bufLen = 128;
	char buf[bufLen];
	std::memset(buf, 0, bufLen);
	OBJ_obj2txt(buf, bufLen, OBJ_nid2obj(nid), 0);
	curveName = buf;
	OPENSSL_free(pCurves);
	return curveName;
}


int ECKeyImpl::getCurveNID(std::string& name)
{
	std::string curveName;
	size_t len = EC_get_builtin_curves(nullptr, 0);
	EC_builtin_curve* pCurves =
		(EC_builtin_curve*)OPENSSL_malloc(static_cast<int>(sizeof(EC_builtin_curve) * len));
	if (!pCurves) return -1;

	if (!EC_get_builtin_curves(pCurves, len))
	{
		OPENSSL_free(pCurves);
		return -1;
	}

	int nid = -1;
	const int bufLen = 128;
	char buf[bufLen];
	if (name.empty())
	{
		std::memset(buf, 0, bufLen);
		OBJ_obj2txt(buf, bufLen, OBJ_nid2obj(nid), 0);
		name = buf;
		nid = pCurves[0].nid;
	}
	else
	{
		for (std::size_t i = 0; i < len; ++i)
		{
			std::memset(buf, 0, bufLen);
			OBJ_obj2txt(buf, bufLen, OBJ_nid2obj(pCurves[i].nid), 0);
			if (strncmp(name.c_str(), buf, std::min(name.size(), static_cast<std::size_t>(bufLen))) == 0)
			{
				nid = pCurves[i].nid;
				break;
			}
		}
	}

	OPENSSL_free(pCurves);
	return nid;
}


bool ECKeyImpl::hasCurve(const std::string& name)
{
	std::string tmp(name);
	return (-1 != getCurveNID(tmp));
}


} } // namespace Poco::Crypto
