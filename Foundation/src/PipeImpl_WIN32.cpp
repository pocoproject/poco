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
#include <ioapiset.h>  // CancelIoEx


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
	close();
}


void PipeImpl::close()
{
	// On Windows, CloseHandle while another thread is in ReadFile/WriteFile
	// is undefined behavior — unlike POSIX where close(2) is safe due to
	// kernel file-description refcounting. Use CancelIoEx to cancel pending
	// IO, wait for completion, then safely close handles.
	//
	// CancelIoEx causes the blocked call to return FALSE with
	// ERROR_OPERATION_ABORTED; the IO thread checks isClosed() and returns 0.
	// Returns FALSE/ERROR_NOT_FOUND if no pending IO — safe to ignore.
	//
	// See: https://learn.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-cancelioex

	_writeLock.markClosed();
	_readLock.markClosed();

	// Cancel any pending IO to unblock blocked ReadFile/WriteFile.
	HANDLE rh = _readHandle.load(std::memory_order_acquire);
	if (rh != INVALID_HANDLE_VALUE)
		CancelIoEx(rh, nullptr);

	HANDLE wh = _writeHandle.load(std::memory_order_acquire);
	if (wh != INVALID_HANDLE_VALUE)
		CancelIoEx(wh, nullptr);

	// Wait for IO threads to finish (they received ERROR_OPERATION_ABORTED).
	_readLock.wait();
	_writeLock.wait();

	// Safe to close handles — no IO is in progress.
	rh = _readHandle.exchange(INVALID_HANDLE_VALUE, std::memory_order_acq_rel);
	if (rh != INVALID_HANDLE_VALUE)
		CloseHandle(rh);

	wh = _writeHandle.exchange(INVALID_HANDLE_VALUE, std::memory_order_acq_rel);
	if (wh != INVALID_HANDLE_VALUE)
		CloseHandle(wh);
}


int PipeImpl::writeBytes(const void* buffer, int length)
{
	ScopedIOLock lock(_writeLock);
	if (!lock)
		return 0;

	HANDLE handle = _writeHandle.load(std::memory_order_relaxed);
	if (handle == INVALID_HANDLE_VALUE)
		return 0;  // handle closed concurrently

	DWORD bytesWritten = 0;
	BOOL ok = WriteFile(handle, buffer, length, &bytesWritten, nullptr);

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

	HANDLE handle = _readHandle.load(std::memory_order_relaxed);
	if (handle == INVALID_HANDLE_VALUE)
		return 0;  // handle closed concurrently

	DWORD bytesRead = 0;
	BOOL ok = ReadFile(handle, buffer, length, &bytesRead, nullptr);
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
	_readLock.wait();
	HANDLE handle = _readHandle.exchange(INVALID_HANDLE_VALUE, std::memory_order_acq_rel);
	if (handle != INVALID_HANDLE_VALUE)
		CloseHandle(handle);
}


void PipeImpl::closeWrite()
{
	_writeLock.markClosed();
	_writeLock.wait();
	HANDLE handle = _writeHandle.exchange(INVALID_HANDLE_VALUE, std::memory_order_acq_rel);
	if (handle != INVALID_HANDLE_VALUE)
		CloseHandle(handle);
}


} // namespace Poco
