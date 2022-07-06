//
// MacKoreanEncoding.h
//
// Library: Encodings
// Package: Encodings
// Module:  MacKoreanEncoding
//
// Definition of the Windows1252Encoding class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef Encodings_MacKoreanEncoding_INCLUDED
#define Encodings_MacKoreanEncoding_INCLUDED


#include "Poco/DoubleByteEncoding.h"


namespace Poco {


class Encodings_API MacKoreanEncoding: public DoubleByteEncoding
	/// MacKorean Encoding.
	///
	/// This text encoding class has been generated from
	/// http://www.unicode.org/Public/MAPPINGS/VENDORS/APPLE/KOREAN.TXT.
{
public:
	MacKoreanEncoding();
	~MacKoreanEncoding();

private:
	static const char* _names[];
	static const CharacterMap _charMap;
	static const Mapping _mappingTable[];
	static const Mapping _reverseMappingTable[];
};


} // namespace Poco


#endif // Encodings_MacKoreanEncoding_INCLUDED
