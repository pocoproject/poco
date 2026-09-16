//
// OrderCollectionEndpointRemoteObject.cpp
//
// Package: Generated
// Module:  OrderCollectionEndpointRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/OrderCollectionEndpointRemoteObject.h"


namespace Pizzeria {


OrderCollectionEndpointRemoteObject::OrderCollectionEndpointRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::OrderCollectionEndpoint> pServiceObject):
	Pizzeria::IOrderCollectionEndpoint(),
	Poco::RemotingNG::RemoteObject(oid),
	_pServiceObject(pServiceObject)
{
}


OrderCollectionEndpointRemoteObject::~OrderCollectionEndpointRemoteObject()
{
	try
	{
	}
	catch (...)
	{
		poco_unexpected();
	}
}


} // namespace Pizzeria

