//
// PizzaCollectionEndpointRemoteObject.cpp
//
// Package: Generated
// Module:  PizzaCollectionEndpointRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaCollectionEndpointRemoteObject.h"


namespace Pizzeria {


PizzaCollectionEndpointRemoteObject::PizzaCollectionEndpointRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::PizzaCollectionEndpoint> pServiceObject):
	Pizzeria::IPizzaCollectionEndpoint(),
	Poco::RemotingNG::RemoteObject(oid),
	_pServiceObject(pServiceObject)
{
}


PizzaCollectionEndpointRemoteObject::~PizzaCollectionEndpointRemoteObject()
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

