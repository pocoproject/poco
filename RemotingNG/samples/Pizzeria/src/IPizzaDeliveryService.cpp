//
// IPizzaDeliveryService.cpp
//
// Package: Generated
// Module:  IPizzaDeliveryService
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/IPizzaDeliveryService.h"


namespace Pizzeria {


IPizzaDeliveryService::IPizzaDeliveryService():
	Poco::RefCountedObject()

{
}


IPizzaDeliveryService::~IPizzaDeliveryService()
{
}


const Poco::RemotingNG::Identifiable::TypeId& IPizzaDeliveryService::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("Pizzeria.PizzaDeliveryService");
	return REMOTING__TYPE_ID;
}


} // namespace Pizzeria

