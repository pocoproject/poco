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
// SPDX-License-Identifier:	BSL-1.0
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
