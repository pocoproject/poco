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
	if (loadKey(PEM_read_PrivateKey, privateKeyFile, privateKeyPassphrase))
		return; // private key is enough

	// no private key, this must be public key only, otherwise throw
	if (!loadKey(PEM_read_PUBKEY, publicKeyFile))
	{
		throw OpenSSLException("ECKeyImpl(const string&, const string&, const string&");
	}
}


ECKeyImpl::ECKeyImpl(std::istream* pPublicKeyStream,
	std::istream* pPrivateKeyStream,
	const std::string& privateKeyPassphrase): KeyPairImpl("ec", KT_EC_IMPL), _pEC(0)
{
	if (loadKey(PEM_read_bio_PrivateKey, pPrivateKeyStream, privateKeyPassphrase))
		return; // private key is enough

	// no private key, this must be public key only, otherwise throw
	if (!loadKey(PEM_read_bio_PUBKEY, pPublicKeyStream))
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


int ECKeyImpl::passCB(char* buf, int size, int, void* pass)
{
	int len = (int) strlen((char*)pass);
	if (len > size) len = size;
	memcpy(buf, pass, len);
	return len;
}


bool ECKeyImpl::loadKey(PEM_read_Key_fn func, const std::string& keyFile, const std::string& pass)
{
	if (!keyFile.empty())
	{
		EVP_PKEY* pKey = EVP_PKEY_new();
		if (pKey)
		{
			FILE* pFile = fopen(keyFile.c_str(), "r");
			if (pFile)
			{
				func(pFile, &pKey, passCB, pass.empty() ? (void*)0 : (void*)pass.c_str());
				_pEC = EVP_PKEY_get1_EC_KEY(pKey);
				EVP_PKEY_free(pKey);
				if (!_pEC) goto error;
				return true;
			}
			else
			{
				EVP_PKEY_free(pKey);
				throw IOException("ECKeyImpl, cannot open file", keyFile);
			}
		}
		else goto error;
	}
	return false;

error:
	throw OpenSSLException("ECKeyImpl(const string&, const string&, const string&)");
}


bool ECKeyImpl::loadKey(PEM_read_bio_Key_fn func, std::istream* pIstr, const std::string& pass)
{
	if (pIstr)
	{
		std::ostringstream ostr;
		Poco::StreamCopier::copyStream(*pIstr, ostr);
		std::string key = ostr.str();
		BIO *pBIO = BIO_new_mem_buf(const_cast<char*>(key.data()), static_cast<int>(key.size()));
		if (pBIO)
		{
			EVP_PKEY* pKey = EVP_PKEY_new();
			if (pKey)
			{
				if (func(pBIO, &pKey, passCB, pass.empty() ? (void*)0 : (void*)pass.c_str()))
				{
					_pEC = EVP_PKEY_get1_EC_KEY(pKey);
					EVP_PKEY_free(pKey);
					BIO_free(pBIO);
					if (!_pEC) goto error;
					return true;
				}
				goto error;
			}
			else
			{
				BIO_free(pBIO);
				goto error;
			}
		}
		else goto error;
	}
	return false;

error:
	throw OpenSSLException("ECKeyImpl::ECKeyImpl(const string&, const string&, const string&)");
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


std::string ECKeyImpl::groupName() const
{
	if (_pEC)
	{
		const EC_GROUP* ecGroup = EC_KEY_get0_group(_pEC);
		if (ecGroup)
		{
			return OBJ_nid2sn(EC_GROUP_get_curve_name(ecGroup));
		}
		else
		{
			throw OpenSSLException("ECKeyImpl::groupName()");
		}
	}
	throw NullPointerException("ECKeyImpl::groupName() => _pEC");
}


void ECKeyImpl::save(const std::string& publicKeyFile,
	const std::string& privateKeyFile,
	const std::string& privateKeyPassphrase)
{
	if (!publicKeyFile.empty())
	{
		BIO* bio = BIO_new(BIO_s_file());
		if (!bio) throw Poco::IOException("Cannot create BIO for writing public key file", publicKeyFile);
		try
		{
			if (BIO_write_filename(bio, const_cast<char*>(publicKeyFile.c_str())))
			{
				EVPPKey pKey(_pEC);
				if (!PEM_write_bio_PUBKEY(bio, pKey))
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
				EVPPKey pKey(_pEC);
				int rc = 0;
				if (privateKeyPassphrase.empty())
				{
					rc = PEM_write_bio_PrivateKey(bio, pKey, 0, 0, 0, 0, 0);
				}
				else
				{
					rc = PEM_write_bio_PrivateKey(bio, pKey, EVP_des_ede3_cbc(),
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


void ECKeyImpl::save(std::ostream* pPublicKeyStream,
	std::ostream* pPrivateKeyStream,
	const std::string& privateKeyPassphrase)
{
	if (pPublicKeyStream)
	{
		BIO* bio = BIO_new(BIO_s_mem());
		if (!bio) throw Poco::IOException("Cannot create BIO for writing public key");
		EVPPKey pKey(_pEC);
		if (!PEM_write_bio_PUBKEY(bio, pKey))
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
		EVPPKey pKey(_pEC);
		int rc = 0;
		if (privateKeyPassphrase.empty())
			rc = PEM_write_bio_PrivateKey(bio, pKey, 0, 0, 0, 0, 0);
		else
			rc = PEM_write_bio_PrivateKey(bio, pKey, EVP_des_ede3_cbc(), 
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


ECKeyImpl::ByteVec ECKeyImpl::convertToByteVec(const BIGNUM* bn)
{
	int numBytes = BN_num_bytes(bn);
	ByteVec byteVector(numBytes);

	ByteVec::value_type* buffer = new ByteVec::value_type[numBytes];
	BN_bn2bin(bn, buffer);

	for (int i = 0; i < numBytes; ++i)
		byteVector[i] = buffer[i];

	delete [] buffer;

	return byteVector;
}


} } // namespace Poco::Crypto
