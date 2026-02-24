//
// PipeImpl_WIN32.cpp
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


#include "Poco/PipeImpl_WIN32.h"
#include "Poco/Exception.h"


namespace Poco {


PipeImpl::PipeImpl()
{
	SECURITY_ATTRIBUTES attr;
	attr.nLength              = sizeof(attr);
	attr.lpSecurityDescriptor = nullptr;
	attr.bInheritHandle       = FALSE;

	HANDLE readHandle, writeHandle;
	if (!CreatePipe(&readHandle, &writeHandle, &attr, 0))
		throw CreateFileException("anonymous pipe");
	_readHandle.store(readHandle, std::memory_order_relaxed);
	_writeHandle.store(writeHandle, std::memory_order_relaxed);
}


PipeImpl::~PipeImpl()
{
	closeWrite();  // close write first to send EOF to blocked readers
	closeRead();
}


int PipeImpl::writeBytes(const void* buffer, int length)
{
	ScopedIOLock lock(_writeLock);
	if (!lock)
		return 0;

	poco_assert (_writeHandle != INVALID_HANDLE_VALUE);

	DWORD bytesWritten = 0;
	BOOL ok = WriteFile(_writeHandle.load(std::memory_order_relaxed), buffer, length, &bytesWritten, nullptr);

	if (lock.isClosed())
		return 0;

	if (!ok)
		throw WriteFileException("anonymous pipe");
	return bytesWritten;
}


int PipeImpl::readBytes(void* buffer, int length)
{
	ScopedIOLock lock(_readLock);
	if (!lock)
		return 0;

	poco_assert (_readHandle != INVALID_HANDLE_VALUE);

	DWORD bytesRead = 0;
	BOOL ok = ReadFile(_readHandle.load(std::memory_order_relaxed), buffer, length, &bytesRead, nullptr);
	DWORD lastError = GetLastError();

	if (lock.isClosed())
		return 0;

	if (ok || lastError == ERROR_BROKEN_PIPE)
		return bytesRead;
	else
		throw ReadFileException("anonymous pipe");
}


PipeImpl::Handle PipeImpl::readHandle() const
{
	return _readHandle.load(std::memory_order_relaxed);
}


PipeImpl::Handle PipeImpl::writeHandle() const
{
	return _writeHandle.load(std::memory_order_relaxed);
}


void PipeImpl::closeRead()
{
	_readLock.markClosed();
	_readLock.wait();  // wait for any in-progress read to finish
	HANDLE handle = _readHandle.exchange(INVALID_HANDLE_VALUE, std::memory_order_acq_rel);
	if (handle != INVALID_HANDLE_VALUE)
		CloseHandle(handle);
}


void PipeImpl::closeWrite()
{
	_writeLock.markClosed();
	_writeLock.wait();  // wait for any in-progress write to finish
	HANDLE handle = _writeHandle.exchange(INVALID_HANDLE_VALUE, std::memory_order_acq_rel);
	if (handle != INVALID_HANDLE_VALUE)
		CloseHandle(handle);
}


} // namespace Poco
