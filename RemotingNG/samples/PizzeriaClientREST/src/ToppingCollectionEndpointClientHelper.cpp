//
// ToppingCollectionEndpointClientHelper.cpp
//
// Package: Generated
// Module:  ToppingCollectionEndpointClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/ToppingCollectionEndpointClientHelper.h"
#include "Pizzeria/ToppingCollectionEndpointProxyFactory.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<ToppingCollectionEndpointClientHelper> shToppingCollectionEndpointClientHelper;
}


ToppingCollectionEndpointClientHelper::ToppingCollectionEndpointClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("Pizzeria.ToppingCollectionEndpoint", new ToppingCollectionEndpointProxyFactory);
}


ToppingCollectionEndpointClientHelper::~ToppingCollectionEndpointClientHelper()
{
}


IToppingCollectionEndpoint::Ptr ToppingCollectionEndpointClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IToppingCollectionEndpoint>();
}


IToppingCollectionEndpoint::Ptr ToppingCollectionEndpointClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IToppingCollectionEndpoint::remoting__typeId(), protocol);
	return pInterface.cast<IToppingCollectionEndpoint>();
}


ToppingCollectionEndpointClientHelper& ToppingCollectionEndpointClientHelper::instance()
{
	return *shToppingCollectionEndpointClientHelper.get();
}


} // namespace Pizzeria

