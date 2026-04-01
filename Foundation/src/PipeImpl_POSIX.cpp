//
// PipeImpl_POSIX.cpp
//
// Library: Foundation
// Package: Processes
// Module:  PipeImpl
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/PipeImpl_POSIX.h"
#include "Poco/Exception.h"
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>


namespace Poco {


PipeImpl::PipeImpl()
{
	int fds[2];
	int rc = pipe(fds);
	if (rc == 0)
	{
		_readfd.store(fds[0], std::memory_order_relaxed);
		_writefd.store(fds[1], std::memory_order_relaxed);
	}
	else throw CreateFileException("anonymous pipe");
}


PipeImpl::~PipeImpl()
{
	close();
}


void PipeImpl::close()
{
	// Coordinated close of both pipe ends to unblock any blocked IO.
	//
	// On POSIX, a blocked write unblocks with EPIPE when the read end
	// is closed, and a blocked read returns 0 (EOF) when the write end
	// is closed. We must close both ends before waiting, because we
	// don't know which end (if any) has blocked IO.
	//
	// This creates a benign race between close(fd) and a concurrent
	// read(fd)/write(fd) on the same fd: the blocked syscall holds a
	// kernel reference to the underlying file description, so it
	// completes safely even after close() removes the fd from the
	// table (see close(2) on Linux). The fd is cached in a local
	// variable by readBytes/writeBytes, so fd recycling after close
	// does not affect the in-progress syscall.
	//
	// TSan reports this as a data race (suppressed in tsan.suppress).

	_writeLock.markClosed();
	_readLock.markClosed();

	// Close write fd — sends EOF to blocked readers.
	int wfd = _writefd.exchange(-1, std::memory_order_acq_rel);
	if (wfd != -1)
		::close(wfd);

	// Wait for reader to finish (it received EOF from closing write end).
	_readLock.wait();

	// Close read fd — sends EPIPE to blocked writers.
	int rfd = _readfd.exchange(-1, std::memory_order_acq_rel);
	if (rfd != -1)
		::close(rfd);

	// Wait for writer to finish (it received EPIPE from closing read end).
	_writeLock.wait();
}


int PipeImpl::writeBytes(const void* buffer, int length)
{
	ScopedIOLock lock(_writeLock);
	if (!lock)
		return 0;

	int fd = _writefd.load(std::memory_order_relaxed);
	if (fd == -1)
		return 0;  // fd closed concurrently

	int n;
	do
	{
		n = write(fd, buffer, length);
	}
	while (n < 0 && errno == EINTR && !lock.isClosed());

	if (lock.isClosed())
		return 0;

	if (n >= 0)
		return n;
	else
		throw WriteFileException("anonymous pipe");
}


int PipeImpl::readBytes(void* buffer, int length)
{
	ScopedIOLock lock(_readLock);
	if (!lock)
		return 0;

	int fd = _readfd.load(std::memory_order_relaxed);
	if (fd == -1)
		return 0;  // fd closed concurrently

	int n;
	do
	{
		n = read(fd, buffer, length);
	}
	while (n < 0 && errno == EINTR && !lock.isClosed());

	if (lock.isClosed())
		return 0;

	if (n >= 0)
		return n;
	else
		throw ReadFileException("anonymous pipe");
}


PipeImpl::Handle PipeImpl::readHandle() const
{
	return _readfd.load(std::memory_order_relaxed);
}


PipeImpl::Handle PipeImpl::writeHandle() const
{
	return _writefd.load(std::memory_order_relaxed);
}


void PipeImpl::closeRead()
{
	_readLock.markClosed();
	_readLock.wait();
	int fd = _readfd.exchange(-1, std::memory_order_acq_rel);
	if (fd != -1)
		::close(fd);
}


void PipeImpl::closeWrite()
{
	_writeLock.markClosed();
	_writeLock.wait();
	int fd = _writefd.exchange(-1, std::memory_order_acq_rel);
	if (fd != -1)
		::close(fd);
}


} // namespace Poco
