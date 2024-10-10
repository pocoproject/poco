//
// FileStreamRWLock_WIN32.h
//
// Library: Foundation
// Package: Processes
// Module:  FileStreamRWLock
//
// Definition of the FileStreamRWLockImpl class for WIN32 FileStream.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FileStreamRWLock_WIN32_INCLUDED
#define Foundation_FileStreamRWLock_WIN32_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/FileStream.h"
#include <Windows.h>

namespace Poco {

struct LockMode
{
	static constexpr DWORD READ = 0;
	static constexpr DWORD WRITE = LOCKFILE_EXCLUSIVE_LOCK;
	static constexpr DWORD TRY_READ = LOCKFILE_FAIL_IMMEDIATELY;
	static constexpr DWORD TRY_WRITE = (LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY);
};

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
	OVERLAPPED _overlapped;
	LARGE_INTEGER _size;
};


//
// inlines
//
inline void FileStreamRWLockImpl::readLockImpl()
{
	BOOL fSuccess = LockFileEx(_fd, LockMode::READ, 0, _size.LowPart, _size.HighPart, &_overlapped);
	if (!fSuccess)
	{
		throw SystemException("cannot lock reader lock", GetLastError());
	}
}


inline bool FileStreamRWLockImpl::tryReadLockImpl()
{
	BOOL fSuccess = LockFileEx(_fd, LockMode::TRY_READ, 0, _size.LowPart, _size.HighPart, &_overlapped);
	if (fSuccess)
	{
		return true;
	}
	else
	{
		auto lastError = GetLastError();
		if (lastError == ERROR_IO_PENDING || lastError == ERROR_LOCK_VIOLATION)
		{
			return false;
		}
		else
		{
			throw SystemException("cannot lock try-reader lock", lastError);
		}
	}
}


inline void FileStreamRWLockImpl::writeLockImpl()
{
	BOOL fSuccess = LockFileEx(_fd, LockMode::WRITE, 0, _size.LowPart, _size.HighPart, &_overlapped);
	if (!fSuccess)
	{
		throw SystemException("cannot lock writer lock", GetLastError());
	}
}


inline bool FileStreamRWLockImpl::tryWriteLockImpl()
{
	BOOL fSuccess = LockFileEx(_fd, LockMode::TRY_WRITE, 0, _size.LowPart, _size.HighPart, &_overlapped);
	if (fSuccess)
	{
		return true;
	}
	else 
	{
		auto lastError = GetLastError();
		if (lastError == ERROR_IO_PENDING || lastError == ERROR_LOCK_VIOLATION)
		{
			return false;
		}
		else
		{
			throw SystemException("cannot lock try-writer lock", lastError);
		}
	}
}


inline void FileStreamRWLockImpl::unlockImpl()
{
	BOOL fSuccess = UnlockFileEx(_fd, 0, _size.LowPart, _size.HighPart, &_overlapped);
	if (!fSuccess)
	{
		throw SystemException("cannot unlock ", GetLastError());
	}
}


} // namespace Poco


#endif // Foundation_FileStreamRWLock_WIN32_INCLUDED
