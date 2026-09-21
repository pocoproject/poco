//
// RSAKeyImpl.cpp
//
// Library: Crypto
// Package: RSA
// Module:  RSAKeyImpl
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/RSAKeyImpl.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/PKCS12Container.h"
#include "Poco/StreamCopier.h"
#include <openssl/pem.h>
#include <openssl/evp.h>

#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
#include <openssl/core_names.h>
#include <openssl/param_build.h>
#else
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#endif


namespace Poco::Crypto {


#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)


// OpenSSL 3.0+ implementation using EVP_PKEY


RSAKeyImpl::RSAKeyImpl(const EVPPKey& key):
	KeyPairImpl("rsa", KT_RSA_IMPL),
	_pEVPPKey(nullptr)
{
	EVPPKey::duplicate(const_cast<EVP_PKEY*>((const EVP_PKEY*)key), &_pEVPPKey);
	ensureRSAKey(_pEVPPKey, "EVPPKey");
}


RSAKeyImpl::RSAKeyImpl(const X509Certificate& cert):
	KeyPairImpl("rsa", KT_RSA_IMPL),
	_pEVPPKey(nullptr)
{
	const X509* pCert = cert.certificate();
	if (pCert == nullptr)
		throw OpenSSLException("RSAKeyImpl(const X509Certificate&): null certificate");
	_pEVPPKey = X509_get_pubkey(const_cast<X509*>(pCert));
	ensureRSAKey(_pEVPPKey, "X509Certificate");
}


RSAKeyImpl::RSAKeyImpl(const PKCS12Container& cont):
	KeyPairImpl("rsa", KT_RSA_IMPL),
	_pEVPPKey(nullptr)
{
	EVPPKey key = cont.getKey();
	EVPPKey::duplicate(static_cast<EVP_PKEY*>(key), &_pEVPPKey);
	ensureRSAKey(_pEVPPKey, "PKCS12Container");
}


RSAKeyImpl::RSAKeyImpl(int keyLength, unsigned long exponent):
	KeyPairImpl("rsa", KT_RSA_IMPL),
	_pEVPPKey(nullptr)
{
	EVP_PKEY_CTX* pCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
	if (pCtx == nullptr)
		throw OpenSSLException("RSAKeyImpl: EVP_PKEY_CTX_new_id()");
	if (EVP_PKEY_keygen_init(pCtx) != 1)
	{
		EVP_PKEY_CTX_free(pCtx);
		throw OpenSSLException("RSAKeyImpl: EVP_PKEY_keygen_init()");
	}
	if (EVP_PKEY_CTX_set_rsa_keygen_bits(pCtx, keyLength) != 1)
	{
		EVP_PKEY_CTX_free(pCtx);
		throw OpenSSLException("RSAKeyImpl: EVP_PKEY_CTX_set_rsa_keygen_bits()");
	}
	if (exponent != RSA_F4)
	{
		BIGNUM* bn = BN_new();
		if (bn == nullptr)
		{
			EVP_PKEY_CTX_free(pCtx);
			throw OpenSSLException("RSAKeyImpl: BN_new()");
		}
		if (BN_set_word(bn, exponent) != 1 || EVP_PKEY_CTX_set1_rsa_keygen_pubexp(pCtx, bn) != 1)
		{
			BN_free(bn);
			EVP_PKEY_CTX_free(pCtx);
			throw OpenSSLException("RSAKeyImpl: BN_set_word()/EVP_PKEY_CTX_set1_rsa_keygen_pubexp()");
		}
		BN_free(bn);
	}
	if (EVP_PKEY_generate(pCtx, &_pEVPPKey) != 1)
	{
		EVP_PKEY_CTX_free(pCtx);
		throw OpenSSLException("RSAKeyImpl: EVP_PKEY_generate()");
	}
	EVP_PKEY_CTX_free(pCtx);
}


RSAKeyImpl::RSAKeyImpl(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase):
	KeyPairImpl("rsa", KT_RSA_IMPL),
	_pEVPPKey(nullptr)
{
	if (EVPPKey::loadKey(&_pEVPPKey, PEM_read_PrivateKey, (EVPPKey::EVP_PKEY_get_Key_fn) nullptr, privateKeyFile, privateKeyPassphrase))
	{
		ensureRSAKey(_pEVPPKey, "file");
		return;
	}

	if (!EVPPKey::loadKey(&_pEVPPKey, PEM_read_PUBKEY, (EVPPKey::EVP_PKEY_get_Key_fn) nullptr, publicKeyFile))
	{
		throw OpenSSLException("RSAKeyImpl(const string&, const string&, const string&)");
	}
	ensureRSAKey(_pEVPPKey, "file");
}


RSAKeyImpl::RSAKeyImpl(std::istream* pPublicKeyStream, std::istream* pPrivateKeyStream, const std::string& privateKeyPassphrase):
	KeyPairImpl("rsa", KT_RSA_IMPL),
	_pEVPPKey(nullptr)
{
	if (EVPPKey::loadKey(&_pEVPPKey, PEM_read_bio_PrivateKey, (EVPPKey::EVP_PKEY_get_Key_fn) nullptr, pPrivateKeyStream, privateKeyPassphrase))
	{
		ensureRSAKey(_pEVPPKey, "stream");
		return;
	}

	if (!EVPPKey::loadKey(&_pEVPPKey, PEM_read_bio_PUBKEY, (EVPPKey::EVP_PKEY_get_Key_fn) nullptr, pPublicKeyStream))
	{
		throw OpenSSLException("RSAKeyImpl(istream*, istream*, const string&)");
	}
	ensureRSAKey(_pEVPPKey, "stream");
}


RSAKeyImpl::~RSAKeyImpl()
{
	freeRSA();
}


void RSAKeyImpl::freeRSA()
{
	if (_pEVPPKey != nullptr) EVP_PKEY_free(_pEVPPKey);
	_pEVPPKey = nullptr;
}


#ifndef OPENSSL_NO_DEPRECATED_3_0

#if defined(__GNUC__) || defined(__clang__)
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable:4996)
#endif

