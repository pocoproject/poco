//
// Signer.cpp
//
// Library: JWT
// Package: JWT
// Module:  Signer
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/JWT/Signer.h"
#include "Poco/JWT/Serializer.h"
#include "Poco/JWT/JWTException.h"
#include "Poco/HMACEngine.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/DynamicFactory.h"
#include "Poco/MemoryStream.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Crypto/DigestEngine.h"
#include "Poco/Crypto/RSADigestEngine.h"
#include "Poco/Crypto/ECDSADigestEngine.h"
#include <sstream>


namespace Poco {
namespace JWT {


//
// Algorithm
//


class Algorithm: public Poco::RefCountedObject
	/// Abstraction of an algorithm for signing JWTs.
{
public:
	typedef Poco::AutoPtr<Algorithm> Ptr;

	Algorithm();
		/// Creates the Algorithm.

	virtual ~Algorithm();
		/// Destroys the Algorithm.

	virtual Poco::DigestEngine::Digest sign(const Signer& signer, const std::string& header, const std::string& payload) = 0;
		/// Signs the header and payload and returns the signature.

	virtual bool verify(const Signer& signer, const std::string& header, const std::string& payload, const Poco::DigestEngine::Digest& signature) = 0;
		/// Verifies the header and payload against the signature.
		///
		/// Returns true if the signature is correct, otherwise false.

	static const std::string SHA256;
	static const std::string SHA384;
	static const std::string SHA512;
};


Algorithm::Algorithm()
{
}


Algorithm::~Algorithm()
{
}


const std::string Algorithm::SHA256("SHA256");
const std::string Algorithm::SHA384("SHA384");
const std::string Algorithm::SHA512("SHA512");


class SHA256Engine: public Poco::Crypto::DigestEngine
{
public:
	enum
	{
		BLOCK_SIZE  = 64,
		DIGEST_SIZE = 32
	};

	SHA256Engine():
		Poco::Crypto::DigestEngine(Algorithm::SHA256)
	{
	}
};


class SHA384Engine: public Poco::Crypto::DigestEngine
{
public:
	enum
	{
		BLOCK_SIZE  = 128,
		DIGEST_SIZE = 48
	};

	SHA384Engine():
		Poco::Crypto::DigestEngine(Algorithm::SHA384)
	{
	}
};


class SHA512Engine: public Poco::Crypto::DigestEngine
{
public:
	enum
	{
		BLOCK_SIZE  = 128,
		DIGEST_SIZE = 64
	};

	SHA512Engine():
		Poco::Crypto::DigestEngine(Algorithm::SHA512)
	{
	}
};


template <typename Engine>
class HMACAlgorithm: public Algorithm
{
public:
	Poco::DigestEngine::Digest sign(const Signer& signer, const std::string& header, const std::string& payload)
	{
		if (signer.getHMACKey().empty()) throw SignatureGenerationException("No HMAC key available");

		Poco::HMACEngine<Engine> hmac(signer.getHMACKey());
		hmac.update(header);
		hmac.update('.');
		hmac.update(payload);
		return hmac.digest();
	}

	bool verify(const Signer& signer, const std::string& header, const std::string& payload, const Poco::DigestEngine::Digest& signature)
	{
		return sign(signer, header, payload) == signature;
	}
};


typedef HMACAlgorithm<SHA256Engine> HS256;
typedef HMACAlgorithm<SHA384Engine> HS384;
typedef HMACAlgorithm<SHA512Engine> HS512;


class RSAAlgorithm: public Algorithm
{
public:
	RSAAlgorithm(const std::string& digestType):
		_digestType(digestType)
	{
	}

	Poco::DigestEngine::Digest sign(const Signer& signer, const std::string& header, const std::string& payload)
	{
		if (!signer.getRSAKey()) throw SignatureGenerationException("No RSA key available");

		Poco::Crypto::RSADigestEngine rsa(*signer.getRSAKey(), _digestType);
		rsa.update(header);
		rsa.update('.');
		rsa.update(payload);
		return rsa.signature();

	}

