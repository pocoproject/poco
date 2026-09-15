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

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

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


} // namespace Services


#endif // TimeServiceRemoteObject_INCLUDED

