//
// RemoteObject.cpp
//
// Library: RemotingNG
// Package: ORB
// Module:  RemoteObject
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/RemoteObject.h"


namespace Poco {
namespace RemotingNG {


RemoteObject::RemoteObject(const Identifiable::ObjectId& oid):
	Identifiable(oid)
{
}


RemoteObject::~RemoteObject()
{
}


bool RemoteObject::remoting__hasEvents() const
{
	return false;
}


void RemoteObject::remoting__enableRemoteEvents(const std::string&)
{
}


} } // namespace Poco::RemotingNG
