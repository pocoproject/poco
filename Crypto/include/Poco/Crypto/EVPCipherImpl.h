//
// EVPCipherImpl.h
//
// Library: Crypto
// Package: EVP
// Module:  EVPCipherImpl
//
// Definition of the EVPCipherImpl class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_EVPCipherImpl_INCLUDED
#define Crypto_EVPCipherImpl_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include <openssl/evp.h>


namespace Poco {
namespace Crypto {


class EVPCipherImpl: public Cipher
	/// An implementation of the Cipher class for
	/// asymmetric (public-private key) encryption
	/// based on the OpenSSL crypto library.
	///
	/// Encryption is using the public key, decryption
	/// requires the private key.
{
public:
	EVPCipherImpl(const EVPPKey& key);
		/// Creates a new EVPCipherImpl object for the given EVPPKey
		/// and using the given padding mode.

	virtual ~EVPCipherImpl();
		/// Destroys the EVPCipherImpl.

	const std::string& name() const;
		/// Returns the name of the Cipher.

	CryptoTransform::Ptr createEncryptor();
		/// Creates an encryptor object.

	CryptoTransform::Ptr createDecryptor();
		/// Creates a decryptor object.

private:
	EVPPKey _key;
	OpenSSLInitializer _openSSLInitializer;
};


//
// Inlines
//

inline const std::string& EVPCipherImpl::name() const
{
	return _key.name();
}


} } // namespace Poco::Crypto


#endif // Crypto_EVPCipherImpl_INCLUDED
