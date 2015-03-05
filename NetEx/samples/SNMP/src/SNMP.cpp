//
// SNMP.cpp
//
// $Id: //poco/1.4/Net/samples/SNMP/src/SNMP.cpp#1 $
//
// This sample demonstrates the Application class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Net/SNMPClient.h"
#include "Poco/AutoPtr.h"
#include "Poco/Delegate.h"
#include <iostream>
#include <sstream>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Net::SNMPClient;
using Poco::Net::IPAddress;
using Poco::Net::SNMPEventArgs;
using Poco::AutoPtr;
using Poco::Delegate;


class SNMP: public Application
	/// This sample demonstrates the Poco::Net::SNMPClient in conjunction with 
	/// Poco Foundation C#-like events functionality.
	///
	/// Try SNMP --help (on Unix platforms) or SNMP /help (elsewhere) for
	/// more information.
{
public:
	SNMP(): 
		_helpRequested(false), 
		_snmpClient(),
		_oid("1.3.6.1.2.1.1.1.0"), 
		_target("localhost")
	{
	}

protected:	
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);
		
		_snmpClient.snmpBegin += delegate(this, &SNMP::onBegin);
		_snmpClient.snmpReply += delegate(this, &SNMP::onReply);
		_snmpClient.snmpError += delegate(this, &SNMP::onError);
		_snmpClient.snmpEnd   += delegate(this, &SNMP::onEnd);
	}
	
	void uninitialize()
	{
		_snmpClient.snmpBegin -= delegate(this, &SNMP::onBegin);
		_snmpClient.snmpReply -= delegate(this, &SNMP::onReply);
		_snmpClient.snmpError -= delegate(this, &SNMP::onError);
		_snmpClient.snmpEnd   -= delegate(this, &SNMP::onEnd);

		Application::uninitialize();
	}

	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false));

		options.addOption(
			Option("oid", "o", "define the target SNMP OID")
				.required(false)
				.repeatable(false)
				.argument("repetitions"));
				
		options.addOption(
			Option("target", "t", "define the target address")
				.required(false)
				.repeatable(false)
				.argument("target"));
	}
	
	void handleOption(const std::string& name, const std::string& value)
	{
		Application::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
		else if (name == "oid")
			_oid = value;
		else if (name == "target")
			_target = value;
	}
	
	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader(
			"A sample application that demonstrates the functionality of the "
			"Poco::Net::SNMPClient class in conjunction with Poco::Events package functionality.");
		helpFormatter.format(std::cout);
	}


	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested) 
			displayHelp();
		else 
		{
			_snmpClient.get(_target, _oid, 920);
		}
		
		return Application::EXIT_OK;
	}


	void onBegin(const void* pSender, SNMPEventArgs& args)
	{
		std::ostringstream os;
		os << "Querying " << args.hostName() << " [" << args.hostAddress() << "] with " << _oid << " OID:" 
		   << std::endl << "---------------------------------------------" << std::endl;
		logger().information(os.str());
	}

	void onReply(const void* pSender, SNMPEventArgs& args)
	{
		std::ostringstream os;
		os << "Reply from " << args.hostAddress() << std::endl;

		for (int i=0; i < args.message()->pdu().varBindList().list().size(); i++)
		{
			std::string curoid(args.message()->pdu().varBindList().list().at(i)->oid());
			os << "-- " << curoid << " :: " << 
				args.message()->pdu().varBindList().list().at(i)->value()->typeName() << " == " << 
				args.message()->pdu().varBindList().list().at(i)->value()->toString() << std::endl;
		}

		logger().information(os.str());
	}

	void onError(const void* pSender, SNMPEventArgs& args)
	{
		std::ostringstream os;
		os << args.error();
		logger().information(os.str());
	}

	void onEnd(const void* pSender, SNMPEventArgs& args)
	{
		std::ostringstream os;
		os << "Query finished for " << args.hostName() << " [" << args.hostAddress() << "] with " << _oid << " OID:" 
		   << std::endl << "---------------------------------------------" << std::endl;
		logger().information(os.str());
	}

private:
	bool        _helpRequested;
	SNMPClient  _snmpClient;
	std::string _oid;
	std::string _target;
};


int main(int argc, char** argv)
{
	AutoPtr<SNMP> pApp = new SNMP;
	try
	{
		pApp->init(argc, argv);
	}
	catch (Poco::Exception& exc)
	{
		pApp->logger().log(exc);
		return Application::EXIT_CONFIG;
	}
	return pApp->run();
}
