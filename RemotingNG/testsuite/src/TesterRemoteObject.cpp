//
// TesterRemoteObject.cpp
//
// Package: Generated
// Module:  TesterRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "TesterRemoteObject.h"
#include "Poco/Delegate.h"
#include "Poco/RemotingNG/ORB.h"
#include "TesterEventDispatcher.h"


TesterRemoteObject::TesterRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Tester> pServiceObject):
	ITester(),
	Poco::RemotingNG::RemoteObject(oid),
	_pServiceObject(pServiceObject)
{
	_pServiceObject->testEnumEvent += Poco::delegate(this, &TesterRemoteObject::event__testEnumEvent);
	_pServiceObject->testEvent += Poco::delegate(this, &TesterRemoteObject::event__testEvent);
	_pServiceObject->testFilteredEvent += Poco::delegate(this, &TesterRemoteObject::event__testFilteredEvent);
	_pServiceObject->testOneWayEvent += Poco::delegate(this, &TesterRemoteObject::event__testOneWayEvent);
	_pServiceObject->testScopedEnumEvent += Poco::delegate(this, &TesterRemoteObject::event__testScopedEnumEvent);
	_pServiceObject->testVoidEvent += Poco::delegate(this, &TesterRemoteObject::event__testVoidEvent);
}


TesterRemoteObject::~TesterRemoteObject()
{
	try
	{
		_pServiceObject->testEnumEvent -= Poco::delegate(this, &TesterRemoteObject::event__testEnumEvent);
		_pServiceObject->testEvent -= Poco::delegate(this, &TesterRemoteObject::event__testEvent);
		_pServiceObject->testFilteredEvent -= Poco::delegate(this, &TesterRemoteObject::event__testFilteredEvent);
		_pServiceObject->testOneWayEvent -= Poco::delegate(this, &TesterRemoteObject::event__testOneWayEvent);
		_pServiceObject->testScopedEnumEvent -= Poco::delegate(this, &TesterRemoteObject::event__testScopedEnumEvent);
		_pServiceObject->testVoidEvent -= Poco::delegate(this, &TesterRemoteObject::event__testVoidEvent);
	}
	catch (...)
	{
		poco_unexpected();
	}
}


std::string TesterRemoteObject::remoting__enableEvents(Poco::RemotingNG::Listener::Ptr pListener, bool enable)
{
	return std::string();
}


void TesterRemoteObject::remoting__enableRemoteEvents(const std::string& protocol)
{
	Poco::RemotingNG::EventDispatcher::Ptr pEventDispatcher = new TesterEventDispatcher(this, remoting__objectId(), protocol);
	Poco::RemotingNG::ORB::instance().registerEventDispatcher(remoting__getURI().toString(), pEventDispatcher);
}


bool TesterRemoteObject::remoting__hasEvents() const
{
	return true;
}


void TesterRemoteObject::event__testEnumEvent(Enum1& data)
{
	testEnumEvent(this, data);
}


void TesterRemoteObject::event__testEvent(std::string& data)
{
	testEvent(this, data);
}


void TesterRemoteObject::event__testFilteredEvent(const int& data)
{
	testFilteredEvent(this, data);
}


void TesterRemoteObject::event__testOneWayEvent(std::string& data)
{
	testOneWayEvent(this, data);
}


void TesterRemoteObject::event__testScopedEnumEvent(ScopedEnum& data)
{
	testScopedEnumEvent(this, data);
}


void TesterRemoteObject::event__testVoidEvent()
{
	testVoidEvent(this);
}


