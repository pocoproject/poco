//
// OrderEndpointServerHelper.h
//
// Package: Generated
// Module:  OrderEndpointServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef OrderEndpointServerHelper_INCLUDED
#define OrderEndpointServerHelper_INCLUDED


#include "Pizzeria/IOrderEndpoint.h"
#include "Pizzeria/OrderEndpoint.h"
#include "Pizzeria/OrderEndpointRemoteObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ServerHelper.h"


namespace Pizzeria {


class OrderEndpointServerHelper
{
public:
	using Service = Pizzeria::OrderEndpoint;

	OrderEndpointServerHelper();
		/// Creates a OrderEndpointServerHelper.

	~OrderEndpointServerHelper();
		/// Destroys the OrderEndpointServerHelper.

	static Poco::AutoPtr<Pizzeria::OrderEndpointRemoteObject> createRemoteObject(Poco::SharedPtr<Pizzeria::OrderEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates and returns a RemoteObject wrapper for the given Pizzeria::OrderEndpoint instance.

	static std::string registerObject(Poco::SharedPtr<Pizzeria::OrderEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId);
		/// Creates a RemoteObject wrapper for the given Pizzeria::OrderEndpoint instance
		/// and registers it with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static std::string registerRemoteObject(Poco::AutoPtr<Pizzeria::OrderEndpointRemoteObject> pRemoteObject, const std::string& listenerId);
		/// Registers the given RemoteObject with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static void shutdown();
		/// Removes the Skeleton for Pizzeria::OrderEndpoint from the ORB.

	static void unregisterObject(const std::string& uri);
		/// Unregisters a service object identified by URI from the ORB.

private:
	static Poco::AutoPtr<Pizzeria::OrderEndpointRemoteObject> createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::OrderEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);

	static OrderEndpointServerHelper& instance();
		/// Returns a static instance of the helper class.

	std::string registerObjectImpl(Poco::AutoPtr<Pizzeria::OrderEndpointRemoteObject> pRemoteObject, const std::string& listenerId);

	void registerSkeleton();

	void unregisterObjectImpl(const std::string& uri);

	void unregisterSkeleton();

	Poco::RemotingNG::ORB* _pORB;
};


inline Poco::AutoPtr<Pizzeria::OrderEndpointRemoteObject> OrderEndpointServerHelper::createRemoteObject(Poco::SharedPtr<Pizzeria::OrderEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	return OrderEndpointServerHelper::instance().createRemoteObjectImpl(pServiceObject, oid);
}


inline std::string OrderEndpointServerHelper::registerObject(Poco::SharedPtr<Pizzeria::OrderEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId)
{
	return OrderEndpointServerHelper::instance().registerObjectImpl(createRemoteObject(pServiceObject, oid), listenerId);
}


inline std::string OrderEndpointServerHelper::registerRemoteObject(Poco::AutoPtr<Pizzeria::OrderEndpointRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return OrderEndpointServerHelper::instance().registerObjectImpl(pRemoteObject, listenerId);
}


inline void OrderEndpointServerHelper::unregisterObject(const std::string& uri)
{
	OrderEndpointServerHelper::instance().unregisterObjectImpl(uri);
}


} // namespace Pizzeria


REMOTING_SPECIALIZE_SERVER_HELPER(Pizzeria, OrderEndpoint)


#endif // OrderEndpointServerHelper_INCLUDED

