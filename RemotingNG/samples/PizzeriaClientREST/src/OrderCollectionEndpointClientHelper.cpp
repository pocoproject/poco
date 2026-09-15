//
// OrderCollectionEndpointClientHelper.cpp
//
// Package: Generated
// Module:  OrderCollectionEndpointClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/OrderCollectionEndpointClientHelper.h"
#include "Pizzeria/OrderCollectionEndpointProxyFactory.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<OrderCollectionEndpointClientHelper> shOrderCollectionEndpointClientHelper;
}


OrderCollectionEndpointClientHelper::OrderCollectionEndpointClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("Pizzeria.OrderCollectionEndpoint", new OrderCollectionEndpointProxyFactory);
}


OrderCollectionEndpointClientHelper::~OrderCollectionEndpointClientHelper()
{
}


IOrderCollectionEndpoint::Ptr OrderCollectionEndpointClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IOrderCollectionEndpoint>();
}


IOrderCollectionEndpoint::Ptr OrderCollectionEndpointClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IOrderCollectionEndpoint::remoting__typeId(), protocol);
	return pInterface.cast<IOrderCollectionEndpoint>();
}


OrderCollectionEndpointClientHelper& OrderCollectionEndpointClientHelper::instance()
{
	return *shOrderCollectionEndpointClientHelper.get();
}


} // namespace Pizzeria

