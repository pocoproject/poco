//
// MultipartReader.cpp
//
// Library: Net
// Package: Messages
// Module:  MultipartReader
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/MultipartReader.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/NetException.h"
#include "Poco/Ascii.h"
#include "Poco/NumberParser.h"
#include <limits>

using namespace std::string_literals;

using Poco::BufferedStreamBuf;


namespace Poco::Net {


//
// ReadWindow -- top-level read-ahead buffer wrapping the underlying stream.
// All reads go through this buffer, so over-read bytes from boundary
// scanning stay available for the next part's header parsing.
//

class ReadWindow: public std::streambuf
	/// Top-level sliding window buffer wrapping the underlying stream.
	/// All reads go through this buffer, so over-read bytes from boundary
	/// scanning remain available for the next part's header parsing.
	///
	/// Unlike BufferedStreamBuf which discards data on refill, this window
	/// compacts unconsumed data to the front and appends new data, preserving
	/// all unread bytes. This enables zero-copy boundary scanning and reliable
	/// sputbackc for any number of bytes.
{
public:
	static constexpr std::streamsize BUFFER_SIZE = 32768;
	static constexpr std::streamsize PUTBACK_SIZE = 512;
		/// Maximum bytes preserved for sputbackc after a read.
		/// Sized for multipart boundary (max 70 per RFC 2046) +
		/// post-boundary delimiter + generous margin.

	explicit ReadWindow(std::streambuf& underlying):
		_underlying(underlying)
	{
		_buf.resize(BUFFER_SIZE);
		setg(_buf.data(), _buf.data(), _buf.data()); // empty initially
	}

	/// Number of buffered bytes available for reading.
	[[nodiscard]] std::streamsize available() const
	{
		return egptr() - gptr();
	}

	/// Pointer to current unread data in the internal buffer.
	[[nodiscard]] const char* data() const
	{
		return gptr();
	}

	/// Advance past n bytes (consume without copying).
	void consume(std::streamsize n)
	{
		poco_assert (n >= 0 && n <= available());
		setg(eback(), gptr() + n, egptr());
	}

	/// Ensure the buffer has data, compacting and refilling as needed.
	/// Always tries to fill the buffer fully, preserving unconsumed data.
	/// Returns bytes available after refill.
	std::streamsize fill()
	{
		std::streamsize unconsumed = egptr() - gptr();

		// Only refill if buffer has space (less than half full)
		if (unconsumed > static_cast<std::streamsize>(_buf.size()) / 2)
			return unconsumed;

		// Compact: move unconsumed data to front, preserving putback area
		std::streamsize consumed = gptr() - eback();
		std::streamsize putback = std::min(consumed, static_cast<std::streamsize>(PUTBACK_SIZE));

		std::memmove(_buf.data(), gptr() - putback, putback + unconsumed);

		// Read new data after the unconsumed portion
		char* readStart = _buf.data() + putback + unconsumed;
		std::streamsize space = static_cast<std::streamsize>(_buf.size()) - putback - unconsumed;
		std::streamsize n = (space > 0) ? _underlying.sgetn(readStart, space) : 0;

		if (n <= 0 && unconsumed == 0) return 0;

		std::streamsize total = unconsumed + std::max(n, std::streamsize(0));
		setg(_buf.data(), _buf.data() + putback, _buf.data() + putback + total);
		return total;
	}

protected:
	int_type underflow() override
	{
		if (gptr() < egptr())
			return traits_type::to_int_type(*gptr());

		std::streamsize avail = fill();
		if (avail <= 0) return traits_type::eof();
		return traits_type::to_int_type(*gptr());
	}

	std::streamsize xsgetn(char* buffer, std::streamsize length) override
	{
		std::streamsize total = 0;
		while (length > 0)
		{
			std::streamsize avail = egptr() - gptr();
			if (avail > 0)
			{
				std::streamsize toCopy = std::min(avail, length);
				traits_type::copy(buffer, gptr(), toCopy);
				setg(eback(), gptr() + toCopy, egptr());
				buffer += toCopy;
				length -= toCopy;
				total += toCopy;
				continue;
			}
			if (traits_type::eq_int_type(underflow(), traits_type::eof()))
				break;
		}
		return total;
	}

	int_type pbackfail(int_type ch) override
	{
		// Support putback by moving gptr backwards if possible
		if (gptr() > _buf.data())
		{
			setg(eback(), gptr() - 1, egptr());
			if (!traits_type::eq_int_type(ch, traits_type::eof()))
				*gptr() = traits_type::to_char_type(ch);
			return traits_type::to_int_type(*gptr());
		}
		return traits_type::eof();
	}

private:
	std::streambuf& _underlying;
	std::vector<char> _buf;
};


//
// MultipartStreamBuf
//


MultipartStreamBuf::MultipartStreamBuf(std::istream& istr, const std::string& boundary):
	MultipartStreamBuf(istr, boundary, -1)
{
}


MultipartStreamBuf::MultipartStreamBuf(std::istream& istr, const std::string& boundary, std::streamsize contentLength):
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::in),
	_istr(istr),
	_boundary(boundary),
	_lastPart(false),
	_boundaryFound(false),
	_contentLength(contentLength),
	_bytesRead(0)
{
}


