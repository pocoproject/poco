//
// PizzaPickupServiceServerHelper.h
//
// Package: Generated
// Module:  PizzaPickupServiceServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaPickupServiceServerHelper_INCLUDED
#define PizzaPickupServiceServerHelper_INCLUDED


#include "Pizzeria/IPizzaPickupService.h"
#include "Pizzeria/PizzaPickupService.h"
#include "Pizzeria/PizzaPickupServiceRemoteObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ServerHelper.h"


namespace Pizzeria {


class PizzaPickupServiceServerHelper
	/// Pizzeria that allows self pickup.
{
public:
	using Service = Pizzeria::PizzaPickupService;

	PizzaPickupServiceServerHelper();
		/// Creates a PizzaPickupServiceServerHelper.

	~PizzaPickupServiceServerHelper();
		/// Destroys the PizzaPickupServiceServerHelper.

	static Poco::AutoPtr<Pizzeria::PizzaPickupServiceRemoteObject> createRemoteObject(Poco::SharedPtr<Pizzeria::PizzaPickupService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates and returns a RemoteObject wrapper for the given Pizzeria::PizzaPickupService instance.

	static std::string registerObject(Poco::SharedPtr<Pizzeria::PizzaPickupService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId);
		/// Creates a RemoteObject wrapper for the given Pizzeria::PizzaPickupService instance
		/// and registers it with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static std::string registerRemoteObject(Poco::AutoPtr<Pizzeria::PizzaPickupServiceRemoteObject> pRemoteObject, const std::string& listenerId);
		/// Registers the given RemoteObject with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static void shutdown();
		/// Removes the Skeleton for Pizzeria::PizzaPickupService from the ORB.

	static void unregisterObject(const std::string& uri);
		/// Unregisters a service object identified by URI from the ORB.

private:
	static Poco::AutoPtr<Pizzeria::PizzaPickupServiceRemoteObject> createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::PizzaPickupService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);

	static PizzaPickupServiceServerHelper& instance();
		/// Returns a static instance of the helper class.

	std::string registerObjectImpl(Poco::AutoPtr<Pizzeria::PizzaPickupServiceRemoteObject> pRemoteObject, const std::string& listenerId);

	void registerSkeleton();

	void unregisterObjectImpl(const std::string& uri);

	void unregisterSkeleton();

	Poco::RemotingNG::ORB* _pORB;
};


inline Poco::AutoPtr<Pizzeria::PizzaPickupServiceRemoteObject> PizzaPickupServiceServerHelper::createRemoteObject(Poco::SharedPtr<Pizzeria::PizzaPickupService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	return PizzaPickupServiceServerHelper::instance().createRemoteObjectImpl(pServiceObject, oid);
}


inline std::string PizzaPickupServiceServerHelper::registerObject(Poco::SharedPtr<Pizzeria::PizzaPickupService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId)
{
	return PizzaPickupServiceServerHelper::instance().registerObjectImpl(createRemoteObject(pServiceObject, oid), listenerId);
}


inline std::string PizzaPickupServiceServerHelper::registerRemoteObject(Poco::AutoPtr<Pizzeria::PizzaPickupServiceRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return PizzaPickupServiceServerHelper::instance().registerObjectImpl(pRemoteObject, listenerId);
}


inline void PizzaPickupServiceServerHelper::unregisterObject(const std::string& uri)
{
	PizzaPickupServiceServerHelper::instance().unregisterObjectImpl(uri);
}


} // namespace Pizzeria


REMOTING_SPECIALIZE_SERVER_HELPER(Pizzeria, PizzaPickupService)


#endif // PizzaPickupServiceServerHelper_INCLUDED

