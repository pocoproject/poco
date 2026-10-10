//
// PizzaPickupServiceSkeleton.h
//
// Package: Generated
// Module:  PizzaPickupServiceSkeleton
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaPickupServiceSkeleton_INCLUDED
#define PizzaPickupServiceSkeleton_INCLUDED


#include "Pizzeria/PizzaPickupServiceRemoteObject.h"
#include "Poco/RemotingNG/Skeleton.h"


namespace Pizzeria {


class PizzaPickupServiceSkeleton: public Poco::RemotingNG::Skeleton
	/// Pizzeria that allows self pickup.
{
public:
	PizzaPickupServiceSkeleton();
		/// Creates a PizzaPickupServiceSkeleton.

	virtual ~PizzaPickupServiceSkeleton();
		/// Destroys a PizzaPickupServiceSkeleton.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& PizzaPickupServiceSkeleton::remoting__typeId() const
{
	return IPizzaPickupService::remoting__typeId();
}


} // namespace Pizzeria


#endif // PizzaPickupServiceSkeleton_INCLUDED

