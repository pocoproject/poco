//
// PizzaCollectionEndpointRemoteObject.h
//
// Package: Generated
// Module:  PizzaCollectionEndpointRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaCollectionEndpointRemoteObject_INCLUDED
#define PizzaCollectionEndpointRemoteObject_INCLUDED


#include "Pizzeria/IPizzaCollectionEndpoint.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


class PizzaCollectionEndpointRemoteObject: public Pizzeria::IPizzaCollectionEndpoint, public Poco::RemotingNG::RemoteObject
{
public:
	using Ptr = Poco::AutoPtr<PizzaCollectionEndpointRemoteObject>;

	PizzaCollectionEndpointRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::PizzaCollectionEndpoint> pServiceObject);
		/// Creates a PizzaCollectionEndpointRemoteObject.

	virtual ~PizzaCollectionEndpointRemoteObject();
		/// Destroys the PizzaCollectionEndpointRemoteObject.

	std::vector<Pizzeria::Pizza> get() const;
		/// Returns a vector containing the available pizzas.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	Poco::SharedPtr<Pizzeria::PizzaCollectionEndpoint> _pServiceObject;
};


inline std::vector<Pizzeria::Pizza> PizzaCollectionEndpointRemoteObject::get() const
{
	return _pServiceObject->get();
}


inline const Poco::RemotingNG::Identifiable::TypeId& PizzaCollectionEndpointRemoteObject::remoting__typeId() const
{
	return IPizzaCollectionEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // PizzaCollectionEndpointRemoteObject_INCLUDED

