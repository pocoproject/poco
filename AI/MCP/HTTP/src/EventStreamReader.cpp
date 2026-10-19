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
#include <utility>


namespace Poco::AI::MCP::HTTP {


namespace
{
	const unsigned char BOM[] = {0xEF, 0xBB, 0xBF};
	constexpr int BOM_SIZE = 3;
}


EventStreamReader::EventStreamReader(std::istream& in, std::size_t maxEventSize):
	_in(in),
	_maxEventSize(maxEventSize),
	_hasDeadline(false),
	_skipLF(false),
	_ended(false),
	_cut(false),
	_bomBytes(0)
{
}


void EventStreamReader::setDeadline(const Poco::Timestamp& deadline)
{
	_deadline = deadline;
	_hasDeadline = true;
}


void EventStreamReader::setReadGuard(ReadGuard guard)
{
	_guard = std::move(guard);
}


void EventStreamReader::beforeRead()
{
	if (_hasDeadline && _deadline.isElapsed(0))
	{
		throw Poco::TimeoutException("Event stream deadline passed");
	}
	if (_guard) _guard();
}


void EventStreamReader::append(std::string& line, int c) const
{
	if (line.size() >= _maxEventSize + LINE_OVERHEAD)
	{
		throw Poco::DataFormatException("Event stream line exceeds the size limit");
	}
	line += static_cast<char>(c);
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
		if (pBuf->in_avail() <= 0) beforeRead();
		const int c = pBuf->sbumpc();
		if (c == std::char_traits<char>::eof())
		{
			_ended = true;
			// A line without its end was cut off.
			if (!line.empty() || (_bomBytes > 0 && _bomBytes < BOM_SIZE)) _cut = true;
			return LineResult::END;
		}
		if (_bomBytes < BOM_SIZE)
		{
			// One byte order mark at the start of the stream is skipped.
			if (static_cast<unsigned char>(c) == BOM[_bomBytes])
			{
				++_bomBytes;
				continue;
			}
			// What looked like the start of one is text.
			for (int i = 0; i < _bomBytes; ++i) append(line, BOM[i]);
			_bomBytes = BOM_SIZE;
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
		append(line, c);
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
		if (readLine(line) == LineResult::END)
		{
			// An event whose blank line has not come is not delivered.
			if (any || hasData) _cut = true;
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
