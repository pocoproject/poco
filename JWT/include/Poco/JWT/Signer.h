//
// Signer.h
//
// Library: JWT
// Package: JWT
// Module:  Signer
//
// Definition of the Signer class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JWT_Signer_INCLUDED
#define JWT_Signer_INCLUDED


#include "Poco/JWT/JWT.h"
#include "Poco/JWT/Token.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/ECKey.h"
#include "Poco/DigestEngine.h"
#include <set>


namespace Poco {
namespace JWT {


class JWT_API Signer
	/// This class signs and verifies the signature of JSON Web Tokens.
	///
	/// The following signing algorithms are supported:
	///   - HS256 (HMAC using SHA256)
	///   - HS384 (HMAC using SHA384)
	///   - HS512 (HMAC using SHA512)
	///   - RS256 (RSA SSA PKCS1 v1.5 using SHA256)
	///   - RS384 (RSA SSA PKCS1 v1.5 using SHA384)
	///   - RS512 (RSA SSA PKCS1 v1.5 using SHA512)
	///   - ES256 (ECDSA using P-256 and SHA-256)
	///   - ES384 (ECDSA using P-256 and SHA-384)
	///   - ES512 (ECDSA using P-256 and SHA-512)
{
public:
	Signer();
		/// Creates a Signer.
		///
		/// For signing and verification, a key must be set using the
		/// setHMACKey(), setRSAKey() or setECKey() methods.
		///
		/// Sets HS256 as the only allowed algorithm.
		/// Call setAlgorithms() or addAlgorithm() to allow additional
		/// algorithms for verification.

	explicit Signer(const std::string& hmacKey);
		/// Creates the Signer using the given secret/key for HMAC-based signing and verification.
		///
		/// Sets HS256 as the only allowed algorithm.
		/// Call setAlgorithms() or addAlgorithm() to allow additional
		/// algorithms for verification.

	explicit Signer(const Poco::SharedPtr<Poco::Crypto::RSAKey>& pRSAKey);
		/// Creates the Signer using the given secret/key for RSA-based signing and verification.
		///
		/// Sets HS256 as the only allowed algorithm.
		/// Call setAlgorithms() or addAlgorithm() to allow additional
		/// algorithms for verification.

	explicit Signer(const Poco::SharedPtr<Poco::Crypto::ECKey>& pECKey);
		/// Creates the Signer using the given secret/key for EC-based signing and verification.
		///
		/// Sets HS256 as the only allowed algorithm.
		/// Call setAlgorithms() or addAlgorithm() to allow additional
		/// algorithms for verification.

	~Signer();
		/// Destroys the Signer.

	Signer& setAlgorithms(const std::set<std::string>& algorithms);
		/// Sets the allowed algorithms for signing.
		///
		/// When verifying JWTs, the algorithm used for signing
		/// must be one of the allowed algorithms.

	const std::set<std::string>& getAlgorithms() const;
		/// Returns the allowed algorithms for signing.

	Signer& addAlgorithm(const std::string& algorithm);
		/// Adds an algorithm to the set of allowed algorithms.

	Signer& addAllAlgorithms();
		/// Adds all supported algorithm to the set of allowed algorithms.

	Signer& setHMACKey(const std::string& key);
		/// Sets the key used for HMAC-based signing and verification.

	const std::string getHMACKey() const;
		/// Returns the key used for HMAC-based signing and verification.

	Signer& setRSAKey(const Poco::SharedPtr<Poco::Crypto::RSAKey>& pKey);
		/// Sets the key used for RSA-based signing and verification.

	Poco::SharedPtr<Poco::Crypto::RSAKey> getRSAKey() const;
		/// Returns the key used for RSA-based signing and verification.

	Signer& setECKey(const Poco::SharedPtr<Poco::Crypto::ECKey>& pKey);
		/// Sets the key used for EC-based signing and verification.

	Poco::SharedPtr<Poco::Crypto::ECKey> getECKey() const;
		/// Returns the key used for EC-based signing and verification.

	std::string sign(Token& token, const std::string& algorithm) const;
		/// Signs the given token using the given algorithm.
		///
		/// An appropriate key must have been provided prior to calling sign().
		///
		/// Returns the serialized JWT including the signature.

	Token verify(const std::string& jwt) const;
		/// Verifies the given serialized JSON Web Token.
		///
		/// An appropriate key must have been provided prior to calling verify().
		///
		/// If successful, returns a Token object.
		/// If not successful, throws a SignatureVerificationException.

	bool tryVerify(const std::string& jwt, Token& token) const;
		/// Verifies the given serialized JSON Web Token and stores
		/// it in the given Token object.
		///
		/// An appropriate key must have been provided prior to calling verify().
		///
		/// If successful, returns true, otherwise false.

	static const std::string ALGO_NONE;
	static const std::string ALGO_HS256;
	static const std::string ALGO_HS384;
	static const std::string ALGO_HS512;
	static const std::string ALGO_RS256;
	static const std::string ALGO_RS384;
	static const std::string ALGO_RS512;
	static const std::string ALGO_ES256;
	static const std::string ALGO_ES384;
	static const std::string ALGO_ES512;

protected:
	static std::string encode(const Poco::DigestEngine::Digest& digest);
	static Poco::DigestEngine::Digest decode(const std::string& signature);

private:
	Signer(const Signer&);
	Signer& operator = (const Signer&);

	std::set<std::string> _algorithms;
	std::string _hmacKey;
	Poco::SharedPtr<Poco::Crypto::RSAKey> _pRSAKey;
	Poco::SharedPtr<Poco::Crypto::ECKey> _pECKey;
};


//
// inlines
//


inline const std::set<std::string>& Signer::getAlgorithms() const
{
	return _algorithms;
}


inline const std::string Signer::getHMACKey() const
{
	return _hmacKey;
}


inline Poco::SharedPtr<Poco::Crypto::RSAKey> Signer::getRSAKey() const
{
	return _pRSAKey;
}


inline Poco::SharedPtr<Poco::Crypto::ECKey> Signer::getECKey() const
{
	return _pECKey;
}


} } // namespace Poco::JWT


#endif // JWT_Signer_INCLUDED
