//
// EventSubscriber.cpp
//
// Library: RemotingNG
// Package: ORB
// Module:  EventSubscriber
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/EventSubscriber.h"


namespace Poco {
namespace RemotingNG {


EventSubscriber::EventSubscriber(const std::string& uri):
	_uri(uri)
{
}


EventSubscriber::~EventSubscriber()
{
}


} } // namespace Poco::RemotingNG
