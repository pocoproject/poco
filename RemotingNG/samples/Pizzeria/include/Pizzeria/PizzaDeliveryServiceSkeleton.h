//
// PizzaDeliveryServiceSkeleton.h
//
// Package: Generated
// Module:  PizzaDeliveryServiceSkeleton
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaDeliveryServiceSkeleton_INCLUDED
#define PizzaDeliveryServiceSkeleton_INCLUDED


#include "Pizzeria/PizzaDeliveryServiceRemoteObject.h"
#include "Poco/RemotingNG/Skeleton.h"


namespace Pizzeria {


class PizzaDeliveryServiceSkeleton: public Poco::RemotingNG::Skeleton
{
public:
	PizzaDeliveryServiceSkeleton();
		/// Creates a PizzaDeliveryServiceSkeleton.

	virtual ~PizzaDeliveryServiceSkeleton();
		/// Destroys a PizzaDeliveryServiceSkeleton.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& PizzaDeliveryServiceSkeleton::remoting__typeId() const
{
	return IPizzaDeliveryService::remoting__typeId();
}


} // namespace Pizzeria


#endif // PizzaDeliveryServiceSkeleton_INCLUDED