MultipartStreamBuf::~MultipartStreamBuf()
{
}


std::streamsize MultipartStreamBuf::readFromDevice(char* buffer, std::streamsize length)
{
	poco_assert (!_boundary.empty());

	if (_boundaryFound)
		return -1;

	return readContent(buffer, length);
}


std::streamsize MultipartStreamBuf::readContent(char* buffer, std::streamsize length)
{
	static const int eof = std::char_traits<char>::eof();
	std::streambuf& buf = *_istr.rdbuf();

	// Content-Length fast path: bulk read
	if (_contentLength >= 0 && _bytesRead < _contentLength)
	{
		std::streamsize remaining = _contentLength - _bytesRead;
		std::streamsize toRead = std::min(remaining, length);
		std::streamsize n = buf.sgetn(buffer, toRead);
		if (n <= 0) { _lastPart = true; return -1; }
		_bytesRead += n;
		return n;
	}

	// Content-Length exhausted -- consume trailing boundary.
	// Stream has [CR]LF--boundary after content. Skip optional CR,
	// then validate the boundary byte-by-byte. If any byte is
	// unexpected (including EOF), treat as last part.
	if (_contentLength >= 0)
	{
		int ch = buf.sgetc();
		if (ch == '\r') buf.sbumpc(); // skip optional CR before LF

		// Expect \n--boundary
		ch = buf.sbumpc();
		if (ch != '\n') { _lastPart = true; return -1; }
		ch = buf.sbumpc();
		if (ch != '-') { _lastPart = true; return -1; }
		ch = buf.sbumpc();
		if (ch != '-') { _lastPart = true; return -1; }
		for (auto bc : _boundary)
		{
			ch = buf.sbumpc();
			if (ch == eof || ch != bc) { _lastPart = true; return -1; }
		}
		consumePostBoundaryDelimiter();
		_boundaryFound = true;
		return 0;
	}

	// No Content-Length: zero-copy boundary scanning in the ReadWindow.
	// Scan the window's internal buffer directly, copy only content bytes
	// to the output. Over-read bytes remain in the window for parseHeader.
	ReadWindow& win = static_cast<ReadWindow&>(*_istr.rdbuf());
	const std::streamsize patLen = static_cast<std::streamsize>(_boundary.size()) + 4;

	std::streamsize avail = win.fill();
	if (avail <= 0)
	{
		_lastPart = true;
		return -1;
	}

	// Scan for boundary directly in the window's buffer
	std::streamsize boundaryPos = scanForBoundary(win.data(), avail);
	if (boundaryPos >= 0)
	{
		// Copy content bytes (before boundary) to output
		std::streamsize contentLen = std::min(boundaryPos, length);
		if (contentLen > 0)
			std::memcpy(buffer, win.data(), contentLen);
		win.consume(contentLen);

		// If only a partial content copy was possible (buffer too small),
		// return what we have. The boundary will be found again next call.
		if (contentLen < boundaryPos)
			return contentLen;

		// Consume boundary + delimiter from the window
		const char* p = win.data();
		avail = win.available();
		std::streamsize pos = 0;
		if (p[pos] == '\r') ++pos;
		++pos; // \n
		pos += 2; // --
		pos += static_cast<std::streamsize>(_boundary.size());

		if (pos < avail)
		{
			if (p[pos] == '-')
			{
				_lastPart = true;
				pos += (pos + 1 < avail && p[pos + 1] == '-') ? 2 : 1;
			}
			else if (p[pos] == '\r' && pos + 1 < avail && p[pos + 1] == '\n')
			{
				pos += 2;
			}
			else if (p[pos] == '\n')
			{
				++pos;
			}
			else
			{
				_lastPart = true;
			}
		}
		else
		{
			// Delimiter not fully in buffer -- consume what we have,
			// then read the rest byte-by-byte
			win.consume(std::min(pos, avail));
			consumePostBoundaryDelimiter();
			_boundaryFound = true;
			return contentLen;
		}
		poco_assert (pos <= avail);
		win.consume(pos);
		// Remaining bytes in window are the next part's headers -- stay there
		// for parseHeader to read via _windowStr.

		_boundaryFound = true;
		return contentLen;
	}

	// No boundary found. Copy safe prefix to output (everything except
	// the tail that could be a partial boundary match).
	if (avail > patLen)
	{
		std::streamsize safe = std::min(avail - patLen, length);
		std::memcpy(buffer, win.data(), safe);
		win.consume(safe);
		return safe;
	}

	// Available data smaller than pattern -- try to get more data
	std::streamsize prevAvail = avail;
	avail = win.fill(); // attempt refill
	if (avail > prevAvail)
	{
		// Got more data -- re-scan for boundary with the new data
		std::streamsize bp = scanForBoundary(win.data(), avail);
		if (bp >= 0)
		{
			// Found it after refill -- recurse to handle it
			return readContent(buffer, length);
		}
		if (avail > patLen)
		{
			std::streamsize safe = std::min(avail - patLen, length);
			std::memcpy(buffer, win.data(), safe);
			win.consume(safe);
			return safe;
		}
	}

	// Truly at EOF or stuck -- emit remaining data
	std::streamsize toCopy = std::min(avail, length);
	if (toCopy > 0)
	{
		std::memcpy(buffer, win.data(), toCopy);
		win.consume(toCopy);
	}
	if (avail == prevAvail) // fill() couldn't get more data
		_lastPart = true;
	return toCopy > 0 ? toCopy : -1;
}


