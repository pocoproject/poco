//
// PizzaCollectionEndpointClientHelper.cpp
//
// Package: Generated
// Module:  PizzaCollectionEndpointClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaCollectionEndpointClientHelper.h"
#include "Pizzeria/PizzaCollectionEndpointProxyFactory.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<PizzaCollectionEndpointClientHelper> shPizzaCollectionEndpointClientHelper;
}


PizzaCollectionEndpointClientHelper::PizzaCollectionEndpointClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("Pizzeria.PizzaCollectionEndpoint", new PizzaCollectionEndpointProxyFactory);
}


PizzaCollectionEndpointClientHelper::~PizzaCollectionEndpointClientHelper()
{
}


IPizzaCollectionEndpoint::Ptr PizzaCollectionEndpointClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IPizzaCollectionEndpoint>();
}


IPizzaCollectionEndpoint::Ptr PizzaCollectionEndpointClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IPizzaCollectionEndpoint::remoting__typeId(), protocol);
	return pInterface.cast<IPizzaCollectionEndpoint>();
}


PizzaCollectionEndpointClientHelper& PizzaCollectionEndpointClientHelper::instance()
{
	return *shPizzaCollectionEndpointClientHelper.get();
}


} // namespace Pizzeria

