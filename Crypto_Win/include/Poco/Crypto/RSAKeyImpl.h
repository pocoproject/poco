//
// RSAKeyImpl.h
//
// $Id$
//
// Library: Crypto_Win
// Package: RSA
// Module:  RSAKeyImpl
//
// Definition of the RSAKeyImpl class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_RSAKeyImpl_INCLUDED
#define Crypto_RSAKeyImpl_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/ServiceProvider.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include <istream>
#include <ostream>
#include <vector>


struct bignum_st;
struct rsa_st;
typedef struct bignum_st BIGNUM;
typedef struct rsa_st RSA;


namespace Poco {
namespace Crypto {


class X509Certificate;


class RSAKeyImpl: public Poco::RefCountedObject
	/// class RSAKeyImpl
{
public:
	typedef Poco::AutoPtr<RSAKeyImpl> Ptr;

	explicit RSAKeyImpl(const X509Certificate& cert);
		/// Extracts the RSA public key from the given certificate.

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

	int size() const;
		/// Returns the RSA modulus size.

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

	HCRYPTKEY privateKey() const;
		/// Returns the underlying Windows-specific handle for the private key.

	HCRYPTKEY publicKey() const;
		/// Returns the underlying Windows-specific handle for the public key.

protected:
	void loadPrivateKey(std::istream& istr);
	void loadPublicKey(std::istream& istr);
	void savePrivateKey(std::ostream& ostr);
	void savePublicKey(std::ostream& ostr);

private:
	ServiceProvider _sp;
	HCRYPTKEY _hPrivateKey;
	HCRYPTKEY _hPublicKey;
};


//
// inlines
//
HCRYPTKEY RSAKeyImpl::privateKey() const
{
	return _hPrivateKey;
}


HCRYPTKEY RSAKeyImpl::publicKey() const
{
	return _hPublicKey;
}


} } // namespace Poco::Crypto


#endif // Crypto_RSAKeyImpl_INCLUDED
