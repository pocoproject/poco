//
// OrderCollectionEndpointSkeleton.h
//
// Package: Generated
// Module:  OrderCollectionEndpointSkeleton
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef OrderCollectionEndpointSkeleton_INCLUDED
#define OrderCollectionEndpointSkeleton_INCLUDED


#include "Pizzeria/OrderCollectionEndpointRemoteObject.h"
#include "Poco/RemotingNG/Skeleton.h"


namespace Pizzeria {


class OrderCollectionEndpointSkeleton: public Poco::RemotingNG::Skeleton
{
public:
	OrderCollectionEndpointSkeleton();
		/// Creates a OrderCollectionEndpointSkeleton.

	virtual ~OrderCollectionEndpointSkeleton();
		/// Destroys a OrderCollectionEndpointSkeleton.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& OrderCollectionEndpointSkeleton::remoting__typeId() const
{
	return IOrderCollectionEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // OrderCollectionEndpointSkeleton_INCLUDED

