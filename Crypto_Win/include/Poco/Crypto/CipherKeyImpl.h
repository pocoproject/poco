//
// CipherKeyImpl.h
//
// $Id$
//
// Library: Crypto_Win
// Package: Cipher
// Module:  CipherKeyImpl
//
// Definition of the CipherKeyImpl class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_CipherKeyImpl_INCLUDED
#define Crypto_CipherKeyImpl_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/ServiceProvider.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include <vector>
#include <WinCrypt.h>


namespace Poco {
namespace Crypto {


class CipherKeyImpl: public RefCountedObject
	/// An implementation of the CipherKey class for WinCrypt.
{
public:
	typedef std::vector<unsigned char> ByteVec;
	typedef Poco::AutoPtr<CipherKeyImpl> Ptr;

	enum Mode
		/// Cipher mode of operation. This mode determines how multiple blocks
		/// are connected; this is essential to improve security.
	{
		MODE_STREAM_CIPHER,	/// Stream cipher
		MODE_ECB,			/// Electronic codebook (plain concatenation)
		MODE_CBC,			/// Cipher block chaining (default)
		MODE_CFB,			/// Cipher feedback
		MODE_OFB			/// Output feedback
	};

	CipherKeyImpl(const std::string& name, 
		const std::string& passphrase, 
		const std::string& salt,
		int iterationCount);
		/// Creates a new CipherKeyImpl object, using
		/// the given cipher name, passphrase, salt value
		/// and iteration count.

	CipherKeyImpl(const std::string& name, 
		const ByteVec& key, 
		const ByteVec& iv);
		/// Creates a new CipherKeyImpl object, using the 
		/// given cipher name, key and initialization vector.

	CipherKeyImpl(const std::string& name);
		/// Creates a new CipherKeyImpl object. Autoinitializes key
		/// and initialization vector.

	virtual ~CipherKeyImpl();
		/// Destroys the CipherKeyImpl.

	const std::string& name() const;
		/// Returns the name of the Cipher.

	int keySize() const;
		/// Returns the key size of the Cipher.

	int blockSize() const;
		/// Returns the block size of the Cipher.

	int ivSize() const;
		/// Returns the IV size of the Cipher.

	Mode mode() const;
		/// Returns the Cipher's mode of operation.
	
	const ByteVec& getKey() const;
		/// Returns the key for the Cipher.

	void setKey(const ByteVec& key);
		/// Sets the key for the Cipher.

	const ByteVec& getIV() const;
		/// Returns the initialization vector (IV) for the Cipher.

	void setIV(const ByteVec& iv);
		/// Sets the initialization vector (IV) for the Cipher.

private:
	void generateKey(const std::string& passphrase,
		const std::string& salt,
		int iterationCount);
	 	/// Generates key and IV from a password and optional salt string.

	void generateKey();
		/// Generates key and IV from random data.

	void getRandomBytes(ByteVec& vec, std::size_t count);
		/// Stores random bytes in vec.

	void importKey();
		/// Imports the key into WinCrypt.

	void importIV();
		/// Imports the IV into WinCrypt.

	static ALG_ID id(const std::string& name);
		/// Returns the WinCrypt algorithm ID for the given name.

private:
	ServiceProvider _sp;
	HCRYPTKEY       _hKey;
	std::string     _name;
	ALG_ID          _id;
	ByteVec         _key;
	ByteVec         _iv;

	friend class CryptoTransformImpl;
};


//
// Inlines
//
inline const std::string& CipherKeyImpl::name() const
{
	return _name;
}


inline const CipherKeyImpl::ByteVec& CipherKeyImpl::getKey() const
{
	return _key;
}


inline void CipherKeyImpl::setKey(const ByteVec& key)
{
	poco_assert(key.size() == static_cast<ByteVec::size_type>(keySize()));
	_key = key;
	importKey();
}


inline const CipherKeyImpl::ByteVec& CipherKeyImpl::getIV() const
{
	return _iv;
}


inline void CipherKeyImpl::setIV(const ByteVec& iv)
{
	poco_assert(iv.size() == static_cast<ByteVec::size_type>(ivSize()));
	_iv = iv;
	importIV();
}


} } // namespace Poco::Crypto


#endif // Crypto_CipherKeyImpl_INCLUDED
