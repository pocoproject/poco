//
// TimeServer.cpp
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TimeService.h"
#include "TimeServiceServerHelper.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/RemotingNG/TCP/Listener.h"
#include "Poco/RemotingNG/TCP/TransportFactory.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/File.h"
#include <iostream>


using namespace Poco::Util;


class TimeServer: public Poco::Util::ServerApplication
	/// The main application class.
	///
	/// This class handles command-line arguments and
	/// configuration files.
	///
	/// Start the TimeServer executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
{
public:
	TimeServer(): _helpRequested(false)
	{
	}
	
	~TimeServer()
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
				.callback(OptionCallback<TimeServer>(this, &TimeServer::handleHelp)));
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
		helpFormatter.setHeader("A TCP service providing the current time.");
		helpFormatter.format(std::cout);
	}

	int main(const std::vector<std::string>& args)
	{
		if (!_helpRequested)
		{
			// register TCP transport (for events)
			Poco::RemotingNG::TCP::TransportFactory::registerFactory();

			// register UDS listener
			Poco::File socketFile("/tmp/TimeServer.sock");
			if (socketFile.exists()) socketFile.remove();

			Poco::RemotingNG::TCP::Listener::Ptr pListener = new Poco::RemotingNG::TCP::Listener(socketFile.path());
			std::string listener = Poco::RemotingNG::ORB::instance().registerListener(pListener);

			// register TimeService service object
			std::string uri = Services::TimeServiceServerHelper::registerObject(new Services::TimeService, "TheClock", listener);
			Services::TimeServiceServerHelper::enableEvents(uri, pListener->protocol());

			std::cout << "Service URI is: " << uri << std::endl;
			
			// wait for CTRL-C or kill
			waitForTerminationRequest();
			// Stop the HTTPServer
			Poco::RemotingNG::ORB::instance().shutdown();
		}
		return Application::EXIT_OK;
	}
	
private:
	bool _helpRequested;
};


POCO_SERVER_MAIN(TimeServer)