std::streamsize MultipartStreamBuf::scanForBoundary(const char* data, std::streamsize size)
{
	const std::size_t bLen = _boundary.size();
	const std::size_t minMatch = 3 + bLen; // \n-- + boundary

	for (std::streamsize i = 0; i + static_cast<std::streamsize>(minMatch) <= size; ++i)
	{
		std::streamsize pos = i;
		if (data[pos] == '\r') ++pos;
		if (pos >= size || data[pos] != '\n') continue;
		++pos;
		if (pos + 1 >= size || data[pos] != '-' || data[pos + 1] != '-') continue;
		pos += 2;
		if (pos + static_cast<std::streamsize>(bLen) > size) return -1;
		if (std::memcmp(data + pos, _boundary.data(), bLen) == 0)
			return i;
	}
	return -1;
}


void MultipartStreamBuf::consumePostBoundaryDelimiter()
{
	std::streambuf& buf = *_istr.rdbuf();
	int ch = buf.sbumpc();
	if (ch == '-')
	{
		if (buf.sgetc() == '-')
			buf.sbumpc();
		_lastPart = true;
	}
	else if (ch == '\r' && buf.sgetc() == '\n')
	{
		buf.sbumpc();
	}
	else if (ch == '\n')
	{
		// Lenient: accept bare LF
	}
	else
	{
		_lastPart = true;
	}
}


bool MultipartStreamBuf::lastPart() const
{
	return _lastPart;
}


//
// MultipartIOS
//


MultipartIOS::MultipartIOS(std::istream& istr, const std::string& boundary):
	MultipartIOS(istr, boundary, -1)
{
}


MultipartIOS::MultipartIOS(std::istream& istr, const std::string& boundary, std::streamsize contentLength):
	_buf(istr, boundary, contentLength)
{
	poco_ios_init(&_buf);
}


MultipartIOS::~MultipartIOS()
{
	try
	{
		_buf.sync();
	}
	catch (...)
	{
	}
}


MultipartStreamBuf* MultipartIOS::rdbuf()
{
	return &_buf;
}


bool MultipartIOS::lastPart() const
{
	return _buf.lastPart();
}


//
// MultipartInputStream
//


MultipartInputStream::MultipartInputStream(std::istream& istr, const std::string& boundary):
	MultipartInputStream(istr, boundary, -1)
{
}


MultipartInputStream::MultipartInputStream(std::istream& istr, const std::string& boundary, std::streamsize contentLength):
	MultipartIOS(istr, boundary, contentLength),
	std::istream(&_buf)
{
}


MultipartInputStream::~MultipartInputStream()
{
}


//
// MultipartReader
//


