//
// MacChineseTradEncoding.h
//
// Library: Encodings
// Package: Encodings
// Module:  MacChineseTradEncoding
//
// Definition of the Windows1252Encoding class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef Encodings_MacChineseTradEncoding_INCLUDED
#define Encodings_MacChineseTradEncoding_INCLUDED


#include "Poco/DoubleByteEncoding.h"


namespace Poco {


class Encodings_API MacChineseTradEncoding: public DoubleByteEncoding
	/// MacChineseTrad Encoding.
	///
	/// This text encoding class has been generated from
	/// http://www.unicode.org/Public/MAPPINGS/VENDORS/APPLE/CHINTRAD.TXT.
{
public:
	MacChineseTradEncoding();
	~MacChineseTradEncoding();

private:
	static const char* _names[];
	static const CharacterMap _charMap;
	static const Mapping _mappingTable[];
	static const Mapping _reverseMappingTable[];
};


} // namespace Poco


#endif // Encodings_MacChineseTradEncoding_INCLUDED
