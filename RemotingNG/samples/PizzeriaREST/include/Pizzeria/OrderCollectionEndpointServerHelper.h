//
// OrderCollectionEndpointServerHelper.h
//
// Package: Generated
// Module:  OrderCollectionEndpointServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef OrderCollectionEndpointServerHelper_INCLUDED
#define OrderCollectionEndpointServerHelper_INCLUDED


#include "Pizzeria/IOrderCollectionEndpoint.h"
#include "Pizzeria/OrderCollectionEndpoint.h"
#include "Pizzeria/OrderCollectionEndpointRemoteObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ServerHelper.h"


namespace Pizzeria {


class OrderCollectionEndpointServerHelper
{
public:
	using Service = Pizzeria::OrderCollectionEndpoint;

	OrderCollectionEndpointServerHelper();
		/// Creates a OrderCollectionEndpointServerHelper.

	~OrderCollectionEndpointServerHelper();
		/// Destroys the OrderCollectionEndpointServerHelper.

	static Poco::AutoPtr<Pizzeria::OrderCollectionEndpointRemoteObject> createRemoteObject(Poco::SharedPtr<Pizzeria::OrderCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates and returns a RemoteObject wrapper for the given Pizzeria::OrderCollectionEndpoint instance.

	static std::string registerObject(Poco::SharedPtr<Pizzeria::OrderCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId);
		/// Creates a RemoteObject wrapper for the given Pizzeria::OrderCollectionEndpoint instance
		/// and registers it with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static std::string registerRemoteObject(Poco::AutoPtr<Pizzeria::OrderCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId);
		/// Registers the given RemoteObject with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static void shutdown();
		/// Removes the Skeleton for Pizzeria::OrderCollectionEndpoint from the ORB.

	static void unregisterObject(const std::string& uri);
		/// Unregisters a service object identified by URI from the ORB.

private:
	static Poco::AutoPtr<Pizzeria::OrderCollectionEndpointRemoteObject> createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::OrderCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);

	static OrderCollectionEndpointServerHelper& instance();
		/// Returns a static instance of the helper class.

	std::string registerObjectImpl(Poco::AutoPtr<Pizzeria::OrderCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId);

	void registerSkeleton();

	void unregisterObjectImpl(const std::string& uri);

	void unregisterSkeleton();

	Poco::RemotingNG::ORB* _pORB;
};


inline Poco::AutoPtr<Pizzeria::OrderCollectionEndpointRemoteObject> OrderCollectionEndpointServerHelper::createRemoteObject(Poco::SharedPtr<Pizzeria::OrderCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	return OrderCollectionEndpointServerHelper::instance().createRemoteObjectImpl(pServiceObject, oid);
}


inline std::string OrderCollectionEndpointServerHelper::registerObject(Poco::SharedPtr<Pizzeria::OrderCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId)
{
	return OrderCollectionEndpointServerHelper::instance().registerObjectImpl(createRemoteObject(pServiceObject, oid), listenerId);
}


inline std::string OrderCollectionEndpointServerHelper::registerRemoteObject(Poco::AutoPtr<Pizzeria::OrderCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return OrderCollectionEndpointServerHelper::instance().registerObjectImpl(pRemoteObject, listenerId);
}


inline void OrderCollectionEndpointServerHelper::unregisterObject(const std::string& uri)
{
	OrderCollectionEndpointServerHelper::instance().unregisterObjectImpl(uri);
}


} // namespace Pizzeria


REMOTING_SPECIALIZE_SERVER_HELPER(Pizzeria, OrderCollectionEndpoint)


#endif // OrderCollectionEndpointServerHelper_INCLUDED

