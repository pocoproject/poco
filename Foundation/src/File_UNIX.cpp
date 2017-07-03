//
// File_UNIX.cpp
//
// $Id: //poco/1.4/Foundation/src/File_UNIX.cpp#1 $
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


#include "Poco/File_UNIX.h"
#include "Poco/Buffer.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#if defined(POCO_OS_FAMILY_BSD)
#include <sys/param.h>
#include <sys/mount.h>
#else
#include <sys/statfs.h>
#endif
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>

namespace{
// Convert timespec structures (seconds and remaining nano secs) to TimeVal (microseconds)
Poco::Timestamp::TimeVal timespec2Microsecs(const struct timespec &ts) {
	return ts.tv_sec * 1000000L + ts.tv_nsec / 1000L;
}
} // namespace


namespace Poco {


FileImpl::FileImpl()
{
}


FileImpl::FileImpl(const std::string& path): _path(path)
{
	std::string::size_type n = _path.size();
	if (n > 1 && _path[n - 1] == '/')
		_path.resize(n - 1);
}


FileImpl::~FileImpl()
{
}


void FileImpl::swapImpl(FileImpl& file)
{
	std::swap(_path, file._path);
}


void FileImpl::setPathImpl(const std::string& path)
{
	_path = path;
	std::string::size_type n = _path.size();
	if (n > 1 && _path[n - 1] == '/')
		_path.resize(n - 1);
}


bool FileImpl::existsImpl() const
{
	poco_assert (!_path.empty());

	struct stat st;
	return stat(_path.c_str(), &st) == 0;
}


bool FileImpl::canReadImpl() const
{
	poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid())
			return (st.st_mode & S_IRUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IRGRP) != 0;
		else
			return (st.st_mode & S_IROTH) != 0 || geteuid() == 0;
	}
	else handleLastErrorImpl(_path);
	return false;
}


bool FileImpl::canWriteImpl() const
{
	poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid())
			return (st.st_mode & S_IWUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IWGRP) != 0;
		else
			return (st.st_mode & S_IWOTH) != 0 || geteuid() == 0;
	}
	else handleLastErrorImpl(_path);
	return false;
}


bool FileImpl::canExecuteImpl() const
{
	poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid() || geteuid() == 0)
			return (st.st_mode & S_IXUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IXGRP) != 0;
		else
			return (st.st_mode & S_IXOTH) != 0;
	}
	else handleLastErrorImpl(_path);
	return false;
}


bool FileImpl::isFileImpl() const
{
	poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISREG(st.st_mode);
	else
		handleLastErrorImpl(_path);
	return false;
}


bool FileImpl::isDirectoryImpl() const
{
	poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISDIR(st.st_mode);
	else
		handleLastErrorImpl(_path);
	return false;
}


bool FileImpl::isLinkImpl() const
{
	poco_assert (!_path.empty());

	struct stat st;
	if (lstat(_path.c_str(), &st) == 0)
		return S_ISLNK(st.st_mode);
	else
		handleLastErrorImpl(_path);
	return false;
}


bool FileImpl::isDeviceImpl() const
{
	poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode);
	else
		handleLastErrorImpl(_path);
	return false;
}


bool FileImpl::isHiddenImpl() const
{
	poco_assert (!_path.empty());
	Path p(_path);
	p.makeFile();

	return p.getFileName()[0] == '.';
}


Timestamp FileImpl::createdImpl() const
{
	poco_assert (!_path.empty());
// first, platforms with birthtime attributes
#if defined(__APPLE__) && defined(st_birthtime) && !defined(POCO_NO_STAT64) // st_birthtime is available only on 10.5
	// a macro st_birthtime makes sure there is a st_birthtimespec (nano sec precision)
	struct stat64 st;
	if (stat64(_path.c_str(), &st) == 0)
		return Timestamp(timespec2Microsecs(st.st_birthtimespec);
#elif defined(__FreeBSD__) && defined(st_birthtime)
	// a macro st_birthtime makes sure there is a st_birthtimespec (nano sec precision)
	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp(timespec2Microsecs(st.st_birthtimespec));
#elif defined(__FreeBSD__)
	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_birthtime);
// then platforms with POSIX 2008-09 compatibility (nanosec precision)
// (linux 2.6 and later)
#elif (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L) \
	|| (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700)
	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp(timespec2Microsecs(st.st_ctim));
// finally try just stat with status change with precision to the second.
#else
	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_ctime);
#endif 
	else
		handleLastErrorImpl(_path);
	return 0;
}


Timestamp FileImpl::getLastModifiedImpl() const
{
	poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0) {
#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L) \
	|| (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700) \
	|| defined(_BSD_SOURCE) || defined(_SVID_SOURCE)
		return Timestamp(timespec2Microsecs(st.st_mtim));
#elif defined(__APPLE__)
		return Timestamp(timespec2Microsecs(st.st_mtimespec));
#else
		return Timestamp::fromEpochTime(st.st_mtime);
#endif
	}
	else
		handleLastErrorImpl(_path);
	return 0;
}


void FileImpl::setLastModifiedImpl(const Timestamp& ts)
{
	poco_assert (!_path.empty());

	struct timeval tb[2];
	tb[0].tv_sec  = ts.epochMicroseconds() / 1000000;
	tb[0].tv_usec  = ts.epochMicroseconds() % 1000000;
	tb[1] = tb[0];
	if (utimes(_path.c_str(), tb) != 0)
		handleLastErrorImpl(_path);
}


FileImpl::FileSizeImpl FileImpl::getSizeImpl() const
{
	poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return st.st_size;
	else
		handleLastErrorImpl(_path);
	return 0;
}


