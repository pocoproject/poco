//
// FileStreamRWLock_POSIX.h
//
// Library: Foundation
// Package: Processes
// Module:  FileStreamRWLock
//
// Definition of the FileStreamRWLockImpl class for POSIX FileStream.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FileStreamRWLock_POSIX_INCLUDED
#define Foundation_FileStreamRWLock_POSIX_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/FileStream.h"
#include <fcntl.h>
#include <cerrno>


namespace Poco {


class Foundation_API FileStreamRWLockImpl
{
protected:
	FileStreamRWLockImpl(const FileStream::NativeHandle &fd, Poco::UInt64 offset, Poco::UInt64 size);
	~FileStreamRWLockImpl();
	void readLockImpl();
	bool tryReadLockImpl();
	void writeLockImpl();
	bool tryWriteLockImpl();
	void unlockImpl();

private:
	FileStream::NativeHandle _fd;
	struct flock _flock;
	int _lockMode;
};


//
// inlines
//
inline void FileStreamRWLockImpl::readLockImpl()
{
	_flock.l_type = F_RDLCK;
	_lockMode = F_SETLKW;
	int rc = fcntl(_fd, _lockMode, &_flock);
	if (rc == -1)
		throw SystemException("cannot lock reader lock", errno);
}


inline bool FileStreamRWLockImpl::tryReadLockImpl()
{
	_flock.l_type = F_RDLCK;
	_lockMode = F_SETLK;
	int rc = fcntl(_fd, _lockMode, &_flock);
	if (rc == 0)
		return true;
	else if (errno == EAGAIN || errno == EACCES)
		return false;
	else
		throw SystemException("cannot lock try-reader lock", errno);

}


inline void FileStreamRWLockImpl::writeLockImpl()
{
	_flock.l_type = F_WRLCK;
	_lockMode = F_SETLKW;
	int rc = fcntl(_fd, _lockMode, &_flock);
	if (rc == -1)
		throw SystemException("cannot lock writer lock", errno);
}


inline bool FileStreamRWLockImpl::tryWriteLockImpl()
{
	_flock.l_type = F_WRLCK;
	_lockMode = F_SETLK;
	int rc = fcntl(_fd, _lockMode, &_flock);
	if (rc != -1)
		return true;
	else if (errno == EAGAIN || errno == EACCES)
		return false;
	else
		throw SystemException("cannot lock writer lock", errno);

}


inline void FileStreamRWLockImpl::unlockImpl()
{
	_flock.l_type = F_UNLCK;
	_lockMode = F_SETLKW;
	int rc = fcntl(_fd, _lockMode, &_flock);
	if (rc == -1)
		throw SystemException("cannot unlock", errno);
}


} // namespace Poco


#endif // Foundation_FileStreamRWLock_POSIX_INCLUDED
