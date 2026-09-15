//
// PizzeriaClient.cpp
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//
// NOTE: The code for invoking the PizzaDeliveryService web service has been
// generated from the WSDL document for PizzaDeliveryService using the
// XSDCodeGen and RemoteGenNG tools.
//


#include "Pizzeria/PizzaDeliveryServiceClientHelper.h"
#include "Pizzeria/DeliveryAddress.h"
#include "Pizzeria/Topping.h"
#include "Pizzeria/Pizza.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/RemotingNG/SOAP/TransportFactory.h"
#include <iostream>


int main(int argc, char** argv)
{
	try
	{
		// register transport
		Poco::RemotingNG::SOAP::TransportFactory::registerFactory();

		// get proxy for remote object
		Pizzeria::IPizzaDeliveryService::Ptr pPizzeria = Pizzeria::PizzaDeliveryServiceClientHelper::find("http://127.0.0.1:8080/soap/Pizzeria.PizzaDeliveryService/ThePizzeria");

		// invoke methods on remote object

		std::cout << "Toppings: " << std::endl;
		std::vector<Pizzeria::ExtTopping> toppings;
		pPizzeria->getToppings(toppings);
		for (std::vector<Pizzeria::ExtTopping>::const_iterator it = toppings.begin(); it != toppings.end(); ++it)
		{
			std::cout << it->getName() << std::endl;
		}

		std::cout << std::endl;
		std::cout << "Pizzas: " << std::endl;
		std::vector<std::string> names;
		pPizzeria->getPizzaNames(names);
		for (std::vector<std::string>::const_iterator it = names.begin(); it != names.end(); ++it)
		{
			std::cout << *it << std::endl;
		}

		std::vector<Pizzeria::Pizza> pizzas;
		pPizzeria->getPizzas(pizzas);

		Pizzeria::DeliveryAddress addr;
		addr.setPersonName("Jane Doe");
		addr.setStreetName("Walnut Street");
		addr.setHouseNo(4231);
		addr.setCityName("Springfield");
		addr.setZip("76543");
		addr.setState("NT");
		addr.setPhoneNo("(939) 123 4567");

		Poco::DateTime deliveryTime;
		pPizzeria->order(addr, pizzas[2], deliveryTime);
		std::cout << std::endl << "Delivery Time: " << Poco::DateTimeFormatter::format(deliveryTime, Poco::DateTimeFormat::SORTABLE_FORMAT) << std::endl;
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
	return 0;
}
