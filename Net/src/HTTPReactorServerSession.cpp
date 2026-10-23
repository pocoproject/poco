#include "Poco/Net/HTTPReactorServerSession.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/NetException.h"
#include "Poco/String.h"
#include "Poco/NumberParser.h"
#include "Poco/Ascii.h"
#include <algorithm>
#include <cstddef>
#include <sstream>

namespace Poco::Net {


HTTPReactorServerSession::HTTPReactorServerSession(
	const StreamSocket& socket, std::string& buf, HTTPServerParams::Ptr pParams)
	: HTTPSession(socket), _buf(buf), _realsocket(socket)
	// Deliver the socket to HTTPSession's base too, so callers that
	// reach for HTTPSession::socket() / detachSocket() (notably the
	// WebSocket(req, rsp) upgrade constructor, which extracts the
	// underlying StreamSocket from the request) get a real socket
	// rather than the default-constructed empty one. The reactor
	// session still overrides read/write/get/peek to drive I/O
	// through _realsocket + the parsed-message buffer.
{
	_pcur = const_cast<char*>(_buf.c_str());
}
/// Creates the HTTPReactorServerSession.

HTTPReactorServerSession::~HTTPReactorServerSession()
{
	if (_complete > 0)
	{
		popCompletedRequest();
	}
	// Clear HTTPSession::_socket before the base destructor's close()
	// would otherwise tear down the per-keep-alive-connection fd. The
	// fd stays alive because _realsocket still holds a ref to the impl.
	(void)detachSocket();
};
/// Destroys the HTTPReactorServerSession.

bool HTTPReactorServerSession::checkRequestComplete()
{
	enum State { PARSING_HEADERS, PARSING_CHUNK_SIZE, PARSING_CHUNK_DATA, PARSING_BODY, COMPLETE };

	State       state = PARSING_HEADERS;
	std::size_t pos = 0;
	std::size_t bodyStart = 0;
	std::size_t contentLength = 0;
	std::size_t chunkSize = 0;

	while (pos < _buf.size())
	{
		switch (state)
		{
		case PARSING_HEADERS:
		{
			bool isChunked = false;
			if (!parseHeaders(pos, bodyStart, contentLength, isChunked))
				return false;
			if (isChunked)
			{
				state = PARSING_CHUNK_SIZE;
				pos = bodyStart;
			} else if (contentLength > 0)
			{
				state = PARSING_BODY;
				pos = bodyStart;
			} else
			{
				_complete = bodyStart;
				return true;
			}
			break;
		}
		case PARSING_CHUNK_SIZE:
		{
			if (!parseChunkSize(pos, chunkSize, _complete))
				return false;
			if (chunkSize == 0)
				return true;
			state = PARSING_CHUNK_DATA;
			break;
		}
		case PARSING_CHUNK_DATA:
		{
			// Compared without adding, so that a large chunk size cannot wrap the
			// sum past the end of the buffer.
			if (chunkSize > _buf.size() - pos || _buf.size() - pos - chunkSize < 2)
				return false; // Incomplete chunk data
			if (_buf.compare(pos + chunkSize, 2, "\r\n") != 0)
				throw MessageException("Missing CRLF after chunk data");
			pos += chunkSize + 2; // Skip chunk data and trailing "\r\n"
			state = PARSING_CHUNK_SIZE;
			break;
		}
		case PARSING_BODY:
		{
			if (_buf.size() >= bodyStart + contentLength)
			{
				_complete = bodyStart + contentLength;
				return true;
			}
			return false; // Incomplete body
		}
		case COMPLETE:
			return true;
		}
	}
	return false; // Request is not complete
}

bool HTTPReactorServerSession::parseHeaders(
	std::size_t pos, std::size_t& bodyStart, std::size_t& contentLength, bool& isChunked)
{
	const std::size_t headerEnd = _buf.find("\r\n\r\n", pos);
	if (headerEnd == std::string::npos)
	{
		return false; // Incomplete headers
	}

	bodyStart = headerEnd + 4; // "\r\n\r\n" is 4 characters
	contentLength = 0;
	isChunked = false;

	// The byte boundary computed here has to agree with the message the request
	// handler is given, so the same parser decides both. A substring search over
	// the raw buffer does not agree: a spelling such as "TRANSFER-ENCODING"
	// would be missed and the body left to be read as a new request.
	const std::size_t fieldsStart = _buf.find("\r\n", pos);
	if (fieldsStart == std::string::npos || fieldsStart >= headerEnd) return true;

	MessageHeader header;
	try
	{
		std::istringstream fields(_buf.substr(fieldsStart + 2, headerEnd + 2 - fieldsStart - 2));
		header.read(fields);
	}
	catch (const Poco::Exception&)
	{
		return false; // Malformed, wait rather than guess a boundary
	}

	isChunked = Poco::icompare(header.get(HTTPMessage::TRANSFER_ENCODING, ""s),
		HTTPMessage::CHUNKED_TRANSFER_ENCODING) == 0;
	if (isChunked) return true;

	const std::string& length = header.get(HTTPMessage::CONTENT_LENGTH, ""s);
	if (length.empty()) return true;
	if (!std::all_of(length.begin(), length.end(),
			[](char c) { return Poco::Ascii::isDigit(c); }))
		return false;
	contentLength = static_cast<std::size_t>(Poco::NumberParser::parseUnsigned64(length));
	return true;
}


bool HTTPReactorServerSession::parseChunkSize(std::size_t& pos, std::size_t& chunkSize, int& complete)
{

	std::size_t chunkSizeEnd = _buf.find("\r\n", pos);
	if (chunkSizeEnd == std::string::npos)
		return false; // Incomplete chunk size

	// Only a size line that HTTPChunkedStreamBuf reads the same way is accepted,
	// so that this framing and the body the request handler reads cannot disagree:
	// one to eight hex digits, then the CR LF that ends the line, a space, a tab
	// or a chunk extension. A sign, a "0x" prefix or any other spelling is rejected
	// rather than guessed. So is a CR or an LF inside the line, because the body
	// reader ends the line at the first LF.
	std::size_t digitsEnd = pos;
	while (digitsEnd < chunkSizeEnd && digitsEnd - pos < 8 && Poco::Ascii::isHexDigit(_buf[digitsEnd]))
		++digitsEnd;
	const bool terminated = digitsEnd == chunkSizeEnd
		|| _buf[digitsEnd] == ' ' || _buf[digitsEnd] == '\t' || _buf[digitsEnd] == ';';
	unsigned chunk = 0;
	if (digitsEnd == pos || !terminated
		|| _buf.find_first_of("\r\n", digitsEnd) != chunkSizeEnd
		|| !Poco::NumberParser::tryParseHex(_buf.substr(pos, digitsEnd - pos), chunk))
		throw MessageException("Malformed chunk size");
	chunkSize = chunk;
	if (chunkSize == 0)
	{
		std::size_t finalChunkEnd = _buf.find("\r\n\r\n", chunkSizeEnd);
		if (finalChunkEnd != std::string::npos)
		{
			complete = finalChunkEnd + 4; // End of "\r\n\r\n"
			return true;
		} else
		{
			return false; // Incomplete final "\r\n\r\n"
		}
	}
	pos = chunkSizeEnd + 2; // Move to the chunk data

	return true;
}

void HTTPReactorServerSession::popCompletedRequest()
{
	if (static_cast<std::size_t>(_complete) >= _buf.length())
	{
		// All data has been processed
		_buf.clear();
	} else
	{
		_buf = _buf.substr(_complete);
	}
	_complete = 0;
	_idx = 0;
	_pcur = const_cast<char*>(_buf.c_str());
	_pend = _pcur + _buf.length();
}

int HTTPReactorServerSession::get()
{
	if (_idx < _complete)
	{
		return static_cast<unsigned char>(_buf[_idx++]);
	} else
	{
		return std::char_traits<char>::eof();
	}
}

int HTTPReactorServerSession::peek()
{
	if (_idx < _complete)
	{

		return static_cast<unsigned char>(_buf[_idx]);
	} else
	{

		return std::char_traits<char>::eof();
	}
}

int HTTPReactorServerSession::read(char* buffer, std::streamsize length)
{
	if (_idx < _complete)
	{
		int n = static_cast<int>(_complete - _idx);
		if (n > static_cast<int>(length)) n = static_cast<int>(length);
		std::memcpy(buffer, _buf.data() + _idx, n);
		_idx += n;
		return n;
	}
	return 0;
}

int HTTPReactorServerSession::write(const char* buffer, std::streamsize length)
{
	try
	{
		return _realsocket.sendBytes(buffer, (int)length);
	} catch (Poco::Exception& exc)
	{
		setException(exc);
		throw;
	}
}

} // namespace Poco::Net

