//
// Pipe.cpp
//
// Library: Foundation
// Package: Processes
// Module:  Pipe
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Pipe.h"


namespace Poco {


Pipe::Pipe():
	_pImpl(new PipeImpl)
{
}


Pipe::Pipe(const Pipe& pipe):
	_pImpl(pipe._pImpl),
	_readClosed(pipe._readClosed.load(std::memory_order_acquire)),
	_writeClosed(pipe._writeClosed.load(std::memory_order_acquire))
{
	_pImpl->duplicate();
}


Pipe::~Pipe()
{
	_pImpl->release();
}


Pipe& Pipe::operator = (const Pipe& pipe)
{
	if (this != &pipe)
	{
		_pImpl->release();
		_pImpl = pipe._pImpl;
		_pImpl->duplicate();
		_readClosed.store(pipe._readClosed.load(std::memory_order_acquire), std::memory_order_release);
		_writeClosed.store(pipe._writeClosed.load(std::memory_order_acquire), std::memory_order_release);
	}
	return *this;
}


void Pipe::close(CloseMode mode)
{
	switch (mode)
	{
	case CLOSE_READ:
		_readClosed.store(true, std::memory_order_release);
		_pImpl->closeRead();
		break;
	case CLOSE_WRITE:
		_writeClosed.store(true, std::memory_order_release);
		_pImpl->closeWrite();
		break;
	default:
		_readClosed.store(true, std::memory_order_release);
		_writeClosed.store(true, std::memory_order_release);
		_pImpl->closeRead();
		_pImpl->closeWrite();
		break;
	}
}


} // namespace Poco
