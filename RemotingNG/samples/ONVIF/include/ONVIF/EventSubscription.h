// 
// EventSubscription.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_EventSubscription_INCLUDED
#define ONVIF_EventSubscription_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace B2 {
class Filter;
} } } 
namespace ONVIF {
class SubscriptionPolicy;
} 


namespace ONVIF {


//@ name=EventSubscription
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API EventSubscription
{
public:
	EventSubscription();

	EventSubscription(
		const Poco::SharedPtr<OASIS::WSN::B2::Filter>& filter, 
		const Poco::SharedPtr<SubscriptionPolicy>& subscriptionPolicy);

	virtual ~EventSubscription();

	const Poco::SharedPtr<OASIS::WSN::B2::Filter>& getFilter() const;

	const Poco::SharedPtr<SubscriptionPolicy>& getSubscriptionPolicy() const;

	void setFilter(const Poco::SharedPtr<OASIS::WSN::B2::Filter>& val);

	void setFilter(Poco::SharedPtr<OASIS::WSN::B2::Filter>&& val);

	void setSubscriptionPolicy(const Poco::SharedPtr<SubscriptionPolicy>& val);

	void setSubscriptionPolicy(Poco::SharedPtr<SubscriptionPolicy>&& val);

private:
	//@ mandatory=false
	//@ name=Filter
	//@ order=0
	Poco::SharedPtr<OASIS::WSN::B2::Filter> _filter;

	//@ mandatory=false
	//@ name=SubscriptionPolicy
	//@ order=1
	Poco::SharedPtr<SubscriptionPolicy> _subscriptionPolicy;

};


inline const Poco::SharedPtr<OASIS::WSN::B2::Filter>& EventSubscription::getFilter() const
{
	return _filter;
}


inline const Poco::SharedPtr<SubscriptionPolicy>& EventSubscription::getSubscriptionPolicy() const
{
	return _subscriptionPolicy;
}


inline void EventSubscription::setFilter(const Poco::SharedPtr<OASIS::WSN::B2::Filter>& val)
{
	_filter = val;
}


inline void EventSubscription::setFilter(Poco::SharedPtr<OASIS::WSN::B2::Filter>&& val)
{
	_filter = std::move(val);
}


inline void EventSubscription::setSubscriptionPolicy(const Poco::SharedPtr<SubscriptionPolicy>& val)
{
	_subscriptionPolicy = val;
}


inline void EventSubscription::setSubscriptionPolicy(Poco::SharedPtr<SubscriptionPolicy>&& val)
{
	_subscriptionPolicy = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "OASIS/WSN/B2/Filter.h"
#include "ONVIF/SubscriptionPolicy.h"


#endif // ONVIF_EventSubscription_INCLUDED
