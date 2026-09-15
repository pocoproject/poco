// 
// Subscribe.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_Subscribe_INCLUDED
#define OASIS_WSN_B2_Subscribe_INCLUDED


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


//@ name="#Subscribe"
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API Subscribe
{
public:
	Subscribe();

	Subscribe(
		const W3::Addressing::EndpointReference& consumerReference, 
		const Poco::SharedPtr<Filter>& filter, 
		const Poco::Nullable<Poco::DateTime>& initialTerminationTime, 
		const Poco::SharedPtr<SubscriptionPolicy>& subscriptionPolicy);

	virtual ~Subscribe();

	const W3::Addressing::EndpointReference& getConsumerReference() const;

	const Poco::SharedPtr<Filter>& getFilter() const;

	//@ return={xsdType}
	const Poco::Nullable<Poco::DateTime>& getInitialTerminationTime() const;

	const Poco::SharedPtr<SubscriptionPolicy>& getSubscriptionPolicy() const;

	void setConsumerReference(const W3::Addressing::EndpointReference& val);

	void setConsumerReference(W3::Addressing::EndpointReference&& val);

	void setFilter(const Poco::SharedPtr<Filter>& val);

	void setFilter(Poco::SharedPtr<Filter>&& val);

	//@ $val={xsdType}
	void setInitialTerminationTime(const Poco::Nullable<Poco::DateTime>& val);

	//@ $val={xsdType}
	void setInitialTerminationTime(Poco::Nullable<Poco::DateTime>&& val);

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
	//@ name=InitialTerminationTime
	//@ order=2
	//@ xsdType
	Poco::Nullable<Poco::DateTime> _initialTerminationTime;

	//@ mandatory=false
	//@ name=SubscriptionPolicy
	//@ order=3
	Poco::SharedPtr<SubscriptionPolicy> _subscriptionPolicy;

};


inline const W3::Addressing::EndpointReference& Subscribe::getConsumerReference() const
{
	return _consumerReference;
}


inline const Poco::SharedPtr<Filter>& Subscribe::getFilter() const
{
	return _filter;
}


inline const Poco::Nullable<Poco::DateTime>& Subscribe::getInitialTerminationTime() const
{
	return _initialTerminationTime;
}


inline const Poco::SharedPtr<SubscriptionPolicy>& Subscribe::getSubscriptionPolicy() const
{
	return _subscriptionPolicy;
}


inline void Subscribe::setConsumerReference(const W3::Addressing::EndpointReference& val)
{
	_consumerReference = val;
}


inline void Subscribe::setConsumerReference(W3::Addressing::EndpointReference&& val)
{
	_consumerReference = std::move(val);
}


inline void Subscribe::setFilter(const Poco::SharedPtr<Filter>& val)
{
	_filter = val;
}


inline void Subscribe::setFilter(Poco::SharedPtr<Filter>&& val)
{
	_filter = std::move(val);
}


inline void Subscribe::setInitialTerminationTime(const Poco::Nullable<Poco::DateTime>& val)
{
	_initialTerminationTime = val;
}


inline void Subscribe::setInitialTerminationTime(Poco::Nullable<Poco::DateTime>&& val)
{
	_initialTerminationTime = std::move(val);
}


inline void Subscribe::setSubscriptionPolicy(const Poco::SharedPtr<SubscriptionPolicy>& val)
{
	_subscriptionPolicy = val;
}


inline void Subscribe::setSubscriptionPolicy(Poco::SharedPtr<SubscriptionPolicy>&& val)
{
	_subscriptionPolicy = std::move(val);
}


} } } // OASIS::WSN::B2


// The following headers are required for template instantiation.
#include "OASIS/WSN/B2/Filter.h"
#include "OASIS/WSN/B2/SubscriptionPolicy.h"


#endif // OASIS_WSN_B2_Subscribe_INCLUDED
