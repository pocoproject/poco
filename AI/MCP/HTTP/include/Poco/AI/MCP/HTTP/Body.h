//
// Body.h
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  Body
//
// Functions that read the body of an HTTP response that carries JSON.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCPHTTP_Body_INCLUDED
#define AIMCPHTTP_Body_INCLUDED


#include "Poco/AI/MCP/HTTP/HTTP.h"
#include "Poco/JSON/Object.h"
#include <functional>
#include <istream>
#include <string>


namespace Poco::AI::MCP::HTTP {


using ReadGuard = std::function<void ()>;
	/// Called before every read of a stream that may block, because nothing
	/// is buffered. It throws to stop the reading, e.g. Poco::TimeoutException
	/// once a deadline has passed, and may bound the read that follows, e.g.
	/// by setting the receive timeout of the socket to the time that is left.


[[nodiscard]] AIMCPHTTP_API std::string readBody(std::istream& stream, std::size_t maxSize, const ReadGuard& guard = ReadGuard());
	/// Reads the stream to its end.
	///
	/// Throws Poco::DataFormatException when it holds more than maxSize
	/// bytes. What the source of the stream throws, a timeout or a reset
	/// connection, is passed on: a body that breaks off is not taken for a
	/// whole one.


[[nodiscard]] AIMCPHTTP_API std::string readAtMost(std::istream& stream, std::size_t maxSize, const ReadGuard& guard = ReadGuard());
	/// Reads at most maxSize bytes of the stream, e.g. the start of the body
	/// of an error. Failures are passed on as by readBody().


AIMCPHTTP_API void cutAtCharacter(std::string& text, std::size_t maxSize);
	/// Cuts text to at most maxSize bytes, at a UTF-8 character boundary.


[[nodiscard]] AIMCPHTTP_API std::string excerpt(const std::string& text, std::size_t maxSize = 500);
	/// The start of text, at most maxSize bytes cut at a character boundary,
	/// followed by "..." when text is longer: for an error message.


[[nodiscard]] AIMCPHTTP_API Poco::JSON::Object::Ptr parseObject(const std::string& text);
	/// The JSON object in text, or null when text is not a JSON object.


} // namespace Poco::AI::MCP::HTTP


#endif // AIMCPHTTP_Body_INCLUDED
