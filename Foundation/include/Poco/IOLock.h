//
// IOLock.h
//
// Library: Foundation
// Package: Threading
// Module:  IOLock
//
// Definition of the IOLock class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC,
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_IOLock_INCLUDED
#define Foundation_IOLock_INCLUDED


#include "Poco/Foundation.h"
#include <atomic>


namespace Poco {


class Foundation_API IOLock
	/// IOLock provides synchronization for IO operations that may race
	/// with close/shutdown. It ensures that close() waits for any in-progress
	/// IO operation to complete before proceeding.
	///
	///
	/// Usage pattern:
	///
	///     // IO operation:
	///     if (!_lock.enter())
	///         return;  // already closed
	///     // ... perform IO ...
	///     _lock.leave();
	///
	///     // Close operation:
	///     _lock.close();  // waits for IO to finish
	///     // ... close resources ...
	///
	/// For blocking IO operations, use the split close pattern:
	///
	///     _lock.markClosed(); // sets the closed flag
	///     close(fd);  // unblocks the IO
	///     _lock.wait(); // waits for IO completion
	///
	/// IMPORTANT: IOLock is designed to protect a single-thread IO channel use,
	/// which is the common scenario where one thread performs IO, while another
	/// may close the resource.
	/// It does NOT support multiple concurrent IO operations on the same resource
	/// (e.g., multiple threads reading from the same pipe).
{
public:
	IOLock() = default;
		/// Creates the IOLock.

	~IOLock() = default;
		/// Destroys the IOLock.

	bool enter();
		/// Marks the start of an IO operation.
		/// Returns true if the operation can proceed, false if already closed.
		/// Must be paired with leave() if true is returned.

	void leave();
		/// Marks the end of an IO operation.
		/// Must be called after enter() returns true.
		/// Use ScopedIOLock to ensure leave() is always called.

	void close();
		/// Marks the lock as closed and waits for any in-progress
		/// IO operation to complete.

	void markClosed();
		/// Marks the lock as closed without waiting.
		/// Use this when you need to close a resource to unblock IO
		/// before waiting. Follow with wait().

	void wait();
		/// Waits for any in-progress IO operation to complete.
		/// Uses atomic wait if available, otherwise busy-waits.
		/// Call after markClosed() and after closing the resource.

	bool tryWait(int milliseconds);
		/// Busy-waits for any in-progress IO operation to complete,
		/// up to the specified timeout.
		/// Returns true if no IO is in progress, false if timed out.

	bool isClosed() const;
		/// Returns true if close() or markClosed() has been called.

private:
	IOLock(const IOLock&) = delete;
	IOLock(IOLock&&) = delete;
	IOLock& operator=(const IOLock&) = delete;
	IOLock& operator=(IOLock&&) = delete;

	std::atomic<bool> _closed{false};
	std::atomic<bool> _inProgress{false};
};


class Foundation_API ScopedIOLock
	/// ScopedIOLock is a RAII wrapper for IOLock that automatically calls
	/// leave() when the scope exits. This ensures proper cleanup even if
	/// an exception is thrown during the IO operation.
	///
	/// Unlike a regular ScopedLock, enter() may fail if the IOLock is already
	/// closed. Always check if the lock was successfully entered before
	/// proceeding with the IO operation.
	///
	/// Usage pattern:
	///
	///     int readBytes(void* buffer, int length)
	///     {
	///         ScopedIOLock lock(_ioLock);
	///         if (!lock)
	///             return 0;  // already closed
	///
	///         // ... perform IO ...
	///         // leave() called automatically at scope exit
	///
	///         if (lock.isClosed())
	///             return 0;  // closed during IO
	///                        //(from another thread, explicitly
	///                        // or through object destruction)
	///         return bytesRead;
	///     }
	///
	/// Note: isClosed() can be called after the scope exits (after leave())
	/// to check if the lock was closed during the IO operation. This is safe
	/// because isClosed() checks the closed flag, not whether the lock is held.
{
public:
	explicit ScopedIOLock(IOLock& lock): _lock(lock), _entered(_lock.enter())
		/// Creates the ScopedIOLock and attempts to enter the IOLock.
		/// Check operator bool() or entered() to see if entry succeeded.
	{
	}

	~ScopedIOLock()
		/// Destroys the ScopedIOLock and calls leave() if enter() succeeded.
	{
		if (_entered)
			_lock.leave();
	}

	explicit operator bool() const
		/// Returns true if enter() succeeded and IO can proceed.
	{
		return _entered;
	}

	bool entered() const
		/// Returns true if enter() succeeded and IO can proceed.
	{
		return _entered;
	}

	bool isClosed() const
		/// Returns true if the underlying IOLock is closed.
		/// Can be called to check if close was requested during IO.
	{
		return _lock.isClosed();
	}

private:
	ScopedIOLock(const ScopedIOLock&) = delete;
	ScopedIOLock& operator=(const ScopedIOLock&) = delete;

	IOLock& _lock;
	bool _entered;
};


//
// inlines
//

inline bool IOLock::isClosed() const
{
	return _closed.load();
}


} // namespace Poco


#endif // Foundation_IOLock_INCLUDED
