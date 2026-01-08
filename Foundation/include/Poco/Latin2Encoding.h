//
// Latin2Encoding.h
//
// Library: Foundation
// Package: Text
// Module:  Latin2Encoding
//
// Definition of the Latin2Encoding class.
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Latin2Encoding_INCLUDED
#define Foundation_Latin2Encoding_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/TextEncoding.h"


namespace Poco {


class Foundation_API Latin2Encoding: public TextEncoding
	/// ISO Latin-2 (8859-2) text encoding.
	///
	/// Latin-2 is basically Latin-1 with the EURO sign plus
	/// some other minor changes.
{
public:
	Latin2Encoding();
	~Latin2Encoding() override;
	[[nodiscard]]
	const char* canonicalName() const override;
	[[nodiscard]]
	bool isA(const std::string& encodingName) const override;
	[[nodiscard]]
	const CharacterMap& characterMap() const override;
	[[nodiscard]]
	int convert(const unsigned char* bytes) const override;
	[[nodiscard]]
	int convert(int ch, unsigned char* bytes, int length) const override;
	[[nodiscard]]
	int queryConvert(const unsigned char* bytes, int length) const override;
	[[nodiscard]]
	int sequenceLength(const unsigned char* bytes, int length) const override;

private:
	static const char* _names[];
	static const CharacterMap _charMap;
};


} // namespace Poco


#endif // Foundation_Latin2Encoding_INCLUDED
