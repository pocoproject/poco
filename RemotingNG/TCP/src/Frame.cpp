//
// Frame.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  Frame
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/TCP/Frame.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


Frame::Frame(Poco::UInt32 type, Poco::UInt32 channel, Poco::UInt16 flags, Poco::UInt16 bufferSize):
	_buffer(bufferSize)
{
	poco_assert (bufferSize >= FRAME_OFFSET_PAYLOAD);
	
	*reinterpret_cast<Poco::UInt32*>(_buffer.begin() + FRAME_OFFSET_TYPE)    = Poco::ByteOrder::toNetwork(type);
	*reinterpret_cast<Poco::UInt32*>(_buffer.begin() + FRAME_OFFSET_CHANNEL) = Poco::ByteOrder::toNetwork(channel);
	*reinterpret_cast<Poco::UInt16*>(_buffer.begin() + FRAME_OFFSET_FLAGS)   = Poco::ByteOrder::toNetwork(flags);
	*reinterpret_cast<Poco::UInt16*>(_buffer.begin() + FRAME_OFFSET_SIZE)    = 0;
}


Frame::~Frame()
{
}


} } } // namespace Poco::RemotingNG::TCP