MultipartReader::MultipartReader(std::istream& istr):
	_istr(istr)
{
	// Defer window creation until boundary is known (guessBoundary sets _boundary)
}


MultipartReader::MultipartReader(std::istream& istr, const std::string& boundary):
	_istr(istr),
	_boundary(boundary),
	_window(std::make_unique<ReadWindow>(*istr.rdbuf())),
	_windowStr(std::make_unique<std::istream>(_window.get()))
{
}


MultipartReader::~MultipartReader()
{
}


void MultipartReader::nextPart(MessageHeader& messageHeader)
{
	if (_pMPI == nullptr)
	{
		if (_boundary.empty())
			guessBoundary();
		else
			findFirstBoundary();

		// Create the ReadWindow now that the first boundary has been consumed.
		// Putback area is sized for over-read bytes from boundary scanning.
		if (!_window)
		{
			_window = std::make_unique<ReadWindow>(*_istr.rdbuf());
			_windowStr = std::make_unique<std::istream>(_window.get());
		}
	}
	else if (_pMPI->lastPart())
	{
		throw MultipartException("No more parts available");
	}

	parseHeader(messageHeader);
	std::streamsize contentLength = -1;
	if (messageHeader.has("Content-Length"))
	{
		Poco::Int64 len = 0;
		if (Poco::NumberParser::tryParse64(messageHeader.get("Content-Length"), len)
			&& len >= 0
			&& len <= static_cast<Poco::Int64>(std::numeric_limits<std::streamsize>::max()))
		{
			contentLength = static_cast<std::streamsize>(len);
		}
	}
	_pMPI = std::make_unique<MultipartInputStream>(*_windowStr, _boundary, contentLength);
}


bool MultipartReader::hasNextPart()
{
	if (_pMPI && _pMPI->lastPart()) return false;
	if (_window && _window->available() > 0) return true;
	return _istr.good();
}


std::istream& MultipartReader::stream() const
{
	poco_check_ptr (_pMPI);

	return *_pMPI;
}


const std::string& MultipartReader::boundary() const
{
	return _boundary;
}


void MultipartReader::findFirstBoundary()
{
	std::string expect("--");
	expect.append(_boundary);
	std::string line;
	line.reserve(expect.length());
	bool ok = true;
	do
	{
		ok = readLine(line, expect.length());
	}
	while (ok && line != expect);

	if (!ok) throw MultipartException("No boundary line found");
}


void MultipartReader::guessBoundary()
{
	// Read directly from _istr -- the ReadWindow is not yet created
	// (boundary length needed for putback area sizing)
	static const int eof = std::char_traits<char>::eof();
	int ch = _istr.get();
	while (Poco::Ascii::isSpace(ch))
		ch = _istr.get();
	if (ch == '-' && _istr.peek() == '-')
	{
		_istr.get();
		ch = _istr.peek();
		// RFC 2046 limits boundary to 70 chars; we accept up to 128 for robustness
		while (ch != eof && ch != '\r' && ch != '\n' && _boundary.size() < 128)
		{
			_boundary += (char) _istr.get();
			ch = _istr.peek();
		}
		if (ch != '\r' && ch != '\n')
			throw MultipartException("Invalid boundary line found");
		if (ch == '\r' || ch == '\n')
			_istr.get();
		if (_istr.peek() == '\n')
			_istr.get();
	}
	else throw MultipartException("No boundary line found");
}


void MultipartReader::parseHeader(MessageHeader& messageHeader)
{
	messageHeader.clear();
	// Read from window -- over-read bytes from previous part are already there
	_windowStr->clear();
	messageHeader.read(*_windowStr);
	int ch = _windowStr->get();
	if (ch == '\r' && _windowStr->peek() == '\n') _windowStr->get();
}


bool MultipartReader::readLine(std::string& line, std::string::size_type n)
{
	// Called before window creation (during findFirstBoundary) -- read from _istr
	static const int eof = std::char_traits<char>::eof();
	static const int maxLength = 1024;

	line.clear();
	int ch = _istr.peek();
	int length = 0;
	while (ch != eof && ch != '\r' && ch != '\n' && length < maxLength)
	{
		ch = (char) _istr.get();
		if (line.length() < n) line += ch;
		ch = _istr.peek();
		length++;
	}
	if (ch != eof) _istr.get();
	if (ch == '\r' && _istr.peek() == '\n') _istr.get();
	return ch != eof && length < maxLength;
}


} // namespace Poco::Net
