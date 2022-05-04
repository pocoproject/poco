//
// DigestEngine.cpp
//
// Library: Foundation
// Package: Crypt
// Module:  DigestEngine
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DigestEngine.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"


namespace Poco
{

DigestEngine::DigestEngine()
{
}


DigestEngine::~DigestEngine()
{
}


std::string DigestEngine::digestToHex(const Digest& bytes, std::size_t length)
{
	static const char digits[] = "0123456789abcdef";
	const std::size_t fullLen = bytes.size()*2;
	std::size_t len = length ? length*2 : fullLen;
	if (len > fullLen)
		throw Poco::InvalidArgumentException(
			Poco::format("DigestEngine::digestToHex(): invalid length : %z,"
				"max alllowed is %z", length, fullLen));
	std::string result;
	result.reserve(len);
	for (auto b: bytes)
	{
		result += digits[(b >> 4) & 0xF];
		result += digits[b & 0xF];
		if (result.size() >= len) break;
	}
	return result;
}


DigestEngine::Digest DigestEngine::digestFromHex(const std::string& digest)
{
	if (digest.size() % 2 != 0)
		throw DataFormatException();
	Digest result;
	result.reserve(digest.size() / 2);
	for (std::size_t i = 0; i < digest.size(); ++i)
	{
		int c = 0;
		// first upper 4 bits
		if (digest[i] >= '0' && digest[i] <= '9')
			c = digest[i] - '0';
		else if (digest[i] >= 'a' && digest[i] <= 'f')
			c = digest[i] - 'a' + 10;
		else if (digest[i] >= 'A' && digest[i] <= 'F')
			c = digest[i] - 'A' + 10;
		else
			throw DataFormatException();
		c <<= 4;
		++i;
		if (digest[i] >= '0' && digest[i] <= '9')
			c += digest[i] - '0';
		else if (digest[i] >= 'a' && digest[i] <= 'f')
			c += digest[i] - 'a' + 10;
		else if (digest[i] >= 'A' && digest[i] <= 'F')
			c += digest[i] - 'A' + 10;
		else
			throw DataFormatException();

		result.push_back(static_cast<unsigned char>(c));
	}
	return result;
}


bool DigestEngine::constantTimeEquals(const Digest& d1, const Digest& d2)
{
	if (d1.size() != d2.size()) return false;

	int result = 0;
	Digest::const_iterator it1 = d1.begin();
	Digest::const_iterator it2 = d2.begin();
	Digest::const_iterator end1 = d1.end();
	while (it1 != end1)
	{
		result |= *it1++ ^ *it2++;
	}
	return result == 0;
}


} // namespace Poco

