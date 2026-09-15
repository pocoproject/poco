//
// IToppingCollectionEndpoint.h
//
// Package: Generated
// Module:  IToppingCollectionEndpoint
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef IToppingCollectionEndpoint_INCLUDED
#define IToppingCollectionEndpoint_INCLUDED


#include "Pizzeria/ToppingCollectionEndpoint.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"


namespace Pizzeria {


class IToppingCollectionEndpoint: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<IToppingCollectionEndpoint>;

	IToppingCollectionEndpoint();
		/// Creates a IToppingCollectionEndpoint.

	virtual ~IToppingCollectionEndpoint();
		/// Destroys the IToppingCollectionEndpoint.

	virtual std::vector < Pizzeria::ExtTopping > get() const = 0;
		/// Returns a vector containing the available toppings.

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace Pizzeria


#endif // IToppingCollectionEndpoint_INCLUDED

