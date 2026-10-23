//
// HTTPMessage.cpp
//
// Library: Net
// Package: HTTP
// Module:  HTTPMessage
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/NetException.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/Ascii.h"
#include "Poco/String.h"
#include <algorithm>
#include <charconv>


using Poco::NumberFormatter;
using Poco::NumberParser;
using Poco::icompare;


namespace Poco::Net {


const std::string HTTPMessage::HTTP_1_0                   = "HTTP/1.0";
const std::string HTTPMessage::HTTP_1_1                   = "HTTP/1.1";
const std::string HTTPMessage::IDENTITY_TRANSFER_ENCODING = "identity";
const std::string HTTPMessage::CHUNKED_TRANSFER_ENCODING  = "chunked";
const std::string HTTPMessage::UNKNOWN_CONTENT_TYPE;
const std::string HTTPMessage::CONTENT_LENGTH             = "Content-Length";
const std::string HTTPMessage::CONTENT_TYPE               = "Content-Type";
const std::string HTTPMessage::TRANSFER_ENCODING          = "Transfer-Encoding";
const std::string HTTPMessage::CONNECTION                 = "Connection";
const std::string HTTPMessage::PROXY_CONNECTION           = "Proxy-Connection";
const std::string HTTPMessage::CONNECTION_KEEP_ALIVE      = "Keep-Alive";
const std::string HTTPMessage::CONNECTION_CLOSE           = "Close";
const std::string HTTPMessage::EMPTY;


HTTPMessage::HTTPMessage():
	_version(HTTP_1_0)
{
}


HTTPMessage::HTTPMessage(const std::string& version):
	_version(version)
{
}


HTTPMessage::HTTPMessage(const HTTPMessage& other):
	MessageHeader(other),
	_version(other._version)
{
}


HTTPMessage::~HTTPMessage()
{
}


HTTPMessage& HTTPMessage::operator = (const HTTPMessage& other)
{
	if (this != &other)
	{
		MessageHeader::operator = (other);
		_version = other._version;
	}
	return *this;
}


void HTTPMessage::setVersion(const std::string& version)
{
	_version = version;
}


void HTTPMessage::setContentLength(std::streamsize length)
{
	if (length != UNKNOWN_CONTENT_LENGTH)
		set(CONTENT_LENGTH, NumberFormatter::format(length));
	else
		erase(CONTENT_LENGTH);
}


std::streamsize HTTPMessage::getContentLength() const
{
	const std::string& contentLength = get(CONTENT_LENGTH, EMPTY);
	if (!contentLength.empty())
	{
		if (sizeof(std::streamsize) == sizeof(Poco::Int64))
			return static_cast<std::streamsize>(NumberParser::parse64(contentLength));
		else
			return static_cast<std::streamsize>(NumberParser::parse(contentLength));
	}
	else return UNKNOWN_CONTENT_LENGTH;
}


#if defined(POCO_HAVE_INT64)
void HTTPMessage::setContentLength64(Poco::Int64 length)
{
	if (length != UNKNOWN_CONTENT_LENGTH)
		set(CONTENT_LENGTH, NumberFormatter::format(length));
	else
		erase(CONTENT_LENGTH);
}


Poco::Int64 HTTPMessage::getContentLength64() const
{
	const std::string& contentLength = get(CONTENT_LENGTH, EMPTY);
	if (!contentLength.empty())
	{
		return NumberParser::parse64(contentLength);
	}
	else return UNKNOWN_CONTENT_LENGTH;
}
#endif // defined(POCO_HAVE_INT64)


void HTTPMessage::setTransferEncoding(const std::string& transferEncoding)
{
	if (icompare(transferEncoding, IDENTITY_TRANSFER_ENCODING) == 0)
		erase(TRANSFER_ENCODING);
	else
		set(TRANSFER_ENCODING, transferEncoding);
}


std::string HTTPMessage::getTransferEncoding() const
{
	return get(TRANSFER_ENCODING, IDENTITY_TRANSFER_ENCODING);
}


void HTTPMessage::setChunkedTransferEncoding(bool flag)
{
	if (flag)
	{
		setTransferEncoding(CHUNKED_TRANSFER_ENCODING);
		// The two together make the message length ambiguous, and a recipient is
		// entitled to reject it.
		erase(CONTENT_LENGTH);
	}
	else setTransferEncoding(IDENTITY_TRANSFER_ENCODING);
}


bool HTTPMessage::getChunkedTransferEncoding() const
{
	return icompare(getTransferEncoding(), CHUNKED_TRANSFER_ENCODING) == 0;
}


void HTTPMessage::checkFramingHeaders() const
{
	// A message with an ambiguous length is refused rather than framed on one
	// of the competing values: an intermediary may pick the other one.
	std::size_t contentLengths = 0;
	std::size_t transferEncodings = 0;
	for (const auto& [name, value]: *this)
	{
		if (icompare(name, CONTENT_LENGTH) == 0) ++contentLengths;
		else if (icompare(name, TRANSFER_ENCODING) == 0) ++transferEncodings;
	}
	if (contentLengths > 1 || transferEncodings > 1)
		throw MessageException("Duplicate Content-Length or Transfer-Encoding header");
	if (transferEncodings == 1 && contentLengths == 1)
		throw MessageException("Both Content-Length and Transfer-Encoding present");

	// NumberParser skips thousands separators without checking their position,
	// so "5,5" would be read as 55 while another recipient sees 5 or rejects it.
	if (contentLengths == 1)
	{
		const std::string& value = get(CONTENT_LENGTH);
		Poco::Int64 length = 0;
		if (value.empty() || !std::all_of(value.begin(), value.end(),
				[](char c) { return Poco::Ascii::isDigit(c); }) ||
			std::from_chars(value.data(), value.data() + value.size(), length).ec != std::errc())
			throw MessageException("Malformed Content-Length header");
	}
}


void HTTPMessage::setContentType(const std::string& mediaType)
{
	if (mediaType.empty())
		erase(CONTENT_TYPE);
	else
		set(CONTENT_TYPE, mediaType);
}


void HTTPMessage::setContentType(const MediaType& mediaType)
{
	setContentType(mediaType.toString());
}


std::string HTTPMessage::getContentType() const
{
	return get(CONTENT_TYPE, UNKNOWN_CONTENT_TYPE);
}


void HTTPMessage::setKeepAlive(bool keepAlive)
{
	if (keepAlive)
		set(CONNECTION, CONNECTION_KEEP_ALIVE);
	else
		set(CONNECTION, CONNECTION_CLOSE);
}


bool HTTPMessage::getKeepAlive() const
{
	const std::string& connection = get(CONNECTION, EMPTY);
	if (!connection.empty())
		return icompare(connection, CONNECTION_CLOSE) != 0;
	else
		return getVersion() == HTTP_1_1;
}


void HTTPMessage::write(std::ostream& ostr) const
{
	checkFields(*this, false);
	writeFields(*this, ostr);
}


void HTTPMessage::writeTrailer(const MessageHeader& trailer, std::ostream& ostr)
{
	checkFields(trailer, false);
	writeFields(trailer, ostr);
}


bool HTTPMessage::isSpaceOrControl(char c)
{
	return c == ' ' || Poco::Ascii::hasProperties(c, Poco::Ascii::ACP_CONTROL);
}


} // namespace Poco::Net