void FileImpl::setSizeImpl(FileSizeImpl size)
{
	poco_assert (!_path.empty());

	if (truncate(_path.c_str(), size) != 0)
		handleLastErrorImpl(_path);
}


void FileImpl::setWriteableImpl(bool flag)
{
	poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) != 0) 
		handleLastErrorImpl(_path);
	mode_t mode;
	if (flag)
	{
		mode = st.st_mode | S_IWUSR;
	}
	else
	{
		mode_t wmask = S_IWUSR | S_IWGRP | S_IWOTH;
		mode = st.st_mode & ~wmask;
	}
	if (chmod(_path.c_str(), mode) != 0) 
		handleLastErrorImpl(_path);
}


void FileImpl::setExecutableImpl(bool flag)
{
	poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) != 0) 
		handleLastErrorImpl(_path);
	mode_t mode;
	if (flag)
	{
		mode = st.st_mode | S_IXUSR;
	}
	else
	{
		mode_t wmask = S_IXUSR | S_IXGRP | S_IXOTH;
		mode = st.st_mode & ~wmask;
	}
	if (chmod(_path.c_str(), mode) != 0) 
		handleLastErrorImpl(_path);
}


void FileImpl::copyToImpl(const std::string& path) const
{
	poco_assert (!_path.empty());

	int sd = open(_path.c_str(), O_RDONLY);
	if (sd == -1) handleLastErrorImpl(_path);

	struct stat st;
	if (fstat(sd, &st) != 0) 
	{
		close(sd);
		handleLastErrorImpl(_path);
	}
	const long blockSize = st.st_blksize;

	int dd = open(path.c_str(), O_CREAT | O_TRUNC | O_WRONLY, st.st_mode);
	if (dd == -1)
	{
		close(sd);
		handleLastErrorImpl(path);
	}
	Buffer<char> buffer(blockSize);
	try
	{
		int n;
		while ((n = read(sd, buffer.begin(), blockSize)) > 0)
		{
			if (write(dd, buffer.begin(), n) != n) 
				handleLastErrorImpl(path);
		}
		if (n < 0)
			handleLastErrorImpl(_path);
	}
	catch (...)
	{
		close(sd);
		close(dd);
		throw;
	}
	close(sd);
	if (fsync(dd) != 0) 
	{
		close(dd);
		handleLastErrorImpl(path);
	}
	if (close(dd) != 0)
		handleLastErrorImpl(path);
}


void FileImpl::renameToImpl(const std::string& path)
{
	poco_assert (!_path.empty());

	if (rename(_path.c_str(), path.c_str()) != 0)
		handleLastErrorImpl(_path);
}


void FileImpl::removeImpl()
{
	poco_assert (!_path.empty());

	int rc;
	if (!isLinkImpl() && isDirectoryImpl())
		rc = rmdir(_path.c_str());
	else
		rc = unlink(_path.c_str());
	if (rc) handleLastErrorImpl(_path);
}


bool FileImpl::createFileImpl()
{
	poco_assert (!_path.empty());
	
	int n = open(_path.c_str(), O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (n != -1)
	{
		close(n);
		return true;
	}
	if (n == -1 && errno == EEXIST)
		return false;
	else
		handleLastErrorImpl(_path);
	return false;
}


bool FileImpl::createDirectoryImpl()
{
	poco_assert (!_path.empty());

	if (existsImpl() && isDirectoryImpl())
		return false;
	if (mkdir(_path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0) 
		handleLastErrorImpl(_path);
	return true;
}


FileImpl::FileSizeImpl FileImpl::totalSpaceImpl() const
{
	poco_assert(!_path.empty());

	struct statfs stats;
	if (statfs(const_cast<char*>(_path.c_str()), &stats) != 0)
		handleLastErrorImpl(_path);

	return (FileSizeImpl)stats.f_blocks * (FileSizeImpl)stats.f_bsize;
}


FileImpl::FileSizeImpl FileImpl::usableSpaceImpl() const
{
	poco_assert(!_path.empty());

	struct statfs stats;
	if (statfs(const_cast<char*>(_path.c_str()), &stats) != 0)
		handleLastErrorImpl(_path);

	return (FileSizeImpl)stats.f_bavail * (FileSizeImpl)stats.f_bsize;
}


FileImpl::FileSizeImpl FileImpl::freeSpaceImpl() const
{
	poco_assert(!_path.empty());

	struct statfs stats;
	if (statfs(const_cast<char*>(_path.c_str()), &stats) != 0)
		handleLastErrorImpl(_path);

	return (FileSizeImpl)stats.f_bfree * (FileSizeImpl)stats.f_bsize;
}


void FileImpl::handleLastErrorImpl(const std::string& path)
{
	switch (errno)
	{
	case EIO:
		throw IOException(path, errno);
	case EPERM:
		throw FileAccessDeniedException("insufficient permissions", path, errno);
	case EACCES:
		throw FileAccessDeniedException(path, errno);
	case ENOENT:
		throw FileNotFoundException(path, errno);
	case ENOTDIR:
		throw OpenFileException("not a directory", path, errno);
	case EISDIR:
		throw OpenFileException("not a file", path, errno);
	case EROFS:
		throw FileReadOnlyException(path, errno);
	case EEXIST:
		throw FileExistsException(path, errno);
	case ENOSPC:
		throw FileException("no space left on device", path, errno);
	case EDQUOT:
		throw FileException("disk quota exceeded", path, errno);
#if !defined(_AIX)
	case ENOTEMPTY:
		throw DirectoryNotEmptyException(path, errno);
#endif
	case ENAMETOOLONG:
		throw PathSyntaxException(path, errno);
	case ENFILE:
	case EMFILE:
		throw FileException("too many open files", path, errno);
	default:
		throw FileException(std::strerror(errno), path, errno);
	}
}


} // namespace Poco
