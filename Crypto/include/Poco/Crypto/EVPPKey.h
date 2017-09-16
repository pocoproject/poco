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
#include <openssl/ec.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>


namespace Poco {
namespace Crypto {


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
		/// Constructs EVPPKey from a "native" key pointer.
	{
		if (!_pEVPPKey) throw OpenSSLException();
		setKey(pKey);
	}

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

	int type() const;
		/// Retuns the EVPPKey type NID.

	operator const EVP_PKEY*() const;
		/// Returns const pointer to the EVP_PKEY structure.

	operator EVP_PKEY*();
		/// Returns pointer to the EVP_PKEY structure.

private:
	EVPPKey();

	void newECKey(const char* group);

	void duplicate(EVP_PKEY* pEVPPKey);
	void setKey(EC_KEY* pKey);
	void setKey(RSA* pKey);

	EVP_PKEY* _pEVPPKey;
};

//
// inlines
//

inline int EVPPKey::type() const
{
	if (!_pEVPPKey) return NID_undef;

	return EVP_PKEY_type(_pEVPPKey->type);
}


inline EVPPKey::operator const EVP_PKEY*() const
	/// Returns const pointer to the EVP_PKEY structure.
{
	return _pEVPPKey;
}


inline EVPPKey::operator EVP_PKEY*()
	/// Returns pointer to the EVP_PKEY structure.
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
