//
// EventStreamFuzz.cpp
//
// Feeds arbitrary bytes to the server-sent event reader, once as a whole and
// once byte by byte, and checks that both readings give the same result.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/HTTP/EventStreamReader.h"
#include "Poco/Exception.h"
#include "Poco/NumberFormatter.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <istream>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>


using Poco::AI::MCP::HTTP::EventStreamReader;
using Poco::NumberFormatter;


namespace
{
	// A limit well below the inputs' length, so that long lines and events
	// reach it.
	constexpr std::size_t MAX_EVENT_SIZE = 256;


	class TrickleBuffer: public std::streambuf
		/// Hands out its text one byte at a time: what it has buffered, and
		/// so in_avail(), is never more. A line, a CR LF pair and a byte
		/// order mark are then split between reads of the reader.
	{
	public:
		explicit TrickleBuffer(const std::string& text):
			_text(text)
		{
		}

	protected:
		int_type underflow() override
		{
			if (gptr() != nullptr && gptr() < egptr()) return traits_type::to_int_type(*gptr());
			if (_offset >= _text.size()) return traits_type::eof();
			char* pByte = &_text[_offset];
			setg(pByte, pByte, pByte + 1);
			++_offset;
			return traits_type::to_int_type(*gptr());
		}

	private:
		std::string _text;
		std::size_t _offset = 0;
	};


	struct Reading
		/// What the reader made of one stream.
	{
		std::vector<EventStreamReader::Event> events;
		bool rejected = false;
			/// next() threw Poco::DataFormatException.
		bool cut = false;
	};


	[[noreturn]] void fail(const std::string& what)
		/// Reports a finding: writes what to std::cerr and aborts.
	{
		std::cerr << what << '\n';
		std::abort();
	}


	Reading readAll(std::istream& in)
		/// Reads every event of in. The data of an event is never longer than
		/// the limit, its id holds no NUL byte, and when next() returns false
		/// the end of the stream has been reached; anything else is a finding.
	{
		Reading reading;
		EventStreamReader reader(in, MAX_EVENT_SIZE);
		EventStreamReader::Event event;
		try
		{
			while (reader.next(event))
			{
				if (event.data.size() > MAX_EVENT_SIZE) fail("The data of an event exceeds the limit");
				if (event.id.find('\0') != std::string::npos) fail("The id of an event contains a NUL byte");
				reading.events.push_back(event);
			}
		}
		catch (const Poco::DataFormatException&)
		{
			// What next() throws for a line or an event over the limit; anything
			// else is a finding.
			reading.rejected = true;
		}
		if (!reading.rejected && !reader.ended()) fail("next() returned false before the end of the stream");
		reading.cut = reader.cut();
		return reading;
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	const std::string input(reinterpret_cast<const char*>(data), size);
	std::istringstream whole(input);
	const Reading first = readAll(whole);
	TrickleBuffer buffer(input);
	std::istream trickle(&buffer);
	const Reading second = readAll(trickle);

	// The limits apply to the bytes of a line or an event, not to what one
	// read of the stream returns: how the bytes arrive changes nothing.
	if (first.events.size() != second.events.size())
	{
		fail("The number of events differs: " + NumberFormatter::format(first.events.size()) + " read whole, "
			+ NumberFormatter::format(second.events.size()) + " read byte by byte");
	}
	for (std::size_t i = 0; i < first.events.size(); ++i)
	{
		if (first.events[i].type != second.events[i].type) fail("The type of event " + NumberFormatter::format(i) + " differs");
		if (first.events[i].data != second.events[i].data) fail("The data of event " + NumberFormatter::format(i) + " differs");
		if (first.events[i].id != second.events[i].id) fail("The id of event " + NumberFormatter::format(i) + " differs");
	}
	if (first.rejected != second.rejected) fail("Only one of the readings threw DataFormatException");
	if (first.cut != second.cut) fail("cut() differs");
	return 0;
}
