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
#include "Poco/Crypto/PKCS12Container.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/ECKey.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/NumberFormatter.h"
#include <iostream>

#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#include <openssl/param_build.h>
#endif

namespace Poco {
namespace Crypto {


const std::map<int, std::string> EVPPKey::KNOWN_TYPES =
	{
		{ EVP_PKEY_RSA, "rsa" },
		// not implemented
		//{ EVP_PKEY_DSA, "dsa" },
		//{ EVP_PKEY_DH, "dh" },
		{ EVP_PKEY_EC, "ec" }
	};


EVPPKey::EVPPKey(const std::string& ecCurveName): _pEVPPKey(0)
{
	newECKey(ecCurveName.c_str());
	poco_check_ptr(_pEVPPKey);
	checkType();
}


EVPPKey::EVPPKey(const char* ecCurveName): _pEVPPKey(0)
{
	newECKey(ecCurveName);
	poco_check_ptr(_pEVPPKey);
	checkType();
}


EVPPKey::EVPPKey(const X509Certificate& cert):
	_pEVPPKey(X509_get_pubkey(const_cast<X509*>(cert.certificate())))
{
	poco_check_ptr(_pEVPPKey);
	checkType();
}


EVPPKey::EVPPKey(const PKCS12Container& cont): EVPPKey(cont.getKey())
{
	poco_check_ptr(_pEVPPKey);
	checkType();
}

#if OPENSSL_VERSION_NUMBER >= 0x30000000L

void pushBuildParamBignum(OSSL_PARAM_BLD* paramBld, const char* key, const std::vector<unsigned char>& bytes, BIGNUM** pBigNum)
{
	poco_check_ptr(pBigNum);
	if (!(*pBigNum = BN_bin2bn(bytes.data(), (int)bytes.size(), nullptr)))
	{
		std::string msg = "pushBuildParamBignum(): BN_bin2bn()\n";
		throw OpenSSLException(getError(msg));
	}

	OSSL_PARAM_BLD_push_BN(paramBld, key, *pBigNum);
}


OSSL_PARAM* getKeyParameters(const std::vector<unsigned char>* publicKey, const std::vector<unsigned char>* privateKey)
{
	BIGNUM* pBigNum1 = nullptr;
	BIGNUM* pBigNum2 = nullptr;
	OSSL_PARAM* parameters = nullptr;
	auto paramBld = OSSL_PARAM_BLD_new();
	if (!paramBld)
	{
		std::string msg = "getKeyParameters(): OSSL_PARAM_BLD_new()\n";
		throw OpenSSLException(getError(msg));
	}

	try
	{
		if (publicKey != nullptr)
			pushBuildParamBignum(paramBld, "n", *publicKey, &pBigNum1);

		if (privateKey != nullptr)
			pushBuildParamBignum(paramBld, "d", *privateKey, &pBigNum2);

		// default rsa exponent
		OSSL_PARAM_BLD_push_ulong(paramBld, "e", RSA_F4);

		parameters = OSSL_PARAM_BLD_to_param(paramBld);
		if (!parameters)
		{
			std::string msg = "getKeyParameters(): OSSL_PARAM_BLD_to_param()\n";
			throw OpenSSLException(getError(msg));
		}
	}
	catch(OpenSSLException&)
	{
		OSSL_PARAM_BLD_free(paramBld);
		throw;
	}

	OSSL_PARAM_BLD_free(paramBld);
	BN_clear_free(pBigNum1);
	BN_clear_free(pBigNum2);

	return parameters;
}


void EVPPKey::setKeyFromParameters(OSSL_PARAM* parameters)
{
	auto ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
	if (EVP_PKEY_fromdata_init(ctx) <= 0)
	{
		OSSL_PARAM_free(parameters);
		EVP_PKEY_CTX_free(ctx);
		throw OpenSSLException("EVPPKey cannot init create key");
	}

	if (_pEVPPKey != 0) EVP_PKEY_free(_pEVPPKey);
	if (EVP_PKEY_fromdata(ctx, &_pEVPPKey, EVP_PKEY_KEYPAIR, parameters) <= 0)
	{
		OSSL_PARAM_free(parameters);
		EVP_PKEY_CTX_free(ctx);
		throw OpenSSLException("EVPPKey cannot create key");
	}

	EVP_PKEY_CTX_free(ctx);
}


EVPPKey::EVPPKey(const std::vector<unsigned char>* public_key, const std::vector<unsigned char>* private_key, unsigned long exponent, int type) : _pEVPPKey(0)
{
	if ((EVP_PKEY_RSA != type) || (RSA_F4 != exponent))
	{
		std::string msg = Poco::format("EVPPKey(%d):Invalid format\n", type);
		throw OpenSSLException(getError(msg));
	}

	OSSL_PARAM* parameters = getKeyParameters(public_key, private_key);
	if (parameters)
	{
		setKeyFromParameters(parameters);
		OSSL_PARAM_free(parameters);
	}
	else
	{
		std::string msg = "EVPPKey(): getKeyParameters()\n";
		throw OpenSSLException(getError(msg));
	}
}

#endif
	
#if OPENSSL_VERSION_NUMBER >= 0x10000000L

EVPPKey::EVPPKey(int type, int param): _pEVPPKey(0)
{
	EVP_PKEY_CTX *pCtx = EVP_PKEY_CTX_new_id(type, NULL);
	if (NULL == pCtx)
	{
		std::string msg = Poco::format(
			"EVPPKey(%d, %d):EVP_PKEY_CTX_new_id()\n", type, param);
		throw OpenSSLException(getError(msg));
	}
	int ret = EVP_PKEY_keygen_init(pCtx);
	if (ret != 1)
	{
		std::string msg = Poco::format(
			"EVPPKey(%d, %d):EVP_PKEY_keygen_init()\n", type, param);
		throw OpenSSLException(getError(msg));
	}

	if (EVP_PKEY_RSA == type)
	{
		ret = EVP_PKEY_CTX_set_rsa_keygen_bits(pCtx, param);
		if (ret != 1)
		{
			std::string msg = Poco::format(
				"EVPPKey(%d, %d):EVP_PKEY_CTX_set_rsa_keygen_bits()\n", type, param);
			throw OpenSSLException(getError(msg));
		}
	}
	else if (EVP_PKEY_EC == type)
	{
		ret = EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pCtx, param);
		if (ret != 1)
		{
			std::string msg = Poco::format(
				"EVPPKey(%d, %d):EVP_PKEY_CTX_set_ec_paramgen_curve_nid()\n", type, param);
			throw OpenSSLException(getError(msg));
		}
	}
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	ret = EVP_PKEY_generate(pCtx, &_pEVPPKey);
	if (ret != 1)
	{
		std::string msg = Poco::format(
			"EVPPKey(%d, %d):EVP_PKEY_generate()\n", type, param);
		throw OpenSSLException(getError(msg));
	}
#else
	ret = EVP_PKEY_keygen(pCtx, &_pEVPPKey);
	if (ret != 1)
	{
		std::string msg = Poco::format(
			"EVPPKey(%d, %d):EVP_PKEY_keygen()\n", type, param);
		throw OpenSSLException(getError(msg));
	}
#endif
	EVP_PKEY_CTX_free(pCtx);
	checkType();
}

#endif // OPENSSL_VERSION_NUMBER >= 0x10000000L


EVPPKey::EVPPKey(EVP_PKEY* pEVPPKey): _pEVPPKey(0)
{
	duplicate(pEVPPKey, &_pEVPPKey);
	poco_check_ptr(_pEVPPKey);
	checkType();
}


EVPPKey::EVPPKey(const std::string& publicKeyFile,
	const std::string& privateKeyFile,
	const std::string& privateKeyPassphrase): _pEVPPKey(0)
{
	if (loadKey(&_pEVPPKey, PEM_read_PrivateKey, (EVP_PKEY_get_Key_fn)0, privateKeyFile, privateKeyPassphrase))
	{
		poco_check_ptr(_pEVPPKey);
		return; // private key is enough
	}

	// no private key, this must be public key only, otherwise throw
	if (!loadKey(&_pEVPPKey, PEM_read_PUBKEY, (EVP_PKEY_get_Key_fn)0, publicKeyFile))
	{
		std::string msg = "EVPPKey(const string&, const string&, const string&)\n";
		throw OpenSSLException(getError(msg));
	}
	poco_check_ptr(_pEVPPKey);
	checkType();
}


EVPPKey::EVPPKey(std::istream* pPublicKeyStream,
	std::istream* pPrivateKeyStream,
	const std::string& privateKeyPassphrase): _pEVPPKey(0)
{
	if (loadKey(&_pEVPPKey, PEM_read_bio_PrivateKey, (EVP_PKEY_get_Key_fn)0, pPrivateKeyStream, privateKeyPassphrase))
	{
		poco_check_ptr(_pEVPPKey);
		return; // private key is enough
	}

	// no private key, this must be public key only, otherwise throw
	if (!loadKey(&_pEVPPKey, PEM_read_bio_PUBKEY, (EVP_PKEY_get_Key_fn)0, pPublicKeyStream))
	{
		std::string msg = "EVPPKey(istream* ,istream* const string&)\n";
		throw OpenSSLException(getError(msg));
	}
	poco_check_ptr(_pEVPPKey);
	checkType();
}


EVPPKey::EVPPKey(const EVPPKey& other)
{
	duplicate(other._pEVPPKey, &_pEVPPKey);
	poco_check_ptr(_pEVPPKey);
	checkType();
}


EVPPKey::EVPPKey(EVPPKey&& other) noexcept:
	_pEVPPKey(other._pEVPPKey)
{
	other._pEVPPKey = nullptr;
	checkType();
}


EVPPKey& EVPPKey::operator = (const EVPPKey& other)
{
	duplicate(other._pEVPPKey, &_pEVPPKey);
	poco_check_ptr(_pEVPPKey);
	checkType();
	return *this;
}


EVPPKey& EVPPKey::operator = (EVPPKey&& other) noexcept
{
	_pEVPPKey = other._pEVPPKey;
	other._pEVPPKey = nullptr;
	checkType();
	return *this;
}


EVPPKey::~EVPPKey()
{
	if (_pEVPPKey) EVP_PKEY_free(_pEVPPKey);
}


const std::string& EVPPKey::name() const
{
	int t = type(_pEVPPKey);
	auto it = KNOWN_TYPES.find(t);
	if (it == KNOWN_TYPES.end())
		throw Poco::NotImplementedException(Poco::format("EVPPKey::type(%d)", t));
	return it->second;
}


void EVPPKey::checkType()
{
	if (_pEVPPKey)
	{
		int t = type(_pEVPPKey);
		if (KNOWN_TYPES.find(t) == KNOWN_TYPES.end())
			throw Poco::NotImplementedException(Poco::format("EVPPKey::type(%d)", t));
	}
}


void EVPPKey::setKey(EC_KEY* pKey)
{
	if (!EVP_PKEY_set1_EC_KEY(_pEVPPKey, pKey))
	{
		std::string msg = "EVPPKey::setKey('EC')\n";
		throw OpenSSLException(getError(msg));
	}
}


void EVPPKey::setKey(RSA* pKey)
{
	if (!EVP_PKEY_set1_RSA(_pEVPPKey, pKey))
	{
		std::string msg = "EVPPKey::setKey('RSA')\n";
		throw OpenSSLException(getError(msg));
	}
}


void EVPPKey::save(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase) const
{
	if (!publicKeyFile.empty() && (publicKeyFile != privateKeyFile))
	{
		BIO* bio = BIO_new(BIO_s_file());
		if (!bio)
		{
			std::string msg = Poco::format(
				"EVPPKey::save(%s) Cannot create BIO for writing public key file\n", publicKeyFile);
			throw Poco::IOException(getError(msg));
		}
		try
		{
			if (BIO_write_filename(bio, const_cast<char*>(publicKeyFile.c_str())))
			{
				if (!PEM_write_bio_PUBKEY(bio, _pEVPPKey))
				{
					std::string msg = Poco::format("EVPPKey::save('%s', '%s', '%s')\n",
						publicKeyFile, privateKeyFile, privateKeyPassphrase);
					msg.append(Poco::format("Failed to write public key '%s' to file", publicKeyFile));
					throw Poco::WriteFileException(getError(msg));
				}
			}
			else throw Poco::CreateFileException("Cannot create public key file");
		}
		catch (...)
		{
			BIO_free(bio);
			throw;
		}
		BIO_free(bio);
	}

	if (!privateKeyFile.empty())
	{
		BIO* bio = BIO_new(BIO_s_file());
		if (!bio)
		{
			std::string msg = Poco::format(
				"EVPPKey::save(%s): Cannot create BIO for writing private key file\n", privateKeyFile);
			throw Poco::IOException(getError(msg));
		}
		try
		{
			if (BIO_write_filename(bio, const_cast<char*>(privateKeyFile.c_str())))
			{
				int rc = 0;
				if (privateKeyPassphrase.empty())
				{
					rc = PEM_write_bio_PrivateKey(bio, _pEVPPKey, 0, 0, 0, 0, 0);
				}
				else
				{
					rc = PEM_write_bio_PrivateKey(bio, _pEVPPKey, EVP_des_ede3_cbc(),
						reinterpret_cast<unsigned char*>(const_cast<char*>(privateKeyPassphrase.c_str())),
						static_cast<int>(privateKeyPassphrase.length()), 0, 0);
				}
				if (!rc)
				{
					std::string msg = Poco::format(
						"EVPPKey::save(%s):PEM_write_bio_PrivateKey()\n", privateKeyFile);
					throw Poco::FileException(getError(msg));
				}
			}
			else
			{
				std::string msg = Poco::format(
					"EVPPKey::save(%s):BIO_write_filename()\n", privateKeyFile);
				throw Poco::CreateFileException(getError(msg));
			}
		}
		catch (...)
		{
			BIO_free(bio);
			throw;
		}
		BIO_free(bio);
	}
}


void EVPPKey::save(std::ostream* pPublicKeyStream, std::ostream* pPrivateKeyStream, const std::string& privateKeyPassphrase) const
{
	if (pPublicKeyStream && (pPublicKeyStream != pPrivateKeyStream))
	{
		BIO* bio = BIO_new(BIO_s_mem());
		if (!bio)
		{
			std::string msg = "EVPPKey::save(ostream*, ostream*, const string&)\n";
			msg.append("Cannot create BIO for writing public key");
			throw Poco::IOException(getError(msg));
		}
		if (!PEM_write_bio_PUBKEY(bio, _pEVPPKey))
		{
			std::string msg = "EVPPKey::save(ostream*, ostream*, const string&)\n";
			msg.append("Failed to write public key to stream");
			BIO_free(bio);
			throw Poco::WriteFileException(getError(msg));
		}
		char* pData;
		long size = BIO_get_mem_data(bio, &pData);
		pPublicKeyStream->write(pData, static_cast<std::streamsize>(size));
		BIO_free(bio);
	}

	if (pPrivateKeyStream)
	{
		BIO* bio = BIO_new(BIO_s_mem());
		if (!bio)
		{
			std::string msg = "EVPPKey::save(ostream*, ostream*, const string&)\n";
			msg.append("Cannot create BIO for writing private key");
			throw Poco::IOException(getError(msg));
		}
		int rc = 0;
		if (privateKeyPassphrase.empty())
			rc = PEM_write_bio_PrivateKey(bio, _pEVPPKey, 0, 0, 0, 0, 0);
		else
			rc = PEM_write_bio_PrivateKey(bio, _pEVPPKey, EVP_des_ede3_cbc(),
				reinterpret_cast<unsigned char*>(const_cast<char*>(privateKeyPassphrase.c_str())),
				static_cast<int>(privateKeyPassphrase.length()), 0, 0);
		if (!rc)
		{
			std::string msg = "EVPPKey::save(ostream*, ostream*, const string&)\n";
			msg.append("Failed to write private key to stream");
			BIO_free(bio);
			throw Poco::FileException(getError(msg));
		}
		char* pData;
		long size = BIO_get_mem_data(bio, &pData);
		pPrivateKeyStream->write(pData, static_cast<std::streamsize>(size));
		BIO_free(bio);
	}
}


EVP_PKEY* EVPPKey::duplicate(const EVP_PKEY* pFromKey, EVP_PKEY** pToKey)
{
	if (!pFromKey) throw NullPointerException("EVPPKey::duplicate(): "
		"provided key pointer is null.");

	*pToKey = EVP_PKEY_new();
	if (!*pToKey)
	{
		std::string msg = "EVPPKey::duplicate():EVP_PKEY_new()\n";
		throw NullPointerException(getError(msg));
	}

	int keyType = type(pFromKey);
	switch (keyType)
	{
		case EVP_PKEY_RSA:
		{
			RSA* pRSA = EVP_PKEY_get1_RSA(const_cast<EVP_PKEY*>(pFromKey));
			if (pRSA)
			{
				EVP_PKEY_set1_RSA(*pToKey, pRSA);
				RSA_free(pRSA);
			}
			else
			{
				std::string msg = "EVPPKey::duplicate():EVP_PKEY_get1_RSA()\n";
				throw OpenSSLException(getError(msg));
			}
			break;
		}
		case EVP_PKEY_EC:
		{
			EC_KEY* pEC = EVP_PKEY_get1_EC_KEY(const_cast<EVP_PKEY*>(pFromKey));
			if (pEC)
			{
				EVP_PKEY_set1_EC_KEY(*pToKey, pEC);
				EC_KEY_free(pEC);
				int cmp = EVP_PKEY_cmp_parameters(*pToKey, pFromKey);
				if (cmp < 0)
				{
					std::string msg = "EVPPKey::duplicate():EVP_PKEY_cmp_parameters()\n";
					throw OpenSSLException(getError(msg));
				}
				if (0 == cmp)
				{
					if(!EVP_PKEY_copy_parameters(*pToKey, pFromKey))
					{
						std::string msg = "EVPPKey::duplicate():EVP_PKEY_copy_parameters()\n";
						throw OpenSSLException(getError(msg));
					}
				}
			}
			else
			{
				std::string msg = "EVPPKey::duplicate():EVP_PKEY_get1_EC_KEY()\n";
				throw OpenSSLException(getError(msg));
			}
			break;
		}
		default:
			throw NotImplementedException("EVPPKey:duplicate(); Key type: " +
				NumberFormatter::format(keyType));
	}

	return *pToKey;
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
	std::string msg = "EVPPKey::newECKey()\n";
	throw OpenSSLException(getError(msg));
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


int EVPPKey::passCB(char* buf, int size, int, void* pass)
{
	if (pass)
	{
		int len = (int)std::strlen((char*)pass);
		if(len > size) len = size;
		std::memcpy(buf, pass, len);
		return len;
	}
	return 0;
}


} } // namespace Poco::Crypto
