//
// IPizzaPickupService.h
//
// Package: Generated
// Module:  IPizzaPickupService
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef IPizzaPickupService_INCLUDED
#define IPizzaPickupService_INCLUDED


#include "Pizzeria/IPizzaDeliveryService.h"
#include "Pizzeria/PizzaPickupService.h"


namespace Pizzeria {


class IPizzaPickupService: public Pizzeria::IPizzaDeliveryService
	/// Pizzeria that allows self pickup.
{
public:
	using Ptr = Poco::AutoPtr<IPizzaPickupService>;

	IPizzaPickupService();
		/// Creates a IPizzaPickupService.

	virtual ~IPizzaPickupService();
		/// Destroys the IPizzaPickupService.

	virtual Poco::DateTime orderForSelfPickup(const Pizzeria::Pizza& pizza, const std::string& phoneNumber) = 0;
		/// Order a pizza, send it to the given deliveryAddress.
		/// Returns the expected time when the Pizza will arrive.

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace Pizzeria


#endif // IPizzaPickupService_INCLUDED

