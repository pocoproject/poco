//
// Server.cpp
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Pizzeria/PizzaDeliveryService.h"
#include "Pizzeria/PizzaPickupService.h"
#include "Pizzeria/PizzaDeliveryServiceServerHelper.h"
#include "Pizzeria/PizzaPickupServiceServerHelper.h"
#include "Poco/RemotingNG/SOAP/Listener.h"
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

	int main(const std::vector<std::string>& args)
	{
		if (!_helpRequested)
		{
			// create and register SOAP listener
			Poco::RemotingNG::SOAP::Listener::Ptr pListener = new Poco::RemotingNG::SOAP::Listener("0.0.0.0:8080", "wsdl");
			pListener->enableCompression(true);
			std::string listener = Poco::RemotingNG::ORB::instance().registerListener(pListener);

			// register Pizzeria remote object
			std::string uri1 = Pizzeria::PizzaDeliveryServiceServerHelper::registerObject(new Pizzeria::PizzaDeliveryService, "ThePizzeria", listener);
			std::string uri2 = Pizzeria::PizzaPickupServiceServerHelper::registerObject(new Pizzeria::PizzaPickupService, "ThePizzeria", listener);

			// wait for CTRL-C or kill
			waitForTerminationRequest();

			Pizzeria::PizzaDeliveryServiceServerHelper::unregisterObject(uri1);
			Pizzeria::PizzaPickupServiceServerHelper::unregisterObject(uri2);

			// Stop the HTTPServer
			Poco::RemotingNG::ORB::instance().shutdown();
		}
		return Application::EXIT_OK;
	}

private:
	bool _helpRequested;
};


POCO_SERVER_MAIN(PizzeriaServer)
