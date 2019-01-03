//
// BLAKE2Engine.h
//
// Library: Foundation
// Package: Crypt
// Module:  BLAKE2Engine
//
// Definition of class BLAKE2Engine.
//
// This class implements the BLAKE2 hashing algorithm.
// (RFC 7693, see https://tools.ietf.org/html/rfc7693)
//
// Based on the BLAKE2 reference implementation (CC0, OpenSSL or Apache 2.0)
// http://creativecommons.org/publicdomain/zero/1.0
// https://www.openssl.org/source/license.html
// http://www.apache.org/licenses/LICENSE-2.0
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef Foundation_BLAKE2Engine_INCLUDED
#define Foundation_BLAKE2Engine_INCLUDED

#include "Poco/Foundation.h"
#include "Poco/DigestEngine.h"

namespace Poco {

class Foundation_API BLAKE2Engine: public DigestEngine
	/// This class implements the BLAKE2 hashing algorithm.
	/// (RFC 7693, see https://tools.ietf.org/html/rfc7693)
{
public:
	enum ALGORITHM
	{
		BLAKE2b_224 = 224,
		BLAKE2b_256 = 256,
		BLAKE2b_384 = 384,
		BLAKE2b_512 = 512
	};

	BLAKE2Engine(ALGORITHM algorithm = BLAKE2b_512);
	~BLAKE2Engine();
		
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

	BLAKE2Engine(const BLAKE2Engine&);
	BLAKE2Engine& operator = (const BLAKE2Engine&);
};

} // namespace Poco

#endif // Foundation_BLAKE2Engine_INCLUDED