	bool verify(const Signer& signer, const std::string& header, const std::string& payload, const Poco::DigestEngine::Digest& signature)
	{
		if (!signer.getRSAKey()) throw SignatureVerificationException("No RSA key available");

		Poco::Crypto::RSADigestEngine rsa(*signer.getRSAKey(), _digestType);
		rsa.update(header);
		rsa.update('.');
		rsa.update(payload);
		return rsa.verify(signature);
	}

private:
	std::string _digestType;
};


class RS256: public RSAAlgorithm
{
public:
	RS256():
		RSAAlgorithm(Algorithm::SHA256)
	{
	}
};


class RS384: public RSAAlgorithm
{
public:
	RS384():
		RSAAlgorithm(Algorithm::SHA384)
	{
	}
};


class RS512: public RSAAlgorithm
{
public:
	RS512():
		RSAAlgorithm(Algorithm::SHA512)
	{
	}
};


class ECDSAAlgorithm: public Algorithm
{
public:
	ECDSAAlgorithm(const std::string& digestType):
		_digestType(digestType)
	{
	}

	Poco::DigestEngine::Digest sign(const Signer& signer, const std::string& header, const std::string& payload)
	{
		if (!signer.getECKey()) throw SignatureGenerationException("No EC key available");

		Poco::Crypto::ECDSADigestEngine ecdsa(*signer.getECKey(), _digestType);
		ecdsa.update(header);
		ecdsa.update('.');
		ecdsa.update(payload);

		Poco::Crypto::ECDSASignature ecdsaSig(ecdsa.signature());
		Poco::DigestEngine::Digest jwtSig(ecdsaSig.rawR());
		Poco::DigestEngine::Digest rawS(ecdsaSig.rawS());
		jwtSig.insert(jwtSig.end(), rawS.begin(), rawS.end());
		return jwtSig;

	}

