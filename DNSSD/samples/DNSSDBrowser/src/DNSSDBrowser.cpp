//
// DNSSDBrowser.cpp
//
// $Id: //poco/1.7/DNSSD/samples/DNSSDBrowser/src/DNSSDBrowser.cpp#1 $
//
// This sample demonstrates the DNSSDBrowser and DNSSDResponder classes.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/IntValidator.h"
#include "Poco/DNSSD/DNSSDResponder.h"
#include "Poco/DNSSD/DNSSDBrowser.h"
#if POCO_OS == POCO_OS_LINUX && !defined(POCO_DNSSD_USE_BONJOUR)
#include "Poco/DNSSD/Avahi/Avahi.h"
#else
#include "Poco/DNSSD/Bonjour/Bonjour.h"
#endif
#include "Poco/Delegate.h"
#include "Poco/NumberParser.h"
#include "Poco/Thread.h"
#include <set>
#include <iostream>


using Poco::Util::Application;
using Poco::Util::ServerApplication;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::DNSSD::DNSSDResponder;
using Poco::DNSSD::Service;
using Poco::DNSSD::BrowseHandle;
using Poco::DNSSD::ServiceHandle;


class DNSSDBrowserApp: public ServerApplication
{
public:
	DNSSDBrowserApp():
		_helpRequested(false),
		_autoResolve(false),
		_enumerateDomains(false),
		_port(0),
		_interface(0)
	{
		Poco::DNSSD::initializeDNSSD();
	}

	~DNSSDBrowserApp()
	{
		Poco::DNSSD::uninitializeDNSSD();
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
				.callback(OptionCallback<DNSSDBrowserApp>(this, &DNSSDBrowserApp::handleHelp)));

		options.addOption(
			Option("browse", "b",
				"Browse for services with the type given in the argument (e.g., _ftp._tcp). "
				"Can be specified multiple times to browse for different types of services.")
				.required(false)
				.repeatable(true)
				.argument("<service-type>")
				.callback(OptionCallback<DNSSDBrowserApp>(this, &DNSSDBrowserApp::handleBrowse)));

