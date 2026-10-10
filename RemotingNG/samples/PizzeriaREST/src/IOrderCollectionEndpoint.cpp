//
// IOrderCollectionEndpoint.cpp
//
// Package: Generated
// Module:  IOrderCollectionEndpoint
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/IOrderCollectionEndpoint.h"


namespace Pizzeria {


IOrderCollectionEndpoint::IOrderCollectionEndpoint():
	Poco::RefCountedObject()

{
}


IOrderCollectionEndpoint::~IOrderCollectionEndpoint()
{
}


const Poco::RemotingNG::Identifiable::TypeId& IOrderCollectionEndpoint::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("Pizzeria.OrderCollectionEndpoint");
	return REMOTING__TYPE_ID;
}


} // namespace Pizzeria

