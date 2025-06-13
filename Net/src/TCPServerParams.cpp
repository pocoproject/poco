//
// TCPServerParams.cpp
//
// Library: Net
// Package: TCPServer
// Module:  TCPServerParams
//
// Copyright (c) 2005-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/TCPServerParams.h"
#include "Poco/Bugcheck.h"


namespace Poco {
namespace Net {


TCPServerParams::TCPServerParams():
	_threadIdleTime(10000000),
	_maxThreads(0),
	_maxQueued(64),
	_threadPriority(Poco::Thread::PRIO_NORMAL),
	_reactorMode(false),
	_acceptorNum(1),
	_useSelfReactor(false)
{
}


TCPServerParams::~TCPServerParams()
{
}


void TCPServerParams::setThreadIdleTime(const Poco::Timespan& milliseconds)
{
	_threadIdleTime = milliseconds;
}


void TCPServerParams::setMaxThreads(int count)
{
	poco_assert (count > 0);

	_maxThreads = count;
}


void TCPServerParams::setMaxQueued(int count)
{
	poco_assert (count >= 0);

	_maxQueued = count;
}


void TCPServerParams::setThreadPriority(Poco::Thread::Priority prio)
{
	_threadPriority = prio;
}

bool TCPServerParams::getReactorMode() const
{
	return _reactorMode;
}
void TCPServerParams::setReactorMode(bool reactorMode)
{
	_reactorMode = reactorMode;
}
int TCPServerParams::getAcceptorNum() const
{
	poco_assert(_reactorMode);
	return _acceptorNum;
}
void TCPServerParams::setAcceptorNum(int acceptorNum)
{
	poco_assert(_reactorMode);
	poco_assert(acceptorNum > 0);
	_acceptorNum = acceptorNum;
}
bool TCPServerParams::getUseSelfReactor() const
{
	poco_assert(_reactorMode);
	return _useSelfReactor;
}
void TCPServerParams::setUseSelfReactor(bool useSelfReactor)
{
	poco_assert(_reactorMode);
	_useSelfReactor = useSelfReactor;
}


} } // namespace Poco::Net
