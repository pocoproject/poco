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
#include "Poco/Format.h"
#include <openssl/ec.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <sstream>
#include <typeinfo>
#include <map>


namespace Poco {
namespace Crypto {

//@deprecated
class ECKey;
//@deprecated
class RSAKey;
class PKCS12Container;
class X509Certificate;


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

	EVPPKey(const X509Certificate& cert);
		/// Constructs EVPPKey from the given certificate.

	EVPPKey(const PKCS12Container& cert);
		/// Constructs EVPPKey from the given container.

#if OPENSSL_VERSION_NUMBER >= 0x10000000L

	EVPPKey(int type, int param);
		/// Creates the EVPPKey.
		/// Creates a new public/private keypair using the given parameters.
		/// Can be used to sign data and verify signatures.
		///
		/// Suported types:
		///   - EVP_PKEY_RSA
		///   - EVP_PKEY_EC
		///
		/// Parameters:
		///   - for EVP_PKEY_RSA: key length in bits
		///   - for EVP_PKEY_EC: curve NID
		///
		/// This constructor is not available for OpenSSL version < 1.0.0

#endif // OPENSSL_VERSION_NUMBER >= 0x10000000L

#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	explicit EVPPKey(const std::vector<unsigned char>* publicKey, const std::vector<unsigned char>* privateKey, unsigned long exponent, int type);
#endif
	
	explicit EVPPKey(EVP_PKEY* pEVPPKey);
		/// Constructs EVPPKey from EVP_PKEY pointer.
		/// The content behind the supplied pointer is internally duplicated.

	//@ deprecated
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

	EVPPKey(EVPPKey&& other) noexcept;
		/// Move constructor.

	EVPPKey& operator = (const EVPPKey& other);
		/// Assignment operator.

	EVPPKey& operator = (EVPPKey&& other) noexcept;
		/// Assignment move operator.

	~EVPPKey();
		/// Destroys the EVPPKey.

	bool operator == (const EVPPKey& other) const;
		/// Comparison operator.
		/// Returns true if public key components and parameters
		/// of the other key are equal to this key.
		///
		/// Works as expected when one key contains only public key,
		/// while the other one contains private (thus also public) key.

	bool operator != (const EVPPKey& other) const;
		/// Comparison operator.
		/// Returns true if public key components and parameters
		/// of the other key are different from this key.
		///
		/// Works as expected when one key contains only public key,
		/// while the other one contains private (thus also public) key.

	void save(const std::string& publicKeyFile, const std::string& privateKeyFile = "", const std::string& privateKeyPassphrase = "") const;
		/// Exports the public and/or private keys to the given files.
		///
		/// If an empty filename is specified, the corresponding key
		/// is not exported.

	void save(std::ostream* pPublicKeyStream, std::ostream* pPrivateKeyStream = 0, const std::string& privateKeyPassphrase = "") const;
		/// Exports the public and/or private key to the given streams.
		///
		/// If a null pointer is passed for a stream, the corresponding
		/// key is not exported.

	int type() const;
		/// Retuns the EVPPKey type NID.

	const std::string& name() const;
		/// Retuns the EVPPKey name.

	bool isSupported(int type) const;
		/// Returns true if OpenSSL type is supported

	operator const EVP_PKEY*() const;
		/// Returns const pointer to the OpenSSL EVP_PKEY structure.

	operator EVP_PKEY*();
		/// Returns pointer to the OpenSSL EVP_PKEY structure.

	static EVP_PKEY* duplicate(const EVP_PKEY* pFromKey, EVP_PKEY** pToKey);
		/// Duplicates pFromKey into *pToKey and returns
		/// the pointer to duplicated EVP_PKEY.

private:
	EVPPKey();
#if OPENSSL_VERSION_NUMBER >= 0x30000000L	
	void setKeyFromParameters(OSSL_PARAM* parameters);
#endif
	static int type(const EVP_PKEY* pEVPPKey);
	void checkType();
	void newECKey(const char* group);
	void duplicate(EVP_PKEY* pEVPPKey);

	//@ deprecated
	void setKey(ECKey* pKey);
	//@ deprecated
	void setKey(RSAKey* pKey);
	//@ deprecated
	void setKey(EC_KEY* pKey);
	//@ deprecated
	void setKey(RSA* pKey);

	static int passCB(char* buf, int size, int, void* pass);

	typedef EVP_PKEY* (*PEM_read_FILE_Key_fn)(FILE*, EVP_PKEY**, pem_password_cb*, void*);
	typedef EVP_PKEY* (*PEM_read_BIO_Key_fn)(BIO*, EVP_PKEY**, pem_password_cb*, void*);
	typedef void* (*EVP_PKEY_get_Key_fn)(EVP_PKEY*);

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
		poco_assert_dbg (((typeid(K*) == typeid(RSA*) || typeid(K*) == typeid(EC_KEY*)) && getFunc) ||
						((typeid(K*) == typeid(EVP_PKEY*)) && !getFunc));
		poco_check_ptr (ppKey);
		poco_assert_dbg (!*ppKey);

