//
// RSAKeyImpl.h
//
// Library: Crypto
// Package: RSA
// Module:  RSAKeyImpl
//
// Definition of the RSAKeyImpl class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_RSAKeyImplImpl_INCLUDED
#define Crypto_RSAKeyImplImpl_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Crypto/KeyPairImpl.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include <istream>
#include <ostream>
#include <vector>


#ifndef OPENSSL_NO_DEPRECATED_3_0
struct bignum_st;
struct rsa_st;
typedef struct bignum_st BIGNUM;
typedef struct rsa_st RSA;
#endif


namespace Poco::Crypto {


class X509Certificate;
class PKCS12Container;


class Crypto_API RSAKeyImpl: public KeyPairImpl
	/// class RSAKeyImpl
{
public:
	using Ptr = Poco::AutoPtr<RSAKeyImpl>;
	using ByteVec = std::vector<unsigned char>;

	RSAKeyImpl(const EVPPKey& key);
		/// Constructs RSAKeyImpl by extracting the RSA key.

	RSAKeyImpl(const X509Certificate& cert);
		/// Extracts the RSA public key from the given certificate.

	RSAKeyImpl(const PKCS12Container& cert);
		/// Extracts the RSA private key from the given certificate.

	RSAKeyImpl(int keyLength, unsigned long exponent);
		/// Creates the RSAKey. Creates a new public/private keypair using the given parameters.
		/// Can be used to sign data and verify signatures.

	RSAKeyImpl(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase);
		/// Creates the RSAKey, by reading public and private key from the given files and
		/// using the given passphrase for the private key. Can only by used for signing if
		/// a private key is available.

	RSAKeyImpl(std::istream* pPublicKeyStream, std::istream* pPrivateKeyStream, const std::string& privateKeyPassphrase);
		/// Creates the RSAKey. Can only by used for signing if pPrivKey
		/// is not null. If a private key file is specified, you don't need to
		/// specify a public key file. OpenSSL will auto-create it from the private key.

	~RSAKeyImpl();
		/// Destroys the RSAKeyImpl.

#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	EVP_PKEY* getEVPPKey();
		/// Returns the OpenSSL EVP_PKEY object.

	const EVP_PKEY* getEVPPKey() const;
		/// Returns the OpenSSL EVP_PKEY object.
#endif

#ifndef OPENSSL_NO_DEPRECATED_3_0
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	POCO_DEPRECATED("use getEVPPKey() instead")
#endif
	RSA* getRSA();
		/// Returns the OpenSSL RSA object.

#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	POCO_DEPRECATED("use getEVPPKey() instead")
#endif
	const RSA* getRSA() const;
		/// Returns the OpenSSL RSA object.
#endif

	int size() const;
		/// Returns the RSA modulus size.

	ByteVec modulus() const;
		/// Returns the RSA modulus.

	ByteVec encryptionExponent() const;
		/// Returns the RSA encryption exponent.

	ByteVec decryptionExponent() const;
		/// Returns the RSA decryption exponent.

	void save(const std::string& publicKeyFile,
		const std::string& privateKeyFile = "",
		const std::string& privateKeyPassphrase = "") const;
		/// Exports the public and private keys to the given files.
		///
		/// If an empty filename is specified, the corresponding key
		/// is not exported.

	void save(std::ostream* pPublicKeyStream,
		std::ostream* pPrivateKeyStream = nullptr,
		const std::string& privateKeyPassphrase = "") const;
		/// Exports the public and private key to the given streams.
		///
		/// If a null pointer is passed for a stream, the corresponding
		/// key is not exported.

private:
	RSAKeyImpl();

	void freeRSA();

#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	EVP_PKEY* _pEVPPKey;
#else
	static ByteVec convertToByteVec(const BIGNUM* bn);

	RSA* _pRSA;
#endif
};


//
// inlines
//

#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)

inline EVP_PKEY* RSAKeyImpl::getEVPPKey()
{
	return _pEVPPKey;
}


inline const EVP_PKEY* RSAKeyImpl::getEVPPKey() const
{
	return _pEVPPKey;
}

#endif


} // namespace Poco::Crypto


#endif // Crypto_RSAKeyImplImpl_INCLUDED
