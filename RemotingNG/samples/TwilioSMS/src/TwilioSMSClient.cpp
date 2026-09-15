//
// TwilioSMSClient.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Twilio/TwilioSMSEndpointClientHelper.h"
#include "Poco/RemotingNG/REST/TransportFactory.h"
#include "Poco/RemotingNG/REST/Transport.h"
#include "Poco/RemotingNG/REST/RESTServerException.h"
#include "Poco/RemotingNG/Proxy.h"
#include "Poco/Net/HTTPSSessionInstantiator.h"
#include <iostream>


template <class I>
Poco::RemotingNG::REST::Transport& transportFromInterface(Poco::AutoPtr<I> pInterface)
{
    Poco::RemotingNG::Proxy::Ptr pProxy = pInterface.template cast<Poco::RemotingNG::Proxy>();
    if (pProxy)
    {
        return static_cast<Poco::RemotingNG::REST::Transport&>(pProxy->remoting__transport());
    }
    else throw Poco::BadCastException();
}


int main(int argc, char** argv)
{
	try
	{
		// register transport
		Poco::RemotingNG::REST::TransportFactory::registerFactory();

		// enable HTTPS
		Poco::RemotingNG::REST::Transport::httpSessionFactory().registerProtocol(
			"https",
			new Poco::Net::HTTPSSessionInstantiator
		);

		// get proxy for Twilio SMS web service
		Twilio::ITwilioSMSEndpoint::Ptr pSMSEndpoint = Twilio::TwilioSMSEndpointClientHelper::find("https://api.twilio.com/2010-04-01/Accounts/{accountSID}/Messages.json", "rest");

		// parameters
		std::string accountSID = "<YourTwilioAccountSID>";
		std::string authToken = "<YourTwilioAuthToken>";
		std::string from = "<YourTwilioSender>";
		std::string to = "<destinationPhoneNumber>";
		std::string body = "Hello, world!";

		// authenticate
		Poco::RemotingNG::REST::Transport& trans = transportFromInterface(pSMSEndpoint);
		trans.setAuthentication(Poco::RemotingNG::REST::Transport::AUTH_BASIC);
		trans.setUsername(accountSID);
		trans.setPassword(authToken);

		// invoke web service
		Twilio::TwilioSMSResponse response = pSMSEndpoint->post(accountSID, from, to, body);

		std::cout << "status: " << response.status << std::endl;
		std::cout << "uri: " << response.uri << std::endl;
	}
	catch (Poco::RemotingNG::REST::RESTServerException& exc)
	{
		std::cerr
			<< exc.displayText() << "\n"
			<< "  Status: " << exc.status() << "\n"
			<< "  Reason: " << exc.reason() << "\n"
			<< "  Error:  " << exc.error() << "\n"
			<< "  Detail: " << exc.detail() << std::endl;
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
	return 0;
}