RSA* RSAKeyImpl::getRSA()
{
	return const_cast<RSA*>(EVP_PKEY_get0_RSA(_pEVPPKey));
}


const RSA* RSAKeyImpl::getRSA() const
{
	return EVP_PKEY_get0_RSA(_pEVPPKey);
}

#if defined(__GNUC__) || defined(__clang__)
#	pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif


int RSAKeyImpl::size() const
{
	return EVP_PKEY_get_size(_pEVPPKey);
}


void RSAKeyImpl::ensureRSAKey(EVP_PKEY* pKey, const std::string& context)
{
	if (pKey == nullptr)
		throw OpenSSLException("RSAKeyImpl(" + context + ")");
	if (EVP_PKEY_base_id(pKey) != EVP_PKEY_RSA)
	{
		EVP_PKEY_free(pKey);
		throw OpenSSLException("RSAKeyImpl(" + context + "): not an RSA key");
	}
}


RSAKeyImpl::ByteVec RSAKeyImpl::keyParam(const char* name, bool clearFree) const
{
	BIGNUM* bn = nullptr;
	if (!EVP_PKEY_get_bn_param(_pEVPPKey, name, &bn))
		return ByteVec();
	int numBytes = BN_num_bytes(bn);
	ByteVec byteVector(numBytes);
	BN_bn2bin(bn, byteVector.data());
	if (clearFree)
		BN_clear_free(bn);
	else
		BN_free(bn);
	return byteVector;
}


RSAKeyImpl::ByteVec RSAKeyImpl::modulus() const
{
	ByteVec result = keyParam(OSSL_PKEY_PARAM_RSA_N);
	if (result.empty())
		throw OpenSSLException("RSAKeyImpl::modulus()");
	return result;
}


RSAKeyImpl::ByteVec RSAKeyImpl::encryptionExponent() const
{
	ByteVec result = keyParam(OSSL_PKEY_PARAM_RSA_E);
	if (result.empty())
		throw OpenSSLException("RSAKeyImpl::encryptionExponent()");
	return result;
}


RSAKeyImpl::ByteVec RSAKeyImpl::decryptionExponent() const
{
	return keyParam(OSSL_PKEY_PARAM_RSA_D, true);
}


void RSAKeyImpl::save(const std::string& publicKeyFile,
	const std::string& privateKeyFile,
	const std::string& privateKeyPassphrase) const
{
	EVPPKey(_pEVPPKey).save(publicKeyFile, privateKeyFile, privateKeyPassphrase);
}


void RSAKeyImpl::save(std::ostream* pPublicKeyStream,
	std::ostream* pPrivateKeyStream,
	const std::string& privateKeyPassphrase) const
{
	EVPPKey(_pEVPPKey).save(pPublicKeyStream, pPrivateKeyStream, privateKeyPassphrase);
}


#else // !POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)


// OpenSSL 1.1.x implementation using RSA


