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
#include "Poco/Net/HTTPMessage.h"
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
	_pTrailer(pTrailer),
	_closed(false),
	_writeFailed(false)
{
}


HTTPChunkedStreamBuf::~HTTPChunkedStreamBuf()
{
}


void HTTPChunkedStreamBuf::close()
{
	if ((_mode & std::ios::out) && !_closed)
	{
		// Set first, so that ~HTTPChunkedIOS does not repeat a close() that threw.
		_closed = true;
		try
		{
			// Without the last chunk, the peer sees an incomplete body instead of a complete-looking truncated one.
			if (_writeFailed || sync() == -1) throw MessageException("Failed to write chunked transfer data");
			// Formatted before sending, so that an invalid trailer sends nothing
			// and a short write is not followed by a second copy.
			std::string lastChunk("0\r\n");
			if (_pTrailer != nullptr && !_pTrailer->empty())
			{
				std::ostringstream ostr;
				HTTPMessage::writeTrailer(*_pTrailer, ostr);
				lastChunk.append(ostr.str());
			}
			lastChunk.append("\r\n");
			const auto size = static_cast<std::streamsize>(lastChunk.size());
			if (_session.write(lastChunk.data(), size) != size) throw MessageException("Failed to write last chunk");
		}
		catch (MessageException& exc)
		{
			// Stored for HTTPClientSession::flushRequest(), as ~HTTPChunkedIOS discards the exception.
			// An exception already stored is the cause of the failure and is kept.
			if (_session.networkException() == nullptr) _session.setException(exc);
			throw;
		}
	}
}


bool HTTPChunkedStreamBuf::isComplete() const noexcept
{
	return _readState == ReadState::DONE;
}


bool HTTPChunkedStreamBuf::consumeTerminator()
{
	if (_mode & std::ios::in)
	{
		try
		{
			readFromDevice(nullptr, 0);
		}
		catch (Poco::Exception&)
		{
			// The body stays incomplete; a read error is stored in the session.
		}
	}
	return isComplete();
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
		// A cause the session recorded, such as a TLS close without close_notify, is kept.
		if (_session.networkException() == nullptr) _session.setException(MessageException("Incomplete chunked transfer encoding"));
		_readState = ReadState::FAILED;
		return 0;
	};

	if (_readState == ReadState::DONE || _readState == ReadState::FAILED) return 0;

	try
	{
		std::string line;
		if (_readState == ReadState::DATA_CRLF)
		{
			// With length 0 (see consumeTerminator()), a line is read only if the peer has sent data.
			if (length == 0 && !_session.dataPending()) return 0;
			if (!readLine(line, 0, "Missing CRLF after chunk data")) return truncated();
			_readState = ReadState::SIZE;
		}
		if (_readState == ReadState::SIZE)
		{
			if (length == 0 && !_session.dataPending()) return 0;
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
		if (length == 0) return 0;
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
	// The peer may have received part of the failed chunk; anything written after it would be read as chunk data.
	if (_writeFailed) return -1;
	_chunkBuffer.clear();
	NumberFormatter::appendHex(_chunkBuffer, length);
	_chunkBuffer.append("\r\n", 2);
	_chunkBuffer.append(buffer, static_cast<std::string::size_type>(length));
	_chunkBuffer.append("\r\n", 2);
	const auto size = static_cast<std::streamsize>(_chunkBuffer.size());
	try
	{
		_writeFailed = _session.write(_chunkBuffer.data(), size) != size;
	}
	catch (...)
	{
		_writeFailed = true;
		throw;
	}
	// Stored as for a thrown error, so that the connection is closed instead of reused.
	if (_writeFailed && _session.networkException() == nullptr) _session.setException(NetException("Incomplete chunk write"));
	return _writeFailed ? -1 : length;
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


HTTPChunkedInputStream::HTTPChunkedInputStream(HTTPSession& session, MessageHeader* pTrailer):
	HTTPChunkedIOS(session, std::ios::in, pTrailer),
	std::istream(&_buf)
{
}


HTTPChunkedInputStream::~HTTPChunkedInputStream()
{
}


//
// HTTPChunkedOutputStream
//


HTTPChunkedOutputStream::HTTPChunkedOutputStream(HTTPSession& session, MessageHeader* pTrailer):
	HTTPChunkedIOS(session, std::ios::out, pTrailer),
	std::ostream(&_buf)
{
}


HTTPChunkedOutputStream::~HTTPChunkedOutputStream()
{
}


} // namespace Poco::Net