	bool verify(const Signer& signer, const std::string& header, const std::string& payload, const Poco::DigestEngine::Digest& signature)
	{
		if (!signer.getECKey()) throw SignatureVerificationException("No EC key available");

		Poco::DigestEngine::Digest rawR(signature.begin(), signature.begin() + signature.size() / 2);
		Poco::DigestEngine::Digest rawS(signature.begin() + signature.size() / 2, signature.end());

		Poco::Crypto::ECDSASignature ecdsaSig(rawR, rawS);
		Poco::DigestEngine::Digest derSig = ecdsaSig.toDER();

		Poco::Crypto::ECDSADigestEngine ecdsa(*signer.getECKey(), _digestType);
		ecdsa.update(header);
		ecdsa.update('.');
		ecdsa.update(payload);
		return ecdsa.verify(derSig);
	}

private:
	std::string _digestType;
};


class ES256: public ECDSAAlgorithm
{
public:
	ES256():
		ECDSAAlgorithm(Algorithm::SHA256)
	{
	}
};


class ES384: public ECDSAAlgorithm
{
public:
	ES384():
		ECDSAAlgorithm(Algorithm::SHA384)
	{
	}
};


class ES512: public ECDSAAlgorithm
{
public:
	ES512():
		ECDSAAlgorithm(Algorithm::SHA512)
	{
	}
};


//
// AlgorithmFactory
//


class AlgorithmFactory: public Poco::DynamicFactory<Algorithm>
{
public:
	AlgorithmFactory()
	{
		registerClass<HS256>(Signer::ALGO_HS256);
		registerClass<HS384>(Signer::ALGO_HS384);
		registerClass<HS512>(Signer::ALGO_HS512);

		registerClass<RS256>(Signer::ALGO_RS256);
		registerClass<RS384>(Signer::ALGO_RS384);
		registerClass<RS512>(Signer::ALGO_RS512);

		registerClass<ES256>(Signer::ALGO_ES256);
		registerClass<ES384>(Signer::ALGO_ES384);
		registerClass<ES512>(Signer::ALGO_ES512);
	}
};


//
// Signer
//


const std::string Signer::ALGO_NONE("none");
const std::string Signer::ALGO_HS256("HS256");
const std::string Signer::ALGO_HS384("HS384");
const std::string Signer::ALGO_HS512("HS512");
const std::string Signer::ALGO_RS256("RS256");
const std::string Signer::ALGO_RS384("RS384");
const std::string Signer::ALGO_RS512("RS512");
const std::string Signer::ALGO_ES256("ES256");
const std::string Signer::ALGO_ES384("ES384");
const std::string Signer::ALGO_ES512("ES512");


Signer::Signer()
{
	_algorithms.insert(ALGO_HS256);
}


Signer::Signer(const std::string& hmacKey):
	_hmacKey(hmacKey)
{
	_algorithms.insert(ALGO_HS256);
}


Signer::Signer(const Poco::SharedPtr<Poco::Crypto::RSAKey>& pRSAKey):
	_pRSAKey(pRSAKey)
{
	_algorithms.insert(ALGO_HS256);
}


Signer::Signer(const Poco::SharedPtr<Poco::Crypto::ECKey>& pECKey):
	_pECKey(pECKey)
{
	_algorithms.insert(ALGO_HS256);
}


Signer::~Signer()
{
}


Signer& Signer::setAlgorithms(const std::set<std::string>& algorithms)
{
	_algorithms = algorithms;
	return *this;
}


Signer& Signer::addAlgorithm(const std::string& algorithm)
{
	_algorithms.insert(algorithm);
	return *this;
}


Signer& Signer::addAllAlgorithms()
{
	_algorithms.insert(ALGO_HS256);
	_algorithms.insert(ALGO_HS384);
	_algorithms.insert(ALGO_HS512);

	_algorithms.insert(ALGO_RS256);
	_algorithms.insert(ALGO_RS384);
	_algorithms.insert(ALGO_RS512);

	_algorithms.insert(ALGO_ES256);
	_algorithms.insert(ALGO_ES384);
	_algorithms.insert(ALGO_ES512);

	return *this;
}


Signer& Signer::setHMACKey(const std::string& key)
{
	_hmacKey = key;
	return *this;
}


Signer& Signer::setRSAKey(const Poco::SharedPtr<Poco::Crypto::RSAKey>& pKey)
{
	_pRSAKey = pKey;
	return *this;
}


Signer& Signer::setECKey(const Poco::SharedPtr<Poco::Crypto::ECKey>& pKey)
{
	_pECKey = pKey;
	return *this;
}


std::string Signer::sign(Token& token, const std::string& algorithm) const
{
	AlgorithmFactory factory;
	if (!factory.isClass(algorithm)) throw UnsupportedAlgorithmException(algorithm);

	token.setAlgorithm(algorithm);

	std::string header = Serializer::serialize(token.header());
	std::string payload = Serializer::serialize(token.payload());

	Algorithm::Ptr pAlgorithm = factory.createInstance(algorithm);
	Poco::DigestEngine::Digest digest = pAlgorithm->sign(*this, header, payload);

	std::string signature = encode(digest);
	token.sign(signature);
	std::string jwt = header;
	jwt += '.';
	jwt += payload;
	jwt += '.';
	jwt += signature;
	return jwt;
}


Token Signer::verify(const std::string& jwt) const
{
	Token token;
	if (tryVerify(jwt, token))
		return token;
	else
		throw SignatureVerificationException();
}


bool Signer::tryVerify(const std::string& jwt, Token& token) const
{
	std::vector<std::string> parts = Serializer::split(jwt);
	if (parts.size() < 3) throw ParseException("Not a valid JWT", jwt);

	token.assign(parts[0], parts[1], parts[2]);
	std::string algorithm = token.getAlgorithm();

	if (_algorithms.find(algorithm) == _algorithms.end()) throw UnallowedAlgorithmException(algorithm);

	AlgorithmFactory factory;
	if (!factory.isClass(algorithm)) throw UnsupportedAlgorithmException(algorithm);

	Algorithm::Ptr pAlgorithm = factory.createInstance(algorithm);

	return pAlgorithm->verify(*this, parts[0], parts[1], decode(parts[2]));
}


std::string Signer::encode(const Poco::DigestEngine::Digest& digest)
{
	std::ostringstream stream;
	Poco::Base64Encoder encoder(stream, Poco::BASE64_URL_ENCODING | Poco::BASE64_NO_PADDING);
	encoder.write(reinterpret_cast<const char*>(&digest[0]), digest.size());
	encoder.close();
	return stream.str();
}


Poco::DigestEngine::Digest Signer::decode(const std::string& signature)
{
	Poco::DigestEngine::Digest digest;
	digest.reserve(64);
	Poco::MemoryInputStream stream(signature.data(), signature.size());
	Poco::Base64Decoder decoder(stream, Poco::BASE64_URL_ENCODING | Poco::BASE64_NO_PADDING);
	int ch = decoder.get();
	while (ch != -1)
	{
		digest.push_back(static_cast<unsigned char>(static_cast<unsigned>(ch)));
		ch = decoder.get();
	}
	return digest;
}


} } // namespace Poco::JWT
