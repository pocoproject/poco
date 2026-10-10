//
// TwilioSMSEndpointProxy.h
//
// Package: Generated
// Module:  TwilioSMSEndpointProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TwilioSMSEndpointProxy_INCLUDED
#define TwilioSMSEndpointProxy_INCLUDED


#include "Poco/RemotingNG/Proxy.h"
#include "Twilio/ITwilioSMSEndpoint.h"


namespace Twilio {


class TwilioSMSEndpointProxy: public Twilio::ITwilioSMSEndpoint, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<TwilioSMSEndpointProxy>;

	TwilioSMSEndpointProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a TwilioSMSEndpointProxy.

	virtual ~TwilioSMSEndpointProxy();
		/// Destroys the TwilioSMSEndpointProxy.

	Twilio::TwilioSMSResponse post(const std::string& accountSID, const std::string& from, const std::string& to, const std::string& body);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
	mutable Twilio::TwilioSMSResponse _postRet;
};


inline const Poco::RemotingNG::Identifiable::TypeId& TwilioSMSEndpointProxy::remoting__typeId() const
{
	return ITwilioSMSEndpoint::remoting__typeId();
}


} // namespace Twilio


#endif // TwilioSMSEndpointProxy_INCLUDED

