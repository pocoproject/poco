//
// HTTPChunkedStream.cpp
//
// Library: Net
// Package: HTTP
// Module:  HTTPChunkedStream
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPChunkedStream.h"
#include "Poco/Net/HTTPHeaderStream.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/NetException.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Ascii.h"
#include <algorithm>
#include <charconv>
#include <sstream>


using Poco::NumberFormatter;


namespace Poco::Net {


//
// HTTPChunkedStreamBuf
//


HTTPChunkedStreamBuf::HTTPChunkedStreamBuf(HTTPSession& session, openmode mode, MessageHeader* pTrailer):
	HTTPBasicStreamBuf(HTTPBufferAllocator::BUFFER_SIZE, mode),
	_session(session),
	_mode(mode),
	_chunk(0),
	_readState(ReadState::SIZE),
	_pTrailer(pTrailer)
{
}


HTTPChunkedStreamBuf::~HTTPChunkedStreamBuf()
{
}


void HTTPChunkedStreamBuf::close()
{
	if (_mode & std::ios::out)
	{
		sync();
		if (_pTrailer != nullptr && !_pTrailer->empty())
		{
			HTTPHeaderOutputStream hos(_session);
			hos.write("0\r\n", 3);
			_pTrailer->write(hos);
			hos.write("\r\n", 2);
		}
		else
		{
			_session.write("0\r\n\r\n", 5); // If possible, send in one write
		}
	}
}


std::streamsize HTTPChunkedStreamBuf::readFromDevice(char* buffer, std::streamsize length)
{
	static constexpr int eof = std::char_traits<char>::eof();
	static constexpr std::size_t MAX_CHUNK_SIZE_DIGITS = 8;
	static constexpr std::size_t MAX_SIZE_LINE_LENGTH = 4096;
	static constexpr std::size_t MAX_TRAILER_LENGTH = 65536;

	// A line ends only with CR LF. A bare CR or LF is rejected, because another
	// parser of the same message may end the line there. Returns false at end of stream.
	const auto readLine = [this](std::string& line, std::size_t maxLength, const char* error)
	{
		line.clear();
		int ch = _session.get();
		while (ch != '\r')
		{
			if (ch == eof) return false;
			if (ch == '\n' || line.size() == maxLength) throw MessageException(error);
			line += static_cast<char>(ch);
			ch = _session.get();
		}
		ch = _session.get();
		if (ch == eof) return false;
		if (ch != '\n') throw MessageException(error);
		return true;
	};

	const auto truncated = [this]() -> std::streamsize
	{
		_session.setException(MessageException("Incomplete chunked transfer encoding"));
		_readState = ReadState::FAILED;
		return 0;
	};

	if (_readState == ReadState::DONE || _readState == ReadState::FAILED) return 0;

	try
	{
		std::string line;
		if (_readState == ReadState::DATA_CRLF)
		{
			if (!readLine(line, 0, "Missing CRLF after chunk data")) return truncated();
			_readState = ReadState::SIZE;
		}
		if (_readState == ReadState::SIZE)
		{
			if (!readLine(line, MAX_SIZE_LINE_LENGTH, "Invalid chunk size line")) return truncated();
			std::size_t digits = 0;
			while (digits < line.size() && digits < MAX_CHUNK_SIZE_DIGITS && Poco::Ascii::isHexDigit(line[digits])) ++digits;
			const std::size_t extension = line.find_first_not_of(" \t", digits);
			const bool validExtension = extension == std::string::npos || (line[extension] == ';' &&
				std::none_of(line.begin() + extension, line.end(), [](char c) { return c != '\t' && Poco::Ascii::hasProperties(c, Poco::Ascii::ACP_CONTROL); }));
			if (digits == 0 || !validExtension || std::from_chars(line.data(), line.data() + digits, _chunk, 16).ec != std::errc())
				throw MessageException("Invalid chunk size line");
			if (_chunk == 0)
			{
				std::string fields;
				if (!readLine(line, MAX_TRAILER_LENGTH, "Invalid chunked transfer trailer")) return truncated();
				while (!line.empty())
				{
					fields.append(line).append("\r\n");
					if (fields.size() > MAX_TRAILER_LENGTH) throw MessageException("Chunked transfer trailer too long");
					if (!readLine(line, MAX_TRAILER_LENGTH, "Invalid chunked transfer trailer")) return truncated();
				}
				if (!fields.empty())
				{
					std::istringstream istr(fields);
					if (_pTrailer != nullptr)
					{
						_pTrailer->read(istr);
					}
					else
					{
						MessageHeader trailer;
						trailer.read(istr);
					}
				}
				_readState = ReadState::DONE;
				return 0;
			}
			_readState = ReadState::DATA;
		}
		if (length > _chunk) length = _chunk;
		std::streamsize n = _session.read(buffer, length);
		// A negative result is no data yet on a non-blocking socket, not an end of file.
		if (n < 0) return n;
		if (n == 0) return truncated();
		_chunk -= n;
		if (_chunk == 0) _readState = ReadState::DATA_CRLF;
		return n;
	}
	catch (Poco::Exception& exc)
	{
		// The rest of a line read in part must not be parsed as a new line.
		if (_session.networkException() == nullptr) _session.setException(exc);
		_readState = ReadState::FAILED;
		throw;
	}
}


std::streamsize HTTPChunkedStreamBuf::writeToDevice(const char* buffer, std::streamsize length)
{
	_chunkBuffer.clear();
	NumberFormatter::appendHex(_chunkBuffer, length);
	_chunkBuffer.append("\r\n", 2);
	_chunkBuffer.append(buffer, static_cast<std::string::size_type>(length));
	_chunkBuffer.append("\r\n", 2);
	_session.write(_chunkBuffer.data(), static_cast<std::streamsize>(_chunkBuffer.size()));
	return length;
}


//
// HTTPChunkedIOS
//


HTTPChunkedIOS::HTTPChunkedIOS(HTTPSession& session, HTTPChunkedStreamBuf::openmode mode, MessageHeader* pTrailer):
	_buf(session, mode, pTrailer)
{
	poco_ios_init(&_buf);
}


HTTPChunkedIOS::~HTTPChunkedIOS()
{
	try
	{
		_buf.close();
	}
	catch (...)
	{
	}
}


HTTPChunkedStreamBuf* HTTPChunkedIOS::rdbuf()
{
	return &_buf;
}


//
// HTTPChunkedInputStream
//


Poco::MemoryPool HTTPChunkedInputStream::_pool(sizeof(HTTPChunkedInputStream));


HTTPChunkedInputStream::HTTPChunkedInputStream(HTTPSession& session, MessageHeader* pTrailer):
	HTTPChunkedIOS(session, std::ios::in, pTrailer),
	std::istream(&_buf)
{
}


HTTPChunkedInputStream::~HTTPChunkedInputStream()
{
}


void* HTTPChunkedInputStream::operator new(std::size_t size)
{
	return _pool.get();
}


void HTTPChunkedInputStream::operator delete(void* ptr)
{
	try
	{
		_pool.release(ptr);
	}
	catch (...)
	{
		poco_unexpected();
	}
}


//
// HTTPChunkedOutputStream
//


Poco::MemoryPool HTTPChunkedOutputStream::_pool(sizeof(HTTPChunkedOutputStream));


HTTPChunkedOutputStream::HTTPChunkedOutputStream(HTTPSession& session, MessageHeader* pTrailer):
	HTTPChunkedIOS(session, std::ios::out, pTrailer),
	std::ostream(&_buf)
{
}


HTTPChunkedOutputStream::~HTTPChunkedOutputStream()
{
}


void* HTTPChunkedOutputStream::operator new(std::size_t size)
{
	return _pool.get();
}


void HTTPChunkedOutputStream::operator delete(void* ptr)
{
	try
	{
		_pool.release(ptr);
	}
	catch (...)
	{
		poco_unexpected();
	}
}


} // namespace Poco::Net
