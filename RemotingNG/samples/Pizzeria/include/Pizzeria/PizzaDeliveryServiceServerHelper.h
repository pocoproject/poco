//
// PizzaDeliveryServiceServerHelper.h
//
// Package: Generated
// Module:  PizzaDeliveryServiceServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaDeliveryServiceServerHelper_INCLUDED
#define PizzaDeliveryServiceServerHelper_INCLUDED


#include "Pizzeria/IPizzaDeliveryService.h"
#include "Pizzeria/PizzaDeliveryService.h"
#include "Pizzeria/PizzaDeliveryServiceRemoteObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ServerHelper.h"


namespace Pizzeria {


class PizzaDeliveryServiceServerHelper
{
public:
	using Service = Pizzeria::PizzaDeliveryService;

	PizzaDeliveryServiceServerHelper();
		/// Creates a PizzaDeliveryServiceServerHelper.

	~PizzaDeliveryServiceServerHelper();
		/// Destroys the PizzaDeliveryServiceServerHelper.

	static Poco::AutoPtr<Pizzeria::PizzaDeliveryServiceRemoteObject> createRemoteObject(Poco::SharedPtr<Pizzeria::PizzaDeliveryService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates and returns a RemoteObject wrapper for the given Pizzeria::PizzaDeliveryService instance.

	static std::string registerObject(Poco::SharedPtr<Pizzeria::PizzaDeliveryService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId);
		/// Creates a RemoteObject wrapper for the given Pizzeria::PizzaDeliveryService instance
		/// and registers it with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static std::string registerRemoteObject(Poco::AutoPtr<Pizzeria::PizzaDeliveryServiceRemoteObject> pRemoteObject, const std::string& listenerId);
		/// Registers the given RemoteObject with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static void shutdown();
		/// Removes the Skeleton for Pizzeria::PizzaDeliveryService from the ORB.

	static void unregisterObject(const std::string& uri);
		/// Unregisters a service object identified by URI from the ORB.

private:
	static Poco::AutoPtr<Pizzeria::PizzaDeliveryServiceRemoteObject> createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::PizzaDeliveryService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);

	static PizzaDeliveryServiceServerHelper& instance();
		/// Returns a static instance of the helper class.

	std::string registerObjectImpl(Poco::AutoPtr<Pizzeria::PizzaDeliveryServiceRemoteObject> pRemoteObject, const std::string& listenerId);

	void registerSkeleton();

	void unregisterObjectImpl(const std::string& uri);

	void unregisterSkeleton();

	Poco::RemotingNG::ORB* _pORB;
};


inline Poco::AutoPtr<Pizzeria::PizzaDeliveryServiceRemoteObject> PizzaDeliveryServiceServerHelper::createRemoteObject(Poco::SharedPtr<Pizzeria::PizzaDeliveryService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	return PizzaDeliveryServiceServerHelper::instance().createRemoteObjectImpl(pServiceObject, oid);
}


inline std::string PizzaDeliveryServiceServerHelper::registerObject(Poco::SharedPtr<Pizzeria::PizzaDeliveryService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId)
{
	return PizzaDeliveryServiceServerHelper::instance().registerObjectImpl(createRemoteObject(pServiceObject, oid), listenerId);
}


inline std::string PizzaDeliveryServiceServerHelper::registerRemoteObject(Poco::AutoPtr<Pizzeria::PizzaDeliveryServiceRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return PizzaDeliveryServiceServerHelper::instance().registerObjectImpl(pRemoteObject, listenerId);
}


inline void PizzaDeliveryServiceServerHelper::unregisterObject(const std::string& uri)
{
	PizzaDeliveryServiceServerHelper::instance().unregisterObjectImpl(uri);
}


} // namespace Pizzeria


REMOTING_SPECIALIZE_SERVER_HELPER(Pizzeria, PizzaDeliveryService)


#endif // PizzaDeliveryServiceServerHelper_INCLUDED

