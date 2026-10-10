//
// TimeServiceServerHelper.h
//
// Package: Generated
// Module:  TimeServiceServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TimeServiceServerHelper_INCLUDED
#define TimeServiceServerHelper_INCLUDED


#include "ITimeService.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ServerHelper.h"
#include "TimeService.h"
#include "TimeServiceRemoteObject.h"


namespace Services {


class TimeServiceServerHelper
{
public:
	using Service = Services::TimeService;

	TimeServiceServerHelper();
		/// Creates a TimeServiceServerHelper.

	~TimeServiceServerHelper();
		/// Destroys the TimeServiceServerHelper.

	static Poco::AutoPtr<Services::TimeServiceRemoteObject> createRemoteObject(Poco::SharedPtr<Services::TimeService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates and returns a RemoteObject wrapper for the given Services::TimeService instance.

	static void enableEvents(const std::string& uri, const std::string& protocol);
		/// Enables remote events for the RemoteObject identified by the given URI.
		///
		/// Events will be delivered using the Transport for the given protocol.
		/// Can be called multiple times for the same URI with different protocols.

	static std::string registerObject(Poco::SharedPtr<Services::TimeService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId);
		/// Creates a RemoteObject wrapper for the given Services::TimeService instance
		/// and registers it with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static std::string registerRemoteObject(Poco::AutoPtr<Services::TimeServiceRemoteObject> pRemoteObject, const std::string& listenerId);
		/// Registers the given RemoteObject with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static void shutdown();
		/// Removes the Skeleton for Services::TimeService from the ORB.

	static void unregisterObject(const std::string& uri);
		/// Unregisters a service object identified by URI from the ORB.

private:
	static Poco::AutoPtr<Services::TimeServiceRemoteObject> createRemoteObjectImpl(Poco::SharedPtr<Services::TimeService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);

	void enableEventsImpl(const std::string& uri, const std::string& protocol);

	static TimeServiceServerHelper& instance();
		/// Returns a static instance of the helper class.

	std::string registerObjectImpl(Poco::AutoPtr<Services::TimeServiceRemoteObject> pRemoteObject, const std::string& listenerId);

	void registerSkeleton();

	void unregisterObjectImpl(const std::string& uri);

	void unregisterSkeleton();

	Poco::RemotingNG::ORB* _pORB;
};


inline Poco::AutoPtr<Services::TimeServiceRemoteObject> TimeServiceServerHelper::createRemoteObject(Poco::SharedPtr<Services::TimeService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	return TimeServiceServerHelper::instance().createRemoteObjectImpl(pServiceObject, oid);
}


inline void TimeServiceServerHelper::enableEvents(const std::string& uri, const std::string& protocol)
{
	TimeServiceServerHelper::instance().enableEventsImpl(uri, protocol);
}


inline std::string TimeServiceServerHelper::registerObject(Poco::SharedPtr<Services::TimeService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId)
{
	return TimeServiceServerHelper::instance().registerObjectImpl(createRemoteObject(pServiceObject, oid), listenerId);
}


inline std::string TimeServiceServerHelper::registerRemoteObject(Poco::AutoPtr<Services::TimeServiceRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return TimeServiceServerHelper::instance().registerObjectImpl(pRemoteObject, listenerId);
}


inline void TimeServiceServerHelper::unregisterObject(const std::string& uri)
{
	TimeServiceServerHelper::instance().unregisterObjectImpl(uri);
}


} // namespace Services


REMOTING_SPECIALIZE_SERVER_HELPER(Services, TimeService)


#endif // TimeServiceServerHelper_INCLUDED

