//
// ECDSADigestEngine.h
//
//
// Library: Crypto
// Package: ECDSA
// Module:  ECDSADigestEngine
//
// Definition of the ECDSADigestEngine class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_ECDSADigestEngine_INCLUDED
#define Crypto_ECDSADigestEngine_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/ECKey.h"
#include "Poco/DigestEngine.h"
#include "Poco/Crypto/DigestEngine.h"
#include <istream>
#include <ostream>


namespace Poco {
namespace Crypto {


class Crypto_API ECDSADigestEngine: public Poco::DigestEngine
	/// This class implements a Poco::DigestEngine that can be
	/// used to compute a secure digital signature.
	///
	/// First another Poco::Crypto::DigestEngine is created and
	/// used to compute a cryptographic hash of the data to be
	/// signed. Then, the hash value is encrypted, using
	/// the ECDSA private key.
	///
	/// To verify a signature, pass it to the verify()
	/// member function. It will decrypt the signature
	/// using the ECDSA public key and compare the resulting
	/// hash with the actual hash of the data.
{
public:

	ECDSADigestEngine(ECKey  key, const std::string &name);
		/// Creates the ECDSADigestEngine with the given ECDSA key,
		/// using the hash algorithm with the given name
		/// (e.g., "SHA1", "SHA256", "SHA512", etc.).
		/// See the OpenSSL documentation for a list of supported digest algorithms.
		///
		/// Throws a Poco::NotFoundException if no algorithm with the given name exists.

	~ECDSADigestEngine() override;
		/// Destroys the ECDSADigestEngine.

	std::size_t digestLength() const override;
		/// Returns the length of the digest in bytes.

	void reset() override;
		/// Resets the engine so that a new
		/// digest can be computed.

	const DigestEngine::Digest& digest() override;
		/// Finishes the computation of the digest
		/// (the first time it's called) and
		/// returns the message digest.
		///
		/// Can be called multiple times.

	const DigestEngine::Digest& signature();
		/// Signs the digest using the ECDSADSA algorithm
		/// and the private key (the first time it's
		/// called) and returns the result.
		///
		/// Can be called multiple times.

	bool verify(const DigestEngine::Digest& signature);
		/// Verifies the data against the signature.
		///
		/// Returns true if the signature can be verified, false otherwise.

protected:
	void updateImpl(const void* data, std::size_t length) override;

private:
	ECKey _key;
	Poco::Crypto::DigestEngine _engine;
	Poco::DigestEngine::Digest _digest;
	Poco::DigestEngine::Digest _signature;
};


class Crypto_API ECDSASignature
	/// A helper class for dealing with ECDSA signatures.
{
public:
	using ByteVec = std::vector<unsigned char>;

	explicit ECDSASignature(const ByteVec& derSignature);
		/// Creates the ECDSASignature from a DER-encoded signature.

	ECDSASignature(const ByteVec& rawR, const ByteVec& rawS);
		/// Creates the ECDSASignature from raw r and s values.

	~ECDSASignature();
		/// Destroys the ECDSASignature.

	ByteVec toDER() const;
		/// Returns a buffer containing the DER-encoded signature.

	ByteVec rawR() const;
		/// Returns a raw P value.

	ByteVec rawS() const;
		/// Returns a raw Q value.

private:
	ECDSA_SIG* _pSig;
};


} } // namespace Poco::Crypto


#endif // Crypto_ECDSADigestEngine_INCLUDED
