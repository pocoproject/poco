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

namespace Poco {


LogFile::LogFile(const std::string& path):
	_path(path),
	_str(_path, std::ios::app),
	_size(static_cast<UInt64>(_str.tellp()))
{
	// There seems to be a strange "optimization" in the Windows NTFS
	// filesystem that causes it to reuse directory entries of deleted
	// files. Example:
	// 1. create a file named "test.dat"
	//    note the file's creation date
	// 2. delete the file "test.dat"
	// 3. wait a few seconds
	// 4. create a file named "test.dat"
	//    the new file will have the same creation
	//    date as the old one.
	// We work around this bug by taking the file's
	// modification date as a reference when the
	// file is empty.
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

#if defined(POCO_OS_FAMILY_WINDOWS)
	// Replace \n with \r\n
	std::string logText;
	logText.reserve(text.size() + 16); // keep some reserve for \n -> \r\n
	char prevChar = 0;
	for (char c: text)
	{
		if (c == '\n' && prevChar != '\r')
			logText += POCO_DEFAULT_NEWLINE_CHARS;
		else
			logText += c;

		prevChar = c;
	}
	_str << logText;
#else
	_str << text;
#endif

	_str << POCO_DEFAULT_NEWLINE_CHARS;

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
