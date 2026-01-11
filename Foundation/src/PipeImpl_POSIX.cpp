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
	closeRead();
	closeWrite();
}


int PipeImpl::writeBytes(const void* buffer, int length)
{
	poco_assert (_writefd != -1);

	int n;
	do
	{
		n = write(_writefd.load(std::memory_order_relaxed), buffer, length);
	}
	while (n < 0 && errno == EINTR);
	if (n >= 0)
		return n;
	else
		throw WriteFileException("anonymous pipe");
}


int PipeImpl::readBytes(void* buffer, int length)
{
	poco_assert (_readfd != -1);

	int n;
	do
	{
		n = read(_readfd.load(std::memory_order_relaxed), buffer, length);
	}
	while (n < 0 && errno == EINTR);
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
	int fd = _readfd.exchange(-1, std::memory_order_acq_rel);
	if (fd != -1)
	{
		close(fd);
	}
}


void PipeImpl::closeWrite()
{
	int fd = _writefd.exchange(-1, std::memory_order_acq_rel);
	if (fd != -1)
	{
		close(fd);
	}
}


} // namespace Poco
