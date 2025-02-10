//
// ArchiveStrategy.h
//
// Library: Foundation
// Package: Logging
// Module:  FileChannel
//
// Definition of the ArchiveStrategy class and subclasses.
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ArchiveStrategy_INCLUDED
#define Foundation_ArchiveStrategy_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/LogFile.h"
#include "Poco/File.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Mutex.h"
#include "Poco/Condition.h"
#include <atomic>


namespace Poco {


class ArchiveCompressor;


class Foundation_API ArchiveStrategy
	/// The ArchiveStrategy is used by FileChannel
	/// to rename a rotated log file for archiving.
	///
	/// Archived files can be automatically compressed,
	/// using the gzip file format.
{
public:
	ArchiveStrategy();
	virtual ~ArchiveStrategy();

	virtual LogFile* open(LogFile* pFile) = 0;
		/// Open a new log file and return it.

	virtual LogFile* archive(LogFile* pFile) = 0;
		/// Renames the given log file for archiving
		/// and creates and returns a new log file.
		/// The given LogFile object is deleted.

	void close();

	void compress(bool flag = true);
		/// Enables or disables compression of archived files.

protected:
	void moveFile(const std::string& oldName, const std::string& newName);
	bool exists(const std::string& name);

	Poco::FastMutex _rotateMutex;

	// Log rotation must wait until all of the compression tasks complete
	int _compressingCount;
	Poco::Condition _compressingComplete;

private:

	friend class ArchiveCompressor;

	ArchiveStrategy(const ArchiveStrategy&);
	ArchiveStrategy& operator = (const ArchiveStrategy&);

	void compressFile(const std::string& path);

	std::atomic<bool> _compress;
	std::atomic<ArchiveCompressor*> _pCompressor;
};


class Foundation_API ArchiveByNumberStrategy: public ArchiveStrategy
	/// A monotonic increasing number is appended to the
	/// log file name. The most recent archived file
	/// always has the number zero.
{
public:
	ArchiveByNumberStrategy();
	~ArchiveByNumberStrategy() override;

	LogFile *open(LogFile *pFile) override;
	LogFile *archive(LogFile *pFile) override;
};


template <class DT>
class ArchiveByTimestampStrategy: public ArchiveStrategy
	/// A timestamp (YYYYMMDDhhmmssiii) is appended to archived
	/// log files.
{
public:
	ArchiveByTimestampStrategy() = default;

	~ArchiveByTimestampStrategy() override = default;

	LogFile* open(LogFile* pFile) override
	{
		return pFile;
	}

	LogFile *archive(LogFile* pFile) override
	/// Archives the file by appending the current timestamp to the
	/// file name. If the new file name exists, additionally a monotonic
	/// increasing number is appended to the log file name.
	{
		std::string path = pFile->path();
		delete pFile;
		std::string archPath = path;
		archPath.append(".");
		DateTimeFormatter::append(archPath, DT().timestamp(), "%Y%m%d%H%M%S%i");

		if (exists(archPath)) archiveByNumber(archPath);
		else moveFile(path, archPath);

		return new LogFile(path);
	}

private:
	void archiveByNumber(const std::string& basePath)
		/// A monotonic increasing number is appended to the
		/// log file name. The most recent archived file
		/// always has the number zero.
	{
		int n = -1;
		std::string path;
		do
		{
			path = basePath;
			path.append(".");
			NumberFormatter::append(path, ++n);
		}
		while (exists(path));

		while (n >= 0)
		{
			std::string oldPath = basePath;
			if (n > 0)
			{
				oldPath.append(".");
				NumberFormatter::append(oldPath, n - 1);
			}
			std::string newPath = basePath;
			newPath.append(".");
			NumberFormatter::append(newPath, n);
			moveFile(oldPath, newPath);
			--n;
		}
	}
};


} // namespace Poco


#endif // Foundation_ArchiveStrategy_INCLUDED
