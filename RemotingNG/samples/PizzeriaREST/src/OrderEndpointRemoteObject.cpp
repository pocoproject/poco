//
// OrderEndpointRemoteObject.cpp
//
// Package: Generated
// Module:  OrderEndpointRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/OrderEndpointRemoteObject.h"


namespace Pizzeria {


OrderEndpointRemoteObject::OrderEndpointRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::OrderEndpoint> pServiceObject):
	Pizzeria::IOrderEndpoint(),
	Poco::RemotingNG::RemoteObject(oid),
	_pServiceObject(pServiceObject)
{
}


OrderEndpointRemoteObject::~OrderEndpointRemoteObject()
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

