//
// Envelope.h
//
// Library: Crypto
// Package: Envelope
// Module:  Envelope
//
// Definition of the Envelope class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_Envelope_INCLUDED
#define Crypto_Envelope_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/EVPPKey.h"
#include <vector>
#include <openssl/evp.h>


namespace Poco {
namespace Crypto {


class Crypto_API Envelope
	/// Envelope encrypts/decrypts data using a symmetric key.
	///
	/// Encryption and decryption with asymmetric keys is computationally expensive.
	/// To alleviate that, Envelope encrypts data using a symmetric session key;
	/// the key is then itself asymmetrically encrypted using a public key.
	/// It is also possible to encrypt the session key with multiple public keys,
	/// so that the message can be sent to multiple recipients.
	///
	/// Each recipient decrypts the session with their private key; the session
	/// key for the message decryption is the same for each recipient.
{
public:
	using Byte = unsigned char;
	using ByteVec = std::vector<Byte>;
	using EVPPKeyVec = std::vector<EVPPKey>;
	using EVP_PKEYVec = std::vector<EVP_PKEY*>;
	using EncKeyVec = std::vector<ByteVec>;

	Envelope() = delete;

	Envelope(const EVPPKey& key, int cipherNID);
		/// Creates a new Envelope object.
		/// Initialization vector is automatically
		/// generated.

	Envelope(const EVPPKeyVec& keys, int cipherNID);
		/// Creates a new Envelope object.
		/// Initialization vector is automatically
		/// generated.

	~Envelope();
		/// Destroys the Envelope.

	const ByteVec& iv() const;
		/// Returns the initialization vector.

	void addKey(const EVPPKey& key);
		/// Adds the key to the list of private keys.

	const EncKeyVec& keys() const;
		/// Returns encrypted symmetric keys.

	int cipherNID() const;
		/// Reurns the cipher NID.

	const ByteVec& seal(const std::string& plainText);
		/// Encrypts the given text and returns the encrypted text.

	const ByteVec& seal(const ByteVec& plainData);
		/// Encrypts the given data and returns the encrypted data.

	const ByteVec& getContent() const;
		/// Returns the encrypted content.
	
	void setContent(const ByteVec& enc);
		/// Sets the encrypted content.

	ByteVec open(const EVPPKey& privKey, const ByteVec& encKeys, const ByteVec& iv = ByteVec());
		/// Decrypts the stored encrypted data and returns it.

	std::string openAsString(const EVPPKey& privKey, const ByteVec& encKeys, const ByteVec& iv = ByteVec());
		/// Decrypts the stored encrypted data and returns it.

	static std::string toString(const ByteVec& data);
		/// Converts and returns string from ByteVec.

private:
	Envelope(int cipherNID);
	Envelope(int cipherNID, const ByteVec& iv);

	int ivSize() const;
	int blockSize() const;
	void handleErrors(std::string&& msg);

	const EVP_CIPHER* _pCipher;
	EVP_CIPHER_CTX*   _pCtx;
	EVP_CIPHER_CTX*   _pDecCtx;
	ByteVec           _iv;
	EVP_PKEYVec       _pubKeys;
	EncKeyVec         _encKeys;
	std::vector<int>  _encKeysSizes;
	ByteVec           _encContent;
};


inline int Envelope::ivSize() const
{
	return EVP_CIPHER_iv_length(_pCipher);
}


inline const Envelope::ByteVec& Envelope::iv() const
{
	return _iv;
}


inline int Envelope::blockSize() const
{
	return EVP_CIPHER_block_size(_pCipher);
}


inline const Envelope::EncKeyVec& Envelope::keys() const
{
	return _encKeys;
}


inline std::string Envelope::toString(const ByteVec& data)
{
	return std::string(data.begin(), data.end());
}


inline std::string Envelope::openAsString(const EVPPKey& privKey, const ByteVec& encKey, const ByteVec& iv)
{
	return toString(open(privKey, encKey, iv));
}


inline const Envelope::ByteVec& Envelope::getContent() const
{
	return _encContent;
}
	

inline void Envelope::setContent(const ByteVec& enc)
{
	_encContent = enc;
}


inline int Envelope::cipherNID() const
{
	return EVP_CIPHER_nid(_pCipher);
}


} } // namespace Poco::Crypto


#endif // Crypto_Envelope_INCLUDED
