//
// FileStreamRWLock.h
//
// Library: Foundation
// Package: Processes
// Module:  FileStreamRWLock
//
// Definition of the FileStreamRWLock class.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FileStreamRWLock_INCLUDED
#define Foundation_FileStreamRWLock_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/FileStream.h"
#include <atomic>

#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/FileStreamRWLock_WIN32.h"
#else
#include "Poco/FileStreamRWLock_POSIX.h"
#endif


namespace Poco {


class ScopedFStreamRWLock;
class ScopedFStreamReadRWLock;
class ScopedFStreamWriteRWLock;


class Foundation_API FileStreamRWLock: private FileStreamRWLockImpl
	/// A reader writer lock on the file region allows multiple concurrent
	/// process-readers or one exclusive process-writer.
{
public:
	using ScopedLock = ScopedFStreamRWLock;
	using ScopedReadLock = ScopedFStreamReadRWLock;
	using ScopedWriteLock = ScopedFStreamWriteRWLock;

	FileStreamRWLock(const FileStream &fs, Poco::UInt64 offset, Poco::UInt64 size);
		/// Creates the Reader/Writer lock on the file region.
		/// offset - from start of the file
		/// size - size of the locker region

	~FileStreamRWLock();
		/// Destroys the Reader/Writer lock on the file region.

	void readLock();
		/// Acquires a read lock. If another process currently holds a write lock,
		/// waits until the write lock is released.

	bool tryReadLock();
		/// Tries to acquire a read lock. Immediately returns true if successful, or
		/// false if another process currently holds a write lock.

	void writeLock();
		/// Acquires a write lock on the file region. If one or more other processes currently hold
		/// locks, waits until all locks are released.

	bool tryWriteLock();
		/// Tries to acquire a write lock on the file region. Immediately returns true if successful,
		/// or false if one or more other processes currently hold
		/// locks.

	void unlock();
		/// Releases the read or write lock.

private:
	std::atomic_bool _locked = false;
	FileStreamRWLock(const FileStreamRWLock&);
	FileStreamRWLock& operator = (const FileStreamRWLock&);
};


class Foundation_API ScopedFStreamRWLock
	/// A variant of ScopedLock for reader/writer locks.
{
public:
	ScopedFStreamRWLock(FileStreamRWLock& rwl, bool write = false);
	~ScopedFStreamRWLock();

	ScopedFStreamRWLock() = delete;
	ScopedFStreamRWLock(const ScopedFStreamRWLock&) = delete;
	ScopedFStreamRWLock& operator=(const ScopedFStreamRWLock&) = delete;

private:
	FileStreamRWLock& _rwl;

};


class Foundation_API ScopedFStreamReadRWLock : public ScopedFStreamRWLock
	/// A variant of ScopedLock for reader locks.
{
public:
	ScopedFStreamReadRWLock(FileStreamRWLock& rwl);
	~ScopedFStreamReadRWLock();
};


class Foundation_API ScopedFStreamWriteRWLock : public ScopedFStreamRWLock
	/// A variant of ScopedLock for writer locks.
{
public:
	ScopedFStreamWriteRWLock(FileStreamRWLock& rwl);
	~ScopedFStreamWriteRWLock();
};


//
// inlines
//
inline void FileStreamRWLock::readLock()
{
	readLockImpl();
	_locked = true;
}


inline bool FileStreamRWLock::tryReadLock()
{
	bool locked = tryReadLockImpl();
	if (locked) _locked = true; // assign only if success lock
	return locked;
}


inline void FileStreamRWLock::writeLock()
{
	writeLockImpl();
	_locked = true;
}


inline bool FileStreamRWLock::tryWriteLock()
{
	bool locked = tryWriteLockImpl();
	if (locked) _locked = true; // assign only if success lock
	return locked;
}


inline void FileStreamRWLock::unlock()
{
	unlockImpl();
	_locked = false;
}


inline ScopedFStreamRWLock::ScopedFStreamRWLock(FileStreamRWLock& rwl, bool write): _rwl(rwl)
{
	if (write)
		_rwl.writeLock();
	else
		_rwl.readLock();
}


inline ScopedFStreamRWLock::~ScopedFStreamRWLock()
{
	try
	{
		_rwl.unlock();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


inline ScopedFStreamReadRWLock::ScopedFStreamReadRWLock(FileStreamRWLock& rwl): ScopedFStreamRWLock(rwl, false)
{
}

inline ScopedFStreamReadRWLock::~ScopedFStreamReadRWLock() = default;

inline ScopedFStreamWriteRWLock::ScopedFStreamWriteRWLock(FileStreamRWLock& rwl): ScopedFStreamRWLock(rwl, true)
{
}

inline ScopedFStreamWriteRWLock::~ScopedFStreamWriteRWLock() = default;

} // namespace Poco


#endif // Foundation_FileStreamRWLock_INCLUDED
