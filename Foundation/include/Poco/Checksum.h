//
// Checksum.h
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// Definition of the Checksum class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Checksum_INCLUDED
#define Foundation_Checksum_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


class Foundation_API Checksum
	/// This class calculates CRC-32, CRC-64 or Adler-32 checksums
	/// for arbitrary data.
	///
	/// A cyclic redundancy check (CRC) is a type of hash function, which is used to produce a
	/// small, fixed-size checksum of a larger block of data, such as a packet of network
	/// traffic or a computer file. CRC-32 is one of the most commonly used CRC algorithms.
	///
	/// Adler-32 is a checksum algorithm which was invented by Mark Adler.
	/// It is almost as reliable as a 32-bit cyclic redundancy check for protecting against
	/// accidental modification of data, such as distortions occurring during a transmission,
	/// but is significantly faster to calculate in software.
	///
	/// CRC-64 comes in two forms with the polynomial of ECMA-182:
	/// CRC-64/XZ, used by xz and 7-Zip, and CRC-64/ECMA-182.
	///
	/// CRC-32 uses the CRC32 instructions of ARMv8 and carry-less
	/// multiplication on x86-64; CRC-64 uses carry-less multiplication on both.
	/// The implementation is selected at run time.

{
public:
	enum Type
	{
		TYPE_ADLER32 = 0,
		TYPE_CRC32,
		TYPE_CRC64_XZ,     /// CRC-64/XZ: reflected, initial value and final XOR all ones
		TYPE_CRC64_ECMA182 /// CRC-64/ECMA-182: not reflected, initial value and final XOR zero
	};

	Checksum();
		/// Creates a CRC-32 checksum initialized to 0.

	Checksum(Type t);
		/// Creates the Checksum, using the given type.

	~Checksum();
		/// Destroys the Checksum.

	void update(const char* data, std::size_t length);
		/// Updates the checksum with the given data.
		///
		/// data may be null only if length is 0.

	void update(const std::string& data);
		/// Updates the checksum with the given data.

	void update(char data);
		/// Updates the checksum with the given data.

	[[nodiscard]] Poco::UInt32 checksum() const;
		/// Returns the calculated CRC-32 or Adler-32 checksum.
		///
		/// Throws an InvalidAccessException for a CRC-64 checksum.

	[[nodiscard]] Poco::UInt64 checksum64() const;
		/// Returns the calculated CRC-64 checksum.
		///
		/// Throws an InvalidAccessException for a CRC-32 or Adler-32 checksum.

	[[nodiscard]] Type type() const;
		/// Which type of checksum are we calulcating

private:
	Type         _type;
	Poco::UInt64 _value;
};


//
// inlines
//
inline void Checksum::update(const std::string& data)
{
	update(data.data(), data.size());
}


inline void Checksum::update(char c)
{
	update(&c, 1);
}


inline Checksum::Type Checksum::type() const
{
	return _type;
}


} // namespace Poco


#endif // Foundation_Checksum_INCLUDED
