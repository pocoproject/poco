//
// TimeServiceRemoteObject.h
//
// Package: Generated
// Module:  TimeServiceRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TimeServiceRemoteObject_INCLUDED
#define TimeServiceRemoteObject_INCLUDED


#include "ITimeService.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/SharedPtr.h"


namespace Services {


class TimeServiceRemoteObject: public Services::ITimeService, public Poco::RemotingNG::RemoteObject
{
public:
	using Ptr = Poco::AutoPtr<TimeServiceRemoteObject>;

	TimeServiceRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Services::TimeService> pServiceObject);
		/// Creates a TimeServiceRemoteObject.

	virtual ~TimeServiceRemoteObject();
		/// Destroys the TimeServiceRemoteObject.

	Poco::DateTime currentTime() const;
		/// Returns the current date and time.

	virtual std::string remoting__enableEvents(Poco::RemotingNG::Listener::Ptr pListener, bool enable = bool(true));

	virtual void remoting__enableRemoteEvents(const std::string& protocol);

	virtual bool remoting__hasEvents() const;

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	void wakeMeUp(const Poco::DateTime& time, const std::string& message);
		/// Schedules a wakeup call.
		///
		/// Fires the wakeUp event at the given time, passing
		/// the given message as argument.

protected:
	void event__wakeUp(const Services::WakeUpEvent& data);

private:
	Poco::SharedPtr<Services::TimeService> _pServiceObject;
};


inline Poco::DateTime TimeServiceRemoteObject::currentTime() const
{
	return _pServiceObject->currentTime();
}


inline const Poco::RemotingNG::Identifiable::TypeId& TimeServiceRemoteObject::remoting__typeId() const
{
	return ITimeService::remoting__typeId();
}


inline void TimeServiceRemoteObject::wakeMeUp(const Poco::DateTime& time, const std::string& message)
{
	_pServiceObject->wakeMeUp(time, message);
}


} // namespace Services


#endif // TimeServiceRemoteObject_INCLUDED

