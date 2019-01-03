//
// SHA3Engine.h
//
// Library: Foundation
// Package: Crypt
// Module:  SHA3Engine
//
// Definition of class SHA3Engine.
//
/// This class implements the SHA-3 message digest algorithm.
/// (FIPS 202, see http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf)
//
// Based on the Keccak Code Package (public domain)
// https://github.com/gvanas/KeccakCodePackage
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Foundation_SHA3Engine_INCLUDED
#define Foundation_SHA3Engine_INCLUDED

#include "Poco/Foundation.h"
#include "Poco/DigestEngine.h"

namespace Poco {

class Foundation_API SHA3Engine: public DigestEngine
	/// This class implements the SHA-3 message digest algorithm.
	/// (FIPS 202, see http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf)
{
public:
	enum ALGORITHM
	{
		SHA3_224 = 224,
		SHA3_256 = 256,
		SHA3_384 = 384,
		SHA3_512 = 512,
	};

	SHA3Engine(ALGORITHM algorithm = SHA3_512);
	~SHA3Engine();
		
	std::size_t digestLength() const;
	void reset();
	const DigestEngine::Digest& digest();

protected:
	void updateImpl(const void* data, std::size_t length);

private:
	void transform();

	void* _context;
	ALGORITHM _algorithm;
	DigestEngine::Digest _digest;

	SHA3Engine(const SHA3Engine&);
	SHA3Engine& operator = (const SHA3Engine&);
};

} // namespace Poco

#endif // Foundation_SHA3Engine_INCLUDED
