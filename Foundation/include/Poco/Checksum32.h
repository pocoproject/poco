//
// Checksum32.h
//
// Library: Foundation
// Package: Core
// Module:  Checksum32
//
// Definition of the Checksum32 class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Checksum32_INCLUDED
#define Foundation_Checksum32_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/ChecksumImpl.h"


namespace Poco {


class Foundation_API Checksum32 : public ChecksumImpl
	/// This class calculates CRC-32 or Adler-32 checksums
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
	
{
public:
	Checksum32();
		/// Creates a CRC-32 checksum initialized to 0.

	Checksum32(ChecksumImpl::Type t);
		/// Creates the Checksum32, using the given type.

	~Checksum32();
		/// Destroys the Checksum32.

	void update(const char* data, unsigned length);
		/// Updates the checksum with the given data.

	void update(const std::string& data);
		/// Updates the checksum with the given data.

	void update(char data);
		/// Updates the checksum with the given data.

	Poco::UInt64 checksum() const;
		/// Returns the calculated checksum.

	ChecksumImpl::Type type() const;
		/// Which type of checksum are we calculating.

private:
	Type         _type;
	Poco::UInt32 _value;
};


//
// inlines
//
inline void Checksum32::update(const std::string& data)
{
	update(data.c_str(), static_cast<unsigned int>(data.size()));
}


inline void Checksum32::update(char c)
{
	update(&c, 1);
}


inline Poco::UInt64 Checksum32::checksum() const
{
	return _value;
}


inline ChecksumImpl::Type Checksum32::type() const
{
	return _type;
}


} // namespace Poco


#endif // Foundation_Checksum32_INCLUDED
