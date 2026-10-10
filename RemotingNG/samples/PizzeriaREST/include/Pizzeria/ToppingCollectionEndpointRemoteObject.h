//
// ToppingCollectionEndpointRemoteObject.h
//
// Package: Generated
// Module:  ToppingCollectionEndpointRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef ToppingCollectionEndpointRemoteObject_INCLUDED
#define ToppingCollectionEndpointRemoteObject_INCLUDED


#include "Pizzeria/IToppingCollectionEndpoint.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


class ToppingCollectionEndpointRemoteObject: public Pizzeria::IToppingCollectionEndpoint, public Poco::RemotingNG::RemoteObject
{
public:
	using Ptr = Poco::AutoPtr<ToppingCollectionEndpointRemoteObject>;

	ToppingCollectionEndpointRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::ToppingCollectionEndpoint> pServiceObject);
		/// Creates a ToppingCollectionEndpointRemoteObject.

	virtual ~ToppingCollectionEndpointRemoteObject();
		/// Destroys the ToppingCollectionEndpointRemoteObject.

	std::vector<Pizzeria::ExtTopping> get() const;
		/// Returns a vector containing the available toppings.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	Poco::SharedPtr<Pizzeria::ToppingCollectionEndpoint> _pServiceObject;
};


inline std::vector<Pizzeria::ExtTopping> ToppingCollectionEndpointRemoteObject::get() const
{
	return _pServiceObject->get();
}


inline const Poco::RemotingNG::Identifiable::TypeId& ToppingCollectionEndpointRemoteObject::remoting__typeId() const
{
	return IToppingCollectionEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // ToppingCollectionEndpointRemoteObject_INCLUDED

