//
// EventStreamReader.cpp
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  EventStreamReader
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/HTTP/EventStreamReader.h"
#include "Poco/Exception.h"


namespace Poco::AI::MCP::HTTP {


EventStreamReader::EventStreamReader(std::istream& in, std::size_t maxEventSize):
	_in(in),
	_maxEventSize(maxEventSize),
	_hasDeadline(false),
	_skipLF(false),
	_ended(false),
	_cut(false)
{
}


void EventStreamReader::setDeadline(const Poco::Timestamp& deadline)
{
	_deadline = deadline;
	_hasDeadline = true;
}


EventStreamReader::LineResult EventStreamReader::readLine(std::string& line)
{
	line.clear();
	std::streambuf* pBuf = _in.rdbuf();
	if (_ended || pBuf == nullptr)
	{
		_ended = true;
		return LineResult::END;
	}
	for (;;)
	{
		const int c = pBuf->sbumpc();
		if (c == std::char_traits<char>::eof())
		{
			_ended = true;
			// A line without its end was cut off.
			if (!line.empty()) _cut = true;
			return LineResult::END;
		}
		if (_skipLF)
		{
			_skipLF = false;
			if (c == '\n') continue;
		}
		if (c == '\n') return LineResult::LINE;
		if (c == '\r')
		{
			// CR LF counts as one line end; the LF is skipped when it comes.
			_skipLF = true;
			return LineResult::LINE;
		}
		if (line.size() >= _maxEventSize)
		{
			throw Poco::DataFormatException("Event stream line exceeds the size limit");
		}
		line += static_cast<char>(c);
	}
}


bool EventStreamReader::next(Event& event)
{
	event = Event();
	bool hasData = false;
	bool any = false;
	std::string line;
	for (;;)
	{
		if (_hasDeadline && _deadline.isElapsed(0))
		{
			throw Poco::TimeoutException("Event stream deadline passed");
		}
		if (readLine(line) == LineResult::END)
		{
			// The lines read so far are complete: deliver the event even though
			// its blank line is missing.
			if (any && !hasData) _cut = true;
			if (hasData)
			{
				_cut = true;
				return true;
			}
			return false;
		}
		if (line.empty())
		{
			if (hasData) return true;
			event = Event();
			any = false;
			continue;
		}
		if (line[0] == ':') continue;
		any = true;

		std::string field;
		std::string value;
		const std::string::size_type colon = line.find(':');
		if (colon == std::string::npos)
		{
			field = line;
		}
		else
		{
			field.assign(line, 0, colon);
			std::string::size_type begin = colon + 1;
			if (begin < line.size() && line[begin] == ' ') ++begin;
			value.assign(line, begin, std::string::npos);
		}

		if (field == "data")
		{
			if (hasData) event.data += '\n';
			if (event.data.size() + value.size() > _maxEventSize)
			{
				throw Poco::DataFormatException("Event stream event exceeds the size limit");
			}
			event.data += value;
			hasData = true;
		}
		else if (field == "event")
		{
			event.type = value;
		}
		else if (field == "id")
		{
			if (value.find('\0') == std::string::npos) event.id = value;
		}
	}
}


} // namespace Poco::AI::MCP::HTTP
