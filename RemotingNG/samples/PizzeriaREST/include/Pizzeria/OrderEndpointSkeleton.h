//
// OrderEndpointSkeleton.h
//
// Package: Generated
// Module:  OrderEndpointSkeleton
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef OrderEndpointSkeleton_INCLUDED
#define OrderEndpointSkeleton_INCLUDED


#include "Pizzeria/OrderEndpointRemoteObject.h"
#include "Poco/RemotingNG/Skeleton.h"


namespace Pizzeria {


class OrderEndpointSkeleton: public Poco::RemotingNG::Skeleton
{
public:
	OrderEndpointSkeleton();
		/// Creates a OrderEndpointSkeleton.

	virtual ~OrderEndpointSkeleton();
		/// Destroys a OrderEndpointSkeleton.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& OrderEndpointSkeleton::remoting__typeId() const
{
	return IOrderEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // OrderEndpointSkeleton_INCLUDED

