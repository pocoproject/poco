//
// FileStreamRWLock.cpp
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


#include "Poco/FileStreamRWLock.h"


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "FileStreamRWLock_WIN32.cpp"
#else
#include "FileStreamRWLock_POSIX.cpp"
#endif


namespace Poco {


FileStreamRWLock::FileStreamRWLock(const FileStream &fs, Poco::UInt64 offset, Poco::UInt64 size) :
	FileStreamRWLockImpl(fs.nativeHandle(), offset, size)
{
}


FileStreamRWLock::~FileStreamRWLock()
{
	if (_locked)
	{
		unlockImpl();
		_locked = false;
	}
}


} // namespace Poco
