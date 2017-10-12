//
// Checksum64.h
//
// Library: Foundation
// Package: Core
// Module:  Checksum64
//
// Definition of the Checksum64 class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Checksum64_INCLUDED
#define Foundation_Checksum64_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/ChecksumImpl.h"


namespace Poco {


class Foundation_API Checksum64 : public ChecksumImpl
	/// This class calculates CRC-64 checksums for arbitrary data.
	///
	/// The algorithm and table are based on the polynomial
	///
	/// x^64 + x^62 + x^57 + x^55 + x^54 + x^53 + x^52 + x^47 + x^46 + x^45 +
	/// x^40 + x^39 + x^38 + x^37 + x^35 + x^33 + x^32 + x^31 + x^29 + x^27 +
	/// x^24 + x^23 + x^22 + x^21 + x^19 + x^17 + x^13 + x^12 + x^10 + x^9 +
	/// x^7 + x^4 + x + 1
	///
	/// Implementation is adapted from the DLT1 spec:
	///   ECMA - 182, http://www.ecma-international.org/publications/standards/Ecma-182.htm
	///
{
public:
	Checksum64();
		/// Creates a CRC-64 checksum initialized to 0.

	Checksum64(Type t);
		/// Creates the Checksum64, using the given type.

	~Checksum64();
		/// Destroys the Checksum64.

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
	static Poco::UInt64 _initCRC64Val;
	static Poco::UInt64 _finXORVal;
	static const unsigned _tableSize = 256;
	static Poco::UInt64 _crcTable[_tableSize];
	Poco::UInt64 _value;
};


//
// inlines
//
inline void Checksum64::update(const std::string& data)
{
	update(data.c_str(), static_cast<unsigned int>(data.size()));
}


inline void Checksum64::update(char c)
{
	update(&c, 1);
}


inline Poco::UInt64 Checksum64::checksum() const
{
	return _value;
}


inline ChecksumImpl::Type Checksum64::type() const
{
	return TYPE_CRC64_IMPL;
}


} // namespace Poco


#endif // Foundation_Checksum64_INCLUDED
