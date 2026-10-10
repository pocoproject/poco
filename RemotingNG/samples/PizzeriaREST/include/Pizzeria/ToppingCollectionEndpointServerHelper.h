//
// ToppingCollectionEndpointServerHelper.h
//
// Package: Generated
// Module:  ToppingCollectionEndpointServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef ToppingCollectionEndpointServerHelper_INCLUDED
#define ToppingCollectionEndpointServerHelper_INCLUDED


#include "Pizzeria/IToppingCollectionEndpoint.h"
#include "Pizzeria/ToppingCollectionEndpoint.h"
#include "Pizzeria/ToppingCollectionEndpointRemoteObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ServerHelper.h"


namespace Pizzeria {


class ToppingCollectionEndpointServerHelper
{
public:
	using Service = Pizzeria::ToppingCollectionEndpoint;

	ToppingCollectionEndpointServerHelper();
		/// Creates a ToppingCollectionEndpointServerHelper.

	~ToppingCollectionEndpointServerHelper();
		/// Destroys the ToppingCollectionEndpointServerHelper.

	static Poco::AutoPtr<Pizzeria::ToppingCollectionEndpointRemoteObject> createRemoteObject(Poco::SharedPtr<Pizzeria::ToppingCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates and returns a RemoteObject wrapper for the given Pizzeria::ToppingCollectionEndpoint instance.

	static std::string registerObject(Poco::SharedPtr<Pizzeria::ToppingCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId);
		/// Creates a RemoteObject wrapper for the given Pizzeria::ToppingCollectionEndpoint instance
		/// and registers it with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static std::string registerRemoteObject(Poco::AutoPtr<Pizzeria::ToppingCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId);
		/// Registers the given RemoteObject with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static void shutdown();
		/// Removes the Skeleton for Pizzeria::ToppingCollectionEndpoint from the ORB.

	static void unregisterObject(const std::string& uri);
		/// Unregisters a service object identified by URI from the ORB.

private:
	static Poco::AutoPtr<Pizzeria::ToppingCollectionEndpointRemoteObject> createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::ToppingCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);

	static ToppingCollectionEndpointServerHelper& instance();
		/// Returns a static instance of the helper class.

	std::string registerObjectImpl(Poco::AutoPtr<Pizzeria::ToppingCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId);

	void registerSkeleton();

	void unregisterObjectImpl(const std::string& uri);

	void unregisterSkeleton();

	Poco::RemotingNG::ORB* _pORB;
};


inline Poco::AutoPtr<Pizzeria::ToppingCollectionEndpointRemoteObject> ToppingCollectionEndpointServerHelper::createRemoteObject(Poco::SharedPtr<Pizzeria::ToppingCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	return ToppingCollectionEndpointServerHelper::instance().createRemoteObjectImpl(pServiceObject, oid);
}


inline std::string ToppingCollectionEndpointServerHelper::registerObject(Poco::SharedPtr<Pizzeria::ToppingCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId)
{
	return ToppingCollectionEndpointServerHelper::instance().registerObjectImpl(createRemoteObject(pServiceObject, oid), listenerId);
}


inline std::string ToppingCollectionEndpointServerHelper::registerRemoteObject(Poco::AutoPtr<Pizzeria::ToppingCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return ToppingCollectionEndpointServerHelper::instance().registerObjectImpl(pRemoteObject, listenerId);
}


inline void ToppingCollectionEndpointServerHelper::unregisterObject(const std::string& uri)
{
	ToppingCollectionEndpointServerHelper::instance().unregisterObjectImpl(uri);
}


} // namespace Pizzeria


REMOTING_SPECIALIZE_SERVER_HELPER(Pizzeria, ToppingCollectionEndpoint)


#endif // ToppingCollectionEndpointServerHelper_INCLUDED

