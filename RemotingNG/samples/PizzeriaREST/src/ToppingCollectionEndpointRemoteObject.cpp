//
// ToppingCollectionEndpointRemoteObject.cpp
//
// Package: Generated
// Module:  ToppingCollectionEndpointRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/ToppingCollectionEndpointRemoteObject.h"


namespace Pizzeria {


ToppingCollectionEndpointRemoteObject::ToppingCollectionEndpointRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::ToppingCollectionEndpoint> pServiceObject):
	Pizzeria::IToppingCollectionEndpoint(),
	Poco::RemotingNG::RemoteObject(oid),
	_pServiceObject(pServiceObject)
{
}


ToppingCollectionEndpointRemoteObject::~ToppingCollectionEndpointRemoteObject()
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

