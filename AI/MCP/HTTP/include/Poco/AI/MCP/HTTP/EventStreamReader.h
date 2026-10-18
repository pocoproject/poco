//
// EventStreamReader.h
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  EventStreamReader
//
// Definition of the EventStreamReader class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCPHTTP_EventStreamReader_INCLUDED
#define AIMCPHTTP_EventStreamReader_INCLUDED


#include "Poco/AI/MCP/HTTP/HTTP.h"
#include "Poco/Timestamp.h"
#include <istream>
#include <string>


namespace Poco::AI::MCP::HTTP {


class AIMCPHTTP_API EventStreamReader
	/// Reads server-sent events, the text/event-stream format of the HTML
	/// Living Standard, from a stream:
	///
	///   - a line ends with CR LF, LF or CR;
	///   - a line that starts with a colon is a comment;
	///   - "field: value" sets a field, with one space after the colon
	///     dropped; a line without a colon is a field with an empty value;
	///   - the data lines of an event are joined with a newline, the event
	///     field sets its type, the id field its id; other fields are ignored;
	///   - a blank line ends the event, which is delivered if it had a data
	///     line.
	///
	/// At the end of the stream, a last line without its line end is dropped
	/// as cut; an event whose lines are complete but whose blank line is
	/// missing is still delivered.
{
public:
	static constexpr std::size_t DEFAULT_MAX_EVENT_SIZE = DEFAULT_MAX_MESSAGE_SIZE;
		/// The default limit, in bytes, of one line and of the data of one event.

	struct Event
		/// One event.
	{
		std::string type;
			/// The value of the event field; empty for the default type, "message".
		std::string data;
			/// The data lines, joined with a newline.
		std::string id;
			/// The value of the id field, if the event had one.
	};

	explicit EventStreamReader(std::istream& in, std::size_t maxEventSize = DEFAULT_MAX_EVENT_SIZE);
		/// Creates a reader of the given stream, which must outlive it.

	void setDeadline(const Poco::Timestamp& deadline);
		/// Sets the time after which next() throws Poco::TimeoutException.
		/// It is checked between lines; a read that blocks is bounded by the
		/// timeout of the stream's source.

	[[nodiscard]] bool next(Event& event);
		/// Reads the next event. Returns false at the end of the stream.
		///
		/// Throws Poco::DataFormatException when a line or the data of an
		/// event exceeds the size limit, and Poco::TimeoutException after the
		/// deadline.

	[[nodiscard]] bool ended() const;
		/// Returns true if the end of the stream has been reached.

	[[nodiscard]] bool cut() const;
		/// Returns true if the stream ended in the middle of a line or of an
		/// event.

private:
	enum class LineResult
	{
		LINE,
		END
	};

	LineResult readLine(std::string& line);

	std::istream& _in;
	std::size_t _maxEventSize;
	Poco::Timestamp _deadline;
	bool _hasDeadline;
	bool _skipLF;
	bool _ended;
	bool _cut;
};


//
// inlines
//
inline bool EventStreamReader::ended() const
{
	return _ended;
}


inline bool EventStreamReader::cut() const
{
	return _cut;
}


} // namespace Poco::AI::MCP::HTTP


#endif // AIMCPHTTP_EventStreamReader_INCLUDED
