//
// PizzeriaClient.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Pizzeria/ToppingCollectionEndpointClientHelper.h"
#include "Pizzeria/PizzaCollectionEndpointClientHelper.h"
#include "Pizzeria/OrderCollectionEndpointClientHelper.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/RemotingNG/REST/TransportFactory.h"
#include <iostream>


int main(int argc, char** argv)
{
	try
	{
		// register transport
		Poco::RemotingNG::REST::TransportFactory::registerFactory();

		// get proxies for remote objects
		Pizzeria::IToppingCollectionEndpoint::Ptr pToppings = Pizzeria::ToppingCollectionEndpointClientHelper::find("http://127.0.0.1:8080/pizzeria/toppings", "rest");
		Pizzeria::IPizzaCollectionEndpoint::Ptr pPizzas = Pizzeria::PizzaCollectionEndpointClientHelper::find("http://127.0.0.1:8080/pizzeria/pizzas", "rest");
		Pizzeria::IOrderCollectionEndpoint::Ptr pOrders = Pizzeria::OrderCollectionEndpointClientHelper::find("http://127.0.0.1:8080/pizzeria/orders", "rest");

		// invoke methods on remote object

		std::cout << "Toppings: " << std::endl;
		std::vector<Pizzeria::ExtTopping> toppings = pToppings->get();
		for (std::vector<Pizzeria::ExtTopping>::const_iterator it = toppings.begin(); it != toppings.end(); ++it)
		{
			std::cout << it->getName() << std::endl;
		}

		std::cout << std::endl;
		std::cout << "Pizzas: " << std::endl;
		std::vector<Pizzeria::Pizza> pizzas = pPizzas->get();
		for (std::vector<Pizzeria::Pizza>::const_iterator it = pizzas.begin(); it != pizzas.end(); ++it)
		{
			std::cout << it->getName() << std::endl;
		}

		Pizzeria::DeliveryAddress addr;
		addr.setPersonName("Jane Doe");
		addr.setStreet("Walnut Street");
		addr.setHouseNumber(4231);
		addr.setCityName("Springfield");
		addr.setZip("76543");
		addr.setState("NT");
		addr.setPhoneNumber("(939) 123 4567");

		Pizzeria::Order order;
		order.pizza = pizzas[2];
		order.deliveryAddress = addr;

		Pizzeria::Order confirmedOrder = pOrders->post(order);
		std::cout << std::endl << "Delivery Time: " << Poco::DateTimeFormatter::format(confirmedOrder.deliveryTime.value(), Poco::DateTimeFormat::SORTABLE_FORMAT) << std::endl;

		std::cout << std::endl;
		std::cout << "Orders: " << std::endl;
		std::vector<Pizzeria::Order> orders = pOrders->get();
		for (std::vector<Pizzeria::Order>::const_iterator it = orders.begin(); it != orders.end(); ++it)
		{
			std::cout << it->pizza.getName() << " --> " << it->deliveryAddress.getPersonName() << " at " << Poco::DateTimeFormatter::format(it->deliveryTime.value(), Poco::DateTimeFormat::SORTABLE_FORMAT) << std::endl;
		}
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
	return 0;
}