		options.addOption(
			Option("resolve", "r",
				"Automatically resolve all discovered services.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<DNSSDBrowserApp>(this, &DNSSDBrowserApp::handleResolve)));

		options.addOption(
			Option("domain", "d",
				"Specify the domain to browse, or register a service in. "
				"If not specified, the default domain will be used.")
				.required(false)
				.repeatable(false)
				.argument("<domain>")
				.callback(OptionCallback<DNSSDBrowserApp>(this, &DNSSDBrowserApp::handleDomain)));

		options.addOption(
			Option("enumerate-domains", "e", "Enumerate browse- and registration-domains.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<DNSSDBrowserApp>(this, &DNSSDBrowserApp::handleEnumerate)));

		options.addOption(
			Option("register", "R",
				"Register a service with the given type (e.g., _ftp._tcp).")
				.required(false)
				.repeatable(false)
				.argument("<service-type>")
				.callback(OptionCallback<DNSSDBrowserApp>(this, &DNSSDBrowserApp::handleRegister)));

		options.addOption(
			Option("name", "n",
				"Specify the service name for the service to be registered. "
				"If not specified, the name of the machine will be used.")
				.required(false)
				.repeatable(false)
				.argument("<name>")
				.callback(OptionCallback<DNSSDBrowserApp>(this, &DNSSDBrowserApp::handleName)));

		options.addOption(
			Option("host", "H",
				"Specify the host name for the service to be registered. "
				"If not specified, the machine's host name will be used.")
				.required(false)
				.repeatable(false)
				.argument("<host>")
				.callback(OptionCallback<DNSSDBrowserApp>(this, &DNSSDBrowserApp::handleHost)));

		options.addOption(
			Option("port", "p",
				"Specify the port number for the service to be registered. "
				"If not specified, the service will be registered with port 0.")
				.required(false)
				.repeatable(false)
				.argument("<port>")
				.validator(new Poco::Util::IntValidator(0, 65535))
				.callback(OptionCallback<DNSSDBrowserApp>(this, &DNSSDBrowserApp::handlePort)));

		options.addOption(
			Option("txt", "t",
				"Specify a key-value pair for a registered service's TXT record. "
				"Can be given multiple times.")
				.required(false)
				.repeatable(true)
				.argument("<key><[=value]>")
				.callback(OptionCallback<DNSSDBrowserApp>(this, &DNSSDBrowserApp::handleTXT)));

		options.addOption(
			Option("interface", "i",
				"Specify a specific network interface for browsing and registration, by its "
				"interface index. Specify 0 to browse/register on all interfaces (default).")
				.required(false)
				.repeatable(false)
				.argument("<intf-index>")
				.validator(new Poco::Util::IntValidator(0, 16))
				.callback(OptionCallback<DNSSDBrowserApp>(this, &DNSSDBrowserApp::handleInterface)));
	}

	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		stopOptionsProcessing();
	}

	void handleBrowse(const std::string& name, const std::string& value)
	{
		_browseTypes.insert(value);
	}

	void handleResolve(const std::string& name, const std::string& value)
	{
		_autoResolve = true;
	}

	void handleDomain(const std::string& name, const std::string& value)
	{
		_domain = value;
	}

	void handleEnumerate(const std::string& name, const std::string& value)
	{
		_enumerateDomains = true;
	}

	void handleRegister(const std::string& name, const std::string& value)
	{
		_registeredService = value;
	}

	void handleName(const std::string& name, const std::string& value)
	{
		_name = value;
	}

	void handleHost(const std::string& name, const std::string& value)
	{
		_host = value;
	}

	void handlePort(const std::string& name, const std::string& value)
	{
		_port = static_cast<Poco::UInt16>(Poco::NumberParser::parseUnsigned(value));
	}

	void handleTXT(const std::string& name, const std::string& value)
	{
		std::string::size_type pos = value.find('=');
		if (pos != std::string::npos)
		{
			_properties.add(value.substr(0, pos), value.substr(pos + 1));
		}
		else
		{
			_properties.add(value, "");
		}
	}

	void handleInterface(const std::string& name, const std::string& value)
	{
		_interface = Poco::NumberParser::parse(value);
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader(
			"\n"
			"A sample application demonstrating the use of the DNSSDBrowser and DNSSDResponder classes. "
			"The following command line options are supported:");
		helpFormatter.setFooter(
			"For more information, please see the Applied Informatics C++ Libraries "
			"and Tools documentation at <http://docs.appinf.com>."
		);
		helpFormatter.setIndent(8);
		helpFormatter.format(std::cout);
	}

	void onError(const void* sender, const Poco::DNSSD::DNSSDBrowser::ErrorEventArgs& args)
	{
		std::cout << args.error.message() << " (" << args.error.code() << ")" << std::endl;
	}

	void onServiceFound(const void* sender, const Poco::DNSSD::DNSSDBrowser::ServiceEventArgs& args)
	{
		std::cout << "Service Found: \n"
			<< "  Name:      " << args.service.name() << "\n"
			<< "  Domain:    " << args.service.domain() << "\n"
			<< "  Type:      " << args.service.type() << "\n"
			<< "  Interface: " << args.service.networkInterface() << "\n" << std::endl;

		if (_autoResolve)
		{
			reinterpret_cast<Poco::DNSSD::DNSSDBrowser*>(const_cast<void*>(sender))->resolve(args.service);
		}
	}

	void onServiceRemoved(const void* sender, const Poco::DNSSD::DNSSDBrowser::ServiceEventArgs& args)
	{
		std::cout << "Service Removed: \n"
			<< "  Name:      " << args.service.name() << "\n"
			<< "  Domain:    " << args.service.domain() << "\n"
			<< "  Type:      " << args.service.type() << "\n"
			<< "  Interface: " << args.service.networkInterface() << "\n" << std::endl;
	}

	void onServiceResolved(const void* sender, const Poco::DNSSD::DNSSDBrowser::ServiceEventArgs& args)
	{
		std::cout << "Service Resolved: \n"
			<< "  Name:      " << args.service.name() << "\n"
			<< "  Full Name: " << args.service.fullName() << "\n"
			<< "  Domain:    " << args.service.domain() << "\n"
			<< "  Type:      " << args.service.type() << "\n"
			<< "  Interface: " << args.service.networkInterface() << "\n"
			<< "  Host:      " << args.service.host() << "\n"
			<< "  Port:      " << args.service.port() << "\n"
			<< "  Properties: \n";

		for (Poco::DNSSD::Service::Properties::ConstIterator it = args.service.properties().begin(); it != args.service.properties().end(); ++it)
		{
			std::cout << "    " << it->first << ": " << it->second << "\n";
		}
		std::cout << std::endl;

		reinterpret_cast<Poco::DNSSD::DNSSDBrowser*>(const_cast<void*>(sender))->resolveHost(args.service.host());
	}

	void onHostResolved(const void* sender, const Poco::DNSSD::DNSSDBrowser::ResolveHostEventArgs& args)
	{
		std::cout << "Host Resolved: \n"
			<< "  Host:      " << args.host << "\n"
			<< "  Interface: " << args.networkInterface << "\n"
			<< "  Address:   " << args.address.toString() << "\n"
			<< "  TTL:       " << args.ttl << "\n" << std::endl;
	}

	void onBrowseDomainFound(const void* sender, const Poco::DNSSD::DNSSDBrowser::DomainEventArgs& args)
	{
		std::cout << "Browse Domain Found:\n"
			<< "  Name:      " << args.domain.name() << "\n"
			<< "  Interface: " << args.domain.networkInterface() << "\n"
			<< "  Default:   " << args.domain.isDefault() << "\n" << std::endl;
	}

	void onBrowseDomainRemoved(const void* sender, const Poco::DNSSD::DNSSDBrowser::DomainEventArgs& args)
	{
		std::cout << "Browse Domain Removed:\n"
			<< "  Name:      " << args.domain.name() << "\n"
			<< "  Interface: " << args.domain.networkInterface() << "\n"
			<< "  Default:   " << args.domain.isDefault() << "\n" << std::endl;
	}

	void onRegistrationDomainFound(const void* sender, const Poco::DNSSD::DNSSDBrowser::DomainEventArgs& args)
	{
		std::cout << "Registration Domain Found:\n"
			<< "  Name:      " << args.domain.name() << "\n"
			<< "  Interface: " << args.domain.networkInterface() << "\n"
			<< "  Default:   " << args.domain.isDefault() << "\n" << std::endl;
	}

	void onRegistrationDomainRemoved(const void* sender, const Poco::DNSSD::DNSSDBrowser::DomainEventArgs& args)
	{
		std::cout << "Registration Domain Removed:\n"
			<< "  Name:      " << args.domain.name() << "\n"
			<< "  Interface: " << args.domain.networkInterface() << "\n"
			<< "  Default:   " << args.domain.isDefault() << "\n" << std::endl;
	}

	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested || (_browseTypes.empty() && _registeredService.empty() && !_enumerateDomains))
		{
			displayHelp();
		}
		else
		{
			DNSSDResponder dnssdResponder;

			dnssdResponder.browser().browseError               += Poco::delegate(this, &DNSSDBrowserApp::onError);
			dnssdResponder.browser().resolveError              += Poco::delegate(this, &DNSSDBrowserApp::onError);
			dnssdResponder.browser().serviceFound              += Poco::delegate(this, &DNSSDBrowserApp::onServiceFound);
			dnssdResponder.browser().serviceRemoved            += Poco::delegate(this, &DNSSDBrowserApp::onServiceRemoved);
			dnssdResponder.browser().serviceResolved           += Poco::delegate(this, &DNSSDBrowserApp::onServiceResolved);
			dnssdResponder.browser().browseDomainError         += Poco::delegate(this, &DNSSDBrowserApp::onError);
			dnssdResponder.browser().browseDomainFound         += Poco::delegate(this, &DNSSDBrowserApp::onBrowseDomainFound);
			dnssdResponder.browser().browseDomainRemoved       += Poco::delegate(this, &DNSSDBrowserApp::onBrowseDomainRemoved);
			dnssdResponder.browser().registrationDomainError   += Poco::delegate(this, &DNSSDBrowserApp::onError);
			dnssdResponder.browser().registrationDomainFound   += Poco::delegate(this, &DNSSDBrowserApp::onRegistrationDomainFound);
			dnssdResponder.browser().registrationDomainRemoved += Poco::delegate(this, &DNSSDBrowserApp::onRegistrationDomainRemoved);
			dnssdResponder.browser().hostResolveError          += Poco::delegate(this, &DNSSDBrowserApp::onError);
			dnssdResponder.browser().hostResolved              += Poco::delegate(this, &DNSSDBrowserApp::onHostResolved);

			std::vector<BrowseHandle> browseHandles;
			for (std::set<std::string>::const_iterator it = _browseTypes.begin(); it != _browseTypes.end(); ++it)
			{
				browseHandles.push_back(dnssdResponder.browser().browse(*it, _domain, 0, _interface));
			}

			if (_enumerateDomains)
			{
				browseHandles.push_back(dnssdResponder.browser().enumerateBrowseDomains(_interface));
				browseHandles.push_back(dnssdResponder.browser().enumerateRegistrationDomains(_interface));
			}

			ServiceHandle serviceHandle;
			if (!_registeredService.empty())
			{
				Service service(_interface, _name, "", _registeredService, _domain, _host, _port, _properties);
				serviceHandle = dnssdResponder.registerService(service);
			}

			dnssdResponder.start();
			waitForTerminationRequest();

			if (serviceHandle.isValid())
			{
				dnssdResponder.unregisterService(serviceHandle);
				Poco::Thread::sleep(2500); // allow time for delivery of remove event
			}

			for (std::vector<BrowseHandle>::iterator it = browseHandles.begin(); it != browseHandles.end(); ++it)
			{
				dnssdResponder.browser().cancel(*it);
			}

			dnssdResponder.stop();

			dnssdResponder.browser().browseError               -= Poco::delegate(this, &DNSSDBrowserApp::onError);
			dnssdResponder.browser().resolveError              -= Poco::delegate(this, &DNSSDBrowserApp::onError);
			dnssdResponder.browser().serviceFound              -= Poco::delegate(this, &DNSSDBrowserApp::onServiceFound);
			dnssdResponder.browser().serviceRemoved            -= Poco::delegate(this, &DNSSDBrowserApp::onServiceRemoved);
			dnssdResponder.browser().serviceResolved           -= Poco::delegate(this, &DNSSDBrowserApp::onServiceResolved);
			dnssdResponder.browser().browseDomainError         -= Poco::delegate(this, &DNSSDBrowserApp::onError);
			dnssdResponder.browser().browseDomainFound         -= Poco::delegate(this, &DNSSDBrowserApp::onBrowseDomainFound);
			dnssdResponder.browser().browseDomainRemoved       -= Poco::delegate(this, &DNSSDBrowserApp::onBrowseDomainRemoved);
			dnssdResponder.browser().registrationDomainError   -= Poco::delegate(this, &DNSSDBrowserApp::onError);
			dnssdResponder.browser().registrationDomainFound   -= Poco::delegate(this, &DNSSDBrowserApp::onRegistrationDomainFound);
			dnssdResponder.browser().registrationDomainRemoved -= Poco::delegate(this, &DNSSDBrowserApp::onRegistrationDomainRemoved);
			dnssdResponder.browser().hostResolveError          -= Poco::delegate(this, &DNSSDBrowserApp::onError);
			dnssdResponder.browser().hostResolved              -= Poco::delegate(this, &DNSSDBrowserApp::onHostResolved);
		}

		return Application::EXIT_OK;
	}

private:
	bool _helpRequested;
	bool _autoResolve;
	bool _enumerateDomains;
	std::set<std::string> _browseTypes;
	std::string _domain;
	std::string _registeredService;
	std::string _name;
	std::string _host;
	Service::Properties _properties;
	Poco::UInt16 _port;
	Poco::Int32 _interface;
};


POCO_SERVER_MAIN(DNSSDBrowserApp)
