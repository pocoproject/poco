//
// ChannelStream.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  ChannelStream
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/TCP/ChannelStream.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


//
// ChannelStreamBuf
//


ChannelStreamBuf::ChannelStreamBuf(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::Timespan timeout):
	Poco::BufferedStreamBuf(Frame::FRAME_MAX_PAYLOAD_SIZE + 4, std::ios::in),
	_pConnection(pConnection),
	_pQueue(new FrameQueue(pConnection, frameType, channel)),
	_timeout(timeout)
{
	_pConnection->pushFrameHandler(_pQueue);
}


ChannelStreamBuf::ChannelStreamBuf(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::UInt16 flags):
	Poco::BufferedStreamBuf(Frame::FRAME_MAX_PAYLOAD_SIZE, std::ios::out),
	_pConnection(pConnection),
	_pFrame(new Frame(frameType, channel, flags, Frame::FRAME_MAX_SIZE))
{
}


ChannelStreamBuf::~ChannelStreamBuf()
{
	try
	{
		if (_pQueue)
		{
			_pConnection->popFrameHandler(_pQueue);
		}
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void ChannelStreamBuf::close()
{
	if (!_pFrame) return;

	_pFrame->updateFlags(_pFrame->flags() | Frame::FRAME_FLAG_EOM);
	if (pptr() && pptr() > pbase())
	{
		sync();
	}
	else
	{
		_pFrame->setPayloadSize(0);
		_pConnection->sendFrame(_pFrame);
	}
	_pFrame = 0;
}


FrameQueue::Ptr ChannelStreamBuf::queue()
{
	return _pQueue;
}


Connection::Ptr ChannelStreamBuf::connection()
{
	return _pConnection;
}


std::streamsize ChannelStreamBuf::readFromDevice(char* buffer, std::streamsize length)
{
	if (_pQueue)
	{
		Frame::Ptr pFrame = _pQueue->dequeueFrame(_timeout);
		if (pFrame)
		{
			poco_assert (length >= pFrame->getPayloadSize());
			std::memcpy(buffer, pFrame->payloadBegin(), pFrame->getPayloadSize());
			std::streamsize n = static_cast<std::streamsize>(pFrame->getPayloadSize());
			bool eof = (pFrame->flags() & Frame::FRAME_FLAG_EOM) != 0;
			_pConnection->returnFrame(pFrame);
			if (eof)
			{
				_pConnection->popFrameHandler(_pQueue);
				_pQueue = 0;
			}
			return n;
		}
		else return -1;
	}
	else return 0;
}


std::streamsize ChannelStreamBuf::writeToDevice(const char* buffer, std::streamsize length)
{
	poco_assert (length <= _pFrame->maxPayloadSize());

	std::memcpy(_pFrame->payloadBegin(), buffer, static_cast<std::size_t>(length));
	_pFrame->setPayloadSize(static_cast<Poco::UInt16>(length));
	_pConnection->sendFrame(_pFrame);
	_pFrame->updateFlags(_pFrame->flags() | Frame::FRAME_FLAG_CONT);
	return static_cast<std::streamsize>(length);
}


//
// ChannelIOS
//


ChannelIOS::ChannelIOS(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::Timespan timeout):
	_buf(pConnection, frameType, channel, timeout)
{
	poco_ios_init(&_buf);
}


ChannelIOS::ChannelIOS(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::UInt16 flags):
	_buf(pConnection, channel, frameType, flags)
{
	poco_ios_init(&_buf);
}


ChannelIOS::~ChannelIOS()
{
	try
	{
		_buf.close();
	}
	catch (...)
	{
	}
}


ChannelStreamBuf* ChannelIOS::rdbuf()
{
	return &_buf;
}


void ChannelIOS::close()
{
	_buf.close();
}


//
// ChannelInputStream
//


ChannelInputStream::ChannelInputStream(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::Timespan timeout):
	ChannelIOS(pConnection, frameType, channel, timeout),
	std::istream(&_buf)
{
}


ChannelInputStream::~ChannelInputStream()
{
}


//
// ChannelOutputStream
//


ChannelOutputStream::ChannelOutputStream(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::UInt16 flags):
	ChannelIOS(pConnection, channel, frameType, flags),
	std::ostream(&_buf)
{
}


ChannelOutputStream::~ChannelOutputStream()
{
}


} } } // namespace Poco::RemotingNG::TCP
