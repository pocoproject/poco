//
// TwilioSMSEndpointClientHelper.cpp
//
// Package: Generated
// Module:  TwilioSMSEndpointClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "Twilio/TwilioSMSEndpointClientHelper.h"
#include "Poco/SingletonHolder.h"
#include "Twilio/TwilioSMSEndpointProxyFactory.h"


namespace Twilio {


namespace
{
	Poco::SingletonHolder<TwilioSMSEndpointClientHelper> shTwilioSMSEndpointClientHelper;
}


TwilioSMSEndpointClientHelper::TwilioSMSEndpointClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("Twilio.TwilioSMSEndpoint", new TwilioSMSEndpointProxyFactory);
}


TwilioSMSEndpointClientHelper::~TwilioSMSEndpointClientHelper()
{
}


ITwilioSMSEndpoint::Ptr TwilioSMSEndpointClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<ITwilioSMSEndpoint>();
}


ITwilioSMSEndpoint::Ptr TwilioSMSEndpointClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, ITwilioSMSEndpoint::remoting__typeId(), protocol);
	return pInterface.cast<ITwilioSMSEndpoint>();
}


TwilioSMSEndpointClientHelper& TwilioSMSEndpointClientHelper::instance()
{
	return *shTwilioSMSEndpointClientHelper.get();
}


} // namespace Twilio

