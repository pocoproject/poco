//
// TwilioSMSEndpointProxyFactory.cpp
//
// Package: Generated
// Module:  TwilioSMSEndpointProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "Twilio/TwilioSMSEndpointProxyFactory.h"


namespace Twilio {


TwilioSMSEndpointProxyFactory::TwilioSMSEndpointProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


TwilioSMSEndpointProxyFactory::~TwilioSMSEndpointProxyFactory()
{
}


Poco::RemotingNG::Proxy* TwilioSMSEndpointProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new Twilio::TwilioSMSEndpointProxy(oid);
}


} // namespace Twilio

