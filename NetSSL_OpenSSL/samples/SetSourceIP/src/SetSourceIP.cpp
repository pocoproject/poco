//
// SetSourceIP.cpp
//
// This sample demonstrates setting the source IP address.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SSLManager.h"
#include "Poco/Net/RejectCertificateHandler.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SocketImpl.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/SharedPtr.h"
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>


Poco::Path programName;


void usage(std::string errorMessage)
{
	std::cerr << "ERROR: " << errorMessage << std::endl;
	std::cerr << std::endl;
	std::cerr << "   syntax: " << std::endl;

	std::cerr << "           " << programName.getBaseName() << " OPTIONS uri" << std::endl;
	std::cerr << std::endl;
	std::cerr << "              OPTIONS:" << std::endl;
	std::cerr << "                      --sourceip <source IP address>" << std::endl;
	std::cerr << "                      --proxyuri <proxy uri>" << std::endl;

	std::cerr << std::endl;
	std::cerr << "           supported uri schemes for target are http and https" << std::endl;
	std::cerr << "           supported uri scheme for proxy is http" << std::endl;
	std::cerr << std::endl;
	std::cerr << "   examples: " << std::endl;
	std::cerr << "           " << programName.getBaseName() << " http://www.example.com/" << std::endl;
	std::cerr << "           " << programName.getBaseName() << " https://www.example.com/" << std::endl;
	std::cerr << "           " << programName.getBaseName() << " --sourceip 10.2.5.6 http://www.example.com/" << std::endl;
	std::cerr << "           " << programName.getBaseName() << " --sourceip 192.168.15.122 https://www.example.com/" << std::endl;
	std::cerr << "           " << programName.getBaseName() << " --proxyuri http://localhost:3128 https://www.example.com/" << std::endl;
	std::cerr << "           " << programName.getBaseName() << " --sourceip 192.168.15.122 --proxyuri http://localhost:3128 https://www.example.com/" << std::endl;
	std::cerr << std::endl;
	exit(1);
}


int main(int argc, char **argv)
{
	// save program name in case usage() gets called
	programName = argv[0];

	std::string uriString;
	std::list<std::string> sourceIpList;
	std::string proxyUriString;

	for (int i = 1; i < argc; ++i)
	{
		std::string optionName = argv[i];

		if (optionName == "--sourceip")
		{
			++i;
			if (i >= argc)
			{
				usage("Missing option arguments");
			}

			sourceIpList.push_back(argv[i]);
			continue;
		}

		if (optionName == "--proxyuri")
		{
			++i;
			if (i >= argc)
			{
				usage("Missing option arguments");
			}

			proxyUriString = argv[i];
			continue;
		}

		// last argument??
		if ((i+1) == argc)
			uriString = argv[i];
		else
			usage("Unknown option");
	}

	if (uriString.empty())
	{
		usage("URI not specified");
	}

	Poco::SharedPtr<Poco::Net::HTTPClientSession> session;

	try
	{
		Poco::URI uri(uriString);

		if (uri.getScheme() == "https")
		{
			Poco::Net::initializeSSL();

			Poco::Net::Context::Params params;
			params.verificationMode = Poco::Net::Context::VERIFY_NONE;
			params.verificationDepth = 9;
			params.loadDefaultCAs = true;
			params.cipherList = "ALL";

			Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::TLSV1_2_CLIENT_USE, params);

			Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrCert = new Poco::Net::ConsoleCertificateHandler(false); // ask the user via console

			Poco::Net::SSLManager::instance().initializeClient(NULL, ptrCert, context);

			session = new Poco::Net::HTTPSClientSession(uri.getHost(), uri.getPort());
		}
		else if (uri.getScheme() == "http")
		{
			session = new Poco::Net::HTTPClientSession(uri.getHost(), uri.getPort());
		}
		else
		{
			usage("wrong scheme '" + uri.getScheme() + "' for target uri, expected http or https");
		}

		while (!sourceIpList.empty())
		{
			std::string sourceIpString = sourceIpList.front();
			sourceIpList.pop_front();

			// Set the sourceIP address, but leave the source port to 0 so ANY port can be used
			Poco::Net::SocketAddress sa = Poco::Net::SocketAddress(sourceIpString, 0);
			session->setSourceAddress(sa);

			std::cout << "Using source IP address" << std::endl;
			std::cout << "source IP address : " << sa.toString() << std::endl;
			std::cout << std::endl;
		}

		if (!proxyUriString.empty())
		{
			Poco::URI proxyUri(proxyUriString);

			if (proxyUri.getScheme() == "http")
				session->setProxy(proxyUri.getHost(), proxyUri.getPort());
			else
				usage("wrong scheme '" + proxyUri.getScheme() + "' for proxy uri, expected http");

			std::cout << "Using proxy" << std::endl;
			std::cout << "Proxy Host: " << proxyUri.getHost() << std::endl;
			std::cout << "Proxy Port: " << proxyUri.getPort() << std::endl;
			std::cout << std::endl;
		}

		std::string path(uri.getPathAndQuery());

		std::cout << "Host: " << uri.getHost() << std::endl;
		std::cout << "Port: " << uri.getPort() << std::endl;
		std::cout << "Path: " << path << std::endl;

		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);

		session->sendRequest(request);

		Poco::Net::HTTPResponse response;
		std::istream &istream = session->receiveResponse(response);

		std::cout << "Status: " << response.getStatus() << std::endl;

		std::string responseString;

		Poco::StreamCopier::copyToString(istream, responseString);

		std::cout << "Response: " << responseString << std::endl;
	}
	catch (Poco::Exception &ex)
	{
		std::cout << "Exception: name (" << ex.name() << ") message [" << ex.message() << "]" << std::endl << std::flush;
	}

	return 0;
}
