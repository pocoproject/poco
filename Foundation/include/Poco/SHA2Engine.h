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
		SHA_224 = 1,
		SHA_256,
		SHA_384,
		SHA_512,
		SHA_512_224,
		SHA_512_256
	};

	SHA2Engine(ALGORITHM algorithm = SHA_256);
	virtual ~SHA2Engine();

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


class Foundation_API SHA2Engine224 : public SHA2Engine
{
public:
	enum
	{
		BLOCK_SIZE = 64,
		DIGEST_SIZE = 28
	};

	SHA2Engine224(): Poco::SHA2Engine(Poco::SHA2Engine::ALGORITHM::SHA_224)
	{
	}

	virtual ~SHA2Engine224()
	{
	}
};


class Foundation_API SHA2Engine256 : public SHA2Engine
{
public:
	enum
	{
		BLOCK_SIZE = 64,
		DIGEST_SIZE = 32
	};

	SHA2Engine256(): Poco::SHA2Engine(Poco::SHA2Engine::ALGORITHM::SHA_256)
	{
	}

	virtual ~SHA2Engine256()
	{
	}
};


class Foundation_API SHA2Engine384 : public SHA2Engine
{
public:
	enum
	{
		BLOCK_SIZE = 128,
		DIGEST_SIZE = 48
	};

	SHA2Engine384(): Poco::SHA2Engine(Poco::SHA2Engine::ALGORITHM::SHA_384)
	{
	}

	virtual ~SHA2Engine384()
	{
	}
};


class Foundation_API SHA2Engine512 : public SHA2Engine
{
public:
	enum
	{
		BLOCK_SIZE = 128,
		DIGEST_SIZE = 64
	};

	SHA2Engine512(): Poco::SHA2Engine(Poco::SHA2Engine::ALGORITHM::SHA_512)
	{
	}

	virtual ~SHA2Engine512()
	{
	}
};


} // namespace Poco


#endif // Foundation_SHA2Engine_INCLUDED
