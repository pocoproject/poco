//
// PizzaDeliveryServiceRemoteObject.cpp
//
// Package: Generated
// Module:  PizzaDeliveryServiceRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaDeliveryServiceRemoteObject.h"


namespace Pizzeria {


PizzaDeliveryServiceRemoteObject::PizzaDeliveryServiceRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::PizzaDeliveryService> pServiceObject):
	Pizzeria::IPizzaDeliveryService(),
	Poco::RemotingNG::RemoteObject(oid),
	_pServiceObject(pServiceObject)
{
}


PizzaDeliveryServiceRemoteObject::~PizzaDeliveryServiceRemoteObject()
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

