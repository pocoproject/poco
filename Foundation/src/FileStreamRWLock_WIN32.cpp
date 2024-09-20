//
// FileStreamRWLock_WIN32.cpp
//
// Library: Foundation
// Package: Processes
// Module:  FileStreamRWLock
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/FileStreamRWLock_WIN32.h"


namespace Poco {


FileStreamRWLockImpl::FileStreamRWLockImpl(const FileStream::NativeHandle &fd, Poco::UInt64 offset, Poco::UInt64 size):
	_fd(fd)
{
	LARGE_INTEGER offt;
	offt.QuadPart = offset;
	memset(&_overlapped, 0, sizeof(OVERLAPPED));

	_overlapped.Offset = offt.LowPart;
	_overlapped.OffsetHigh = offt.HighPart;
	_size.QuadPart = size;
}


FileStreamRWLockImpl::~FileStreamRWLockImpl()
{
}


} // namespace Poco
