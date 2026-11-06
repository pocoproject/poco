//
// SimpleFileChannel.cpp
//
// Library: Foundation
// Package: Logging
// Module:  SimpleFileChannel
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SimpleFileChannel.h"
#include "Poco/LogFile.h"
#include "Poco/File.h"
#include "Poco/Message.h"
#include "Poco/Exception.h"
#include "Poco/Ascii.h"
#include "Poco/String.h"
#include "Poco/NumberParser.h"
#include <limits>


namespace Poco {


const std::string SimpleFileChannel::PROP_PATH          = "path";
const std::string SimpleFileChannel::PROP_SECONDARYPATH = "secondaryPath";
const std::string SimpleFileChannel::PROP_ROTATION      = "rotation";
const std::string SimpleFileChannel::PROP_FLUSH         = "flush";


SimpleFileChannel::SimpleFileChannel():
	_limit(0),
	_flush(false),
	_pFile(nullptr)
{
}


SimpleFileChannel::SimpleFileChannel(const std::string& path):
	_path(path),
	_secondaryPath(path + ".0"),
	_limit(0),
	_flush(false),
	_pFile(nullptr)
{
}


SimpleFileChannel::~SimpleFileChannel()
{
	try
	{
		close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void SimpleFileChannel::open()
{
	FastMutex::ScopedLock lock(_mutex);

	if (!_pFile)
	{
		File primary(_path);
		File secondary(_secondaryPath);
		Timestamp pt = primary.exists() ? primary.getLastModified() : 0;
		Timestamp st = secondary.exists() ? secondary.getLastModified() : 0;
		std::string path;
		if (pt >= st)
			path = _path;
		else
			path = _secondaryPath;
		_pFile = new LogFile(path);
	}
}


void SimpleFileChannel::close()
{
	FastMutex::ScopedLock lock(_mutex);

	delete _pFile;
	_pFile = nullptr;
}


void SimpleFileChannel::log(const Message& msg)
{
	open();

	FastMutex::ScopedLock lock(_mutex);

	if (_limit > 0 && _pFile->size() >= _limit)
	{
		rotate();
	}
	_pFile->write(msg.getText(), _flush);
}


void SimpleFileChannel::setProperty(const std::string& name, const std::string& value)
{
	FastMutex::ScopedLock lock(_mutex);

	if (name == PROP_PATH)
	{
		_path = value;
		if (_secondaryPath.empty())
			_secondaryPath = _path + ".0";
	}
	else if (name == PROP_SECONDARYPATH)
		_secondaryPath = value;
	else if (name == PROP_ROTATION)
		setRotation(value);
	else if (name == PROP_FLUSH)
		setFlush(value);
	else
		Channel::setProperty(name, value);
}


std::string SimpleFileChannel::getProperty(const std::string& name) const
{
	FastMutex::ScopedLock lock(_mutex);

	if (name == PROP_PATH)
		return _path;
	else if (name == PROP_SECONDARYPATH)
		return _secondaryPath;
	else if (name == PROP_ROTATION)
		return _rotation;
	else if (name == PROP_FLUSH)
		return (_flush ? "true"s : "false"s);
	else
		return Channel::getProperty(name);
}


Timestamp SimpleFileChannel::creationDate() const
{
	FastMutex::ScopedLock lock(_mutex);

	if (_pFile != nullptr)
		return _pFile->creationDate();
	else
		return 0;
}


UInt64 SimpleFileChannel::size() const
{
	FastMutex::ScopedLock lock(_mutex);

	if (_pFile != nullptr)
		return _pFile->size();
	else
		return 0;
}


std::string SimpleFileChannel::path() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _path;
}


std::string SimpleFileChannel::secondaryPath() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _secondaryPath;
}


namespace
{
	[[nodiscard]] UInt64 scaled(UInt64 n, UInt64 factor, const std::string& rotation)
	{
		if (n > std::numeric_limits<UInt64>::max()/factor)
			throw InvalidArgumentException("rotation", rotation);
		return n*factor;
	}
}


void SimpleFileChannel::setRotation(const std::string& rotation)
{
	std::string::const_iterator it  = rotation.begin();
	std::string::const_iterator end = rotation.end();
	UInt64 n = 0;
	while (it != end && Ascii::isSpace(*it)) ++it;
	std::string digits;
	while (it != end && Ascii::isDigit(*it)) digits += *it++;
	if (!digits.empty() && !NumberParser::tryParseUnsigned64(digits, n))
		throw InvalidArgumentException("rotation", rotation);
	while (it != end && Ascii::isSpace(*it)) ++it;
	std::string unit;
	while (it != end && Ascii::isAlpha(*it)) unit += *it++;

	if (unit == "K")
		_limit = scaled(n, 1024, rotation);
	else if (unit == "M")
		_limit = scaled(n, 1024*1024, rotation);
	else if (unit == "G")
		_limit = scaled(n, static_cast<UInt64>(1024)*1024*1024, rotation);
	else if (unit.empty())
		_limit = n;
	else if (unit == "never")
		_limit = 0;
	else
		throw InvalidArgumentException("rotation", rotation);
	_rotation = rotation;
}


void SimpleFileChannel::setFlush(const std::string& flush)
{
	_flush = icompare(flush, "true") == 0;
}


void SimpleFileChannel::rotate()
{
	std::string newPath;
	if (_pFile->path() == _path)
		newPath = _secondaryPath;
	else
		newPath = _path;
	File f(newPath);
	if (f.exists())
	{
		try
		{
			f.remove();
		}
		catch (...)
		{
		}
	}
	delete _pFile;
	_pFile = new LogFile(newPath);
}


} // namespace Poco
