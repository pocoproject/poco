//
// LogFile.cpp
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


#include "Poco/LogFile.h"
#include "Poco/File.h"
#include "Poco/Exception.h"

#include <unistd.h>

namespace Poco {


LogFile::LogFile(const std::string& path):
	_path(path),
	_str(_path, std::ios::app),
	_size(static_cast<UInt64>(_str.tellp()))
{
	if (_size == 0)
		_creationDate = File(path).getLastModified();
	else
		_creationDate = File(path).created();
}


LogFile::~LogFile()
{
}


void LogFile::write(const std::string& text, bool flush)
{
	std::streampos pos = _str.tellp();

	_str << text << '\n';

	if (flush)
		_str.flushToDisk();
	else
		_str.flush();

	if (!_str.good())
	{
		_str.clear();
		_str.seekp(pos);
		throw WriteFileException(_path);
	}

	_size = static_cast<UInt64>(_str.tellp());
}


UInt64 LogFile::size() const
{
	return _size;
}


Timestamp LogFile::creationDate() const
{
	return _creationDate;
}


const std::string& LogFile::path() const
{
	return _path;
}


} // namespace Poco
