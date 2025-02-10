//
// Latin1Encoding.h
//
// Library: Foundation
// Package: Text
// Module:  Latin1Encoding
//
// Definition of the Latin1Encoding class.
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Latin1Encoding_INCLUDED
#define Foundation_Latin1Encoding_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/TextEncoding.h"


namespace Poco {


class Foundation_API Latin1Encoding: public TextEncoding
	/// ISO Latin-1 (8859-1) text encoding.
{
public:
	Latin1Encoding();
	~Latin1Encoding() override;
	const char* canonicalName() const override;
	bool isA(const std::string& encodingName) const override;
	const CharacterMap& characterMap() const override;
	int convert(const unsigned char* bytes) const override;
	int convert(int ch, unsigned char* bytes, int length) const override;
	int queryConvert(const unsigned char* bytes, int length) const override;
	int sequenceLength(const unsigned char* bytes, int length) const override;

private:
	static const char* _names[];
	static const CharacterMap _charMap;
};


} // namespace Poco


#endif // Foundation_Latin1Encoding_INCLUDED
