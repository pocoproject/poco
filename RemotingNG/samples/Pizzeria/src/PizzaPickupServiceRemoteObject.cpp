//
// PizzaPickupServiceRemoteObject.cpp
//
// Package: Generated
// Module:  PizzaPickupServiceRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaPickupServiceRemoteObject.h"


namespace Pizzeria {


PizzaPickupServiceRemoteObject::PizzaPickupServiceRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::PizzaPickupService> pServiceObject):
	Pizzeria::IPizzaPickupService(),
	Poco::RemotingNG::RemoteObject(oid),
	_pServiceObject(pServiceObject)
{
}


PizzaPickupServiceRemoteObject::~PizzaPickupServiceRemoteObject()
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

