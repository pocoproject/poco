//
// FileChannel.cpp
//
// Library: Foundation
// Package: Logging
// Module:  FileChannel
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/FileChannel.h"
#include "Poco/ArchiveStrategy.h"
#include "Poco/RotateStrategy.h"
#include "Poco/PurgeStrategy.h"
#include "Poco/Message.h"
#include "Poco/NumberParser.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTime.h"
#include "Poco/LocalDateTime.h"
#include "Poco/String.h"
#include "Poco/Exception.h"
#include "Poco/Ascii.h"


namespace Poco {


const std::string FileChannel::PROP_PATH         = "path";
const std::string FileChannel::PROP_ROTATION     = "rotation";
const std::string FileChannel::PROP_ARCHIVE      = "archive";
const std::string FileChannel::PROP_TIMES        = "times";
const std::string FileChannel::PROP_COMPRESS     = "compress";
const std::string FileChannel::PROP_PURGEAGE     = "purgeAge";
const std::string FileChannel::PROP_PURGECOUNT   = "purgeCount";
const std::string FileChannel::PROP_FLUSH        = "flush";
const std::string FileChannel::PROP_ROTATEONOPEN = "rotateOnOpen";

FileChannel::FileChannel():
	_times("utc"),
	_compress(false),
	_flush(false),
	_rotateOnOpen(false),
	_pFile(nullptr),
	_pRotateStrategy(new NullRotateStrategy()),
	_pArchiveStrategy(new ArchiveByNumberStrategy),
	_pPurgeStrategy(new NullPurgeStrategy())
{
}


FileChannel::FileChannel(const std::string& path):
	_path(path),
	_times("utc"),
	_compress(false),
	_flush(false),
	_rotateOnOpen(false),
	_pFile(nullptr),
	_pRotateStrategy(new NullRotateStrategy()),
	_pArchiveStrategy(new ArchiveByNumberStrategy),
	_pPurgeStrategy(new NullPurgeStrategy())
{
}


FileChannel::~FileChannel()
{
	try
	{
		close();
		delete _pRotateStrategy;
		delete _pArchiveStrategy;
		delete _pPurgeStrategy;
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void FileChannel::open()
{
	FastMutex::ScopedLock lock(_mutex);

	if (!_pFile)
	{
		_pFile = new LogFile(_path);
		if (_rotateOnOpen && _pFile->size() > 0)
		{
			try
			{
				_pFile = _pArchiveStrategy->archive(_pFile);
				purge();
			}
			catch (...)
			{
				_pFile = new LogFile(_path);
			}
		}

		_pFile = _pArchiveStrategy->open(_pFile);
	}
}


void FileChannel::close()
{
	FastMutex::ScopedLock lock(_mutex);

	if (_pFile != nullptr)
		_pArchiveStrategy->close();

	delete _pFile;
	_pFile = nullptr;
}


void FileChannel::log(const Message& msg)
{
	open();

	FastMutex::ScopedLock lock(_mutex);

	if (_pRotateStrategy->mustRotate(_pFile))
	{
		try
		{
			_pFile = _pArchiveStrategy->archive(_pFile);
			purge();
		}
		catch (...)
		{
			_pFile = new LogFile(_path);
		}
		// we must call mustRotate() again to give the
		// RotateByIntervalStrategy a chance to write its timestamp
		// to the new file.
		_pRotateStrategy->mustRotate(_pFile);
	}
	_pFile->write(msg.getText(), _flush);
}


void FileChannel::setProperty(const std::string& name, const std::string& value)
{
	FastMutex::ScopedLock lock(_mutex);

	if (name == PROP_TIMES)
	{
		_times = value;

		if (!_rotation.empty())
			setRotation(_rotation);

		if (!_archive.empty())
			setArchive(_archive);
	}
	else if (name == PROP_PATH)
		_path = value;
	else if (name == PROP_ROTATION)
		setRotation(value);
	else if (name == PROP_ARCHIVE)
		setArchive(value);
	else if (name == PROP_COMPRESS)
		setCompress(value);
	else if (name == PROP_PURGEAGE)
		setPurgeAge(value);
	else if (name == PROP_PURGECOUNT)
		setPurgeCount(value);
	else if (name == PROP_FLUSH)
		setFlush(value);
	else if (name == PROP_ROTATEONOPEN)
		setRotateOnOpen(value);
	else
		Channel::setProperty(name, value);
}


std::string FileChannel::getProperty(const std::string& name) const
{
	if (name == PROP_TIMES)
		return _times;
	else if (name == PROP_PATH)
		return _path;
	else if (name == PROP_ROTATION)
		return _rotation;
	else if (name == PROP_ARCHIVE)
		return _archive;
	else if (name == PROP_COMPRESS)
		return std::string(_compress ? "true" : "false");
	else if (name == PROP_PURGEAGE)
		return _purgeAge;
	else if (name == PROP_PURGECOUNT)
		return _purgeCount;
	else if (name == PROP_FLUSH)
		return std::string(_flush ? "true" : "false");
	else if (name == PROP_ROTATEONOPEN)
		return std::string(_rotateOnOpen ? "true" : "false");
	else
		return Channel::getProperty(name);
}


Timestamp FileChannel::creationDate() const
{
	if (_pFile)
		return _pFile->creationDate();
	else
		return 0;
}


UInt64 FileChannel::size() const
{
	if (_pFile)
		return _pFile->size();
	else
		return 0;
}


const std::string& FileChannel::path() const
{
	return _path;
}


RotateStrategy* FileChannel::createRotationStrategy(const std::string& rotation, const std::string& times) const
{
	std::string::const_iterator it  = rotation.begin();
	std::string::const_iterator end = rotation.end();
	Poco::Int64 n = 0;
	while (it != end && Ascii::isSpace(*it)) ++it;
	while (it != end && Ascii::isDigit(*it)) { n *= 10; n += *it++ - '0'; }
	while (it != end && Ascii::isSpace(*it)) ++it;
	std::string unit;
	while (it != end && Ascii::isAlpha(*it)) unit += *it++;

	RotateStrategy* pStrategy = 0;
	if ((rotation.find(',') != std::string::npos) || (rotation.find(':') != std::string::npos))
	{
		if (times == "utc")
			pStrategy = new RotateAtTimeStrategy<DateTime>(rotation);
		else if (times == "local")
			pStrategy = new RotateAtTimeStrategy<LocalDateTime>(rotation);
		else
			throw PropertyNotSupportedException("times", times);
	}
	else if (unit == "daily")
		pStrategy = new RotateByIntervalStrategy(Timespan(1*Timespan::DAYS));
	else if (unit == "weekly")
		pStrategy = new RotateByIntervalStrategy(Timespan(7*Timespan::DAYS));
	else if (unit == "monthly")
		pStrategy = new RotateByIntervalStrategy(Timespan(30*Timespan::DAYS));
	else if (unit == "seconds") // for testing only
		pStrategy = new RotateByIntervalStrategy(Timespan(n*Timespan::SECONDS));
	else if (unit == "minutes")
		pStrategy = new RotateByIntervalStrategy(Timespan(n*Timespan::MINUTES));
	else if (unit == "hours")
		pStrategy = new RotateByIntervalStrategy(Timespan(n*Timespan::HOURS));
	else if (unit == "days")
		pStrategy = new RotateByIntervalStrategy(Timespan(n*Timespan::DAYS));
	else if (unit == "weeks")
		pStrategy = new RotateByIntervalStrategy(Timespan(n*7*Timespan::DAYS));
	else if (unit == "months")
		pStrategy = new RotateByIntervalStrategy(Timespan(n*30*Timespan::DAYS));
	else if (unit == "K")
		pStrategy = new RotateBySizeStrategy(n*1024);
	else if (unit == "M")
		pStrategy = new RotateBySizeStrategy(n*1024*1024);
	else if (unit.empty())
		pStrategy = new RotateBySizeStrategy(n);
	else if (unit == "never")
		pStrategy = new NullRotateStrategy();
	else
		throw InvalidArgumentException("rotation", rotation);

	return pStrategy;
}


void FileChannel::setRotationStrategy(RotateStrategy* strategy)
{
	poco_check_ptr(strategy);

	delete _pRotateStrategy;
	_pRotateStrategy = strategy;
}


void FileChannel::setRotation(const std::string& rotation)
{
	setRotationStrategy(createRotationStrategy(rotation, _times));
	_rotation = rotation;
}


ArchiveStrategy* FileChannel::createArchiveStrategy(const std::string& archive, const std::string& times) const
{
	ArchiveStrategy* pStrategy = nullptr;
	if (archive == "number")
	{
		pStrategy = new ArchiveByNumberStrategy;
	}
	else if (archive == "timestamp")
	{
		if (times == "utc")
			pStrategy = new ArchiveByTimestampStrategy<DateTime>;
		else if (times == "local")
			pStrategy = new ArchiveByTimestampStrategy<LocalDateTime>;
		else
			throw PropertyNotSupportedException("times", times);
	}
	else throw InvalidArgumentException("archive", archive);
	return pStrategy;
}


void FileChannel::setArchiveStrategy(ArchiveStrategy* strategy)
{
	poco_check_ptr(strategy);

	delete _pArchiveStrategy;
	_pArchiveStrategy = strategy;
}


void FileChannel::setArchive(const std::string& archive)
{
	ArchiveStrategy* pStrategy = nullptr;
	if (archive == "number")
	{
		pStrategy = new ArchiveByNumberStrategy;
	}
	else if (archive == "timestamp")
	{
		if (_times == "utc")
			pStrategy = new ArchiveByTimestampStrategy<DateTime>;
		else if (_times == "local")
			pStrategy = new ArchiveByTimestampStrategy<LocalDateTime>;
		else
			throw PropertyNotSupportedException("times", _times);
	}
	else throw InvalidArgumentException("archive", archive);
	delete _pArchiveStrategy;
	pStrategy->compress(_compress);
	_pArchiveStrategy = pStrategy;
	_archive = archive;
}


void FileChannel::setCompress(const std::string& compress)
{
	_compress = icompare(compress, "true") == 0;
	if (_pArchiveStrategy)
	_pArchiveStrategy->compress(_compress);
}


void FileChannel::setPurgeAge(const std::string& age)
{
	if (setNoPurge(age)) return;

	std::string::const_iterator nextToDigit;
	int num = extractDigit(age, &nextToDigit);
	Timespan::TimeDiff factor = extractFactor(age, nextToDigit);

	setPurgeStrategy(new PurgeByAgeStrategy(Timespan(num * factor)));
	_purgeAge = age;
}


void FileChannel::setPurgeCount(const std::string& count)
{
	if (setNoPurge(count)) return;

	setPurgeStrategy(new PurgeByCountStrategy(extractDigit(count)));
	_purgeCount = count;
}


void FileChannel::setFlush(const std::string& flush)
{
	_flush = icompare(flush, "true") == 0;
}


void FileChannel::setRotateOnOpen(const std::string& rotateOnOpen)
{
	_rotateOnOpen = icompare(rotateOnOpen, "true") == 0;
}


void FileChannel::purge()
{
	if (_pPurgeStrategy)
	{
	try
	{
		_pPurgeStrategy->purge(_path);
	}
	catch (...)
	{
	}
	}
}


bool FileChannel::setNoPurge(const std::string& value)
{
	if (value.empty() || 0 == icompare(value, "none"))
	{
		delete _pPurgeStrategy;
		_pPurgeStrategy = new NullPurgeStrategy();
		_purgeAge = "none";
		return true;
	}
	else return false;
}


int FileChannel::extractDigit(const std::string& value, std::string::const_iterator* nextToDigit) const
{
	std::string::const_iterator it  = value.begin();
	std::string::const_iterator end = value.end();
	int digit 						= 0;

	while (it != end && Ascii::isSpace(*it)) ++it;
	while (it != end && Ascii::isDigit(*it))
	{
		digit *= 10;
		digit += *it++ - '0';
	}

	if (digit == 0)
		throw InvalidArgumentException("Zero is not valid purge age.");

	if (nextToDigit) *nextToDigit = it;
	return digit;
}


void FileChannel::setPurgeStrategy(PurgeStrategy* strategy)
{
	poco_check_ptr(strategy);

	delete _pPurgeStrategy;
	_pPurgeStrategy = strategy;
}


Timespan::TimeDiff FileChannel::extractFactor(const std::string& value, std::string::const_iterator start) const
{
	while (start != value.end() && Ascii::isSpace(*start)) ++start;

	std::string unit;
	while (start != value.end() && Ascii::isAlpha(*start)) unit += *start++;

	if (unit == "seconds")
		return Timespan::SECONDS;
	if (unit == "minutes")
		return Timespan::MINUTES;
	else if (unit == "hours")
		return Timespan::HOURS;
	else if (unit == "days")
		return Timespan::DAYS;
	else if (unit == "weeks")
		return 7 * Timespan::DAYS;
	else if (unit == "months")
		return 30 * Timespan::DAYS;
	else throw InvalidArgumentException("purgeAge", value);

	return Timespan::TimeDiff();
}



} // namespace Poco
