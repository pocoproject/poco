//
// EVPPKey.h
//
//
// Library: Crypto
// Package: CryptoCore
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
#include "Poco/Crypto/CryptoException.h"
#include "Poco/StreamCopier.h"
#include <openssl/ec.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <sstream>


namespace Poco {
namespace Crypto {


class ECKey;
class RSAKey;


class Crypto_API EVPPKey
	/// Utility class for conversion of native keys to EVP.
	/// Currently, only RSA and EC keys are supported.
{
public:
	explicit EVPPKey(const std::string& ecCurveName);
		/// Constructs EVPPKey from ECC curve name.
		///
		/// Only EC keys can be wrapped by an EVPPKey
		/// created using this constructor.

	explicit EVPPKey(const char* ecCurveName);
		/// Constructs EVPPKey from ECC curve name.
		///
		/// Only EC keys can be wrapped by an EVPPKey
		/// created using this constructor.

	explicit EVPPKey(EVP_PKEY* pEVPPKey);
		/// Constructs EVPPKey from EVP_PKEY pointer.
		/// The content behind the supplied pointer is internally duplicated.

	template<typename K>
	explicit EVPPKey(K* pKey): _pEVPPKey(EVP_PKEY_new())
		/// Constructs EVPPKey from a "native" OpenSSL (RSA or EC_KEY),
		/// or a Poco wrapper (RSAKey, ECKey) key pointer.
	{
		if (!_pEVPPKey) throw OpenSSLException();
		setKey(pKey);
	}

	EVPPKey(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase = "");
		/// Creates the EVPPKey, by reading public and private key from the given files and
		/// using the given passphrase for the private key. Can only by used for signing if
		/// a private key is available.

	EVPPKey(std::istream* pPublicKeyStream, std::istream* pPrivateKeyStream, const std::string& privateKeyPassphrase = "");
		/// Creates the EVPPKey. Can only by used for signing if pPrivKey
		/// is not null. If a private key file is specified, you don't need to
		/// specify a public key file. OpenSSL will auto-create it from the private key.

	EVPPKey(const EVPPKey& other);
		/// Copy constructor.

	EVPPKey& operator=(const EVPPKey& other);
		/// Assignment operator.

#ifdef POCO_ENABLE_CPP11
	EVPPKey(EVPPKey&& other);
		/// Move constructor.

	EVPPKey& operator=(EVPPKey&& other);
		/// Assignment move operator.
#endif // POCO_ENABLE_CPP11

	~EVPPKey();
		/// Destroys the EVPPKey.

	void save(const std::string& publicKeyFile, const std::string& privateKeyFile = "", const std::string& privateKeyPassphrase = "");
		/// Exports the public and/or private keys to the given files.
		///
		/// If an empty filename is specified, the corresponding key
		/// is not exported.

	void save(std::ostream* pPublicKeyStream, std::ostream* pPrivateKeyStream = 0, const std::string& privateKeyPassphrase = "");
		/// Exports the public and/or private key to the given streams.
		///
		/// If a null pointer is passed for a stream, the corresponding
		/// key is not exported.

	int type() const;
		/// Retuns the EVPPKey type NID.

	bool isSupported(int type) const;
		/// Returns true if OpenSSL type is supported

	operator const EVP_PKEY*() const;
		/// Returns const pointer to the OpenSSL EVP_PKEY structure.

	operator EVP_PKEY*();
		/// Returns pointer to the OpenSSL EVP_PKEY structure.

private:
	EVPPKey();

	void newECKey(const char* group);

	void duplicate(EVP_PKEY* pEVPPKey);

	void setKey(ECKey* pKey);
	void setKey(RSAKey* pKey);
	void setKey(EC_KEY* pKey);
	void setKey(RSA* pKey);
	static int passCB(char* buf, int size, int, void* pass);

	typedef EVP_PKEY* (*PEM_read_FILE_Key_fn)(FILE*, EVP_PKEY**, pem_password_cb*, void*);
	typedef EVP_PKEY* (*PEM_read_BIO_Key_fn)(BIO*, EVP_PKEY**, pem_password_cb*, void*);
	typedef void* (*EVP_PKEY_get_Key_fn)(EVP_PKEY* pkey);

	// The following load*() functions are used by both native and EVP_PKEY type key
	// loading from BIO/FILE.
	// When used for EVP key loading, getFunc is null (ie. native key is not extracted
	// from the loaded EVP_PKEY).
	template <typename K, typename F>
	static bool loadKey(K** ppKey,
		PEM_read_FILE_Key_fn readFunc,
		F getFunc,
		const std::string& keyFile,
		const std::string& pass = "")
	{
		poco_assert_dbg (((typeid(K) == typeid(RSA) || typeid(K) == typeid(EC_KEY)) && getFunc) ||
						((typeid(K) == typeid(EVP_PKEY)) && !getFunc));
		poco_check_ptr (ppKey);
		poco_assert_dbg (!*ppKey);

		if (!keyFile.empty())
		{
			if (!getFunc) *ppKey = (K*)EVP_PKEY_new();
			EVP_PKEY* pKey = getFunc ? EVP_PKEY_new() : (EVP_PKEY*)*ppKey;
			if (pKey)
			{
				FILE* pFile = fopen(keyFile.c_str(), "r");
				if (pFile)
				{
					readFunc(pFile, &pKey, passCB, pass.empty() ? (void*)0 : (void*)pass.c_str());
					fclose(pFile);
					if (getFunc)
					{
						*ppKey = (K*)getFunc(pKey);
						EVP_PKEY_free(pKey);
					}
					else *ppKey = (K*)pKey;
					if (!*ppKey) goto error;
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
		throw OpenSSLException("EVPKey::loadKey(string)");
	}

	template <typename K, typename F>
	static bool loadKey(K** ppKey,
		PEM_read_BIO_Key_fn readFunc,
		F getFunc,
		std::istream* pIstr,
		const std::string& pass = "")
	{
		poco_assert_dbg (((typeid(K) == typeid(RSA) || typeid(K) == typeid(EC_KEY)) && getFunc) ||
						((typeid(K) == typeid(EVP_PKEY)) && !getFunc));
		poco_check_ptr(ppKey);
		poco_assert_dbg(!*ppKey);

		if (pIstr)
		{
			std::ostringstream ostr;
			Poco::StreamCopier::copyStream(*pIstr, ostr);
			std::string key = ostr.str();
			BIO *pBIO = BIO_new_mem_buf(const_cast<char*>(key.data()), static_cast<int>(key.size()));
			if (pBIO)
			{
				if (!getFunc) *ppKey = (K*)EVP_PKEY_new();
				EVP_PKEY* pKey = getFunc ? EVP_PKEY_new() : (EVP_PKEY*)*ppKey;
				if (pKey)
				{
					if (readFunc(pBIO, &pKey, passCB, pass.empty() ? (void*)0 : (void*)pass.c_str()))
					{
						BIO_free(pBIO);
						if (getFunc)
						{
							*ppKey = (K*)getFunc(pKey);
							EVP_PKEY_free(pKey);
						}
						else *ppKey = (K*)pKey;
						if (!*ppKey) goto error;
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
		throw OpenSSLException("EVPKey::loadKey(stream)");
	}

	EVP_PKEY* _pEVPPKey;

	friend class ECKeyImpl;
	friend class RSAKeyImpl;
};

//
// inlines
//

inline int EVPPKey::type() const
{
	if (!_pEVPPKey) return NID_undef;

	return EVP_PKEY_type(_pEVPPKey->type);
}


inline bool EVPPKey::isSupported(int type) const
{
	return type == EVP_PKEY_EC || type == EVP_PKEY_RSA;
}


inline EVPPKey::operator const EVP_PKEY*() const
{
	return _pEVPPKey;
}


inline EVPPKey::operator EVP_PKEY*()
{
	return _pEVPPKey;
}


inline void EVPPKey::setKey(EC_KEY* pKey)
{
	if (!EVP_PKEY_set1_EC_KEY(_pEVPPKey, pKey))
		throw OpenSSLException();
}


inline void EVPPKey::setKey(RSA* pKey)
{
	if (!EVP_PKEY_set1_RSA(_pEVPPKey, pKey))
		throw OpenSSLException();
}


} } // namespace Poco::Crypto


#endif // Crypto_EVPPKeyImpl_INCLUDED
