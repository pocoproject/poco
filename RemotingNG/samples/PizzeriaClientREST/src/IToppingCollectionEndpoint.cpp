//
// IToppingCollectionEndpoint.cpp
//
// Package: Generated
// Module:  IToppingCollectionEndpoint
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/IToppingCollectionEndpoint.h"


namespace Pizzeria {


IToppingCollectionEndpoint::IToppingCollectionEndpoint():
	Poco::RefCountedObject()

{
}


IToppingCollectionEndpoint::~IToppingCollectionEndpoint()
{
}


const Poco::RemotingNG::Identifiable::TypeId& IToppingCollectionEndpoint::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("Pizzeria.ToppingCollectionEndpoint");
	return REMOTING__TYPE_ID;
}


} // namespace Pizzeria

