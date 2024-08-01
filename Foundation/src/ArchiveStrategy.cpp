//
// ArchiveStrategy.cpp
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


#include "Poco/ArchiveStrategy.h"
#include "Poco/NumberFormatter.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/DeflatingStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"
#include "Poco/ActiveDispatcher.h"
#include "Poco/ActiveMethod.h"
#include "Poco/Void.h"
#include "Poco/FileStream.h"

#include <string_view>

namespace Poco {


//
// ArchiveCompressor
//


class ArchiveCompressor: public ActiveDispatcher
{
public:
	ArchiveCompressor():
		compress(this, &ArchiveCompressor::compressImpl)
	{
	}

	~ArchiveCompressor()
	{
	}

	struct ArchiveToCompress
	{
		ArchiveStrategy* as;
		std::string path;
	};

	ActiveMethod<void, ArchiveToCompress, ArchiveCompressor, ActiveStarter<ActiveDispatcher>> compress;

protected:
	void compressImpl(const ArchiveToCompress& ac)
	{
		ac.as->compressFile(ac.path);
	}
};


//
// ArchiveStrategy
//

// Prefix that is added to the file being compressed to be skipped by the
// purge strategy.
static const std::string compressFilePrefix ( ".~" );


ArchiveStrategy::ArchiveStrategy():
	_compressingCount(0),
	_compress(false),
	_pCompressor(nullptr)
{
}


ArchiveStrategy::~ArchiveStrategy()
{
	try
	{
		close();
	}
	catch(...)
	{
		poco_unexpected();
	}
}


void ArchiveStrategy::close()
{
	FastMutex::ScopedLock l(_rotateMutex);

	while (_compressingCount > 0)
		_compressingComplete.wait(_rotateMutex, 1000);

	delete _pCompressor;
	_pCompressor = nullptr;
}


void ArchiveStrategy::compress(bool flag)
{
	_compress = flag;
}


void ArchiveStrategy::moveFile(const std::string& oldPath, const std::string& newPath)
{
	bool compressed = false;
	const Path p(oldPath);
	File f(oldPath);
	if (!f.exists())
	{
		f = oldPath + ".gz";
		compressed = true;
	}
	std::string mvPath(newPath);
	if (_compress || compressed)
		mvPath.append(".gz");

	if (!_compress || compressed)
	{
		f.renameTo(mvPath);
	}
	else
	{
		_compressingCount++;
		Path logdir { newPath };
		logdir.makeParent();
		const auto logfile { Path(newPath).getFileName() };
		const auto compressPath = logdir.append(compressFilePrefix + logfile).toString();
		f.renameTo(compressPath);
		if (!_pCompressor)
			_pCompressor = new ArchiveCompressor;

		_pCompressor.load()->compress( {this, compressPath} );
	}
}


bool ArchiveStrategy::exists(const std::string& name)
{
	File f(name);
	if (f.exists())
	{
		return true;
	}
	else if (_compress)
	{
		std::string gzName(name);
		gzName.append(".gz");
		File gzf(gzName);
		return gzf.exists();
	}
	else return false;
}


void ArchiveStrategy::compressFile(const std::string& path)
{
	FastMutex::ScopedLock l(_rotateMutex);

	Path logdir { path };
	logdir.makeParent();

	auto removeFilePrefix = [&logdir](const std::string& path, const std::string& prefix) -> std::string
	{
		auto fname { Path(path).getFileName() };
		const std::string_view fprefix(fname.data(), prefix.size());
		if (fprefix == prefix)
			return Path(logdir, fname.substr(prefix.size())).toString();

		return path;
	};

	File f(path);
	std::string gzPath(path);
	gzPath.append(".gz");
	FileInputStream istr(path);
	FileOutputStream ostr(gzPath);
	try
	{
		DeflatingOutputStream deflater(ostr, DeflatingStreamBuf::STREAM_GZIP);
		StreamCopier::copyStream(istr, deflater);
		if (!deflater.good() || !ostr.good())
			throw WriteFileException(gzPath);

		deflater.close();
		ostr.close();
		istr.close();

		// Remove temporary prefix and set modification time to
		// the time of the uncompressed file for purge strategy to work correctly
		File zf(gzPath);
		zf.renameTo(removeFilePrefix(gzPath, compressFilePrefix));
		zf.setLastModified(f.getLastModified());
	}
	catch (const Poco::Exception&)
	{
		// deflating failed - remove gz file and leave uncompressed log file
		ostr.close();
		Poco::File gzf(gzPath);
		gzf.remove();

		f.renameTo(removeFilePrefix(path, compressFilePrefix));
	}
	f.remove();

	_compressingCount--;
	if (_compressingCount < 1)
		_compressingComplete.broadcast();
}


//
// ArchiveByNumberStrategy
//


ArchiveByNumberStrategy::ArchiveByNumberStrategy()
{
}


ArchiveByNumberStrategy::~ArchiveByNumberStrategy()
{
}


LogFile* ArchiveByNumberStrategy::open(LogFile* pFile)
{
	return pFile;
}


LogFile* ArchiveByNumberStrategy::archive(LogFile* pFile)
{
	FastMutex::ScopedLock l(_rotateMutex);

	while (_compressingCount > 0)
		_compressingComplete.wait(_rotateMutex, 1000);

	std::string basePath = pFile->path();
	delete pFile;
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
	return new LogFile(basePath);
}


} // namespace Poco
