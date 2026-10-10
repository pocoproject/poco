//
// OrderCollectionEndpointServerHelper.cpp
//
// Package: Generated
// Module:  OrderCollectionEndpointServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/OrderCollectionEndpointServerHelper.h"
#include "Pizzeria/OrderCollectionEndpointSkeleton.h"
#include "Poco/RemotingNG/URIUtility.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<OrderCollectionEndpointServerHelper> shOrderCollectionEndpointServerHelper;
}


OrderCollectionEndpointServerHelper::OrderCollectionEndpointServerHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	registerSkeleton();
}


OrderCollectionEndpointServerHelper::~OrderCollectionEndpointServerHelper()
{
}


void OrderCollectionEndpointServerHelper::shutdown()
{
	OrderCollectionEndpointServerHelper::instance().unregisterSkeleton();
	shOrderCollectionEndpointServerHelper.reset();
}


Poco::AutoPtr<Pizzeria::OrderCollectionEndpointRemoteObject> OrderCollectionEndpointServerHelper::createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::OrderCollectionEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	Poco::AutoPtr<OrderCollectionEndpointRemoteObject > pRemoteObject = new OrderCollectionEndpointRemoteObject(oid, pServiceObject);
	pRemoteObject->remoting__setURI(Poco::URI("/pizzeria/orders"));
	return pRemoteObject;
}


OrderCollectionEndpointServerHelper& OrderCollectionEndpointServerHelper::instance()
{
	return *shOrderCollectionEndpointServerHelper.get();
}


std::string OrderCollectionEndpointServerHelper::registerObjectImpl(Poco::AutoPtr<Pizzeria::OrderCollectionEndpointRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return _pORB->registerObject(pRemoteObject, listenerId);
}


void OrderCollectionEndpointServerHelper::registerSkeleton()
{
	_pORB->registerSkeleton("Pizzeria.OrderCollectionEndpoint", new OrderCollectionEndpointSkeleton);
}


void OrderCollectionEndpointServerHelper::unregisterObjectImpl(const std::string& uri)
{
	_pORB->unregisterObject(uri);
}


void OrderCollectionEndpointServerHelper::unregisterSkeleton()
{
	_pORB->unregisterSkeleton("Pizzeria.OrderCollectionEndpoint", true);
}


} // namespace Pizzeria

