//
// MacJapaneseEncoding.h
//
// Library: Encodings
// Package: Encodings
// Module:  MacJapaneseEncoding
//
// Definition of the Windows1252Encoding class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef Encodings_MacJapaneseEncoding_INCLUDED
#define Encodings_MacJapaneseEncoding_INCLUDED


#include "Poco/DoubleByteEncoding.h"


namespace Poco {


class Encodings_API MacJapaneseEncoding: public DoubleByteEncoding
	/// MacJapanese Encoding.
	///
	/// This text encoding class has been generated from
	/// http://www.unicode.org/Public/MAPPINGS/VENDORS/APPLE/JAPANESE.TXT.
{
public:
	MacJapaneseEncoding();
	~MacJapaneseEncoding();

private:
	static const char* _names[];
	static const CharacterMap _charMap;
	static const Mapping _mappingTable[];
	static const Mapping _reverseMappingTable[];
};


} // namespace Poco


#endif // Encodings_MacJapaneseEncoding_INCLUDED
