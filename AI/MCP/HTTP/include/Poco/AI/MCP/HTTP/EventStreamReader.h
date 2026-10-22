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
#include "Poco/AI/MCP/HTTP/Body.h"
#include <istream>
#include <string>


namespace Poco::AI::MCP::HTTP {


class AIMCPHTTP_API EventStreamReader
	/// Reads server-sent events, the text/event-stream format of the HTML
	/// Living Standard, from a stream:
	///
	///   - one byte order mark at the start of the stream is skipped;
	///   - a line ends with CR LF, LF or CR;
	///   - a line that starts with a colon is a comment;
	///   - "field: value" sets a field, with one space after the colon
	///     dropped; a line without a colon is a field with an empty value;
	///   - the data lines of an event are joined with a newline, the event
	///     field sets its type, the id field its id; other fields are ignored;
	///   - a blank line ends the event, which is delivered if it had a data
	///     line.
	///
	/// At the end of the stream, an event whose blank line has not come is
	/// dropped, as the standard has it, and cut() tells so: a server that
	/// stops in the middle of an event has not delivered it.
	///
	/// The reader takes what the stream has buffered in blocks, ahead of the
	/// event it returns: nothing else reads the stream while the reader is
	/// used.
{
public:
	static constexpr std::size_t DEFAULT_MAX_EVENT_SIZE = DEFAULT_MAX_MESSAGE_SIZE;
		/// The default limit, in bytes, of the data of one event.

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

	void setReadGuard(ReadGuard guard);
		/// Sets the function called before every read of the stream that may
		/// block, because nothing is buffered: it bounds the reading, e.g. by
		/// throwing Poco::TimeoutException once a deadline has passed and by
		/// setting the timeout of the stream's source to the time that is left.

	[[nodiscard]] bool next(Event& event);
		/// Reads the next event. Returns false at the end of the stream.
		///
		/// Throws Poco::DataFormatException when a line or the data of an
		/// event exceeds the size limit, and what the read guard and the
		/// source of the stream throw.

	[[nodiscard]] bool ended() const;
		/// Returns true if the end of the stream has been reached.

	[[nodiscard]] bool cut() const;
		/// Returns true if the stream ended in the middle of a line or of an
		/// event.

private:
	static constexpr std::size_t LINE_OVERHEAD = 64;
		// What a line may have beyond the limit of the data: its field name,
		// the colon and the space, so that data of exactly the limit fits.

	enum class LineResult
	{
		LINE,
		END
	};

	LineResult readLine(std::string& line);
	bool fill();
	void beforeRead();
	void append(std::string& line, const char* pText, std::size_t size) const;

	std::istream& _in;
	std::size_t _maxEventSize;
	ReadGuard _guard;
	bool _skipLF;
	bool _ended;
	bool _cut;
	int _bomBytes;
	char _buffer[8192];
	std::size_t _begin;
	std::size_t _end;
		/// What the reader took from the stream and has not used yet:
		/// _buffer[_begin, _end).
	std::string _line;
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
