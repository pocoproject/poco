// 
// NotificationMessageHolder.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_NotificationMessageHolder_INCLUDED
#define OASIS_WSN_B2_NotificationMessageHolder_INCLUDED


#include "OASIS/WSN/B2/Message.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace B2 {
class TopicExpression;
} } } 
namespace W3 {
namespace Addressing {
class EndpointReference;
} } 


namespace OASIS {
namespace WSN {
namespace B2 {


//@ name=NotificationMessageHolderType
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API NotificationMessageHolder
{
public:
	NotificationMessageHolder();

	NotificationMessageHolder(
		const Poco::SharedPtr<W3::Addressing::EndpointReference>& subscriptionReference, 
		const Poco::SharedPtr<TopicExpression>& topic, 
		const Poco::SharedPtr<W3::Addressing::EndpointReference>& producerReference, 
		const Message& message);

	virtual ~NotificationMessageHolder();

	const Message& getMessage() const;

	const Poco::SharedPtr<W3::Addressing::EndpointReference>& getProducerReference() const;

	const Poco::SharedPtr<W3::Addressing::EndpointReference>& getSubscriptionReference() const;

	const Poco::SharedPtr<TopicExpression>& getTopic() const;

	void setMessage(const Message& val);

	void setMessage(Message&& val);

	void setProducerReference(const Poco::SharedPtr<W3::Addressing::EndpointReference>& val);

	void setProducerReference(Poco::SharedPtr<W3::Addressing::EndpointReference>&& val);

	void setSubscriptionReference(const Poco::SharedPtr<W3::Addressing::EndpointReference>& val);

	void setSubscriptionReference(Poco::SharedPtr<W3::Addressing::EndpointReference>&& val);

	void setTopic(const Poco::SharedPtr<TopicExpression>& val);

	void setTopic(Poco::SharedPtr<TopicExpression>&& val);

private:
	//@ mandatory=false
	//@ name=SubscriptionReference
	//@ order=0
	Poco::SharedPtr<W3::Addressing::EndpointReference> _subscriptionReference;

	//@ mandatory=false
	//@ name=Topic
	//@ order=1
	Poco::SharedPtr<TopicExpression> _topic;

	//@ mandatory=false
	//@ name=ProducerReference
	//@ order=2
	Poco::SharedPtr<W3::Addressing::EndpointReference> _producerReference;

	//@ name=Message
	//@ order=3
	Message _message;

};


inline const Message& NotificationMessageHolder::getMessage() const
{
	return _message;
}


inline const Poco::SharedPtr<W3::Addressing::EndpointReference>& NotificationMessageHolder::getProducerReference() const
{
	return _producerReference;
}


inline const Poco::SharedPtr<W3::Addressing::EndpointReference>& NotificationMessageHolder::getSubscriptionReference() const
{
	return _subscriptionReference;
}


inline const Poco::SharedPtr<TopicExpression>& NotificationMessageHolder::getTopic() const
{
	return _topic;
}


inline void NotificationMessageHolder::setMessage(const Message& val)
{
	_message = val;
}


inline void NotificationMessageHolder::setMessage(Message&& val)
{
	_message = std::move(val);
}


inline void NotificationMessageHolder::setProducerReference(const Poco::SharedPtr<W3::Addressing::EndpointReference>& val)
{
	_producerReference = val;
}


inline void NotificationMessageHolder::setProducerReference(Poco::SharedPtr<W3::Addressing::EndpointReference>&& val)
{
	_producerReference = std::move(val);
}


inline void NotificationMessageHolder::setSubscriptionReference(const Poco::SharedPtr<W3::Addressing::EndpointReference>& val)
{
	_subscriptionReference = val;
}


inline void NotificationMessageHolder::setSubscriptionReference(Poco::SharedPtr<W3::Addressing::EndpointReference>&& val)
{
	_subscriptionReference = std::move(val);
}


inline void NotificationMessageHolder::setTopic(const Poco::SharedPtr<TopicExpression>& val)
{
	_topic = val;
}


inline void NotificationMessageHolder::setTopic(Poco::SharedPtr<TopicExpression>&& val)
{
	_topic = std::move(val);
}


} } } // OASIS::WSN::B2


// The following headers are required for template instantiation.
#include "OASIS/WSN/B2/TopicExpression.h"
#include "W3/Addressing/EndpointReference.h"


#endif // OASIS_WSN_B2_NotificationMessageHolder_INCLUDED
