//
// PizzaCollectionEndpointServerHelper.h
//
// Package: Generated
// Module:  PizzaCollectionEndpointServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaCollectionEndpointServerHelper_INCLUDED
#define PizzaCollectionEndpointServerHelper_INCLUDED


#include "Pizzeria/IPizzaCollectionEndpoint.h"
#include "Pizzeria/PizzaCollectionEndpoint.h"
#include "Pizzeria/PizzaCollectionEndpointRemoteObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ServerHelper.h"


namespace Pizzeria {


class PizzaCollectionEndpointServerHelper
{
public:
	using Service = Pizzeria::PizzaCollectionEndpoint;

	PizzaCollectionEndpointServerHelper();
		/// Creates a PizzaCollectionEndpointServerHelper.

	~PizzaCollectionEndpointServerHelper();
		/// Destroys the PizzaCollectionEndpointServerHelper.

	static Poco::AutoPtr<Pizzeria::PizzaCollectionEndpointRemoteObject> createRemoteObject(Poco::SharedPtr<Pizzeria::PizzaCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates and returns a RemoteObject wrapper for the given Pizzeria::PizzaCollectionEndpoint instance.

	static std::string registerObject(Poco::SharedPtr<Pizzeria::PizzaCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId);
		/// Creates a RemoteObject wrapper for the given Pizzeria::PizzaCollectionEndpoint instance
		/// and registers it with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static std::string registerRemoteObject(Poco::AutoPtr<Pizzeria::PizzaCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId);
		/// Registers the given RemoteObject with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static void shutdown();
		/// Removes the Skeleton for Pizzeria::PizzaCollectionEndpoint from the ORB.

	static void unregisterObject(const std::string& uri);
		/// Unregisters a service object identified by URI from the ORB.

private:
	static Poco::AutoPtr<Pizzeria::PizzaCollectionEndpointRemoteObject> createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::PizzaCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);

	static PizzaCollectionEndpointServerHelper& instance();
		/// Returns a static instance of the helper class.

	std::string registerObjectImpl(Poco::AutoPtr<Pizzeria::PizzaCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId);

	void registerSkeleton();

	void unregisterObjectImpl(const std::string& uri);

	void unregisterSkeleton();

	Poco::RemotingNG::ORB* _pORB;
};


inline Poco::AutoPtr<Pizzeria::PizzaCollectionEndpointRemoteObject> PizzaCollectionEndpointServerHelper::createRemoteObject(Poco::SharedPtr<Pizzeria::PizzaCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	return PizzaCollectionEndpointServerHelper::instance().createRemoteObjectImpl(pServiceObject, oid);
}


inline std::string PizzaCollectionEndpointServerHelper::registerObject(Poco::SharedPtr<Pizzeria::PizzaCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId)
{
	return PizzaCollectionEndpointServerHelper::instance().registerObjectImpl(createRemoteObject(pServiceObject, oid), listenerId);
}


inline std::string PizzaCollectionEndpointServerHelper::registerRemoteObject(Poco::AutoPtr<Pizzeria::PizzaCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return PizzaCollectionEndpointServerHelper::instance().registerObjectImpl(pRemoteObject, listenerId);
}


inline void PizzaCollectionEndpointServerHelper::unregisterObject(const std::string& uri)
{
	PizzaCollectionEndpointServerHelper::instance().unregisterObjectImpl(uri);
}


} // namespace Pizzeria


REMOTING_SPECIALIZE_SERVER_HELPER(Pizzeria, PizzaCollectionEndpoint)


#endif // PizzaCollectionEndpointServerHelper_INCLUDED

