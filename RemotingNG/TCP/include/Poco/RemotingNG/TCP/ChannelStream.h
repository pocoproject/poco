//
// ChannelStream.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  ChannelStream
//
// Definition of the ChannelStream class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_TCP_ChannelStream_INCLUDED
#define RemotingNG_TCP_ChannelStream_INCLUDED


#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/RemotingNG/TCP/Frame.h"
#include "Poco/RemotingNG/TCP/FrameQueue.h"
#include "Poco/BufferedStreamBuf.h"
#include "Poco/Timespan.h"
#include <cstddef>
#include <istream>
#include <ostream>


namespace Poco::RemotingNG::TCP {


class RemotingNGTCP_API ChannelStreamBuf: public Poco::BufferedStreamBuf
	/// This is the streambuf class used for reading from
	/// and writing to channels.
{
public:
	ChannelStreamBuf(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::Timespan timeout);
	ChannelStreamBuf(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::UInt16 flags);
	~ChannelStreamBuf() override;
	
	void close();
	[[nodiscard]] FrameQueue::Ptr queue();
	[[nodiscard]] Connection::Ptr connection();
	
protected:
	std::streamsize readFromDevice(char* buffer, std::streamsize length) override;
	std::streamsize writeToDevice(const char* buffer, std::streamsize length) override;

private:
	Connection::Ptr _pConnection;
	FrameQueue::Ptr _pQueue;
	Poco::Timespan _timeout;
	Frame::Ptr _pFrame;
};


class RemotingNGTCP_API ChannelIOS: public virtual std::ios
	/// The base class for ChannelInputStream and ChannelOutputStream.
{
public:
	ChannelIOS(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::Timespan timeout);
	ChannelIOS(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::UInt16 flags);
	~ChannelIOS() override;
	void close();
	[[nodiscard]] ChannelStreamBuf* rdbuf();
	
protected:
	ChannelStreamBuf _buf;
};


class RemotingNGTCP_API ChannelInputStream: public ChannelIOS, public std::istream
	/// Stream for reading from a Connection channel.
{
public:
	ChannelInputStream(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::Timespan timeout);
	~ChannelInputStream() override;
};


class RemotingNGTCP_API ChannelOutputStream: public ChannelIOS, public std::ostream
	/// Stream for writing to a Connection channel.
{
public:
	ChannelOutputStream(Connection::Ptr pConnection, Poco::UInt32 frameType, Poco::UInt32 channel, Poco::UInt16 flags);
	~ChannelOutputStream() override;
};


} // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_ChannelStream_INCLUDED
