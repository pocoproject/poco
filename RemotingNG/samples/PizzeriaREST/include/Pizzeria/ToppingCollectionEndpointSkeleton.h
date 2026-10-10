//
// ToppingCollectionEndpointSkeleton.h
//
// Package: Generated
// Module:  ToppingCollectionEndpointSkeleton
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef ToppingCollectionEndpointSkeleton_INCLUDED
#define ToppingCollectionEndpointSkeleton_INCLUDED


#include "Pizzeria/ToppingCollectionEndpointRemoteObject.h"
#include "Poco/RemotingNG/Skeleton.h"


namespace Pizzeria {


class ToppingCollectionEndpointSkeleton: public Poco::RemotingNG::Skeleton
{
public:
	ToppingCollectionEndpointSkeleton();
		/// Creates a ToppingCollectionEndpointSkeleton.

	virtual ~ToppingCollectionEndpointSkeleton();
		/// Destroys a ToppingCollectionEndpointSkeleton.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& ToppingCollectionEndpointSkeleton::remoting__typeId() const
{
	return IToppingCollectionEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // ToppingCollectionEndpointSkeleton_INCLUDED

