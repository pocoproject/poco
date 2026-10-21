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
#include <algorithm>
#include <cstring>
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
	_bomBytes(0),
	_begin(0),
	_end(0)
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


void EventStreamReader::append(std::string& line, const char* pText, std::size_t size) const
{
	if (line.size() + size > _maxEventSize + LINE_OVERHEAD)
	{
		throw Poco::DataFormatException("Event stream line exceeds the size limit");
	}
	line.append(pText, size);
}


bool EventStreamReader::fill()
	/// Takes what the stream has into the buffer, waiting for the stream only
	/// when it has nothing buffered. Returns false at its end.
{
	std::streambuf* pBuf = _in.rdbuf();
	if (pBuf->in_avail() <= 0)
	{
		beforeRead();
		if (pBuf->sgetc() == std::char_traits<char>::eof()) return false;
	}
	const std::streamsize wanted = std::max<std::streamsize>(1, std::min<std::streamsize>(sizeof(_buffer), pBuf->in_avail()));
	const std::streamsize n = pBuf->sgetn(_buffer, wanted);
	if (n <= 0) return false;
	_begin = 0;
	_end = static_cast<std::size_t>(n);
	return true;
}


EventStreamReader::LineResult EventStreamReader::readLine(std::string& line)
{
	line.clear();
	if (_ended || _in.rdbuf() == nullptr)
	{
		_ended = true;
		return LineResult::END;
	}
	for (;;)
	{
		if (_begin == _end && !fill())
		{
			_ended = true;
			// A line without its end was cut off.
			if (!line.empty() || (_bomBytes > 0 && _bomBytes < BOM_SIZE)) _cut = true;
			return LineResult::END;
		}
		if (_bomBytes < BOM_SIZE)
		{
			// One byte order mark at the start of the stream is skipped.
			if (static_cast<unsigned char>(_buffer[_begin]) == BOM[_bomBytes])
			{
				++_bomBytes;
				++_begin;
				continue;
			}
			// What looked like the start of one is text.
			append(line, reinterpret_cast<const char*>(BOM), static_cast<std::size_t>(_bomBytes));
			_bomBytes = BOM_SIZE;
		}
		if (_skipLF)
		{
			_skipLF = false;
			if (_buffer[_begin] == '\n')
			{
				++_begin;
				continue;
			}
		}
		// The first line end of what the buffer holds, LF or CR.
		const char* pText = _buffer + _begin;
		const std::size_t available = _end - _begin;
		const char* pLF = static_cast<const char*>(std::memchr(pText, '\n', available));
		const char* pCR = static_cast<const char*>(std::memchr(pText, '\r', pLF != nullptr ? static_cast<std::size_t>(pLF - pText) : available));
		const char* pLineEnd = pCR != nullptr ? pCR : pLF;
		if (pLineEnd == nullptr)
		{
			append(line, pText, available);
			_begin = _end;
			continue;
		}
		const std::size_t size = static_cast<std::size_t>(pLineEnd - pText);
		append(line, pText, size);
		_begin += size + 1;
		// CR LF counts as one line end; the LF is skipped when it comes.
		if (*pLineEnd == '\r') _skipLF = true;
		return LineResult::LINE;
	}
}


bool EventStreamReader::next(Event& event)
{
	// The strings keep what they have taken from one event to the next.
	event.type.clear();
	event.data.clear();
	event.id.clear();
	bool hasData = false;
	bool any = false;
	for (;;)
	{
		if (readLine(_line) == LineResult::END)
		{
			// An event whose blank line has not come is not delivered.
			if (any || hasData) _cut = true;
			return false;
		}
		if (_line.empty())
		{
			if (hasData) return true;
			event.type.clear();
			event.data.clear();
			event.id.clear();
			any = false;
			continue;
		}
		if (_line[0] == ':') continue;
		any = true;

		const std::string::size_type colon = _line.find(':');
		const std::string::size_type fieldSize = colon == std::string::npos ? _line.size() : colon;
		std::string::size_type valueBegin = _line.size();
		if (colon != std::string::npos)
		{
			valueBegin = colon + 1;
			if (valueBegin < _line.size() && _line[valueBegin] == ' ') ++valueBegin;
		}
		const std::size_t valueSize = _line.size() - valueBegin;

		if (_line.compare(0, fieldSize, "data") == 0)
		{
			if (hasData) event.data += '\n';
			if (event.data.size() + valueSize > _maxEventSize)
			{
				throw Poco::DataFormatException("Event stream event exceeds the size limit");
			}
			event.data.append(_line, valueBegin, valueSize);
			hasData = true;
		}
		else if (_line.compare(0, fieldSize, "event") == 0)
		{
			event.type.assign(_line, valueBegin, valueSize);
		}
		else if (_line.compare(0, fieldSize, "id") == 0)
		{
			if (_line.find('\0', valueBegin) == std::string::npos) event.id.assign(_line, valueBegin, valueSize);
		}
	}
}


} // namespace Poco::AI::MCP::HTTP
