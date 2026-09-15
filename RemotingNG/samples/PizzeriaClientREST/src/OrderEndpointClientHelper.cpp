//
// OrderEndpointClientHelper.cpp
//
// Package: Generated
// Module:  OrderEndpointClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/OrderEndpointClientHelper.h"
#include "Pizzeria/OrderEndpointProxyFactory.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<OrderEndpointClientHelper> shOrderEndpointClientHelper;
}


OrderEndpointClientHelper::OrderEndpointClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("Pizzeria.OrderEndpoint", new OrderEndpointProxyFactory);
}


OrderEndpointClientHelper::~OrderEndpointClientHelper()
{
}


IOrderEndpoint::Ptr OrderEndpointClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IOrderEndpoint>();
}


IOrderEndpoint::Ptr OrderEndpointClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IOrderEndpoint::remoting__typeId(), protocol);
	return pInterface.cast<IOrderEndpoint>();
}


OrderEndpointClientHelper& OrderEndpointClientHelper::instance()
{
	return *shOrderEndpointClientHelper.get();
}


} // namespace Pizzeria

