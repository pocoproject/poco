//
// PizzaDeliveryServiceClientHelper.cpp
//
// Package: Generated
// Module:  PizzaDeliveryServiceClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaDeliveryServiceClientHelper.h"
#include "Pizzeria/PizzaDeliveryServiceProxyFactory.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<PizzaDeliveryServiceClientHelper> shPizzaDeliveryServiceClientHelper;
}


PizzaDeliveryServiceClientHelper::PizzaDeliveryServiceClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("Pizzeria.PizzaDeliveryService", new PizzaDeliveryServiceProxyFactory);
}


PizzaDeliveryServiceClientHelper::~PizzaDeliveryServiceClientHelper()
{
}


IPizzaDeliveryService::Ptr PizzaDeliveryServiceClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IPizzaDeliveryService>();
}


IPizzaDeliveryService::Ptr PizzaDeliveryServiceClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IPizzaDeliveryService::remoting__typeId(), protocol);
	return pInterface.cast<IPizzaDeliveryService>();
}


PizzaDeliveryServiceClientHelper& PizzaDeliveryServiceClientHelper::instance()
{
	return *shPizzaDeliveryServiceClientHelper.get();
}


} // namespace Pizzeria

