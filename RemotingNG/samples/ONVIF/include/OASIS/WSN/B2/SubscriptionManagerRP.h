// 
// SubscriptionManagerRP.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_SubscriptionManagerRP_INCLUDED
#define OASIS_WSN_B2_SubscriptionManagerRP_INCLUDED


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
class Filter;
class SubscriptionPolicy;
} } } 


namespace OASIS {
namespace WSN {
namespace B2 {


//@ name="#SubscriptionManagerRP"
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API SubscriptionManagerRP
{
public:
	SubscriptionManagerRP();

	SubscriptionManagerRP(
		const W3::Addressing::EndpointReference& consumerReference, 
		const Poco::SharedPtr<Filter>& filter, 
		const Poco::SharedPtr<SubscriptionPolicy>& subscriptionPolicy, 
		const Poco::Optional<Poco::DateTime>& creationTime);

	virtual ~SubscriptionManagerRP();

	const W3::Addressing::EndpointReference& getConsumerReference() const;

	//@ return={xsdType=dateTime}
	const Poco::Optional<Poco::DateTime>& getCreationTime() const;

	const Poco::SharedPtr<Filter>& getFilter() const;

	const Poco::SharedPtr<SubscriptionPolicy>& getSubscriptionPolicy() const;

	void setConsumerReference(const W3::Addressing::EndpointReference& val);

	void setConsumerReference(W3::Addressing::EndpointReference&& val);

	//@ $val={xsdType=dateTime}
	void setCreationTime(const Poco::Optional<Poco::DateTime>& val);

	//@ $val={xsdType=dateTime}
	void setCreationTime(Poco::Optional<Poco::DateTime>&& val);

	void setFilter(const Poco::SharedPtr<Filter>& val);

	void setFilter(Poco::SharedPtr<Filter>&& val);

	void setSubscriptionPolicy(const Poco::SharedPtr<SubscriptionPolicy>& val);

	void setSubscriptionPolicy(Poco::SharedPtr<SubscriptionPolicy>&& val);

private:
	//@ name=ConsumerReference
	//@ order=0
	W3::Addressing::EndpointReference _consumerReference;

	//@ mandatory=false
	//@ name=Filter
	//@ order=1
	Poco::SharedPtr<Filter> _filter;

	//@ mandatory=false
	//@ name=SubscriptionPolicy
	//@ order=2
	Poco::SharedPtr<SubscriptionPolicy> _subscriptionPolicy;

	//@ mandatory=false
	//@ name=CreationTime
	//@ order=3
	//@ xsdType=dateTime
	Poco::Optional<Poco::DateTime> _creationTime;

};


inline const W3::Addressing::EndpointReference& SubscriptionManagerRP::getConsumerReference() const
{
	return _consumerReference;
}


inline const Poco::Optional<Poco::DateTime>& SubscriptionManagerRP::getCreationTime() const
{
	return _creationTime;
}


inline const Poco::SharedPtr<Filter>& SubscriptionManagerRP::getFilter() const
{
	return _filter;
}


inline const Poco::SharedPtr<SubscriptionPolicy>& SubscriptionManagerRP::getSubscriptionPolicy() const
{
	return _subscriptionPolicy;
}


inline void SubscriptionManagerRP::setConsumerReference(const W3::Addressing::EndpointReference& val)
{
	_consumerReference = val;
}


inline void SubscriptionManagerRP::setConsumerReference(W3::Addressing::EndpointReference&& val)
{
	_consumerReference = std::move(val);
}


inline void SubscriptionManagerRP::setCreationTime(const Poco::Optional<Poco::DateTime>& val)
{
	_creationTime = val;
}


inline void SubscriptionManagerRP::setCreationTime(Poco::Optional<Poco::DateTime>&& val)
{
	_creationTime = std::move(val);
}


inline void SubscriptionManagerRP::setFilter(const Poco::SharedPtr<Filter>& val)
{
	_filter = val;
}


inline void SubscriptionManagerRP::setFilter(Poco::SharedPtr<Filter>&& val)
{
	_filter = std::move(val);
}


inline void SubscriptionManagerRP::setSubscriptionPolicy(const Poco::SharedPtr<SubscriptionPolicy>& val)
{
	_subscriptionPolicy = val;
}


inline void SubscriptionManagerRP::setSubscriptionPolicy(Poco::SharedPtr<SubscriptionPolicy>&& val)
{
	_subscriptionPolicy = std::move(val);
}


} } } // OASIS::WSN::B2


// The following headers are required for template instantiation.
#include "OASIS/WSN/B2/Filter.h"
#include "OASIS/WSN/B2/SubscriptionPolicy.h"


#endif // OASIS_WSN_B2_SubscriptionManagerRP_INCLUDED
