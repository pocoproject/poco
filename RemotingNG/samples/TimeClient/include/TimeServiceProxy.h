//
// TimeServiceProxy.h
//
// Package: Generated
// Module:  TimeServiceProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TimeServiceProxy_INCLUDED
#define TimeServiceProxy_INCLUDED


#include "ITimeService.h"
#include "Poco/RemotingNG/Proxy.h"


namespace Services {


class TimeServiceProxy: public Services::ITimeService, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<TimeServiceProxy>;

	TimeServiceProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a TimeServiceProxy.

	virtual ~TimeServiceProxy();
		/// Destroys the TimeServiceProxy.

	Poco::DateTime currentTime() const;
		/// Returns the current date and time.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
	mutable Poco::DateTime _currentTimeRet;
};


inline const Poco::RemotingNG::Identifiable::TypeId& TimeServiceProxy::remoting__typeId() const
{
	return ITimeService::remoting__typeId();
}


} // namespace Services


#endif // TimeServiceProxy_INCLUDED

