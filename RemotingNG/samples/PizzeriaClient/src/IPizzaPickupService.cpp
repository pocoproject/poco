//
// IPizzaPickupService.cpp
//
// Package: Generated
// Module:  IPizzaPickupService
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/IPizzaPickupService.h"


namespace Pizzeria {


IPizzaPickupService::IPizzaPickupService():
	Pizzeria::IPizzaDeliveryService()

{
}


IPizzaPickupService::~IPizzaPickupService()
{
}


const Poco::RemotingNG::Identifiable::TypeId& IPizzaPickupService::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("Pizzeria.PizzaPickupService");
	return REMOTING__TYPE_ID;
}


} // namespace Pizzeria

