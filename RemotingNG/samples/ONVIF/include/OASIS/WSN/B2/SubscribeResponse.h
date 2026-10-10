// 
// SubscribeResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_SubscribeResponse_INCLUDED
#define OASIS_WSN_B2_SubscribeResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "W3/Addressing/EndpointReference.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace B2 {


//@ name="#SubscribeResponse"
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API SubscribeResponse
{
public:
	SubscribeResponse();

	SubscribeResponse(
		const W3::Addressing::EndpointReference& subscriptionReference, 
		const Poco::Optional<Poco::DateTime>& currentTime, 
		const Poco::Nullable<Poco::DateTime>& terminationTime);

	virtual ~SubscribeResponse();

	//@ return={xsdType=dateTime}
	const Poco::Optional<Poco::DateTime>& getCurrentTime() const;

	const W3::Addressing::EndpointReference& getSubscriptionReference() const;

	//@ return={xsdType=dateTime}
	const Poco::Nullable<Poco::DateTime>& getTerminationTime() const;

	//@ $val={xsdType=dateTime}
	void setCurrentTime(const Poco::Optional<Poco::DateTime>& val);

	//@ $val={xsdType=dateTime}
	void setCurrentTime(Poco::Optional<Poco::DateTime>&& val);

	void setSubscriptionReference(const W3::Addressing::EndpointReference& val);

	void setSubscriptionReference(W3::Addressing::EndpointReference&& val);

	//@ $val={xsdType=dateTime}
	void setTerminationTime(const Poco::Nullable<Poco::DateTime>& val);

	//@ $val={xsdType=dateTime}
	void setTerminationTime(Poco::Nullable<Poco::DateTime>&& val);

private:
	//@ name=SubscriptionReference
	//@ order=0
	W3::Addressing::EndpointReference _subscriptionReference;

	//@ mandatory=false
	//@ name=CurrentTime
	//@ order=1
	//@ xsdType=dateTime
	Poco::Optional<Poco::DateTime> _currentTime;

	//@ mandatory=false
	//@ name=TerminationTime
	//@ order=2
	//@ xsdType=dateTime
	Poco::Nullable<Poco::DateTime> _terminationTime;

};


inline const Poco::Optional<Poco::DateTime>& SubscribeResponse::getCurrentTime() const
{
	return _currentTime;
}


inline const W3::Addressing::EndpointReference& SubscribeResponse::getSubscriptionReference() const
{
	return _subscriptionReference;
}


inline const Poco::Nullable<Poco::DateTime>& SubscribeResponse::getTerminationTime() const
{
	return _terminationTime;
}


inline void SubscribeResponse::setCurrentTime(const Poco::Optional<Poco::DateTime>& val)
{
	_currentTime = val;
}


inline void SubscribeResponse::setCurrentTime(Poco::Optional<Poco::DateTime>&& val)
{
	_currentTime = std::move(val);
}


inline void SubscribeResponse::setSubscriptionReference(const W3::Addressing::EndpointReference& val)
{
	_subscriptionReference = val;
}


inline void SubscribeResponse::setSubscriptionReference(W3::Addressing::EndpointReference&& val)
{
	_subscriptionReference = std::move(val);
}


inline void SubscribeResponse::setTerminationTime(const Poco::Nullable<Poco::DateTime>& val)
{
	_terminationTime = val;
}


inline void SubscribeResponse::setTerminationTime(Poco::Nullable<Poco::DateTime>&& val)
{
	_terminationTime = std::move(val);
}


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_SubscribeResponse_INCLUDED
