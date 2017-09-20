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


EVPPKey::EVPPKey(const std::string& publicKeyFile,
	const std::string& privateKeyFile,
	const std::string& privateKeyPassphrase): _pEVPPKey(0)
{
	if (loadKey(&_pEVPPKey, PEM_read_PrivateKey, (EVP_PKEY_get_Key_fn)0, privateKeyFile, privateKeyPassphrase))
		return; // private key is enough

	// no private key, this must be public key only, otherwise throw
	if (!loadKey(&_pEVPPKey, PEM_read_PUBKEY, (EVP_PKEY_get_Key_fn)0, publicKeyFile))
	{
		throw OpenSSLException("ECKeyImpl(const string&, const string&, const string&");
	}
}


EVPPKey::EVPPKey(std::istream* pPublicKeyStream,
	std::istream* pPrivateKeyStream,
	const std::string& privateKeyPassphrase): _pEVPPKey(0)
{
	if (loadKey(&_pEVPPKey, PEM_read_bio_PrivateKey, (EVP_PKEY_get_Key_fn)0, pPrivateKeyStream, privateKeyPassphrase))
		return; // private key is enough

	// no private key, this must be public key only, otherwise throw
	if (!loadKey(&_pEVPPKey, PEM_read_bio_PUBKEY, (EVP_PKEY_get_Key_fn)0, pPublicKeyStream))
	{
		throw OpenSSLException("ECKeyImpl(istream*, istream*, const string&");
	}
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


void EVPPKey::save(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase)
{
	if (!publicKeyFile.empty())
	{
		BIO* bio = BIO_new(BIO_s_file());
		if (!bio) throw Poco::IOException("Cannot create BIO for writing public key file", publicKeyFile);
		try
		{
			if (BIO_write_filename(bio, const_cast<char*>(publicKeyFile.c_str())))
			{
				if (!PEM_write_bio_PUBKEY(bio, _pEVPPKey))
				{
					throw Poco::WriteFileException("Failed to write public key to file", publicKeyFile);
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
		if (!bio) throw Poco::IOException("Cannot create BIO for writing private key file", privateKeyFile);
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
					throw Poco::FileException("Failed to write private key to file", privateKeyFile);
			}
			else throw Poco::CreateFileException("Cannot create private key file", privateKeyFile);
		}
		catch (...)
		{
			BIO_free(bio);
			throw;
		}
		BIO_free(bio);
	}
}


void EVPPKey::save(std::ostream* pPublicKeyStream, std::ostream* pPrivateKeyStream, const std::string& privateKeyPassphrase)
{
	if (pPublicKeyStream)
	{
		BIO* bio = BIO_new(BIO_s_mem());
		if (!bio) throw Poco::IOException("Cannot create BIO for writing public key");
		if (!PEM_write_bio_PUBKEY(bio, _pEVPPKey))
		{
			BIO_free(bio);
			throw Poco::WriteFileException("Failed to write public key to stream");
		}
		char* pData;
		long size = BIO_get_mem_data(bio, &pData);
		pPublicKeyStream->write(pData, static_cast<std::streamsize>(size));
		BIO_free(bio);
	}

	if (pPrivateKeyStream)
	{
		BIO* bio = BIO_new(BIO_s_mem());
		if (!bio) throw Poco::IOException("Cannot create BIO for writing public key");
		int rc = 0;
		if (privateKeyPassphrase.empty())
			rc = PEM_write_bio_PrivateKey(bio, _pEVPPKey, 0, 0, 0, 0, 0);
		else
			rc = PEM_write_bio_PrivateKey(bio, _pEVPPKey, EVP_des_ede3_cbc(),
				reinterpret_cast<unsigned char*>(const_cast<char*>(privateKeyPassphrase.c_str())),
				static_cast<int>(privateKeyPassphrase.length()), 0, 0);
		if (!rc)
		{
			BIO_free(bio);
			throw Poco::FileException("Failed to write private key to stream");
		}
		char* pData;
		long size = BIO_get_mem_data(bio, &pData);
		pPrivateKeyStream->write(pData, static_cast<std::streamsize>(size));
		BIO_free(bio);
	}
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
	throw OpenSSLException("EVPPKey:newECKey()");
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
	int len = (int) strlen((char*)pass);
	if (len > size) len = size;
	memcpy(buf, pass, len);
	return len;
}


} } // namespace Poco::Crypto
