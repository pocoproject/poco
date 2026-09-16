//
// PizzaPickupServiceClientHelper.cpp
//
// Package: Generated
// Module:  PizzaPickupServiceClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaPickupServiceClientHelper.h"
#include "Pizzeria/PizzaPickupServiceProxyFactory.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<PizzaPickupServiceClientHelper> shPizzaPickupServiceClientHelper;
}


PizzaPickupServiceClientHelper::PizzaPickupServiceClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("Pizzeria.PizzaPickupService", new PizzaPickupServiceProxyFactory);
}


PizzaPickupServiceClientHelper::~PizzaPickupServiceClientHelper()
{
}


IPizzaPickupService::Ptr PizzaPickupServiceClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IPizzaPickupService>();
}


IPizzaPickupService::Ptr PizzaPickupServiceClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IPizzaPickupService::remoting__typeId(), protocol);
	return pInterface.cast<IPizzaPickupService>();
}


PizzaPickupServiceClientHelper& PizzaPickupServiceClientHelper::instance()
{
	return *shPizzaPickupServiceClientHelper.get();
}


} // namespace Pizzeria

