//
// OrderEndpointServerHelper.cpp
//
// Package: Generated
// Module:  OrderEndpointServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/OrderEndpointServerHelper.h"
#include "Pizzeria/OrderEndpointSkeleton.h"
#include "Poco/RemotingNG/URIUtility.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<OrderEndpointServerHelper> shOrderEndpointServerHelper;
}


OrderEndpointServerHelper::OrderEndpointServerHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	registerSkeleton();
}


OrderEndpointServerHelper::~OrderEndpointServerHelper()
{
}


void OrderEndpointServerHelper::shutdown()
{
	OrderEndpointServerHelper::instance().unregisterSkeleton();
	shOrderEndpointServerHelper.reset();
}


Poco::AutoPtr<Pizzeria::OrderEndpointRemoteObject> OrderEndpointServerHelper::createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::OrderEndpoint> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	Poco::AutoPtr<OrderEndpointRemoteObject > pRemoteObject = new OrderEndpointRemoteObject(oid, pServiceObject);
	pRemoteObject->remoting__setURI(Poco::URI("/pizzeria/orders/{orderNumber}"));
	return pRemoteObject;
}


OrderEndpointServerHelper& OrderEndpointServerHelper::instance()
{
	return *shOrderEndpointServerHelper.get();
}


std::string OrderEndpointServerHelper::registerObjectImpl(Poco::AutoPtr<Pizzeria::OrderEndpointRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return _pORB->registerObject(pRemoteObject, listenerId);
}


void OrderEndpointServerHelper::registerSkeleton()
{
	_pORB->registerSkeleton("Pizzeria.OrderEndpoint", new OrderEndpointSkeleton);
}


void OrderEndpointServerHelper::unregisterObjectImpl(const std::string& uri)
{
	_pORB->unregisterObject(uri);
}


void OrderEndpointServerHelper::unregisterSkeleton()
{
	_pORB->unregisterSkeleton("Pizzeria.OrderEndpoint", true);
}


} // namespace Pizzeria

