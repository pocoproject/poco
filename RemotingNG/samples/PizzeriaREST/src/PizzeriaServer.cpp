//
// PizzeriaServer.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Pizzeria/OrderCollection.h"
#include "Pizzeria/OrderCollectionEndpoint.h"
#include "Pizzeria/OrderEndpoint.h"
#include "Pizzeria/PizzaCollectionEndpoint.h"
#include "Pizzeria/ToppingCollectionEndpoint.h"
#include "Pizzeria/OrderCollectionEndpointServerHelper.h"
#include "Pizzeria/OrderEndpointServerHelper.h"
#include "Pizzeria/PizzaCollectionEndpointServerHelper.h"
#include "Pizzeria/ToppingCollectionEndpointServerHelper.h"
#include "Pizzeria/Pizza.h"
#include "Pizzeria/Topping.h"
#include "Poco/RemotingNG/REST/Listener.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>


using namespace Poco::Util;


class PizzeriaServer: public Poco::Util::ServerApplication
	/// The main application class.
	///
	/// This class handles command-line arguments and
	/// configuration files.
	///
	/// Start the PizzeriaServer executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
{
public:
	PizzeriaServer(): _helpRequested(false)
	{
	}

	~PizzeriaServer()
	{
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		ServerApplication::initialize(self);
	}

	void uninitialize()
	{
		ServerApplication::uninitialize();
	}

	void defineOptions(OptionSet& options)
	{
		ServerApplication::defineOptions(options);

		options.addOption(
			Option("help", "h", "Display help information on command line arguments.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<PizzeriaServer>(this, &PizzeriaServer::handleHelp)));
	}

	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("Pizzeria web service sample for Remoting NG.");
		helpFormatter.format(std::cout);
	}

	void initToppings()
	{
		_toppings.push_back(Pizzeria::ExtTopping("Salami", 80));
		_toppings.push_back(Pizzeria::ExtTopping("Onion", 30));
		_toppings.push_back(Pizzeria::ExtTopping("Ham", 80));
		_toppings.push_back(Pizzeria::ExtTopping("Tabasco", 0));
		_toppings.push_back(Pizzeria::ExtTopping("Cheese", 50));
		_toppings.push_back(Pizzeria::ExtTopping("Egg", 40));
		_toppings.push_back(Pizzeria::ExtTopping("Tuna", 70));
		_toppings.push_back(Pizzeria::ExtTopping("Artichoke", 60));
	}

	void initPizzas()
	{
		Pizzeria::Pizza salami("Salami");
		salami.add(_toppings[0]);
		salami.add(_toppings[4]);
		salami.getDetails().calories += 50;
		salami.getDetails().percentFat += 8.2;
		salami.getDetails().carboHydrates += 3.3;
		salami.setBasePrice(600);
		_pizzas.push_back(salami);

		Pizzeria::Pizza tuna("Tuna");
		tuna.add(_toppings[6]);
		tuna.add(_toppings[4]);
		tuna.getDetails().calories += 40;
		tuna.getDetails().percentFat += 3.2;
		tuna.getDetails().carboHydrates += 3.3;
		tuna.setBasePrice(700);
		_pizzas.push_back(tuna);

		Pizzeria::Pizza capricciosa("Capricciosa");
		capricciosa.add(_toppings[2]);
		capricciosa.add(_toppings[4]);
		capricciosa.add(_toppings[7]);
		capricciosa.getDetails().calories += 48;
		capricciosa.getDetails().percentFat += 7.8;
		capricciosa.getDetails().carboHydrates += 4.5;
		capricciosa.setBasePrice(550);
		_pizzas.push_back(capricciosa);
	}

	int main(const std::vector<std::string>& args)
	{
		if (!_helpRequested)
		{
			// create and register REST listener
			Poco::RemotingNG::REST::Listener::Ptr pListener = new Poco::RemotingNG::REST::Listener("0.0.0.0:8080");
			pListener->enableCompression(true);
			std::string listener = Poco::RemotingNG::ORB::instance().registerListener(pListener);

			initToppings();
			initPizzas();

			// register endpoints
			std::string uri1 = Pizzeria::OrderCollectionEndpointServerHelper::registerObject(new Pizzeria::OrderCollectionEndpoint(_orderCollection), "endpoint", listener);
			std::string uri2 = Pizzeria::OrderEndpointServerHelper::registerObject(new Pizzeria::OrderEndpoint(_orderCollection), "endpoint", listener);
			std::string uri3 = Pizzeria::PizzaCollectionEndpointServerHelper::registerObject(new Pizzeria::PizzaCollectionEndpoint(_pizzas), "endpoint", listener);
			std::string uri4 = Pizzeria::ToppingCollectionEndpointServerHelper::registerObject(new Pizzeria::ToppingCollectionEndpoint(_toppings), "endpoint", listener);

			// wait for CTRL-C or kill
			waitForTerminationRequest();

			Pizzeria::OrderCollectionEndpointServerHelper::unregisterObject(uri1);
			Pizzeria::OrderEndpointServerHelper::unregisterObject(uri2);
			Pizzeria::PizzaCollectionEndpointServerHelper::unregisterObject(uri3);
			Pizzeria::ToppingCollectionEndpointServerHelper::unregisterObject(uri4);

			// Stop the HTTPServer
			Poco::RemotingNG::ORB::instance().shutdown();
		}
		return Application::EXIT_OK;
	}

private:
	bool _helpRequested;
	std::vector<Pizzeria::Pizza> _pizzas;
	std::vector<Pizzeria::ExtTopping> _toppings;
	Pizzeria::OrderCollection _orderCollection;
};


POCO_SERVER_MAIN(PizzeriaServer)