RSAKeyImpl::RSAKeyImpl(const EVPPKey& key):
	KeyPairImpl("rsa", KT_RSA_IMPL),
	_pRSA(EVP_PKEY_get1_RSA(const_cast<EVP_PKEY*>((const EVP_PKEY*)key)))
{
	if (!_pRSA) throw OpenSSLException();
}


RSAKeyImpl::RSAKeyImpl(const X509Certificate& cert):
	KeyPairImpl("rsa", KT_RSA_IMPL),
	_pRSA(nullptr)
{
	const X509* pCert = cert.certificate();
	EVP_PKEY* pKey = X509_get_pubkey(const_cast<X509*>(pCert));
	if (pKey != nullptr)
	{
		_pRSA = EVP_PKEY_get1_RSA(pKey);
		EVP_PKEY_free(pKey);
	}
	if (_pRSA == nullptr)
		throw OpenSSLException("RSAKeyImpl(const X509Certificate&)");
}


RSAKeyImpl::RSAKeyImpl(const PKCS12Container& cont):
	KeyPairImpl("rsa", KT_RSA_IMPL),
	_pRSA(nullptr)
{
	EVPPKey key = cont.getKey();
	_pRSA = EVP_PKEY_get1_RSA(key);
	if (_pRSA == nullptr)
		throw OpenSSLException("RSAKeyImpl(const PKCS12Container&)");
}


RSAKeyImpl::RSAKeyImpl(int keyLength, unsigned long exponent) : KeyPairImpl("rsa", KT_RSA_IMPL),
	_pRSA(nullptr)
{
	_pRSA = RSA_new();
	int ret = 0;
	BIGNUM* bn = nullptr;
	try
	{
		bn = BN_new();
		if (_pRSA == nullptr || bn == nullptr || BN_set_word(bn, exponent) != 1)
			throw OpenSSLException("RSAKeyImpl: RSA_new()/BN_new()/BN_set_word()");
		ret = RSA_generate_key_ex(_pRSA, keyLength, bn, nullptr);
		BN_free(bn);
	}
	catch (...)
	{
		BN_free(bn);
		freeRSA();
		throw;
	}
	if (!ret)
	{
		freeRSA();
		std::string msg = "Failed to create RSA context";
		throw Poco::InvalidArgumentException(getError(msg));
	}
}


RSAKeyImpl::RSAKeyImpl(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase):
	KeyPairImpl("rsa", KT_RSA_IMPL),
	_pRSA(nullptr)
{
	poco_assert_dbg(_pRSA == nullptr);

	_pRSA = RSA_new();
	if (_pRSA == nullptr)
		throw OpenSSLException("RSAKeyImpl: RSA_new()");
	if (!publicKeyFile.empty())
	{
		BIO* bio = BIO_new(BIO_s_file());
		if (!bio) throw Poco::IOException("Cannot create BIO for reading public key", publicKeyFile);
		int rc = BIO_read_filename(bio, publicKeyFile.c_str());
		if (rc)
		{
			ERR_set_mark();
			RSA* pubKey = PEM_read_bio_RSAPublicKey(bio, &_pRSA, nullptr, nullptr);
			if (!pubKey)
			{
				int rc = BIO_reset(bio);
				// BIO_reset() normally returns 1 for success and 0 or -1 for failure.
				// File BIOs are an exception, they return 0 for success and -1 for failure.
				if (rc != 0)
				{
					ERR_clear_last_mark();
					std::string msg = "Failed to load public key";
					throw Poco::FileException(getError(msg), publicKeyFile);
				}
				pubKey = PEM_read_bio_RSA_PUBKEY(bio, &_pRSA, nullptr, nullptr);
			}
			// The errors of the first attempt are expected for a SubjectPublicKeyInfo key.
			if (pubKey != nullptr) ERR_pop_to_mark();
			else ERR_clear_last_mark();
			BIO_free(bio);
			if (!pubKey)
			{
				freeRSA();
				std::string msg = "Failed to load public key";
				throw Poco::FileException(getError(msg), publicKeyFile);
			}
		}
		else
		{
			freeRSA();
			std::string msg = "Public key file";
			throw Poco::FileNotFoundException(getError(msg), publicKeyFile);
		}
	}

	if (!privateKeyFile.empty())
	{
		BIO* bio = BIO_new(BIO_s_file());
		if (!bio) throw Poco::IOException("Cannot create BIO for reading private key", privateKeyFile);
		int rc = BIO_read_filename(bio, privateKeyFile.c_str());
		if (rc)
		{
			RSA* privKey = nullptr;
			if (privateKeyPassphrase.empty())
				privKey = PEM_read_bio_RSAPrivateKey(bio, &_pRSA, nullptr, nullptr);
			else
				privKey = PEM_read_bio_RSAPrivateKey(bio, &_pRSA, nullptr, const_cast<char *>( privateKeyPassphrase.c_str()));
			BIO_free(bio);
			if (!privKey)
			{
				freeRSA();
				std::string msg = "Failed to load private key";
				throw Poco::FileException(getError(msg), privateKeyFile);
			}
		}
		else
		{
			freeRSA();
			std::string msg = "Private key file";
			throw Poco::FileNotFoundException(getError(msg), privateKeyFile);
		}
	}
}


