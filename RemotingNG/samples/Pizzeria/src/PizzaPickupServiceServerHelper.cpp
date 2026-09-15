//
// PizzaPickupServiceServerHelper.cpp
//
// Package: Generated
// Module:  PizzaPickupServiceServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaPickupServiceServerHelper.h"
#include "Pizzeria/PizzaPickupServiceSkeleton.h"
#include "Poco/RemotingNG/URIUtility.h"
#include "Poco/SingletonHolder.h"


namespace Pizzeria {


namespace
{
	Poco::SingletonHolder<PizzaPickupServiceServerHelper> shPizzaPickupServiceServerHelper;
}


PizzaPickupServiceServerHelper::PizzaPickupServiceServerHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	registerSkeleton();
}


PizzaPickupServiceServerHelper::~PizzaPickupServiceServerHelper()
{
}


void PizzaPickupServiceServerHelper::shutdown()
{
	PizzaPickupServiceServerHelper::instance().unregisterSkeleton();
	shPizzaPickupServiceServerHelper.reset();
}


Poco::AutoPtr<Pizzeria::PizzaPickupServiceRemoteObject> PizzaPickupServiceServerHelper::createRemoteObjectImpl(Poco::SharedPtr<Pizzeria::PizzaPickupService> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	return new PizzaPickupServiceRemoteObject(oid, pServiceObject);
}


PizzaPickupServiceServerHelper& PizzaPickupServiceServerHelper::instance()
{
	return *shPizzaPickupServiceServerHelper.get();
}


std::string PizzaPickupServiceServerHelper::registerObjectImpl(Poco::AutoPtr<Pizzeria::PizzaPickupServiceRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return _pORB->registerObject(pRemoteObject, listenerId);
}


void PizzaPickupServiceServerHelper::registerSkeleton()
{
	_pORB->registerSkeleton("Pizzeria.PizzaPickupService", new PizzaPickupServiceSkeleton);
}


void PizzaPickupServiceServerHelper::unregisterObjectImpl(const std::string& uri)
{
	_pORB->unregisterObject(uri);
}


void PizzaPickupServiceServerHelper::unregisterSkeleton()
{
	_pORB->unregisterSkeleton("Pizzeria.PizzaPickupService", true);
}


} // namespace Pizzeria

