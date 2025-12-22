//
// PurgeStrategy.cpp
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


#include "Poco/PurgeStrategy.h"
#include "Poco/Path.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Timestamp.h"
#include <algorithm>


namespace Poco {


//
// PurgeStrategy
//


PurgeStrategy::PurgeStrategy()
{
}


PurgeStrategy::~PurgeStrategy()
{
}


void PurgeStrategy::list(const std::string& path, std::vector<File>& files)
{
	Path p(path);
	p.makeAbsolute();
	Path parent = p.parent();
	std::string baseName = p.getFileName();
	baseName.append(".");

	DirectoryIterator it(parent);
	DirectoryIterator end;
	while (it != end)
	{
		if (it.name().compare(0, baseName.size(), baseName) == 0)
		{
			files.push_back(*it);
		}
		++it;
	}
}


//
// NullPurgeStrategy
// 


NullPurgeStrategy::NullPurgeStrategy()
{
}


NullPurgeStrategy::~NullPurgeStrategy()
{
}


void NullPurgeStrategy::purge(const std::string& path)
{
}


//
// PurgeByAgeStrategy
//


PurgeByAgeStrategy::PurgeByAgeStrategy(const Timespan& age): _age(age)
{
}


PurgeByAgeStrategy::~PurgeByAgeStrategy()
{
}


void PurgeByAgeStrategy::purge(const std::string& path)
{
	std::vector<File> files;
	list(path, files);
	for (auto& f: files)
	{
		if (f.getLastModified().isElapsed(_age.totalMicroseconds()))
		{
			f.remove();
		}
	}
}


//
// PurgeByCountStrategy
//


PurgeByCountStrategy::PurgeByCountStrategy(int count): _count(count)
{
	poco_assert(count > 0);
}


PurgeByCountStrategy::~PurgeByCountStrategy()
{
}


void PurgeByCountStrategy::purge(const std::string& path)
{
	std::vector<File> files;
	list(path, files);

	// Order files in ascending name order. Files with largest
	// sequence number will be deleted in case that multiple files
	// have the same modification time.
	std::sort (files.begin(), files.end(),
		[](const Poco::File& a, const Poco::File& b) { return a.path() < b.path(); }
	);

	while (files.size() > _count)
	{
		std::vector<File>::iterator it = files.begin();
		std::vector<File>::iterator purgeIt = it;
		Timestamp purgeTS = purgeIt->getLastModified();
		++it;
		while (it != files.end())
		{
			Timestamp md(it->getLastModified());
			if (md <= purgeTS)
			{
				purgeTS = md;
				purgeIt = it;
			}
			++it;
		}
		purgeIt->remove();
		files.erase(purgeIt);
	}
}


} // namespace Poco
