//
// LogFile_STD.cpp
//
// Library: Foundation
// Package: Logging
// Module:  LogFile
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/LogFile_STD.h"
#include "Poco/File.h"
#include "Poco/Exception.h"

#include <unistd.h>

namespace Poco {


LogFileImpl::LogFileImpl(const std::string& path):
	_path(path),
	_str(_path, std::ios::app),
	_size(static_cast<UInt64>(_str.tellp()))
{
	if (_size == 0)
		_creationDate = File(path).getLastModified();
	else
		_creationDate = File(path).created();
}


LogFileImpl::~LogFileImpl()
{
}


void LogFileImpl::writeImpl(const std::string& text, bool flush)
{
	std::streampos pos = _str.tellp();

	_str << text << '\n';

	// Flush the stream buffer to file to match the implementation on Windows
	_str.flush();

	if (!_str.good())
	{
		_str.clear();
		_str.seekp(pos);
		throw WriteFileException(_path);
	}

	if (flush)
	{
		// Sync the file to disk as it is done on Windows
		if (fsync(_str.nativeHandle()) != 0)
			throw WriteFileException(_path);
	}

	_size = static_cast<UInt64>(_str.tellp());
}


UInt64 LogFileImpl::sizeImpl() const
{
	return _size;
}


Timestamp LogFileImpl::creationDateImpl() const
{
	return _creationDate;
}


const std::string& LogFileImpl::pathImpl() const
{
	return _path;
}


} // namespace Poco
