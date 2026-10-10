//
// ITimeService.h
//
// Package: Generated
// Module:  ITimeService
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef ITimeService_INCLUDED
#define ITimeService_INCLUDED


#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/Listener.h"
#include "TimeService.h"


namespace Services {


class ITimeService: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<ITimeService>;

	ITimeService();
		/// Creates a ITimeService.

	virtual ~ITimeService();
		/// Destroys the ITimeService.

	virtual Poco::DateTime currentTime() const = 0;
		/// Returns the current date and time.

	virtual std::string remoting__enableEvents(Poco::RemotingNG::Listener::Ptr pListener, bool enable = bool(true)) = 0;
		/// Enable or disable delivery of remote events.
		///
		/// The given Listener instance must implement the Poco::RemotingNG::EventListener
		/// interface, otherwise this method will fail with a RemotingException.
		///
		/// This method is only used with Proxy objects; calling this method on a
		/// RemoteObject will do nothing.

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

	virtual void wakeMeUp(const Poco::DateTime& time, const std::string& message) = 0;
		/// Schedules a wakeup call.
		///
		/// Fires the wakeUp event at the given time, passing
		/// the given message as argument.

	Poco::BasicEvent<const Services::WakeUpEvent> wakeUp;
};


} // namespace Services


#endif // ITimeService_INCLUDED

