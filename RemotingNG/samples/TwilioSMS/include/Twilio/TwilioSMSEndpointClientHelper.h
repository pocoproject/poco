//
// TwilioSMSEndpointClientHelper.h
//
// Package: Generated
// Module:  TwilioSMSEndpointClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TwilioSMSEndpointClientHelper_INCLUDED
#define TwilioSMSEndpointClientHelper_INCLUDED


#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "Twilio/ITwilioSMSEndpoint.h"


namespace Twilio {


class TwilioSMSEndpointClientHelper
{
public:
	TwilioSMSEndpointClientHelper();
		/// Creates a TwilioSMSEndpointClientHelper.

	~TwilioSMSEndpointClientHelper();
		/// Destroys the TwilioSMSEndpointClientHelper.

	static ITwilioSMSEndpoint::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static ITwilioSMSEndpoint::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	ITwilioSMSEndpoint::Ptr findImpl(const std::string& uri);

	ITwilioSMSEndpoint::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static TwilioSMSEndpointClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline ITwilioSMSEndpoint::Ptr TwilioSMSEndpointClientHelper::find(const std::string& uri)
{
	return TwilioSMSEndpointClientHelper::instance().findImpl(uri);
}


inline ITwilioSMSEndpoint::Ptr TwilioSMSEndpointClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return TwilioSMSEndpointClientHelper::instance().findImpl(uri, protocol);
}


} // namespace Twilio


#endif // TwilioSMSEndpointClientHelper_INCLUDED

