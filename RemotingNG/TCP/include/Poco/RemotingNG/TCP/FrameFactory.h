//
// FrameFactory.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  FrameFactory
//
// Definition of the FrameFactory class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_TCP_FrameFactory_INCLUDED
#define RemotingNG_TCP_FrameFactory_INCLUDED


#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/RemotingNG/TCP/Frame.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


class RemotingNGTCP_API FrameFactory
	/// A factory for Frame objects, used by the
	/// Connection's Frame object pool.
{
public:
	Frame::Ptr createObject()
	{
		return new Frame(0, 0, 0, Frame::FRAME_MAX_SIZE);
	}
	
	bool validateObject(Frame::Ptr pFrame)
	{
		return true;
	}
	
	void activateObject(Frame::Ptr pFrame)
	{
	}
	
	void deactivateObject(Frame::Ptr pFrame)
	{
	}
	
	void destroyObject(Frame::Ptr pFrame)
	{
	}
};


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_FrameFactory_INCLUDED
