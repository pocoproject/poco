//
// IPizzaCollectionEndpoint.h
//
// Package: Generated
// Module:  IPizzaCollectionEndpoint
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef IPizzaCollectionEndpoint_INCLUDED
#define IPizzaCollectionEndpoint_INCLUDED


#include "Pizzeria/PizzaCollectionEndpoint.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"


namespace Pizzeria {


class IPizzaCollectionEndpoint: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<IPizzaCollectionEndpoint>;

	IPizzaCollectionEndpoint();
		/// Creates a IPizzaCollectionEndpoint.

	virtual ~IPizzaCollectionEndpoint();
		/// Destroys the IPizzaCollectionEndpoint.

	virtual std::vector<Pizzeria::Pizza> get() const = 0;
		/// Returns a vector containing the available pizzas.

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace Pizzeria


#endif // IPizzaCollectionEndpoint_INCLUDED

