//
// File.cpp
//
// Library: Foundation
// Package: Filesystem
// Module:  File
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Environment.h"
#include "Poco/StringTokenizer.h"


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "File_WIN32U.cpp"
#elif defined(POCO_VXWORKS)
#include "File_VX.cpp"
#elif defined(POCO_OS_FAMILY_UNIX)
#include "File_UNIX.cpp"
#endif
#include "Poco/Thread.h"


namespace Poco {


File::File()
{
}


File::File(const std::string& path): FileImpl(path)
{
}


File::File(const char* path): FileImpl(std::string(path))
{
}


File::File(const Path& path): FileImpl(path.toString())
{
}


File::File(const File& file): FileImpl(file.getPathImpl())
{
}


File::~File()
{
}


File& File::operator = (const File& file)
{
	setPathImpl(file.getPathImpl());
	return *this;
}


File& File::operator = (const std::string& path)
{
	setPathImpl(path);
	return *this;
}


File& File::operator = (const char* path)
{
	poco_check_ptr (path);
	setPathImpl(path);
	return *this;
}


File& File::operator = (const Path& path)
{
	setPathImpl(path.toString());
	return *this;
}


void File::swap(File& file) noexcept
{
	swapImpl(file);
}


std::string File::absolutePath() const
{
	std::string ret;

	if (Path(path()).isAbsolute())
		// TODO: Should this return empty string if file does not exists to be consistent
		// with the function documentation?
		ret = getPathImpl();
	else
	{
		Path curPath(Path::current());
		curPath.append(path());
		if (File(curPath).exists())
			ret = curPath.toString();
		else
		{
			const std::string envPath = Environment::get("PATH", "");
			const std::string pathSeparator(1, Path::pathSeparator());
			if (!envPath.empty())
			{
				const StringTokenizer st(envPath, pathSeparator,
					StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);

				for (const auto& p: st)
				{
					try
					{
						std::string fileName(p);
						if (p.size() && p.back() != Path::separator())
							fileName.append(1, Path::separator());
						fileName.append(path());
						if (File(fileName).exists())
						{
							ret = fileName;
							break;
						}
					}
					catch (const Poco::PathSyntaxException&)
					{
						// shield against bad PATH environment entries
					}
				}
			}
		}
	}

	return ret;
}


bool File::exists() const
{
	if (path().empty()) return false;
	return existsImpl();
}


bool File::existsAnywhere() const
{
	if (path().empty()) return false;

	if (Path(path()).isAbsolute())
		return existsImpl();

	if (File(absolutePath()).exists())
		return true;

	return false;
}


bool File::canRead() const
{
	return canReadImpl();
}


bool File::canWrite() const
{
	return canWriteImpl();
}


bool File::canExecute() const
{
	// Resolve (platform-specific) executable path and absolute path from relative.
	const auto execPath { getExecutablePathImpl() };
	const auto absPath { File(execPath).absolutePath() };
	if (absPath.empty() || !File(absPath).exists())
	{
		return false;
	}
	return canExecuteImpl(absPath);
}


bool File::isFile() const
{
	return isFileImpl();
}


bool File::isDirectory() const
{
	return isDirectoryImpl();
}


bool File::isLink() const
{
	return isLinkImpl();
}


bool File::isDevice() const
{
	return isDeviceImpl();
}


bool File::isHidden() const
{
	return isHiddenImpl();
}


Timestamp File::created() const
{
	return createdImpl();
}


Timestamp File::getLastModified() const
{
	return getLastModifiedImpl();
}


File& File::setLastModified(const Timestamp& ts)
{
	setLastModifiedImpl(ts);
	return *this;
}


File::FileSize File::getSize() const
{
	return getSizeImpl();
}


File& File::setSize(FileSizeImpl size)
{
	setSizeImpl(size);
	return *this;
}


File& File::setWriteable(bool flag)
{
	setWriteableImpl(flag);
	return *this;
}


File& File::setReadOnly(bool flag)
{
	setWriteableImpl(!flag);
	return *this;
}


File& File::setExecutable(bool flag)
{
	setExecutableImpl(flag);
	return *this;
}


void File::copyTo(const std::string& path, int options) const
{
	Path src(getPathImpl());
	Path dest(path);
	File destFile(path);
	if ((destFile.exists() && destFile.isDirectory()) || dest.isDirectory())
	{
		dest.makeDirectory();
		dest.setFileName(src.getFileName());
	}
	if (isDirectory())
		copyDirectory(dest.toString(), options);
	else
		copyToImpl(dest.toString(), options);
}


void File::copyDirectory(const std::string& path, int options) const
{
	File target(path);
	target.createDirectories();

	Path src(getPathImpl());
	src.makeFile();
	DirectoryIterator it(src);
	DirectoryIterator end;
	for (; it != end; ++it)
	{
		it->copyTo(path, options);
	}
}


void File::moveTo(const std::string& path, int options)
{
	copyTo(path, options);
	remove(true);
	setPathImpl(path);
}


void File::renameTo(const std::string& path, int options)
{
	renameToImpl(path, options);
	setPathImpl(path);
}


void File::linkTo(const std::string& path, LinkType type) const
{
	linkToImpl(path, type);
}


void File::remove(bool recursive)
{
	if (recursive && !isLink() && isDirectory())
	{
		std::vector<File> files;
		list(files);
		for (auto& f: files)
		{
			f.remove(true);
		}

		// Note: On Windows, removing a directory may not succeed at first
		// try because deleting files is not a synchronous operation. Files
		// are merely marked as deleted, and actually removed at a later time.
		//
		// An alternate strategy would be moving files to a different directory
		// first (on the same drive, but outside the deleted tree), and marking
		// them as hidden, before deleting them, but this could lead to other issues.
		// So we simply retry after some time until we succeed, or give up.

		int retry = 8;
		long sleep = 10;
		while (retry > 0)
		{
			try
			{
				removeImpl();
				retry = 0;
			}
			catch (DirectoryNotEmptyException&)
			{
				if (--retry == 0) throw;
				Poco::Thread::sleep(sleep);
				sleep *= 2;
			}
		}
	}
	else
	{
		removeImpl();
	}
}


bool File::createFile()
{
	return createFileImpl();
}


bool File::createDirectory()
{
	return createDirectoryImpl();
}


void File::createDirectories()
{
	if (!exists())
	{
		Path p(getPathImpl());
		p.makeDirectory();
		if (p.depth() > 1)
		{
			p.makeParent();
			File f(p);
			f.createDirectories();
		}
		try
		{
			createDirectoryImpl();
		}
		catch (FileExistsException&)
		{
		}
	}
}


void File::list(std::vector<std::string>& files) const
{
	files.clear();
	DirectoryIterator it(*this);
	DirectoryIterator end;
	while (it != end)
	{
		files.push_back(it.name());
		++it;
	}
}


File::FileSize File::totalSpace() const
{
	return totalSpaceImpl();
}


File::FileSize File::usableSpace() const
{
	return usableSpaceImpl();
}


File::FileSize File::freeSpace() const
{
	return freeSpaceImpl();
}


void File::list(std::vector<File>& files) const
{
	files.clear();
	DirectoryIterator it(*this);
	DirectoryIterator end;
	while (it != end)
	{
		files.push_back(*it);
		++it;
	}
}


void File::handleLastError(const std::string& path)
{
	handleLastErrorImpl(path);
}


} // namespace Poco
