//
// RSACipherImpl.h
//
// $Id$
//
// Library: Crypto_Win
// Package: RSA
// Module:  RSACipherImpl
//
// Definition of the RSACipherImpl class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_RSACipherImpl_INCLUDED
#define Crypto_RSACipherImpl_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/RSAKey.h"


namespace Poco {
namespace Crypto {


class RSACipherImpl: public Cipher
	/// An implementation of the Cipher class for 
	/// asymmetric (public-private key) encryption
	/// based on the the RSA algorithm.
	///
	/// Encryption is using the public key, decryption
	/// requires the private key.
{
public:
	RSACipherImpl(const RSAKey& key, RSAPaddingMode paddingMode);
		/// Creates a new RSACipherImpl object for the given RSAKey
		/// and using the given padding mode.

	virtual ~RSACipherImpl();
		/// Destroys the RSACipherImpl.

	const std::string& name() const;
		/// Returns the name of the Cipher.
	
	CryptoTransform* createEncryptor();
		/// Creates an CryptoTransform object for encryption.

	CryptoTransform* createDecryptor();
		/// Creates a CryptoTransform object for decryption.

private:
	RSAKey _key;
	RSAPaddingMode _paddingMode;
};


//
// Inlines
//
inline const std::string& RSACipherImpl::name() const
{
	return _key.name();
}


} } // namespace Poco::Crypto


#endif // Crypto_RSACipherImpl_INCLUDED
