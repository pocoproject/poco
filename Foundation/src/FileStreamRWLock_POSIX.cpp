//
// FileStreamRWLock_POSIX.cpp
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


#include "Poco/FileStreamRWLock_POSIX.h"


namespace Poco {


FileStreamRWLockImpl::FileStreamRWLockImpl(const FileStream::NativeHandle &fd, Poco::UInt64 offset, Poco::UInt64 size):
	_fd(fd), _lockMode(0)
{
	_flock.l_whence = SEEK_SET;
	_flock.l_start = offset;
	_flock.l_len = size;
	_flock.l_pid = 0;
}


FileStreamRWLockImpl::~FileStreamRWLockImpl()
{
}


} // namespace Poco
