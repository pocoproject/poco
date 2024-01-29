//
// File_UNIX.cpp
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
#include "Poco/Error.h"
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#if defined(POCO_OS_FAMILY_BSD)
#include <sys/param.h>
#include <sys/mount.h>
#elif (POCO_OS == POCO_OS_SOLARIS) || (POCO_OS == POCO_OS_QNX)
#include <sys/statvfs.h>
#else
#include <sys/statfs.h>
#endif
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <utime.h>
#include <cstring>

#if (POCO_OS == POCO_OS_SOLARIS) || (POCO_OS == POCO_OS_QNX)
#define STATFSFN statvfs
#define STATFSSTRUCT statvfs
#else
#define STATFSFN statfs
#define STATFSSTRUCT statfs
#endif


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

	using TV = Timestamp::TimeVal;

	// Nanosecond to timestamp resolution factor
	static constexpr TV nsk = 1'000'000'000ll / Timestamp::resolution();

	struct stat st;
	if (::stat(_path.c_str(), &st) == 0)
	{
#if defined(__FreeBSD__) || (defined(__APPLE__) && defined(_DARWIN_FEATURE_64_BIT_INODE))
		const TV tv = static_cast<TV>(st.st_birthtimespec.tv_sec) * Timestamp::resolution() + st.st_birthtimespec.tv_nsec/nsk;
		return Timestamp(tv);
#elif POCO_OS == POCO_OS_LINUX
		const TV tv = static_cast<TV>(st.st_ctim.tv_sec) * Timestamp::resolution() + st.st_ctim.tv_nsec/nsk;
		return Timestamp(tv);
#else
		return Timestamp::fromEpochTime(st.st_ctime);
#endif
	}
	else
		handleLastErrorImpl(_path);
	return 0;
}


Timestamp FileImpl::getLastModifiedImpl() const
{
	poco_assert (!_path.empty());

	using TV = Timestamp::TimeVal;

	// Nanosecond to timestamp resolution factor
	static constexpr TV nsk = 1'000'000'000ll / Timestamp::resolution();

	struct stat st;
	if (::stat(_path.c_str(), &st) == 0)
	{
#if defined(__FreeBSD__) || (defined(__APPLE__) && defined(_DARWIN_FEATURE_64_BIT_INODE))
		const TV tv = static_cast<TV>(st.st_mtimespec.tv_sec) * Timestamp::resolution() + st.st_mtimespec.tv_nsec/nsk;
		return Timestamp(tv);
#elif POCO_OS == POCO_OS_LINUX
		const TV tv = static_cast<TV>(st.st_mtim.tv_sec) * Timestamp::resolution() + st.st_mtim.tv_nsec/nsk;
		return Timestamp(tv);
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

	const ::time_t s = ts.epochTime();
	const ::suseconds_t us = ts.epochMicroseconds() % 1'000'000;
	const ::timeval times[2] = { {s, us}, {s, us} };

	if (::utimes(_path.c_str(), times) != 0)
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
		if (st.st_mode & S_IRGRP)
			mode |= S_IXGRP;
		if (st.st_mode & S_IROTH)
			mode |= S_IXOTH;
	}
	else
	{
		mode_t wmask = S_IXUSR | S_IXGRP | S_IXOTH;
		mode = st.st_mode & ~wmask;
	}
	if (chmod(_path.c_str(), mode) != 0)
		handleLastErrorImpl(_path);
}


