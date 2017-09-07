//
// ECKeyImpl.h
//
// $Id: //poco/1.4/Crypto/include/Poco/Crypto/ECKeyImpl.h#3 $
//
// Library: Crypto
// Package: EC
// Module:  ECKeyImpl
//
// Definition of the ECKeyImpl class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_ECKeyImplImpl_INCLUDED
#define Crypto_ECKeyImplImpl_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include <istream>
#include <ostream>
#include <vector>
#include <openssl/objects.h>
#include <openssl/ec.h>

/*
struct bignum_st;
struct rsa_st;
typedef struct bignum_st BIGNUM;
typedef struct rsa_st EC;
*/

namespace Poco {
namespace Crypto {


class X509Certificate;


class ECKeyImpl: public Poco::RefCountedObject
	/// class ECKeyImpl
{
public:
	typedef Poco::AutoPtr<ECKeyImpl> Ptr;
	typedef std::vector<unsigned char> ByteVec;

	explicit ECKeyImpl(const X509Certificate& cert);
		/// Extracts the EC public key from the given certificate.

	ECKeyImpl(int eccGroup);
		/// Creates the ECKey of the specified group. Creates a new public/private keypair using the given parameters.
		/// Can be used to sign data and verify signatures.

	ECKeyImpl(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase);
		/// Creates the ECKey, by reading public and private key from the given files and
		/// using the given passphrase for the private key. Can only by used for signing if 
		/// a private key is available. 

	ECKeyImpl(std::istream* pPublicKeyStream, std::istream* pPrivateKeyStream, const std::string& privateKeyPassphrase);
		/// Creates the ECKey. Can only by used for signing if pPrivKey
		/// is not null. If a private key file is specified, you don't need to
		/// specify a public key file. OpenSSL will auto-create it from the private key.

	~ECKeyImpl();
		/// Destroys the ECKeyImpl.

	EC_KEY* getECKey();
		/// Returns the OpenSSL EC key.

	const EC_KEY* getECKey() const;
		/// Returns the OpenSSL EC key.

	int size() const;
		/// Returns the EC key length in bits.

	int groupId() const;
		/// Returns the EC key group integer Id.

	std::string groupName() const;
		/// Returns the EC key group name.

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

private:
	class EVPPKey
	{
	public:
		EVPPKey() = delete;
		EVPPKey(EC_KEY* pEC);
		~EVPPKey();
		operator EVP_PKEY*();

	private:
		EC_KEY*   _pEC = nullptr;
		EVP_PKEY* _pKey = nullptr;
	};

	static int passCB(char* buf, int size, int, void* pass);
	bool loadKey(const std::string& keyFile, const std::string& pass = "");
	bool loadKey(std::istream* pKeyStream, const std::string& pass = "");
	void freeEC();

	static ByteVec convertToByteVec(const BIGNUM* bn);

private:
	EC_KEY* _pEC;
	int _eccGroup = -1;
	OpenSSLInitializer _openSSLInitializer;
};


//
// inlines
//
inline EC_KEY* ECKeyImpl::getECKey()
{
	return _pEC;
}


inline const EC_KEY* ECKeyImpl::getECKey() const
{
	return _pEC;
}


inline int ECKeyImpl::groupId() const
{
	return _eccGroup;
}


} } // namespace Poco::Crypto


#endif // Crypto_ECKeyImplImpl_INCLUDED
