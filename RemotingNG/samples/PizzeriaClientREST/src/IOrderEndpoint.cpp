//
// IOrderEndpoint.cpp
//
// Package: Generated
// Module:  IOrderEndpoint
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/IOrderEndpoint.h"


namespace Pizzeria {


IOrderEndpoint::IOrderEndpoint():
	Poco::RefCountedObject()

{
}


IOrderEndpoint::~IOrderEndpoint()
{
}


const Poco::RemotingNG::Identifiable::TypeId& IOrderEndpoint::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("Pizzeria.OrderEndpoint");
	return REMOTING__TYPE_ID;
}


} // namespace Pizzeria

