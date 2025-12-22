//
// ULIDGenerator.h
//
// Library: Foundation
// Package: ULID
// Module:  ULID
//
// Definition of the ULIDGenerator class.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ULIDGenerator_INCLUDED
#define Foundation_ULIDGenerator_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/ULID.h"
#include "Poco/Mutex.h"


namespace Poco {


class Foundation_API ULIDGenerator
	/// This class implements a generator for 
	/// [Universally Unique Lexicographically Sortable Identifier](https://github.com/ulid/spec) (ULID)s.
{
public:
	ULIDGenerator();
		/// Creates the ULIDGenerator.

	~ULIDGenerator();
		/// Destroys the ULIDGenerator.

	ULID create();
		/// Creates a new ULID based on the current time.
		///
		/// NOTE: In the very rare case that the random part of the ULID overflows due to too many
		/// subsequent calls to create() in the same millisecond, create() will wait for the 
		/// current millisecond to elapse before generating a new ULID.

	ULID create(UInt64 time);
		/// Creates a new ULID based on the given time (milliseconds
		/// since UNIX epoch).
		///
		/// NOTE: In the very rare case of an overflow of the random part due to too many subsequent
		/// calls to create() with the same time, throws a RuntimeException.

	static ULIDGenerator& defaultGenerator();
		/// Returns a reference to the default ULIDGenerator.

private:
	FastMutex    _mutex;
	Poco::UInt64 _lastTime = 0;
	Poco::UInt16 _lastRandomHi = 0;
	Poco::UInt64 _lastRandomLo = 0;

	ULIDGenerator(const ULIDGenerator&);
	ULIDGenerator& operator = (const ULIDGenerator&);
};


} // namespace Poco


#endif // Foundation_ULIDGenerator_INCLUDED
