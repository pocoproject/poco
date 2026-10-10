//
// PizzaPickupServiceClientHelper.h
//
// Package: Generated
// Module:  PizzaPickupServiceClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaPickupServiceClientHelper_INCLUDED
#define PizzaPickupServiceClientHelper_INCLUDED


#include "Pizzeria/IPizzaPickupService.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


namespace Pizzeria {


class PizzaPickupServiceClientHelper
	/// Pizzeria that allows self pickup.
{
public:
	PizzaPickupServiceClientHelper();
		/// Creates a PizzaPickupServiceClientHelper.

	~PizzaPickupServiceClientHelper();
		/// Destroys the PizzaPickupServiceClientHelper.

	static IPizzaPickupService::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static IPizzaPickupService::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	IPizzaPickupService::Ptr findImpl(const std::string& uri);

	IPizzaPickupService::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static PizzaPickupServiceClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline IPizzaPickupService::Ptr PizzaPickupServiceClientHelper::find(const std::string& uri)
{
	return PizzaPickupServiceClientHelper::instance().findImpl(uri);
}


inline IPizzaPickupService::Ptr PizzaPickupServiceClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return PizzaPickupServiceClientHelper::instance().findImpl(uri, protocol);
}


} // namespace Pizzeria


#endif // PizzaPickupServiceClientHelper_INCLUDED

