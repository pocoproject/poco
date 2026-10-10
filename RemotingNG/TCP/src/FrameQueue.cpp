//
// FrameQueue.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  FrameQueue
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/TCP/FrameQueue.h"
#include "Poco/ScopedUnlock.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


FrameQueue::FrameQueue(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel):
	_pConnection(pConnection),
	_frameType(frameType),
	_channel(channel),
	_sema(0, MAX_QUEUE_SIZE)
{
}

	
FrameQueue::~FrameQueue()
{
	try
	{
		while (!_queue.empty())
		{
			Frame::Ptr pFrame = _queue.back();
			_queue.pop_back();
			_pConnection->returnFrame(pFrame);
		}
	}
	catch (...)
	{
		poco_unexpected();
	}
}


Frame::Ptr FrameQueue::dequeueFrame(Poco::Timespan timeout)
{
	Frame::Ptr pFrame;
	if (_sema.tryWait(static_cast<long>(timeout.totalMilliseconds())))
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		pFrame = _queue.front();
		_queue.pop_front();
	}
	return pFrame;
}


bool FrameQueue::handleFrame(Connection::Ptr pConnection, Frame::Ptr pFrame)
{
	poco_assert_dbg (pConnection == _pConnection);

	if ((_frameType == 0 || pFrame->type() == _frameType) && pFrame->channel() == _channel)
	{
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			int rounds = 0;
			while (_queue.size() == MAX_QUEUE_SIZE && rounds < 100)
			{
				Poco::ScopedUnlock<Poco::FastMutex> unlock(_mutex);
				Poco::Thread::sleep(5);
				rounds++;
			}
			if (_queue.size() < MAX_QUEUE_SIZE)
			{
				_queue.push_back(pFrame);
				_sema.set();
				return true;
			}
		}
	}
	return false;
}


} } } // namespace Poco::RemotingNG::TCP
