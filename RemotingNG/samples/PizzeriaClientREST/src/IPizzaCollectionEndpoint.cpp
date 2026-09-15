//
// IPizzaCollectionEndpoint.cpp
//
// Package: Generated
// Module:  IPizzaCollectionEndpoint
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/IPizzaCollectionEndpoint.h"


namespace Pizzeria {


IPizzaCollectionEndpoint::IPizzaCollectionEndpoint():
	Poco::RefCountedObject()

{
}


IPizzaCollectionEndpoint::~IPizzaCollectionEndpoint()
{
}


const Poco::RemotingNG::Identifiable::TypeId& IPizzaCollectionEndpoint::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("Pizzeria.PizzaCollectionEndpoint");
	return REMOTING__TYPE_ID;
}


} // namespace Pizzeria

