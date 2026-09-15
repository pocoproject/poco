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
#include "Poco/Delegate.h"
#include "Poco/RemotingNG/ORB.h"
#include "TimeServiceEventDispatcher.h"


namespace Services {


TimeServiceRemoteObject::TimeServiceRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Services::TimeService> pServiceObject):
	Services::ITimeService(),
	Poco::RemotingNG::RemoteObject(oid),
	_pServiceObject(pServiceObject)
{
	_pServiceObject->wakeUp += Poco::delegate(this, &TimeServiceRemoteObject::event__wakeUp);
}


TimeServiceRemoteObject::~TimeServiceRemoteObject()
{
	try
	{
		_pServiceObject->wakeUp -= Poco::delegate(this, &TimeServiceRemoteObject::event__wakeUp);
	}
	catch (...)
	{
		poco_unexpected();
	}
}


std::string TimeServiceRemoteObject::remoting__enableEvents(Poco::RemotingNG::Listener::Ptr pListener, bool enable)
{
	return std::string();
}


void TimeServiceRemoteObject::remoting__enableRemoteEvents(const std::string& protocol)
{
	Poco::RemotingNG::EventDispatcher::Ptr pEventDispatcher = new TimeServiceEventDispatcher(this, remoting__objectId(), protocol);
	Poco::RemotingNG::ORB::instance().registerEventDispatcher(remoting__getURI().toString(), pEventDispatcher);
}


bool TimeServiceRemoteObject::remoting__hasEvents() const
{
	return true;
}


void TimeServiceRemoteObject::event__wakeUp(const Services::WakeUpEvent& data)
{
	wakeUp(this, data);
}


} // namespace Services

