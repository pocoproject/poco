//
// TwilioSMSEndpointProxyFactory.h
//
// Package: Generated
// Module:  TwilioSMSEndpointProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TwilioSMSEndpointProxyFactory_INCLUDED
#define TwilioSMSEndpointProxyFactory_INCLUDED


#include "Poco/RemotingNG/ProxyFactory.h"
#include "Twilio/TwilioSMSEndpointProxy.h"


namespace Twilio {


class TwilioSMSEndpointProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	TwilioSMSEndpointProxyFactory();
		/// Creates a TwilioSMSEndpointProxyFactory.

	~TwilioSMSEndpointProxyFactory();
		/// Destroys the TwilioSMSEndpointProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace Twilio


#endif // TwilioSMSEndpointProxyFactory_INCLUDED

