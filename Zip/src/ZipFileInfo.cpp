//
// ZipFileInfo.cpp
//
// Library: Zip
// Package: Zip
// Module:  ZipFileInfo
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Zip/ZipFileInfo.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "Poco/Buffer.h"
#include <istream>
#include <cstring>


namespace Poco::Zip {


ZipFileInfo::ZipFileInfo(const ZipLocalFileHeader& header):
	_rawInfo(),
	_crc32(0),
	_compressedSize(0),
	_uncompressedSize(0),
	_localHeaderOffset(0),
	_fileName(),
	_lastModifiedAt(),
	_extraField()
{
	std::memset(_rawInfo, 0, FULLHEADER_SIZE);
	std::memcpy(_rawInfo, HEADER, ZipCommon::HEADER_SIZE);
	setCompressedSize(header.getCompressedSize());
	setUncompressedSize(header.getUncompressedSize());
	setCRC(header.getCRC());
	setCompressionMethod(header.getCompressionMethod());
	setCompressionLevel(header.getCompressionLevel());
	setRequiredVersion(header.getMajorVersionNumber(), header.getMinorVersionNumber());
	setHostSystem(header.getHostSystem());
	setLastModifiedAt(header.lastModifiedAt());
	setEncryption(false);
	setFileName(header.getFileName());

	if (getHostSystem() == ZipCommon::HS_UNIX)
		setUnixAttributes();

	_rawInfo[GENERAL_PURPOSE_POS+1] |= 0x08; // Set "language encoding flag" to indicate that filenames and paths are in UTF-8.

	if (header.searchCRCAndSizesAfterData())
		_rawInfo[GENERAL_PURPOSE_POS] |= 0x08;
}


ZipFileInfo::ZipFileInfo(std::istream& in, bool assumeHeaderRead):
	_rawInfo(),
	_crc32(0),
	_compressedSize(0),
	_uncompressedSize(0),
	_localHeaderOffset(0),
	_fileName(),
	_lastModifiedAt(),
	_extraField()
{
	// sanity check
	poco_assert_dbg (RELATIVEOFFSETLOCALHEADER_POS + RELATIVEOFFSETLOCALHEADER_SIZE == FULLHEADER_SIZE);
	parse(in, assumeHeaderRead);
}


ZipFileInfo::~ZipFileInfo()
{
}


void ZipFileInfo::parse(std::istream& inp, bool assumeHeaderRead)
{
	if (!assumeHeaderRead)
	{
		inp.read(_rawInfo, ZipCommon::HEADER_SIZE);
		if (inp.gcount() != ZipCommon::HEADER_SIZE)
			throw Poco::IOException("Failed to read file info header");
		if (std::memcmp(_rawInfo, HEADER, ZipCommon::HEADER_SIZE) != 0)
			throw Poco::DataFormatException("Bad file info header");
	}
	else
	{
		std::memcpy(_rawInfo, HEADER, ZipCommon::HEADER_SIZE);
	}

	// read the rest of the header
	inp.read(_rawInfo + ZipCommon::HEADER_SIZE, static_cast<std::streamsize>(FULLHEADER_SIZE) - ZipCommon::HEADER_SIZE);
	_crc32 = getCRCFromHeader();
	_compressedSize = getCompressedSizeFromHeader();
	_uncompressedSize = getUncompressedSizeFromHeader();
	_localHeaderOffset = getOffsetFromHeader();
	parseDateTime();
	Poco::UInt16 len = getFileNameLength();
	if (len > 0)
	{
		Poco::Buffer<char> buf(len);
		inp.read(buf.begin(), len);
		_fileName = std::string(buf.begin(), len);
	}
	if (hasExtraField())
	{
		len = getExtraFieldLength();
		if (len > 0)
		{
			Poco::Buffer<char> xtra(len);
			inp.read(xtra.begin(), len);
			_extraField = std::string(xtra.begin(), len);
			// The field sizes come from the archive, so every read is bounded by
			// the buffer that was actually read rather than by the declared size.
			const char* const end = xtra.begin() + len;
			char* ptr = xtra.begin();
			while (end - ptr >= ZipCommon::EXTRA_FIELD_HEADER_SIZE)
			{
				const Poco::UInt16 id = ZipUtil::get16BitValue(ptr, 0);
				const Poco::UInt16 size = ZipUtil::get16BitValue(ptr, 2);
				ptr += ZipCommon::EXTRA_FIELD_HEADER_SIZE;
				if (size > end - ptr) break;
				if (id == ZipCommon::ZIP64_EXTRA_ID)
				{
					char* field = ptr;
					Poco::UInt16 remaining = size;
					const auto readNext = [&field, &remaining]()
					{
						const Poco::UInt64 value = ZipUtil::get64BitValue(field, 0);
						field += ZipCommon::ZIP64_VALUE_SIZE;
						remaining -= ZipCommon::ZIP64_VALUE_SIZE;
						return value;
					};
					if (remaining >= ZipCommon::ZIP64_VALUE_SIZE && getUncompressedSizeFromHeader() == ZipCommon::ZIP64_MAGIC)
						setUncompressedSize(readNext());
					if (remaining >= ZipCommon::ZIP64_VALUE_SIZE && getCompressedSizeFromHeader() == ZipCommon::ZIP64_MAGIC)
						setCompressedSize(readNext());
					if (remaining >= ZipCommon::ZIP64_VALUE_SIZE && getOffsetFromHeader() == ZipCommon::ZIP64_MAGIC)
						setOffset(readNext());
				}
				ptr += size;
			}
		}
	}
	len = getFileCommentLength();
	if (len > 0)
	{
		Poco::Buffer<char> buf2(len);
		inp.read(buf2.begin(), len);
		_fileComment = std::string(buf2.begin(), len);
	}
}


std::string ZipFileInfo::createHeader() const
{
	std::string result(_rawInfo, FULLHEADER_SIZE);
	result.append(_fileName);
	result.append(_extraField);
	result.append(_fileComment);
	return result;
}


void ZipFileInfo::setUnixAttributes()
{
	bool isDir = isDirectory();
	int mode;
	if (isDir)
		mode = DEFAULT_UNIX_DIR_MODE;
	else
		mode = DEFAULT_UNIX_FILE_MODE;
	Poco::UInt32 attrs = (mode << 16) | (isDir ? 0x10 : 0);
	setExternalFileAttributes(attrs);
}


} // namespace Poco::Zip
