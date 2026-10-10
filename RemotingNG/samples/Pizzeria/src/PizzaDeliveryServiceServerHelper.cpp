//
// PizzaDeliveryServiceServerHelper.cpp
//
// Package: Generated
// Module:  PizzaDeliveryServiceServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaDeliveryServiceServerHelper.h"
#include "Pizzeria/PizzaDeliveryServiceSkeleton.h"
#include "Poco/RemotingNG/URIUtility.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<PizzaDeliveryServiceServerHelper> shPizzaDeliveryServiceServerHelper;
}


PizzaDeliveryServiceServerHelper::PizzaDeliveryServiceServerHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	registerSkeleton();
}


PizzaDeliveryServiceServerHelper::~PizzaDeliveryServiceServerHelper()
{
}


void PizzaDeliveryServiceServerHelper::shutdown()
{
	PizzaDeliveryServiceServerHelper::instance().unregisterSkeleton();
	shPizzaDeliveryServiceServerHelper.reset();
}


Poco::AutoPtr<Pizzeria::PizzaDeliveryServiceRemoteObject> PizzaDeliveryServiceServerHelper::createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::PizzaDeliveryService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	return new PizzaDeliveryServiceRemoteObject(oid, pServiceObject);
}


PizzaDeliveryServiceServerHelper& PizzaDeliveryServiceServerHelper::instance()
{
	return *shPizzaDeliveryServiceServerHelper.get();
}


std::string PizzaDeliveryServiceServerHelper::registerObjectImpl(Poco::AutoPtr<Pizzeria::PizzaDeliveryServiceRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return _pORB->registerObject(pRemoteObject, listenerId);
}


void PizzaDeliveryServiceServerHelper::registerSkeleton()
{
	_pORB->registerSkeleton("Pizzeria.PizzaDeliveryService", new PizzaDeliveryServiceSkeleton);
}


void PizzaDeliveryServiceServerHelper::unregisterObjectImpl(const std::string& uri)
{
	_pORB->unregisterObject(uri);
}


void PizzaDeliveryServiceServerHelper::unregisterSkeleton()
{
	_pORB->unregisterSkeleton("Pizzeria.PizzaDeliveryService", true);
}


} // namespace Pizzeria

