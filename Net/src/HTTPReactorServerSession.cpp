#include "Poco/Net/HTTPReactorServerSession.h"
#include <cstddef>

namespace Poco { 
namespace Net {


HTTPReactorServerSession::HTTPReactorServerSession(
	const StreamSocket& socket, std::string& buf, HTTPServerParams::Ptr pParams)
	: // do not deliver socket to HTTPSession
	  HTTPSession(), _buf(buf), _realsocket(socket), _complete(0), _idx(0)
{
	_pcur = const_cast<char*>(_buf.c_str());
	_idx = 0;
}
/// Creates the HTTPReactorServerSession.

HTTPReactorServerSession::~HTTPReactorServerSession()
{
	if (_complete > 0)
	{
		popCompletedRequest();
	}
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
			if (pos + chunkSize + 2 <= _buf.size())
			{
				pos += chunkSize + 2; // Skip chunk data and trailing "\r\n"
				state = PARSING_CHUNK_SIZE;
			} else
			{
				return false; // Incomplete chunk data
			}
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
	std::size_t headerEnd = _buf.find("\r\n\r\n", pos);
	if (headerEnd == std::string::npos)
	{
		return false; // Incomplete headers
	}

	bodyStart = headerEnd + 4; // "\r\n\r\n" is 4 characters
	std::size_t chunkedPos = _buf.find("Transfer-Encoding: chunked", pos);
	if (chunkedPos != std::string::npos)
	{
		isChunked = true;
		return true;
	}
	std::size_t contentLengthPos = _buf.find("Content-Length:", pos);
	if (contentLengthPos != std::string::npos)
	{
		std::size_t valueStart = contentLengthPos + 15; // "Content-Length:" is 15 characters
		std::size_t valueEnd = _buf.find("\r\n", valueStart);
		if (valueEnd != std::string::npos)
		{
			contentLength = std::stoi(_buf.substr(valueStart, valueEnd - valueStart));
			isChunked = false;
			return true;
		} else
		{
			return false; // Incomplete Content-Length header
		}
	}
	contentLength = 0;
	isChunked = false;
	return true;
}

bool HTTPReactorServerSession::parseChunkSize(std::size_t& pos, std::size_t& chunkSize, int& complete)
{

	std::size_t chunkSizeEnd = _buf.find("\r\n", pos);
	if (chunkSizeEnd == std::string::npos)
		return false; // Incomplete chunk size

	std::string chunkSizeStr = _buf.substr(pos, chunkSizeEnd - pos);
	chunkSize = std::stoi(chunkSizeStr, nullptr, 16); // Parse hex chunk size
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
	if (_complete >= _buf.length())
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
		return _buf[_idx++];
	} else
	{
		return std::char_traits<char>::eof();
	}
}

int HTTPReactorServerSession::peek()
{
	if (_idx < _complete)
	{

		return _buf[_idx];
	} else
	{

		return std::char_traits<char>::eof();
	}
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

}} // namespace Poco::Net

