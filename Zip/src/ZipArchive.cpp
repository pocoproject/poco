//
// ZipArchive.cpp
//
// Library: Zip
// Package: Zip
// Module:	ZipArchive
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "Poco/Zip/ZipArchive.h"
#include "Poco/Zip/SkipCallback.h"
#include "Poco/Zip/ZipException.h"
#include <cstring>
#include <map>
#include <string>


namespace Poco::Zip {


namespace {


template <typename T>
std::string uniqueName(const std::map<std::string, T>& entries, std::map<std::string, int>& suffixes, const std::string& name)
	/// Returns name, or name with the first unused ".duplicate-N" suffix if entries has it.
{
	// find(): std::map::contains() needs C++20, and the library is still built as C++17 for Emscripten.
	if (entries.find(name) == entries.end())
		return name;

	// Names are never removed, so every suffix below the one remembered is still in use.
	int& suffix = suffixes.try_emplace(name, 1).first->second;
	std::string candidate;
	do
	{
		candidate = name + ".duplicate-" + std::to_string(suffix++);
	}
	while (entries.find(candidate) != entries.end());
	return candidate;
}


} // namespace


const std::string ZipArchive::EMPTY_COMMENT;


ZipArchive::ZipArchive(std::istream& in):
	_entries(),
	_infos(),
	_disks(),
	_disks64()
{
	poco_assert_dbg (in);
	SkipCallback skip;
	parse(in, skip);
}


ZipArchive::ZipArchive(const FileHeaders& entries, const FileInfos& infos, const DirectoryInfos& dirs, const DirectoryInfos64& dirs64):
	_entries(entries),
	_infos(infos),
	_disks(dirs),
	_disks64(dirs64)
{
}


ZipArchive::ZipArchive(std::istream& in, ParseCallback& pc):
	_entries(),
	_infos(),
	_disks(),
	_disks64()
{
	poco_assert_dbg (in);
	parse(in, pc);
}


ZipArchive::~ZipArchive()
{
}


void ZipArchive::checkConsistency()
{
	for (const auto& [filename, dirHeader]: _infos)
	{
		Poco::UInt32 dirCRC = dirHeader.getCRC();
		Poco::UInt64 dirUncompressedSize = dirHeader.getUncompressedSize();

		const auto dataIt = _entries.find(filename);
		if (dataIt != _entries.end())
		{
			const ZipLocalFileHeader &dataHeader = dataIt->second;

			Poco::UInt32 dataCRC = dataHeader.getCRC();
			Poco::UInt64 dataUncompressedSize = dataHeader.getUncompressedSize();

			if (dataCRC != dirCRC)
				throw ZipException("CRC-32 mismatch: ", filename);

			if (dataUncompressedSize != dirUncompressedSize)
				throw ZipException("Uncompressed size mismatch: ", filename);
		}
	}

}


void ZipArchive::parse(std::istream& in, ParseCallback& pc)
{
	// read 4 bytes
	bool haveSynced = false;
	std::map<std::string, int> entrySuffixes;
	std::map<std::string, int> infoSuffixes;
	while (in.good() && !in.eof())
	{
		char header[ZipCommon::HEADER_SIZE]={'\x00', '\x00', '\x00', '\x00'};
		in.read(header, ZipCommon::HEADER_SIZE);
		if (in.eof())
			return;
		if (std::memcmp(header, ZipLocalFileHeader::HEADER, ZipCommon::HEADER_SIZE) == 0)
		{
			ZipLocalFileHeader entry(in, true, pc);
			_entries.insert(std::make_pair(uniqueName(_entries, entrySuffixes, entry.getFileName()), entry));
			haveSynced = false;
		}
		else if (std::memcmp(header, ZipFileInfo::HEADER, ZipCommon::HEADER_SIZE) == 0)
		{
			ZipFileInfo info(in, true);
			FileHeaders::iterator it = _entries.find(info.getFileName());
			if (it != _entries.end())
			{
				it->second.setStartPos(info.getOffset());
			}
			_infos.insert(std::make_pair(uniqueName(_infos, infoSuffixes, info.getFileName()), info));
			haveSynced = false;
		}
		else if (std::memcmp(header, ZipArchiveInfo::HEADER, ZipCommon::HEADER_SIZE) == 0)
		{
			ZipArchiveInfo nfo(in, true);
			if (!_disks.insert(std::make_pair(nfo.getDiskNumber(), nfo)).second)
				throw ZipException("Duplicate end of central directory record");
			haveSynced = false;
		}
		else if (std::memcmp(header, ZipArchiveInfo64::HEADER, ZipCommon::HEADER_SIZE) == 0)
		{
			ZipArchiveInfo64 nfo(in, true);
			if (!_disks64.insert(std::make_pair(nfo.getDiskNumber(), nfo)).second)
				throw ZipException("Duplicate ZIP64 end of central directory record");
			haveSynced = false;
		}
		else
		{
			if (!haveSynced)
			{
				// Some Zip files have extra data behind the ZipLocalFileHeader.
				// Try to re-sync.
				ZipUtil::sync(in);
				haveSynced = true;
			}
			else
			{
				if (_disks.empty() && _disks64.empty())
					throw ZipException("Illegal header in zip file");
				else
					throw ZipException("Garbage after directory header");
			}
		}
	}
}


const std::string& ZipArchive::getZipComment() const
{
	// Only the "first" disk is populated (see Compress::close()). A ZIP64 end of
	// central directory record carries no comment field, so an archive that has
	// only that record has no comment to return.
	DirectoryInfos::const_iterator it = _disks.begin();
	if (it != _disks.end())
		return it->second.getZipComment();
	return EMPTY_COMMENT;
}


} // namespace Poco::Zip
