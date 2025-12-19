//
// ULID.h
//
// Library: Foundation
// Package: ULID
// Module:  ULID
//
// Definition of the ULID class.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ULID_INCLUDED
#define Foundation_ULID_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


class Foundation_API ULID
	/// This class implements a [Universally Unique Lexicographically Sortable Identifier](https://github.com/ulid/spec) (ULID).
	///
	/// To generate a new random ULID, see ULIDGenerator::create().
{
public:
	ULID() = default;
		/// Creates a nil (all zero) ULID.

	ULID(const ULID& ulid) = default;
		/// Copy constructor.

	explicit ULID(const std::string& ulid);
		/// Parses the ULID from a string.

	explicit ULID(const char* ulid);
		/// Parses the ULID from a string.

	ULID(UInt64 time, UInt16 randomHigh, UInt64 randomLow);
		/// Creates a ULID from its components.

	~ULID() = default;
		/// Destroys the ULID.

	ULID& operator = (const ULID& ulid);
		/// Assignment operator.

	void swap(ULID& ulid) noexcept;
		/// Swaps the ULID with another one.

	void parse(const std::string& ulid);
		/// Parses the ULID from its string representation.

	bool tryParse(const std::string& ulid);
		/// Tries to interpret the given string as an ULID.
		/// If the ULID is syntactically valid, assigns the
		/// members and returns true. Otherwise leaves the
		/// object unchanged and returns false.

	std::string toString() const;
		/// Returns a string representation of the ULID, 
		/// using Crockford's Base 32 encoding.

	void copyFrom(const char* buffer);
		/// Copies the ULID (16 bytes) from a buffer or byte array.
		/// The ULID fields are expected to be
		/// stored in network byte order.
		/// The buffer need not be aligned.

	void copyTo(char* buffer) const;
		/// Copies the ULID to the buffer. The fields
		/// are in network byte order.
		/// The buffer need not be aligned.
		/// There must have room for at least 16 bytes.

	Poco::UInt64 time() const;
		/// Returns the 48-bit time part of the ULID
		/// (milliseconds since Unix epoch).

	Poco::UInt16 randomHigh() const;
		/// Returns the most significant 16 bits of the 80-bit
		/// random part of the ULID.

	Poco::UInt64 randomLow() const;
		/// Returns the least significant 64 bits of the 80-bit
		/// random part of the ULID.

	bool operator == (const ULID& ulid) const;
	bool operator != (const ULID& ulid) const;
	bool operator <  (const ULID& ulid) const;
	bool operator <= (const ULID& ulid) const;
	bool operator >  (const ULID& ulid) const;
	bool operator >= (const ULID& ulid) const;

	bool isNull() const;
		/// Returns true iff the ULID is nil (in other words,
		/// consists of all zeros).

	static const ULID& null();
		/// Returns a null/nil ULID.

protected:
	int compare(const ULID& ulid) const;

	template <typename T>
	static T decode(char c)
	{
		return static_cast<T>(REVERSE_ENCODING[static_cast<unsigned char>(c)]);
	}

private:
	UInt32 _timeHi = 0;
	UInt16 _timeLo = 0;
	UInt16 _randomHi = 0;
	UInt64 _randomLo = 0;

	static const char ENCODING[32];
	static const UInt8 REVERSE_ENCODING[256];
};


//
// inlines
//
inline Poco::UInt64 ULID::time() const
{
	return (static_cast<Poco::UInt64>(_timeHi) << 16) + _timeLo;
}


inline Poco::UInt16 ULID::randomHigh() const
{
	return _randomHi;
}


inline Poco::UInt64 ULID::randomLow() const
{
	return _randomLo;
}


inline bool ULID::operator == (const ULID& ulid) const
{
	return compare(ulid) == 0;
}


inline bool ULID::operator != (const ULID& ulid) const
{
	return compare(ulid) != 0;
}


inline bool ULID::operator < (const ULID& ulid) const
{
	return compare(ulid) < 0;
}


inline bool ULID::operator <= (const ULID& ulid) const
{
	return compare(ulid) <= 0;
}


inline bool ULID::operator > (const ULID& ulid) const
{
	return compare(ulid) > 0;
}


inline bool ULID::operator >= (const ULID& ulid) const
{
	return compare(ulid) >= 0;
}


inline bool ULID::isNull() const
{
	return compare(null()) == 0;
}


inline void swap(ULID& u1, ULID& u2) noexcept
{
	u1.swap(u2);
}


} // namespace Poco


#endif // Foundation_ULID_INCLUDED
