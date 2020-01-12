//
// SHA2Engine.h
//
// Library: Foundation
// Package: Crypt
// Module:  SHA2Engine
//
// Definition of class SHA2Engine.
//
// Secure Hash Standard SHA-2 algorithm
// (FIPS 180-4, see http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)
//
// Based on the implementation of mbed TLS (Apache 2.0)
// http://www.apache.org/licenses/LICENSE-2.0
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_SHA2Engine_INCLUDED
#define Foundation_SHA2Engine_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/DigestEngine.h"


namespace Poco {


class Foundation_API SHA2Engine: public DigestEngine
	/// This class implements the SHA-2 message digest algorithm.
	/// (FIPS 180-4, see http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)
{
public:
	enum ALGORITHM
	{
		SHA_224 = 224,
		SHA_256 = 256,
		SHA_384 = 384,
		SHA_512 = 512
	};

	SHA2Engine(ALGORITHM algorithm = SHA_256);
	~SHA2Engine();

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

	SHA2Engine(const SHA2Engine&);
	SHA2Engine& operator = (const SHA2Engine&);
};


} // namespace Poco


#endif // Foundation_SHA2Engine_INCLUDED
