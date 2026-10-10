//
// PizzaDeliveryServiceClientHelper.h
//
// Package: Generated
// Module:  PizzaDeliveryServiceClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaDeliveryServiceClientHelper_INCLUDED
#define PizzaDeliveryServiceClientHelper_INCLUDED


#include "Pizzeria/IPizzaDeliveryService.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


namespace Pizzeria {


class PizzaDeliveryServiceClientHelper
{
public:
	PizzaDeliveryServiceClientHelper();
		/// Creates a PizzaDeliveryServiceClientHelper.

	~PizzaDeliveryServiceClientHelper();
		/// Destroys the PizzaDeliveryServiceClientHelper.

	static IPizzaDeliveryService::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static IPizzaDeliveryService::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	IPizzaDeliveryService::Ptr findImpl(const std::string& uri);

	IPizzaDeliveryService::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static PizzaDeliveryServiceClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline IPizzaDeliveryService::Ptr PizzaDeliveryServiceClientHelper::find(const std::string& uri)
{
	return PizzaDeliveryServiceClientHelper::instance().findImpl(uri);
}


inline IPizzaDeliveryService::Ptr PizzaDeliveryServiceClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return PizzaDeliveryServiceClientHelper::instance().findImpl(uri, protocol);
}


} // namespace Pizzeria


#endif // PizzaDeliveryServiceClientHelper_INCLUDED

