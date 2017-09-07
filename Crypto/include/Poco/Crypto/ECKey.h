//
// ECKey.h
//
// $Id: //poco/1.4/Crypto/include/Poco/Crypto/ECKey.h#2 $
//
// Library: Crypto
// Package: EC
// Module:  ECKey
//
// Definition of the ECKey class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_ECKey_INCLUDED
#define Crypto_ECKey_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/ECKeyImpl.h"


namespace Poco {
namespace Crypto {


class X509Certificate;


class Crypto_API ECKey
	/// This class stores an EC key pair, consisting
	/// of private and public key. Storage of the private
	/// key is optional.
	///
	/// If a private key is available, the ECKey can be
	/// used for decrypting data (encrypted with the public key)
	/// or computing secure digital signatures.
{
public:
	explicit ECKey(const X509Certificate& cert);
		/// Extracts the EC public key from the given certificate.

	ECKey(int eccGroup);
		/// Creates the ECKey. Creates a new public/private keypair using the given parameters.
		/// Can be used to sign data and verify signatures.

	ECKey(const std::string& publicKeyFile, const std::string& privateKeyFile = "", const std::string& privateKeyPassphrase = "");
		/// Creates the ECKey, by reading public and private key from the given files and
		/// using the given passphrase for the private key.
		///
		/// Cannot be used for signing or decryption unless a private key is available.
		///
		/// If a private key is specified, you don't need to specify a public key file.
		/// OpenSSL will auto-create the public key from the private key.

	ECKey(std::istream* pPublicKeyStream, std::istream* pPrivateKeyStream = 0, const std::string& privateKeyPassphrase = "");
		/// Creates the ECKey, by reading public and private key from the given streams and
		/// using the given passphrase for the private key.
		///
		/// Cannot be used for signing or decryption unless a private key is available.
		///
		/// If a private key is specified, you don't need to specify a public key file.
		/// OpenSSL will auto-create the public key from the private key.

	~ECKey();
		/// Destroys the ECKey.

	int size() const;
		/// Returns the EC modulus size.
/*
	ECKeyImpl::ByteVec modulus() const;
		/// Returns the EC modulus.

	ECKeyImpl::ByteVec encryptionExponent() const;
		/// Returns the EC encryption exponent.

	ECKeyImpl::ByteVec decryptionExponent() const;
		/// Returns the EC decryption exponent.
		*/
	void save(const std::string& publicKeyFile, const std::string& privateKeyFile = "", const std::string& privateKeyPassphrase = "");
		/// Exports the public and private keys to the given files. 
		///
		/// If an empty filename is specified, the corresponding key
		/// is not exported.

	void save(std::ostream* pPublicKeyStream, std::ostream* pPrivateKeyStream = 0, const std::string& privateKeyPassphrase = "");
		/// Exports the public and private key to the given streams.
		///
		/// If a null pointer is passed for a stream, the corresponding
		/// key is not exported.

	ECKeyImpl::Ptr impl() const;
		/// Returns the impl object.

	const std::string& name() const;
		/// Returns "rsa"
	
private:
	ECKeyImpl::Ptr _pImpl;
};


//
// inlines
//
inline ECKeyImpl::Ptr ECKey::impl() const
{
	return _pImpl;
}


} } // namespace Poco::Crypto


#endif // Crypto_ECKey_INCLUDED
