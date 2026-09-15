//
// ToppingCollectionEndpointServerHelper.cpp
//
// Package: Generated
// Module:  ToppingCollectionEndpointServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/ToppingCollectionEndpointServerHelper.h"
#include "Pizzeria/ToppingCollectionEndpointSkeleton.h"
#include "Poco/RemotingNG/URIUtility.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<ToppingCollectionEndpointServerHelper> shToppingCollectionEndpointServerHelper;
}


ToppingCollectionEndpointServerHelper::ToppingCollectionEndpointServerHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	registerSkeleton();
}


ToppingCollectionEndpointServerHelper::~ToppingCollectionEndpointServerHelper()
{
}


void ToppingCollectionEndpointServerHelper::shutdown()
{
	ToppingCollectionEndpointServerHelper::instance().unregisterSkeleton();
	shToppingCollectionEndpointServerHelper.reset();
}


Poco::AutoPtr<Pizzeria::ToppingCollectionEndpointRemoteObject> ToppingCollectionEndpointServerHelper::createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::ToppingCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	Poco::AutoPtr<ToppingCollectionEndpointRemoteObject > pRemoteObject = new ToppingCollectionEndpointRemoteObject(oid, pServiceObject);
	pRemoteObject->remoting__setURI(Poco::URI("/pizzeria/toppings"));
	return pRemoteObject;
}


ToppingCollectionEndpointServerHelper& ToppingCollectionEndpointServerHelper::instance()
{
	return *shToppingCollectionEndpointServerHelper.get();
}


std::string ToppingCollectionEndpointServerHelper::registerObjectImpl(Poco::AutoPtr<Pizzeria::ToppingCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return _pORB->registerObject(pRemoteObject, listenerId);
}


void ToppingCollectionEndpointServerHelper::registerSkeleton()
{
	_pORB->registerSkeleton("Pizzeria.ToppingCollectionEndpoint", new ToppingCollectionEndpointSkeleton);
}


void ToppingCollectionEndpointServerHelper::unregisterObjectImpl(const std::string& uri)
{
	_pORB->unregisterObject(uri);
}


void ToppingCollectionEndpointServerHelper::unregisterSkeleton()
{
	_pORB->unregisterSkeleton("Pizzeria.ToppingCollectionEndpoint", true);
}


} // namespace Pizzeria

