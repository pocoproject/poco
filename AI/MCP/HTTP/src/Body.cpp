//
// Body.cpp
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  Body
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/HTTP/Body.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Exception.h"
#include <algorithm>


namespace Poco::AI::MCP::HTTP {


namespace
{
	std::string read(std::istream& stream, std::size_t maxSize, const ReadGuard& guard, bool refuseMore)
		/// Reads from the stream buffer itself, so that what its source throws
		/// reaches the caller instead of only setting the stream's badbit.
	{
		std::string text;
		std::streambuf* pBuf = stream.rdbuf();
		if (pBuf == nullptr) return text;
		char buffer[8192];
		for (;;)
		{
			// A prefix that is complete needs nothing more: neither the guard
			// nor a source that has no more yet holds the caller up.
			if (!refuseMore && text.size() >= maxSize) break;
			if (pBuf->in_avail() <= 0)
			{
				if (guard) guard();
				if (pBuf->sgetc() == std::char_traits<char>::eof()) break;
			}
			std::streamsize wanted = std::max<std::streamsize>(1, std::min<std::streamsize>(sizeof(buffer), pBuf->in_avail()));
			if (!refuseMore)
			{
				wanted = std::min<std::streamsize>(wanted, static_cast<std::streamsize>(maxSize - text.size()));
			}
			const std::streamsize n = pBuf->sgetn(buffer, wanted);
			if (n <= 0) break;
			if (refuseMore && text.size() + static_cast<std::size_t>(n) > maxSize)
			{
				throw Poco::DataFormatException("The body exceeds the size limit");
			}
			text.append(buffer, static_cast<std::size_t>(n));
		}
		return text;
	}
}


namespace Impl {


std::string readBody(std::istream& stream, std::size_t maxSize, const ReadGuard& guard)
{
	return read(stream, maxSize, guard, true);
}


std::string readAtMost(std::istream& stream, std::size_t maxSize, const ReadGuard& guard)
{
	return read(stream, maxSize, guard, false);
}


void cutAtCharacter(std::string& text, std::size_t maxSize)
{
	if (text.size() <= maxSize) return;
	std::size_t n = maxSize;
	// A continuation byte (10xxxxxx) belongs to the character before it.
	while (n > 0 && (static_cast<unsigned char>(text[n]) & 0xC0) == 0x80) --n;
	text.resize(n);
}


std::string excerpt(const std::string& text, std::size_t maxSize)
{
	if (text.size() <= maxSize) return text;
	std::string result(text);
	cutAtCharacter(result, maxSize);
	return result + "...";
}


Poco::JSON::Object::Ptr parseObject(const std::string& text)
{
	try
	{
		Poco::JSON::Parser parser;
		const Poco::Dynamic::Var parsed = parser.parse(text);
		if (parsed.type() == typeid(Poco::JSON::Object::Ptr)) return parsed.extract<Poco::JSON::Object::Ptr>();
	}
	catch (const Poco::Exception&)
	{
	}
	return Poco::JSON::Object::Ptr();
}


bool brokeOff(const Poco::Net::HTTPMessage& message, std::size_t received)
{
	return message.hasContentLength() && static_cast<Poco::Int64>(received) < message.getContentLength64();
}


bool brokeOff(const Poco::Net::HTTPSession& session, const Poco::Net::HTTPMessage& message, std::size_t received)
{
	return session.networkException() != nullptr || brokeOff(message, received);
}


} // namespace Impl


} // namespace Poco::AI::MCP::HTTP
