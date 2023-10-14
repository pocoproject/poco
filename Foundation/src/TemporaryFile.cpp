//
// TemporaryFile.cpp
//
// Library: Foundation
// Package: Filesystem
// Module:  TemporaryFile
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/TemporaryFile.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"
#if !defined(POCO_VXWORKS)
#include "Poco/Process.h"
#endif
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include <algorithm>
#include <random>
#include <set>
#include <sstream>


namespace Poco {


class TempFileCollector
{
public:
	TempFileCollector()
	{
	}

	~TempFileCollector()
	{
		try
		{
			for (auto& f: _files)
			{
				try
				{
					File file(f);
					if (file.exists())
						file.remove(true);
				}
				catch (Exception&)
				{
				}
			}
		}
		catch (...)
		{
			poco_unexpected();
		}
	}

	void registerFile(const std::string& path)
	{
		FastMutex::ScopedLock lock(_mutex);

		Path p(path);
		_files.insert(p.absolute().toString());
	}

private:
	std::set<std::string> _files;
	FastMutex _mutex;
};


TemporaryFile::TemporaryFile():
	File(tempName()),
	_keep(false)
{
}


TemporaryFile::TemporaryFile(const std::string& tempDir):
	File(tempName(tempDir)),
	_keep(false)
{
}


TemporaryFile::~TemporaryFile()
{
	try
	{
		if (!_keep)
		{
			try
			{
				if (exists())
					remove(true);
			}
			catch (Exception&)
			{
			}
		}
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void TemporaryFile::keep()
{
	_keep = true;
}


void TemporaryFile::keepUntilExit()
{
	_keep = true;
	registerForDeletion(path());
}


namespace
{
	static TempFileCollector fc;
}


void TemporaryFile::registerForDeletion(const std::string& path)
{
	fc.registerFile(path);
}


std::string TemporaryFile::tempName(const std::string& tempDir)
{
	static constexpr int UNIQUE_LENGTH = 8;
	static FastMutex mutex;
	static unsigned long count = 0;
	static Poco::Random random;
	static std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
	static std::string randomChars;

	unsigned long n;
	{
		Poco::FastMutex::ScopedLock lock(mutex);

		if (count == 0) 
		{
			random.seed();
			
			std::random_device rd;
			std::mt19937 mt(rd());
			randomChars.reserve(alphabet.size() * UNIQUE_LENGTH);
			for (int i = 0; i < UNIQUE_LENGTH; i++)
			{
				std::shuffle(alphabet.begin(), alphabet.end(), mt);
				randomChars.append(alphabet);
			}
		}
		n = (count += random.next(1000) + 1);
	}

	std::ostringstream name;
	name << (tempDir.empty() ? Path::temp() : tempDir);
	if (name.str().at(name.str().size() - 1) != Path::separator())
	{
		name << Path::separator();
	}
#if defined(POCO_VXWORKS)
	name << "tmp";
#else
	name << "tmp" << Process::id();
#endif
	for (int i = 0; i < UNIQUE_LENGTH; i++)
	{
		name << randomChars[alphabet.size()*i + (n % 26)];
		n /= 26;
	}
	return name.str();
}


} // namespace Poco