RSAKeyImpl::RSAKeyImpl(std::istream* pPublicKeyStream, std::istream* pPrivateKeyStream, const std::string& privateKeyPassphrase):
	KeyPairImpl("rsa", KT_RSA_IMPL),
	_pRSA(nullptr)
{
	poco_assert_dbg(_pRSA == nullptr);

	_pRSA = RSA_new();
	if (_pRSA == nullptr)
		throw OpenSSLException("RSAKeyImpl: RSA_new()");
	if (pPublicKeyStream)
	{
		std::string publicKeyData;
		Poco::StreamCopier::copyToString(*pPublicKeyStream, publicKeyData);
		BIO* bio = BIO_new_mem_buf(const_cast<char*>(publicKeyData.data()), static_cast<int>(publicKeyData.size()));
		if (!bio) throw Poco::IOException("Cannot create BIO for reading public key");
		ERR_set_mark();
		RSA* publicKey = PEM_read_bio_RSAPublicKey(bio, &_pRSA, nullptr, nullptr);
		if (!publicKey)
		{
			int rc = BIO_reset(bio);
			// BIO_reset() normally returns 1 for success and 0 or -1 for failure.
			// File BIOs are an exception, they return 0 for success and -1 for failure.
			if (rc != 1)
			{
				ERR_clear_last_mark();
				std::string msg = "Failed to load public key";
				throw Poco::FileException(getError(msg));
			}
			publicKey = PEM_read_bio_RSA_PUBKEY(bio, &_pRSA, nullptr, nullptr);
		}
		// The errors of the first attempt are expected for a SubjectPublicKeyInfo key.
		if (publicKey != nullptr) ERR_pop_to_mark();
		else ERR_clear_last_mark();
		BIO_free(bio);
		if (!publicKey)
		{
			freeRSA();
			std::string msg = "Failed to load public key";
			throw Poco::FileException(getError(msg));
		}
	}

	if (pPrivateKeyStream)
	{
		std::string privateKeyData;
		Poco::StreamCopier::copyToString(*pPrivateKeyStream, privateKeyData);
		BIO* bio = BIO_new_mem_buf(const_cast<char*>(privateKeyData.data()), static_cast<int>(privateKeyData.size()));
		if (!bio) throw Poco::IOException("Cannot create BIO for reading private key");
		RSA* privateKey = nullptr;
		if (privateKeyPassphrase.empty())
			privateKey = PEM_read_bio_RSAPrivateKey(bio, &_pRSA, nullptr, nullptr);
		else
			privateKey = PEM_read_bio_RSAPrivateKey(bio, &_pRSA, nullptr, const_cast<char *>(privateKeyPassphrase.c_str()));
		BIO_free(bio);
		if (!privateKey)
		{
			freeRSA();
			std::string msg = "Failed to load private key";
			throw Poco::FileException(getError(msg));
		}
	}
}


RSAKeyImpl::~RSAKeyImpl()
{
	freeRSA();
}


void RSAKeyImpl::freeRSA()
{
	if (_pRSA) RSA_free(_pRSA);
	_pRSA = nullptr;
}


int RSAKeyImpl::size() const
{
	return RSA_size(_pRSA);
}


RSAKeyImpl::ByteVec RSAKeyImpl::modulus() const
{
	const BIGNUM* n = nullptr;
	const BIGNUM* e = nullptr;
	const BIGNUM* d = nullptr;
	RSA_get0_key(_pRSA, &n, &e, &d);
	return convertToByteVec(n);
}


RSAKeyImpl::ByteVec RSAKeyImpl::encryptionExponent() const
{
	const BIGNUM* n = nullptr;
	const BIGNUM* e = nullptr;
	const BIGNUM* d = nullptr;
	RSA_get0_key(_pRSA, &n, &e, &d);
	return convertToByteVec(e);
}


