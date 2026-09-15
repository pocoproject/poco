//
// TimeServiceClientHelper.h
//
// Package: Generated
// Module:  TimeServiceClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TimeServiceClientHelper_INCLUDED
#define TimeServiceClientHelper_INCLUDED


#include "ITimeService.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


namespace Services {


class TimeServiceClientHelper
{
public:
	TimeServiceClientHelper();
		/// Creates a TimeServiceClientHelper.

	~TimeServiceClientHelper();
		/// Destroys the TimeServiceClientHelper.

	static ITimeService::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static ITimeService::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	ITimeService::Ptr findImpl(const std::string& uri);

	ITimeService::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static TimeServiceClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline ITimeService::Ptr TimeServiceClientHelper::find(const std::string& uri)
{
	return TimeServiceClientHelper::instance().findImpl(uri);
}


inline ITimeService::Ptr TimeServiceClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return TimeServiceClientHelper::instance().findImpl(uri, protocol);
}


} // namespace Services


#endif // TimeServiceClientHelper_INCLUDED

