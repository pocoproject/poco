//
// TimeServiceRemoteObject.cpp
//
// Package: Generated
// Module:  TimeServiceRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "TimeServiceRemoteObject.h"


namespace Services {


TimeServiceRemoteObject::TimeServiceRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Services::TimeService> pServiceObject):
	Services::ITimeService(),
	Poco::RemotingNG::RemoteObject(oid),
	_pServiceObject(pServiceObject)
{
}


TimeServiceRemoteObject::~TimeServiceRemoteObject()
{
	try
	{
	}
	catch (...)
	{
		poco_unexpected();
	}
}


} // namespace Services

