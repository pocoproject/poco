//
// OrderEndpointClientHelper.h
//
// Package: Generated
// Module:  OrderEndpointClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef OrderEndpointClientHelper_INCLUDED
#define OrderEndpointClientHelper_INCLUDED


#include "Pizzeria/IOrderEndpoint.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


namespace Pizzeria {


class OrderEndpointClientHelper
{
public:
	OrderEndpointClientHelper();
		/// Creates a OrderEndpointClientHelper.

	~OrderEndpointClientHelper();
		/// Destroys the OrderEndpointClientHelper.

	static IOrderEndpoint::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static IOrderEndpoint::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	IOrderEndpoint::Ptr findImpl(const std::string& uri);

	IOrderEndpoint::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static OrderEndpointClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline IOrderEndpoint::Ptr OrderEndpointClientHelper::find(const std::string& uri)
{
	return OrderEndpointClientHelper::instance().findImpl(uri);
}


inline IOrderEndpoint::Ptr OrderEndpointClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return OrderEndpointClientHelper::instance().findImpl(uri, protocol);
}


} // namespace Pizzeria


#endif // OrderEndpointClientHelper_INCLUDED

