//
// MacCentralEurRomanEncoding.h
//
// Library: Encodings
// Package: Encodings
// Module:  MacCentralEurRomanEncoding
//
// Definition of the Windows1252Encoding class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef Encodings_MacCentralEurRomanEncoding_INCLUDED
#define Encodings_MacCentralEurRomanEncoding_INCLUDED


#include "Poco/DoubleByteEncoding.h"


namespace Poco {


class Encodings_API MacCentralEurRomanEncoding: public DoubleByteEncoding
	/// MacCentralEurRoman Encoding.
	///
	/// This text encoding class has been generated from
	/// http://www.unicode.org/Public/MAPPINGS/VENDORS/APPLE/CENTEURO.TXT.
{
public:
	MacCentralEurRomanEncoding();
	~MacCentralEurRomanEncoding();

private:
	static const char* _names[];
	static const CharacterMap _charMap;
	static const Mapping _mappingTable[];
	static const Mapping _reverseMappingTable[];
};


} // namespace Poco


#endif // Encodings_MacCentralEurRomanEncoding_INCLUDED
