//
// Headers.cpp
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  Headers
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Headers.h"
#include "Poco/JSON/Array.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/NumberFormatter.h"
#include "Poco/StreamCopier.h"
#include "Poco/String.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/Exception.h"
#include <cmath>
#include <sstream>


namespace Poco::AI::MCP::HTTP {


namespace
{
	const std::string SENTINEL_PREFIX("=?base64?");
	const std::string SENTINEL_SUFFIX("?=");
	constexpr Poco::Int64 MAX_SAFE_INTEGER = 9007199254740991;
		/// The largest integer a header may carry: the safe range of JavaScript.


	bool isCanonicalBase64(const std::string& payload)
		/// Returns true if payload is base64 in its one canonical spelling:
		/// whole groups of four characters of the alphabet, the last of which
		/// may end with one or two '=' of padding.
	{
		if (payload.size() % 4 != 0) return false;
		std::size_t padding = 0;
		if (!payload.empty() && payload.back() == '=') ++padding;
		if (padding == 1 && payload[payload.size() - 2] == '=') ++padding;
		for (std::size_t i = 0; i < payload.size() - padding; ++i)
		{
			const char c = payload[i];
			const bool alphabet = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '+' || c == '/';
			if (!alphabet) return false;
		}
		return true;
	}


	bool isUTF8(const std::string& text)
		/// Returns true if text is well-formed UTF-8.
	{
		const unsigned char* pByte = reinterpret_cast<const unsigned char*>(text.data());
		const unsigned char* pEnd = pByte + text.size();
		while (pByte < pEnd)
		{
			int length = 1;
			if (*pByte >= 0xF0) length = 4;
			else if (*pByte >= 0xE0) length = 3;
			else if (*pByte >= 0xC0) length = 2;
			if (pEnd - pByte < length || !Poco::UTF8Encoding::isLegal(pByte, length)) return false;
			pByte += length;
		}
		return true;
	}
}


bool isPlain(const std::string& value)
{
	for (const char c: value)
	{
		const unsigned char u = static_cast<unsigned char>(c);
		if (u < 0x20 || u > 0x7E) return false;
	}
	if (!value.empty() && (value.front() == ' ' || value.back() == ' ')) return false;
	// "=?" starts the base64 form, and an RFC 2047 encoded word, which a
	// server that decodes its request headers would rewrite.
	return value.find("=?") == std::string::npos;
}


bool isHeaderText(const std::string& value)
{
	for (const char c: value)
	{
		const unsigned char u = static_cast<unsigned char>(c);
		if ((u < 0x20 && u != '\t') || u > 0x7E) return false;
	}
	return true;
}


std::string headerValue(const std::string& value)
{
	if (isPlain(value)) return value;
	std::ostringstream encoded;
	Poco::Base64Encoder encoder(encoded);
	encoder.rdbuf()->setLineLength(0);
	encoder << value;
	encoder.close();
	return SENTINEL_PREFIX + encoded.str() + SENTINEL_SUFFIX;
}


bool decodeHeaderValue(const std::string& received, std::string& value)
{
	if (received.size() < SENTINEL_PREFIX.size() + SENTINEL_SUFFIX.size()
		|| received.compare(0, SENTINEL_PREFIX.size(), SENTINEL_PREFIX) != 0
		|| received.compare(received.size() - SENTINEL_SUFFIX.size(), SENTINEL_SUFFIX.size(), SENTINEL_SUFFIX) != 0)
	{
		if (!isHeaderText(received)) return false;
		value = received;
		return true;
	}
	const std::string payload = received.substr(SENTINEL_PREFIX.size(), received.size() - SENTINEL_PREFIX.size() - SENTINEL_SUFFIX.size());
	if (!isCanonicalBase64(payload)) return false;
	try
	{
		std::istringstream encoded(payload);
		Poco::Base64Decoder decoder(encoded);
		std::string decoded;
		Poco::StreamCopier::copyToString(decoder, decoded);
		if (decoder.bad() || !isUTF8(decoded)) return false;
		value = decoded;
		return true;
	}
	catch (const Poco::DataFormatException&)
	{
		return false;
	}
}


bool renderParam(const Poco::Dynamic::Var& value, std::string& text)
{
	if (value.isEmpty()) return false;
	if (value.isString())
	{
		text = value.extract<std::string>();
		return true;
	}
	if (value.isBoolean())
	{
		text = value.extract<bool>() ? "true" : "false";
		return true;
	}
	// An integer outside the safe range has no header, whether it came as
	// an integer or as a number with no fraction.
	if (value.isInteger())
	{
		Poco::Int64 number = 0;
		try
		{
			number = value.convert<Poco::Int64>();
		}
		catch (const Poco::RangeException&)
		{
			return false;
		}
		if (number < -MAX_SAFE_INTEGER || number > MAX_SAFE_INTEGER) return false;
		text = Poco::NumberFormatter::format(number);
		return true;
	}
	if (value.isNumeric())
	{
		const double number = value.convert<double>();
		if (std::fabs(number) > static_cast<double>(MAX_SAFE_INTEGER) || std::floor(number) != number) return false;
		text = Poco::NumberFormatter::format(static_cast<Poco::Int64>(number));
		return true;
	}
	return false;
}


Poco::Dynamic::Var valueAt(const Poco::JSON::Object::Ptr& pArguments, const std::vector<std::string>& path)
{
	Poco::JSON::Object::Ptr pObject = pArguments;
	for (std::size_t i = 0; i + 1 < path.size(); ++i)
	{
		if (pObject.isNull() || !pObject->isObject(path[i])) return Poco::Dynamic::Var();
		pObject = pObject->getObject(path[i]);
	}
	if (pObject.isNull() || path.empty()) return Poco::Dynamic::Var();
	return pObject->get(path.back());
}


} // namespace Poco::AI::MCP::HTTP
