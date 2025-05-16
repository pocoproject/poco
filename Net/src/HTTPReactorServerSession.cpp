#include "Poco/Net/HTTPReactorServerSession.h"

namespace Poco {
namespace Net {


HTTPReactorServerSession::HTTPReactorServerSession(
	const StreamSocket& socket, std::string& buf, HTTPServerParams::Ptr pParams)
	: // do not deliver socket to HTTPSession
	  HTTPSession(), _buf(buf), _realsocket(socket)
{
	_pcur = const_cast<char*>(_buf.c_str());
	_idx = 0;
}
/// Creates the HTTPReactorServerSession.

HTTPReactorServerSession::~HTTPReactorServerSession(){};
/// Destroys the HTTPReactorServerSession.

bool HTTPReactorServerSession::checkRequestComplete()
{
	enum State { PARSING_HEADERS, PARSING_CHUNK_SIZE, PARSING_CHUNK_DATA, PARSING_TRAILER, PARSING_BODY, COMPLETE };

	State       state = PARSING_HEADERS;
	std::size_t pos = 0;
	std::size_t bodyStart = 0;
	std::size_t contentLength = 0;
	std::size_t chunkSize = 0;
	bool        hasTrailer = false;

	while (pos < _buf.size())
	{
		switch (state)
		{
		case PARSING_HEADERS: {
			std::size_t headerEnd = _buf.find("\r\n\r\n", pos);
			if (headerEnd != std::string::npos)
			{
				// Headers are complete
				bodyStart = headerEnd + 4; // "\r\n\r\n" is 4 characters
				std::size_t chunkedPos = _buf.find("Transfer-Encoding: chunked", pos);
				if (chunkedPos != std::string::npos)
				{
					// Check if there's a Trailer header
					std::size_t trailerPos = _buf.find("Trailer:", pos);
					if (trailerPos != std::string::npos)
					{
						hasTrailer = true;
					}
					state = PARSING_CHUNK_SIZE;
					pos = bodyStart;
				} else
				{
					std::size_t contentLengthPos = _buf.find("Content-Length:", pos);
					if (contentLengthPos != std::string::npos)
					{
						std::size_t valueStart = contentLengthPos + 15; // "Content-Length:" is 15 characters
						std::size_t valueEnd = _buf.find("\r\n", valueStart);
						if (valueEnd != std::string::npos)
						{
							contentLength = std::stoi(_buf.substr(valueStart, valueEnd - valueStart));
							state = PARSING_BODY;
							pos = bodyStart;
						} else
						{
							return false; // Incomplete Content-Length header
						}
					} else
					{
						// No Content-Length, assume no body (e.g., GET request)
						_complete = headerEnd + 3;
						return true;
					}
				}
			} else
			{
				return false; // Incomplete headers
			}
			break;
		}
		case PARSING_CHUNK_SIZE: {
			std::size_t chunkSizeEnd = _buf.find("\r\n", pos);
			if (chunkSizeEnd != std::string::npos)
			{
				std::string chunkSizeStr = _buf.substr(pos, chunkSizeEnd - pos);
				chunkSize = std::stoi(chunkSizeStr, nullptr, 16); // Parse hex chunk size
				if (chunkSize == 0)
				{
					// Last chunk found
					if (hasTrailer)
					{
						state = PARSING_TRAILER;
						pos = chunkSizeEnd + 2; // Move past the "0\r\n"
					} else
					{
						// No trailer, check for final "\r\n\r\n"
						if (_buf.find("\r\n\r\n", chunkSizeEnd + 2) == chunkSizeEnd + 2)
						{
							_complete = chunkSizeEnd + 4; // End of "0\r\n\r\n"
							return true;
						}
						return false; // Incomplete final "\r\n\r\n"
					}
				} else
				{
					state = PARSING_CHUNK_DATA;
					pos = chunkSizeEnd + 2; // Move to the chunk data
				}
			} else
			{
				return false; // Incomplete chunk size
			}
			break;
		}
		case PARSING_CHUNK_DATA: {
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
		case PARSING_TRAILER: {
			std::size_t trailerEnd = _buf.find("\r\n\r\n", pos);
			if (trailerEnd != std::string::npos)
			{
				// Trailer is complete
				_complete = trailerEnd + 3; // End of the trailer
				return true;
			} else
			{
				return false; // Incomplete trailer
			}
			break;
		}
		case PARSING_BODY: {
			if (_buf.size() >= bodyStart + contentLength)
			{
				_complete = bodyStart + contentLength - 1;
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

void HTTPReactorServerSession::popCompletedRequest()
{
	poco_assert(_complete + 1 == _idx);
	if (_complete + 1 >= _buf.length())
	{
		// All data has been processed
		_buf.clear();
	} else
	{
		_buf = _buf.substr(_complete + 1);
	}
	_complete = 0;
	_idx = 0;
	_pcur = const_cast<char*>(_buf.c_str());
	_pend = _pcur + _buf.length();
}

int HTTPReactorServerSession::get()
{
	if (_idx < _buf.length())
	{
		return _buf[_idx++];
	} else
	{
		return std::char_traits<char>::eof();
	}
}

int HTTPReactorServerSession::peek()
{
	if (_idx < _buf.length())
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

