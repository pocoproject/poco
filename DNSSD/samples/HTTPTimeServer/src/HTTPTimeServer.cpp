//
// HTTPTimeServer.cpp
//
// $Id: //poco/1.7/DNSSD/samples/HTTPTimeServer/src/HTTPTimeServer.cpp#1 $
//
// This sample demonstrates how a web server can advertise itself
// on the network using DNS Service Discovery.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Net/MulticastSocket.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/DNSSD/DNSSDResponder.h"
#if POCO_OS == POCO_OS_LINUX && !defined(POCO_DNSSD_USE_BONJOUR)
#include "Poco/DNSSD/Avahi/Avahi.h"
#else
#include "Poco/DNSSD/Bonjour/Bonjour.h"
#endif
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include <sstream>
#include <iostream>


using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;


class TimeRequestHandler: public HTTPRequestHandler
	/// Return a HTML document with the current date and time.
{
public:
	TimeRequestHandler(const std::string& format):
		_format(format)
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		Application& app = Application::instance();
		app.logger().information("Request from " + request.clientAddress().toString());

		Timestamp now;
		std::string dt(DateTimeFormatter::format(now, _format));

		response.setChunkedTransferEncoding(true);
		response.setContentType("text/html");

		std::ostream& ostr = response.send();
		ostr << "<html><head><title>HTTPTimeServer powered by POCO C++ Libraries</title>";
		ostr << "<meta http-equiv=\"refresh\" content=\"1\"></head>";
		ostr << "<body><p style=\"text-align: center; font-size: 48px;\">";
		ostr << dt;
		ostr << "</p></body></html>";
	}

private:
	std::string _format;
};


class TimeRequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
	TimeRequestHandlerFactory(const std::string& format):
		_format(format)
	{
	}

	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		if (request.getURI() == "/")
			return new TimeRequestHandler(_format);
		else
			return 0;
	}

private:
	std::string _format;
};


class HTTPTimeServer: public Poco::Util::ServerApplication
	/// The main application class.
	///
	/// This class handles command-line arguments and
	/// configuration files.
	/// Start the HTTPTimeServer executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
	///
	/// To use the sample configuration file (HTTPTimeServer.properties),
	/// copy the file to the directory where the HTTPTimeServer executable
	/// resides. If you start the debug version of the HTTPTimeServer
	/// (HTTPTimeServerd[.exe]), you must also create a copy of the configuration
	/// file named HTTPTimeServerd.properties. In the configuration file, you
	/// can specify the port on which the server is listening (default
	/// 9980) and the format of the date/time string sent back to the client.
	///
	/// To test the TimeServer you can use any web browser (http://localhost:9980/).
{
public:
	HTTPTimeServer(): _helpRequested(false)
	{
		Poco::DNSSD::initializeDNSSD();
	}

	~HTTPTimeServer()
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
				.repeatable(false));
	}

	void handleOption(const std::string& name, const std::string& value)
	{
		ServerApplication::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A web server that serves the current date and time and announces itself via UPnP SSDP.");
		helpFormatter.format(std::cout);
	}

	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested)
		{
			displayHelp();
		}
		else
		{
			// get parameters from configuration file
			unsigned short httpPort = static_cast<unsigned short>(config().getInt("http.port", 9980));
			std::string format(config().getString("datetime.format", DateTimeFormat::SORTABLE_FORMAT));

			Poco::Net::HTTPServerParams::Ptr pHTTPParams = new Poco::Net::HTTPServerParams;
			pHTTPParams->setSoftwareVersion(config().getString("http.softwareVersion", ""));
			pHTTPParams->setMaxQueued(config().getInt("http.maxQueued", 4));
			pHTTPParams->setMaxThreads(config().getInt("http.maxThreads", 4));
			pHTTPParams->setTimeout(Poco::Timespan(config().getInt("http.timeout", 5), 0));
			pHTTPParams->setKeepAlive(config().getBool("http.keepAlive", false));
			pHTTPParams->setMaxKeepAliveRequests(config().getInt("http.maxKeepAliveRequests", 10));
			pHTTPParams->setKeepAliveTimeout(Poco::Timespan(config().getInt("http.keepAliveTimeout", 10), 0));

			ThreadPool::defaultPool().addCapacity(pHTTPParams->getMaxThreads());

			// set-up a server socket
			SocketAddress httpSA(Poco::Net::IPAddress(), httpPort);
			ServerSocket httpSocket(httpSA);
			// set-up a HTTPServer instance
			HTTPServer srv(new TimeRequestHandlerFactory(format), httpSocket, pHTTPParams);
			// start the HTTPServer
			srv.start();

			// register with DNSSDResponder
			Poco::DNSSD::DNSSDResponder dnssdResponder;
			dnssdResponder.start();

			Poco::DNSSD::Service service("_http._tcp", httpPort);
			Poco::DNSSD::ServiceHandle serviceHandle = dnssdResponder.registerService(service);

			// wait for CTRL-C or kill
			waitForTerminationRequest();

			// shut down UPnP
			dnssdResponder.unregisterService(serviceHandle);
			dnssdResponder.stop();

			// Stop the HTTPServer
			srv.stop();
		}
		return Application::EXIT_OK;
	}

private:
	bool _helpRequested;
};


int main(int argc, char** argv)
{
	HTTPTimeServer app;
	return app.run(argc, argv);
}
