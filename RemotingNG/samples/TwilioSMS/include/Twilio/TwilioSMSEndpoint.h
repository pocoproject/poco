//
// TwilioSMSEndpoint.h
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef TwilioSMSEndpoint_INCLUDED
#define TwilioSMSEndpoint_INCLUDED


#include "Poco/Poco.h"
#include "Poco/Nullable.h"


namespace Twilio {


//@ serialize
struct TwilioSMSResponse
{
	std::string sid;

	//@ name=date_created
	std::string dateCreated;

    //@ name=date_updated
    std::string dateUpdated;

    //@ name=date_sent
    Poco::Nullable<std::string> dateSent;

    //@ name=account_sid
    std::string accountSID;

    std::string to;

    std::string from;

    std::string body;

    std::string status;

    std::string direction;

    //@ name=api_version
    std::string apiVersion;

    Poco::Nullable<std::string> price;

	//@ name=price_unit
	std::string price_unit;

	std::string uri;

	//@ name=num_segments
	std::string num_segments;
};


//@ remote
//@ path="/2010-04-01/Accounts/{accountSID}/Messages.json"
class TwilioSMSEndpoint
{
public:
	//@ $accountSID={in=path}
	//@ $from={in=form, name=From}
	//@ $to={in=form, name=To}
	//@ $body={in=form, name=Body}
	TwilioSMSResponse post(const std::string& accountSID, const std::string& from, const std::string& to, const std::string& body);
};


} // namespace Twilio


#endif // TwilioSMSEndpoint_INCLUDED