void FileImpl::copyToImpl(const std::string& path, int options) const
{
	poco_assert (!_path.empty());

	int sd = open(_path.c_str(), O_RDONLY);
	if (sd == -1) handleLastErrorImpl(_path);

	struct stat st;
	if (fstat(sd, &st) != 0)
	{
		int err = errno;
		close(sd);
		handleLastErrorImpl(err, _path);
	}
	const long blockSize = st.st_blksize;
	int dd;
	if (options & OPT_FAIL_ON_OVERWRITE_IMPL) {
		dd = open(path.c_str(), O_CREAT | O_TRUNC | O_EXCL | O_WRONLY, st.st_mode);
	} else {
		dd = open(path.c_str(), O_CREAT | O_TRUNC | O_WRONLY, st.st_mode);
	}
	if (dd == -1)
	{
		int err = errno;
		close(sd);
		handleLastErrorImpl(err, path);
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
		{
			handleLastErrorImpl(_path);
		}
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
		int err = errno;
		close(dd);
		handleLastErrorImpl(err, path);
	}
	if (close(dd) != 0)
	{
		handleLastErrorImpl(path);
	}
}


void FileImpl::renameToImpl(const std::string& path, int options)
{
	poco_assert (!_path.empty());

	struct stat st;

	if (stat(path.c_str(), &st) == 0 && (options & OPT_FAIL_ON_OVERWRITE_IMPL))
		throw FileExistsException(path, EEXIST);

	if (rename(_path.c_str(), path.c_str()) != 0)
		handleLastErrorImpl(_path);
}


void FileImpl::linkToImpl(const std::string& path, int type) const
{
	poco_assert (!_path.empty());

	if (type == 0)
	{
		if (link(_path.c_str(), path.c_str()) != 0)
			handleLastErrorImpl(_path);
	}
	else
	{
		if (symlink(_path.c_str(), path.c_str()) != 0)
			handleLastErrorImpl(_path);
	}
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

	struct STATFSSTRUCT stats;
	if (STATFSFN(const_cast<char*>(_path.c_str()), &stats) != 0)
		handleLastErrorImpl(_path);

	return (FileSizeImpl)stats.f_blocks * (FileSizeImpl)stats.f_bsize;
}


FileImpl::FileSizeImpl FileImpl::usableSpaceImpl() const
{
	poco_assert(!_path.empty());

	struct STATFSSTRUCT stats;
	if (STATFSFN(const_cast<char*>(_path.c_str()), &stats) != 0)
		handleLastErrorImpl(_path);

	return (FileSizeImpl)stats.f_bavail * (FileSizeImpl)stats.f_bsize;
}


FileImpl::FileSizeImpl FileImpl::freeSpaceImpl() const
{
	poco_assert(!_path.empty());

	struct STATFSSTRUCT stats;
	if (STATFSFN(const_cast<char*>(_path.c_str()), &stats) != 0)
		handleLastErrorImpl(_path);

	return (FileSizeImpl)stats.f_bfree * (FileSizeImpl)stats.f_bsize;
}


void FileImpl::handleLastErrorImpl(int err, const std::string& path)
{
	switch (err)
	{
	case EIO:
		throw IOException(path, err);
	case EPERM:
		throw FileAccessDeniedException("insufficient permissions", path, err);
	case EACCES:
		throw FileAccessDeniedException(path, err);
	case ENOENT:
		throw FileNotFoundException(path, err);
	case ENOTDIR:
		throw OpenFileException("not a directory", path, err);
	case EISDIR:
		throw OpenFileException("not a file", path, err);
	case EROFS:
		throw FileReadOnlyException(path, err);
	case EEXIST:
		throw FileExistsException(path, err);
	case ENOSPC:
		throw FileException("no space left on device", path, err);
	case EDQUOT:
		throw FileException("disk quota exceeded", path, err);
#if !defined(_AIX)
	case ENOTEMPTY:
		throw DirectoryNotEmptyException(path, err);
#endif
	case ENAMETOOLONG:
		throw PathSyntaxException(path, err);
	case ENFILE:
	case EMFILE:
		throw FileException("too many open files", path, err);
	default:
		throw FileException(Error::getMessage(err), path, err);
	}
}


void FileImpl::handleLastErrorImpl(const std::string& path)
{
	handleLastErrorImpl(errno, path);
}


} // namespace Poco