		FILE* pFile = 0;
		if (!keyFile.empty())
		{
			if (!getFunc) *ppKey = (K*)EVP_PKEY_new();
			EVP_PKEY* pKey = getFunc ? EVP_PKEY_new() : (EVP_PKEY*)*ppKey;
			if (pKey)
			{
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996) // deprecation warnings
#endif
				pFile = fopen(keyFile.c_str(), "r");
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

				if (pFile)
				{
					pem_password_cb* pCB = pass.empty() ? (pem_password_cb*)0 : &passCB;
					void* pPassword = pass.empty() ? (void*)0 : (void*)pass.c_str();
					if (readFunc(pFile, &pKey, pCB, pPassword))
					{
						fclose(pFile); pFile = 0;
						if(getFunc)
						{
							*ppKey = (K*)getFunc(pKey);
							EVP_PKEY_free(pKey);
						}
						else
						{
							poco_assert_dbg (typeid(K*) == typeid(EVP_PKEY*));
							*ppKey = (K*)pKey;
						}
						if (!*ppKey) goto error;
						return true;
					}
					if (getFunc) EVP_PKEY_free(pKey);
					goto error;
				}
				else
				{
					std::string msg = Poco::format("EVPPKey::loadKey('%s')\n", keyFile);
					getError(msg);
					if (getFunc) EVP_PKEY_free(pKey);
					throw IOException(msg);
				}
			}
			else goto error;
		}
		return false;

	error:
		std::string msg = Poco::format("EVPPKey::loadKey('%s')\n", keyFile);
		getError(msg);
		if (pFile) fclose(pFile);
		throw OpenSSLException(msg);
	}

	template <typename K, typename F>
	static bool loadKey(K** ppKey,
		PEM_read_BIO_Key_fn readFunc,
		F getFunc,
		std::istream* pIstr,
		const std::string& pass = "")
	{
		poco_assert_dbg (((typeid(K*) == typeid(RSA*) || typeid(K*) == typeid(EC_KEY*)) && getFunc) ||
						((typeid(K*) == typeid(EVP_PKEY*)) && !getFunc));
		poco_check_ptr(ppKey);
		poco_assert_dbg(!*ppKey);

		BIO* pBIO = 0;
		if (pIstr)
		{
			std::ostringstream ostr;
			Poco::StreamCopier::copyStream(*pIstr, ostr);
			std::string key = ostr.str();
			pBIO = BIO_new_mem_buf(const_cast<char*>(key.data()), static_cast<int>(key.size()));
			if (pBIO)
			{
				if (!getFunc) *ppKey = (K*)EVP_PKEY_new();
				EVP_PKEY* pKey = getFunc ? EVP_PKEY_new() : (EVP_PKEY*)*ppKey;
				if (pKey)
				{
					pem_password_cb* pCB = pass.empty() ? (pem_password_cb*)0 : &passCB;
					void* pPassword = pass.empty() ? (void*)0 : (void*)pass.c_str();
					if (readFunc(pBIO, &pKey, pCB, pPassword))
					{
						BIO_free(pBIO); pBIO = 0;
						if (getFunc)
						{
							*ppKey = (K*)getFunc(pKey);
							EVP_PKEY_free(pKey);
						}
						else
						{
							poco_assert_dbg (typeid(K*) == typeid(EVP_PKEY*));
							*ppKey = (K*)pKey;
						}
						if (!*ppKey) goto error;
						return true;
					}
					if (getFunc) EVP_PKEY_free(pKey);
					goto error;
				}
				else goto error;
			}
			else goto error;
		}
		return false;

	error:
		std::string msg = "EVPPKey::loadKey(istream)\n";
		getError(msg);
		if (pBIO) BIO_free(pBIO);
		throw OpenSSLException(msg);
	}

	EVP_PKEY* _pEVPPKey = 0;
	static const std::map<int, std::string> KNOWN_TYPES;

	//@deprecated
	friend class ECKeyImpl;
	//@deprecated
	friend class RSAKeyImpl;
};


//
// inlines
//


inline bool EVPPKey::operator == (const EVPPKey& other) const
{
	poco_check_ptr (other._pEVPPKey);
	poco_check_ptr (_pEVPPKey);
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	return (1 == EVP_PKEY_eq(_pEVPPKey, other._pEVPPKey));
#else
	return (1 == EVP_PKEY_cmp(_pEVPPKey, other._pEVPPKey));
#endif
}


inline bool EVPPKey::operator != (const EVPPKey& other) const
{
	return !(other == *this);
}


inline int EVPPKey::type(const EVP_PKEY* pEVPPKey)
{
	if (!pEVPPKey) return NID_undef;

	return EVP_PKEY_type(EVP_PKEY_base_id(pEVPPKey));
}


inline int EVPPKey::type() const
{
	return type(_pEVPPKey);
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


} } // namespace Poco::Crypto


#endif // Crypto_EVPPKeyImpl_INCLUDED
