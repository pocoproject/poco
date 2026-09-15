//
// PizzaCollectionEndpointSkeleton.h
//
// Package: Generated
// Module:  PizzaCollectionEndpointSkeleton
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaCollectionEndpointSkeleton_INCLUDED
#define PizzaCollectionEndpointSkeleton_INCLUDED


#include "Pizzeria/PizzaCollectionEndpointRemoteObject.h"
#include "Poco/RemotingNG/Skeleton.h"


namespace Pizzeria {


class PizzaCollectionEndpointSkeleton: public Poco::RemotingNG::Skeleton
{
public:
	PizzaCollectionEndpointSkeleton();
		/// Creates a PizzaCollectionEndpointSkeleton.

	virtual ~PizzaCollectionEndpointSkeleton();
		/// Destroys a PizzaCollectionEndpointSkeleton.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& PizzaCollectionEndpointSkeleton::remoting__typeId() const
{
	return IPizzaCollectionEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // PizzaCollectionEndpointSkeleton_INCLUDED