RSAKeyImpl::ByteVec RSAKeyImpl::decryptionExponent() const
{
	const BIGNUM* n = nullptr;
	const BIGNUM* e = nullptr;
	const BIGNUM* d = nullptr;
	RSA_get0_key(_pRSA, &n, &e, &d);
	return convertToByteVec(d);
}


void RSAKeyImpl::save(const std::string& publicKeyFile,
	const std::string& privateKeyFile,
	const std::string& privateKeyPassphrase) const
{
	if (!publicKeyFile.empty())
	{
		BIO* bio = BIO_new(BIO_s_file());
		if (!bio) throw Poco::IOException("Cannot create BIO for writing public key file", publicKeyFile);
		try
		{
			if (BIO_write_filename(bio, const_cast<char*>(publicKeyFile.c_str())))
			{
				if (!PEM_write_bio_RSAPublicKey(bio, _pRSA))
				{
					std::string msg = "Failed to write public key to file";
					throw Poco::WriteFileException(getError(msg), publicKeyFile);
				}
				if (BIO_flush(bio) != 1)
				{
					std::string msg = "Failed to flush public key to file";
					throw Poco::WriteFileException(getError(msg), publicKeyFile);
				}
			}
			else
			{
				std::string msg = "Cannot create public key file";
				throw Poco::CreateFileException(getError(msg), publicKeyFile);
			}
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
					rc = PEM_write_bio_RSAPrivateKey(bio, _pRSA, nullptr, nullptr, 0, nullptr, nullptr);
				else
					rc = PEM_write_bio_RSAPrivateKey(bio, _pRSA, EVP_des_ede3_cbc(),
							 reinterpret_cast<unsigned char *>( const_cast<char *>( privateKeyPassphrase.c_str())),
							 static_cast<int>(privateKeyPassphrase.length()), nullptr, nullptr);
				if (!rc)
				{
					std::string msg = "Failed to write private key to file";
					throw Poco::FileException(getError(msg), privateKeyFile);
				}
				if (BIO_flush(bio) != 1)
				{
					std::string msg = "Failed to flush private key to file";
					throw Poco::WriteFileException(getError(msg), privateKeyFile);
				}
			}
			else
			{
				std::string msg = "Cannot create private key file";
				throw Poco::CreateFileException(getError(msg), privateKeyFile);
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


void RSAKeyImpl::save(std::ostream* pPublicKeyStream,
	std::ostream* pPrivateKeyStream,
	const std::string& privateKeyPassphrase) const
{
	if (pPublicKeyStream)
	{
		BIO* bio = BIO_new(BIO_s_mem());
		if (!bio) throw Poco::IOException("Cannot create BIO for writing public key");
		if (!PEM_write_bio_RSAPublicKey(bio, _pRSA))
		{
			BIO_free(bio);
			std::string msg = "Failed to write public key to stream";
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
		if (!bio) throw Poco::IOException("Cannot create BIO for writing public key");
		int rc = 0;
		if (privateKeyPassphrase.empty())
			rc = PEM_write_bio_RSAPrivateKey(bio, _pRSA, nullptr, nullptr, 0, nullptr, nullptr);
		else
			rc = PEM_write_bio_RSAPrivateKey(bio,
					_pRSA, EVP_des_ede3_cbc(),
					reinterpret_cast<unsigned char *>( const_cast<char *>(privateKeyPassphrase.c_str())),
					static_cast<int>(privateKeyPassphrase.length()), nullptr, nullptr);
		if (!rc)
		{
			BIO_free(bio);
			std::string msg = "Failed to write private key to stream";
			throw Poco::FileException(getError(msg));
		}
		char* pData;
		long size = BIO_get_mem_data(bio, &pData);
		pPrivateKeyStream->write(pData, static_cast<std::streamsize>(size));
		BIO_free(bio);
	}
}


RSAKeyImpl::ByteVec RSAKeyImpl::convertToByteVec(const BIGNUM* bn)
{
	if (bn == nullptr) return ByteVec();
	int numBytes = BN_num_bytes(bn);
	ByteVec byteVector(numBytes);

	ByteVec::value_type* buffer = new ByteVec::value_type[numBytes];
	BN_bn2bin(bn, buffer);

	for (int i = 0; i < numBytes; ++i)
		byteVector[i] = buffer[i];

	delete [] buffer;

	return byteVector;
}


#endif // POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)


} // namespace Poco::Crypto
