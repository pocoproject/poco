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
	_str << text;
	if (flush)
		_str << std::endl;
	else
		_str << "\n";
	if (!_str.good())
	{
		_str.clear();
		_str.seekp(pos);
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
