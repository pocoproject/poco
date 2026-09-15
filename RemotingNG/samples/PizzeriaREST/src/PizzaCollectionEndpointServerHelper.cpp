//
// PizzaCollectionEndpointServerHelper.cpp
//
// Package: Generated
// Module:  PizzaCollectionEndpointServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaCollectionEndpointServerHelper.h"
#include "Pizzeria/PizzaCollectionEndpointSkeleton.h"
#include "Poco/RemotingNG/URIUtility.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<PizzaCollectionEndpointServerHelper> shPizzaCollectionEndpointServerHelper;
}


PizzaCollectionEndpointServerHelper::PizzaCollectionEndpointServerHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	registerSkeleton();
}


PizzaCollectionEndpointServerHelper::~PizzaCollectionEndpointServerHelper()
{
}


void PizzaCollectionEndpointServerHelper::shutdown()
{
	PizzaCollectionEndpointServerHelper::instance().unregisterSkeleton();
	shPizzaCollectionEndpointServerHelper.reset();
}


Poco::AutoPtr<Pizzeria::PizzaCollectionEndpointRemoteObject> PizzaCollectionEndpointServerHelper::createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::PizzaCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	Poco::AutoPtr<PizzaCollectionEndpointRemoteObject > pRemoteObject = new PizzaCollectionEndpointRemoteObject(oid, pServiceObject);
	pRemoteObject->remoting__setURI(Poco::URI("/pizzeria/pizzas"));
	return pRemoteObject;
}


PizzaCollectionEndpointServerHelper& PizzaCollectionEndpointServerHelper::instance()
{
	return *shPizzaCollectionEndpointServerHelper.get();
}


std::string PizzaCollectionEndpointServerHelper::registerObjectImpl(Poco::AutoPtr<Pizzeria::PizzaCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return _pORB->registerObject(pRemoteObject, listenerId);
}


void PizzaCollectionEndpointServerHelper::registerSkeleton()
{
	_pORB->registerSkeleton("Pizzeria.PizzaCollectionEndpoint", new PizzaCollectionEndpointSkeleton);
}


void PizzaCollectionEndpointServerHelper::unregisterObjectImpl(const std::string& uri)
{
	_pORB->unregisterObject(uri);
}


void PizzaCollectionEndpointServerHelper::unregisterSkeleton()
{
	_pORB->unregisterSkeleton("Pizzeria.PizzaCollectionEndpoint", true);
}


} // namespace Pizzeria

