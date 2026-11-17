//
// ZipArchiveInfo.cpp
//
// Library: Zip
// Package: Zip
// Module:  ZipArchiveInfo
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Zip/ZipArchiveInfo.h"
#include "Poco/Zip/ZipException.h"
#include "Poco/Buffer.h"
#include "Poco/Exception.h"
#include "Poco/StreamCopier.h"
#include <istream>
#include <sstream>
#include <limits>
#include <cstring>


namespace Poco::Zip {


ZipArchiveInfo::ZipArchiveInfo(std::istream& in, bool assumeHeaderRead):
	_rawInfo(),
	_startPos(in.tellg()),
	_comment()
{
	if (assumeHeaderRead)
		_startPos -= ZipCommon::HEADER_SIZE;
	parse(in, assumeHeaderRead);
}


ZipArchiveInfo::ZipArchiveInfo():
	_rawInfo(),
	_startPos(0),
	_comment()
{
	std::memset(_rawInfo, 0, FULLHEADER_SIZE);
	std::memcpy(_rawInfo, HEADER, ZipCommon::HEADER_SIZE);
}


ZipArchiveInfo::~ZipArchiveInfo()
{
}


void ZipArchiveInfo::parse(std::istream& inp, bool assumeHeaderRead)
{
	if (!assumeHeaderRead)
	{
		inp.read(_rawInfo, ZipCommon::HEADER_SIZE);
		if (inp.gcount() != ZipCommon::HEADER_SIZE)
			throw Poco::IOException("Failed to read archive info header");
		if (std::memcmp(_rawInfo, HEADER, ZipCommon::HEADER_SIZE) != 0)
			throw Poco::DataFormatException("Bad archive info header");
	}
	else
	{
		std::memcpy(_rawInfo, HEADER, ZipCommon::HEADER_SIZE);
	}

	// read the rest of the header
	inp.read(_rawInfo + ZipCommon::HEADER_SIZE, static_cast<std::streamsize>(FULLHEADER_SIZE) - ZipCommon::HEADER_SIZE);
	if (inp.gcount() != static_cast<std::streamsize>(FULLHEADER_SIZE) - ZipCommon::HEADER_SIZE)
		throw Poco::IOException("Failed to read archive info header");
	Poco::UInt16 len = getZipCommentSize();
	if (len > 0)
	{
		Poco::Buffer<char> buf(len);
		inp.read(buf.begin(), len);
		// A comment that runs past the end of the archive is cut, as other readers do.
		_comment = std::string(buf.begin(), static_cast<std::size_t>(inp.gcount()));
	}
}


std::string ZipArchiveInfo::createHeader() const
{
	std::string result(_rawInfo, FULLHEADER_SIZE);
	result.append(_comment);
	return result;
}


void ZipArchiveInfo::setZipComment(const std::string& comment)
{
	// Confirm string is of valid size
	if (comment.size() > 65535)
		throw ZipException("Maximum number of entries for a ZIP file reached: 65535");

	// Change the value of the ZIP Comment Size to reflect new comment size
	ZipUtil::set16BitValue(static_cast<Poco::UInt16>(comment.size()), _rawInfo, ZIPCOMMENT_LENGTH_POS);

	// Now change our internal comment
	_comment = comment;
}


ZipArchiveInfo64::ZipArchiveInfo64(std::istream& in, bool assumeHeaderRead):
	_rawInfo(),
	_startPos(in.tellg())
{
	if (assumeHeaderRead)
		_startPos -= ZipCommon::HEADER_SIZE;
	parse(in, assumeHeaderRead);
}


ZipArchiveInfo64::ZipArchiveInfo64():
	_rawInfo(),
	_startPos(0)
{
	std::memset(_rawInfo, 0, FULL_HEADER_SIZE);
	std::memcpy(_rawInfo, HEADER, ZipCommon::HEADER_SIZE);
	ZipUtil::set64BitValue(FULL_HEADER_SIZE - (RECORDSIZE_POS + RECORDSIZE_SIZE), _rawInfo, RECORDSIZE_POS);
	std::memset(_locInfo, 0, FULL_LOCATOR_SIZE);
	std::memcpy(_locInfo, LOCATOR_HEADER, ZipCommon::HEADER_SIZE);
	setRequiredVersion(4, 5);
}


ZipArchiveInfo64::~ZipArchiveInfo64()
{
}


void ZipArchiveInfo64::parse(std::istream& inp, bool assumeHeaderRead)
{
	if (!assumeHeaderRead)
	{
		inp.read(_rawInfo, ZipCommon::HEADER_SIZE);
		if (inp.gcount() != ZipCommon::HEADER_SIZE)
			throw Poco::IOException("Failed to read archive info header");
		if (std::memcmp(_rawInfo, HEADER, ZipCommon::HEADER_SIZE) != 0)
			throw Poco::DataFormatException("Bad archive info header");
	}
	else
	{
		std::memcpy(_rawInfo, HEADER, ZipCommon::HEADER_SIZE);
	}

	std::memset(_rawInfo + ZipCommon::HEADER_SIZE, 0, static_cast<std::size_t>(FULL_HEADER_SIZE) - ZipCommon::HEADER_SIZE);

	// read the rest of the header
	Poco::UInt64 offset = RECORDSIZE_POS;
	inp.read(_rawInfo + ZipCommon::HEADER_SIZE, RECORDSIZE_SIZE);
	if (inp.gcount() != RECORDSIZE_SIZE)
		throw Poco::IOException("Failed to read archive info header");
	offset += RECORDSIZE_SIZE;
	Poco::UInt64 len = ZipUtil::get64BitValue(_rawInfo, RECORDSIZE_POS);
	if (len <= FULL_HEADER_SIZE - offset)
	{
		inp.read(_rawInfo + offset, len);
		if (inp.gcount() != static_cast<std::streamsize>(len))
			throw Poco::IOException("Failed to read archive info header");
		ZipUtil::set64BitValue(FULL_HEADER_SIZE - offset, _rawInfo, RECORDSIZE_POS);
	}
	else
	{
		inp.read(_rawInfo + offset, FULL_HEADER_SIZE - offset);
		if (inp.gcount() != static_cast<std::streamsize>(FULL_HEADER_SIZE - offset))
			throw Poco::IOException("Failed to read archive info header");
		len -= (FULL_HEADER_SIZE - offset);
		// The record size is read from the archive, so the extensible data grows
		// as the data actually arrives rather than being allocated from the
		// declared length in one go. No stream holds more than the largest
		// std::streamsize, so a larger declared length is a truncated record.
		std::ostringstream extraField;
		if (len > static_cast<Poco::UInt64>(std::numeric_limits<std::streamsize>::max()) ||
			Poco::StreamCopier::copyStreamN64(inp, extraField, static_cast<std::streamsize>(len)) != len)
		{
			throw Poco::DataFormatException("Truncated ZIP64 end of central directory record");
		}
		_extraField = extraField.str();
		ZipUtil::set64BitValue(FULL_HEADER_SIZE + _extraField.size() - offset, _rawInfo, RECORDSIZE_POS);
	}
	inp.read(_locInfo, FULL_LOCATOR_SIZE);
	if (inp.gcount() != FULL_LOCATOR_SIZE)
		throw Poco::IOException("Failed to read locator");
	if (std::memcmp(_locInfo, LOCATOR_HEADER, ZipCommon::HEADER_SIZE) != 0)
		throw Poco::DataFormatException("Bad locator header");

}


std::string ZipArchiveInfo64::createHeader() const
{
	std::string result(_rawInfo, FULL_HEADER_SIZE);
	result.append(_extraField);
	result.append(_locInfo, FULL_LOCATOR_SIZE);
	return result;
}


} // namespace Poco::Zip
