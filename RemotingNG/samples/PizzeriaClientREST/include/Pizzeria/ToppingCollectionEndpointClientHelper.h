//
// ToppingCollectionEndpointClientHelper.h
//
// Package: Generated
// Module:  ToppingCollectionEndpointClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef ToppingCollectionEndpointClientHelper_INCLUDED
#define ToppingCollectionEndpointClientHelper_INCLUDED


#include "Pizzeria/IToppingCollectionEndpoint.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


namespace Pizzeria {


class ToppingCollectionEndpointClientHelper
{
public:
	ToppingCollectionEndpointClientHelper();
		/// Creates a ToppingCollectionEndpointClientHelper.

	~ToppingCollectionEndpointClientHelper();
		/// Destroys the ToppingCollectionEndpointClientHelper.

	static IToppingCollectionEndpoint::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static IToppingCollectionEndpoint::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	IToppingCollectionEndpoint::Ptr findImpl(const std::string& uri);

	IToppingCollectionEndpoint::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static ToppingCollectionEndpointClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline IToppingCollectionEndpoint::Ptr ToppingCollectionEndpointClientHelper::find(const std::string& uri)
{
	return ToppingCollectionEndpointClientHelper::instance().findImpl(uri);
}


inline IToppingCollectionEndpoint::Ptr ToppingCollectionEndpointClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return ToppingCollectionEndpointClientHelper::instance().findImpl(uri, protocol);
}


} // namespace Pizzeria


#endif // ToppingCollectionEndpointClientHelper_INCLUDED

