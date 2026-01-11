//
// PipeImpl_POSIX.h
//
// Library: Foundation
// Package: Processes
// Module:  PipeImpl
//
// Definition of the PipeImpl class for POSIX.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_PipeImpl_POSIX_INCLUDED
#define Foundation_PipeImpl_POSIX_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/RefCountedObject.h"
#include "Poco/IOLock.h"
#include <atomic>


namespace Poco {


class Foundation_API PipeImpl: public RefCountedObject
	/// A dummy implementation of PipeImpl for platforms
	/// that do not support pipes.
{
public:
	using Handle = int;

	PipeImpl();
	~PipeImpl() override;
	int writeBytes(const void* buffer, int length);
	int readBytes(void* buffer, int length);
	Handle readHandle() const;
	Handle writeHandle() const;
	void closeRead();
	void closeWrite();

private:
	std::atomic<int> _readfd{-1};
	std::atomic<int> _writefd{-1};
	IOLock _readLock;
	IOLock _writeLock;
};


} // namespace Poco


#endif // Foundation_PipeImpl_POSIX_INCLUDED
