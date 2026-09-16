// 
// Amalgamated.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "OASIS/WSN/B2/CreatePullPoint.h"


namespace OASIS {
namespace WSN {
namespace B2 {


CreatePullPoint::CreatePullPoint()
{
}


CreatePullPoint::~CreatePullPoint()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/CreatePullPointResponse.h"


namespace OASIS {
namespace WSN {
namespace B2 {


CreatePullPointResponse::CreatePullPointResponse():
	_pullPoint()
{
}


CreatePullPointResponse::CreatePullPointResponse(const W3::Addressing::EndpointReference& pullPoint):
	_pullPoint(pullPoint)
{
}


CreatePullPointResponse::~CreatePullPointResponse()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/DestroyPullPoint.h"


namespace OASIS {
namespace WSN {
namespace B2 {


DestroyPullPoint::DestroyPullPoint()
{
}


DestroyPullPoint::~DestroyPullPoint()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/DestroyPullPointResponse.h"


namespace OASIS {
namespace WSN {
namespace B2 {


DestroyPullPointResponse::DestroyPullPointResponse()
{
}


DestroyPullPointResponse::~DestroyPullPointResponse()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/Filter.h"


namespace OASIS {
namespace WSN {
namespace B2 {


Filter::Filter()
{
}


Filter::~Filter()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/GetCurrentMessage.h"


namespace OASIS {
namespace WSN {
namespace B2 {


GetCurrentMessage::GetCurrentMessage():
	_topic()
{
}


GetCurrentMessage::GetCurrentMessage(const TopicExpression& topic):
	_topic(topic)
{
}


GetCurrentMessage::~GetCurrentMessage()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/GetCurrentMessageResponse.h"


namespace OASIS {
namespace WSN {
namespace B2 {


GetCurrentMessageResponse::GetCurrentMessageResponse()
{
}


GetCurrentMessageResponse::~GetCurrentMessageResponse()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/GetMessages.h"


namespace OASIS {
namespace WSN {
namespace B2 {


GetMessages::GetMessages():
	_maximumNumber()
{
}


GetMessages::GetMessages(Poco::Optional<Poco::UInt32> maximumNumber):
	_maximumNumber(maximumNumber)
{
}


GetMessages::~GetMessages()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/GetMessagesResponse.h"


namespace OASIS {
namespace WSN {
namespace B2 {


GetMessagesResponse::GetMessagesResponse():
	_notificationMessage()
{
}


GetMessagesResponse::GetMessagesResponse(const std::vector<NotificationMessageHolder>& notificationMessage):
	_notificationMessage(notificationMessage)
{
}


GetMessagesResponse::~GetMessagesResponse()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/InvalidFilterFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


InvalidFilterFault::InvalidFilterFault():
	_unknownFilter()
{
}


InvalidFilterFault::InvalidFilterFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause, const std::vector<std::string>& unknownFilter):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause),
	_unknownFilter(unknownFilter)
{
}


InvalidFilterFault::~InvalidFilterFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/InvalidMessageContentExpressionFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


InvalidMessageContentExpressionFault::InvalidMessageContentExpressionFault()
{
}


InvalidMessageContentExpressionFault::InvalidMessageContentExpressionFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


InvalidMessageContentExpressionFault::~InvalidMessageContentExpressionFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/InvalidProducerPropertiesExpressionFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


InvalidProducerPropertiesExpressionFault::InvalidProducerPropertiesExpressionFault()
{
}


InvalidProducerPropertiesExpressionFault::InvalidProducerPropertiesExpressionFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


InvalidProducerPropertiesExpressionFault::~InvalidProducerPropertiesExpressionFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/InvalidTopicExpressionFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


InvalidTopicExpressionFault::InvalidTopicExpressionFault()
{
}


InvalidTopicExpressionFault::InvalidTopicExpressionFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


InvalidTopicExpressionFault::~InvalidTopicExpressionFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/Message.h"


namespace OASIS {
namespace WSN {
namespace B2 {


Message::Message()
{
}


Message::~Message()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/MultipleTopicsSpecifiedFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


MultipleTopicsSpecifiedFault::MultipleTopicsSpecifiedFault()
{
}


MultipleTopicsSpecifiedFault::MultipleTopicsSpecifiedFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


MultipleTopicsSpecifiedFault::~MultipleTopicsSpecifiedFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/NoCurrentMessageOnTopicFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


NoCurrentMessageOnTopicFault::NoCurrentMessageOnTopicFault()
{
}


NoCurrentMessageOnTopicFault::NoCurrentMessageOnTopicFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


NoCurrentMessageOnTopicFault::~NoCurrentMessageOnTopicFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/NotificationMessageHolder.h"


namespace OASIS {
namespace WSN {
namespace B2 {


NotificationMessageHolder::NotificationMessageHolder():
	_subscriptionReference(),
	_topic(),
	_producerReference(),
	_message()
{
}


NotificationMessageHolder::NotificationMessageHolder(const Poco::SharedPtr<W3::Addressing::EndpointReference>& subscriptionReference, const Poco::SharedPtr<TopicExpression>& topic, const Poco::SharedPtr<W3::Addressing::EndpointReference>& producerReference, const Message& message):
	_subscriptionReference(subscriptionReference),
	_topic(topic),
	_producerReference(producerReference),
	_message(message)
{
}


NotificationMessageHolder::~NotificationMessageHolder()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/NotificationProducerRP.h"


namespace OASIS {
namespace WSN {
namespace B2 {


NotificationProducerRP::NotificationProducerRP():
	_topicExpression(),
	_fixedTopicSet(true),
	_topicExpressionDialect(),
	_topicSet()
{
}


NotificationProducerRP::NotificationProducerRP(const std::vector<TopicExpression>& topicExpression, Poco::Optional<bool> fixedTopicSet, const std::vector<Poco::URI>& topicExpressionDialect, const Poco::SharedPtr<OASIS::WSN::T1::TopicSet>& topicSet):
	_topicExpression(topicExpression),
	_fixedTopicSet(fixedTopicSet),
	_topicExpressionDialect(topicExpressionDialect),
	_topicSet(topicSet)
{
}


NotificationProducerRP::~NotificationProducerRP()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/Notify.h"


namespace OASIS {
namespace WSN {
namespace B2 {


Notify::Notify():
	_notificationMessage()
{
}


Notify::Notify(const std::vector<NotificationMessageHolder>& notificationMessage):
	_notificationMessage(notificationMessage)
{
}


Notify::~Notify()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/NotifyMessageNotSupportedFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


NotifyMessageNotSupportedFault::NotifyMessageNotSupportedFault()
{
}


NotifyMessageNotSupportedFault::NotifyMessageNotSupportedFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


NotifyMessageNotSupportedFault::~NotifyMessageNotSupportedFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/PauseFailedFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


PauseFailedFault::PauseFailedFault()
{
}


PauseFailedFault::PauseFailedFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


PauseFailedFault::~PauseFailedFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/PauseSubscription.h"


namespace OASIS {
namespace WSN {
namespace B2 {


PauseSubscription::PauseSubscription()
{
}


PauseSubscription::~PauseSubscription()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/PauseSubscriptionResponse.h"


namespace OASIS {
namespace WSN {
namespace B2 {


PauseSubscriptionResponse::PauseSubscriptionResponse()
{
}


PauseSubscriptionResponse::~PauseSubscriptionResponse()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/QueryExpression.h"


namespace OASIS {
namespace WSN {
namespace B2 {


QueryExpression::QueryExpression():
	_dialect()
{
}


QueryExpression::QueryExpression(const Poco::URI& dialect):
	_dialect(dialect)
{
}


QueryExpression::~QueryExpression()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/Renew.h"


namespace OASIS {
namespace WSN {
namespace B2 {


Renew::Renew():
	_terminationTime()
{
}


Renew::Renew(const Poco::Nullable<Poco::DateTime>& terminationTime):
	_terminationTime(terminationTime)
{
}


Renew::~Renew()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/RenewResponse.h"


namespace OASIS {
namespace WSN {
namespace B2 {


RenewResponse::RenewResponse():
	_terminationTime(),
	_currentTime()
{
}


RenewResponse::RenewResponse(const Poco::Nullable<Poco::DateTime>& terminationTime, const Poco::Optional<Poco::DateTime>& currentTime):
	_terminationTime(terminationTime),
	_currentTime(currentTime)
{
}


RenewResponse::~RenewResponse()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/ResumeFailedFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


ResumeFailedFault::ResumeFailedFault()
{
}


ResumeFailedFault::ResumeFailedFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


ResumeFailedFault::~ResumeFailedFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/ResumeSubscription.h"


namespace OASIS {
namespace WSN {
namespace B2 {


ResumeSubscription::ResumeSubscription()
{
}


ResumeSubscription::~ResumeSubscription()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/ResumeSubscriptionResponse.h"


namespace OASIS {
namespace WSN {
namespace B2 {


ResumeSubscriptionResponse::ResumeSubscriptionResponse()
{
}


ResumeSubscriptionResponse::~ResumeSubscriptionResponse()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/Subscribe.h"


namespace OASIS {
namespace WSN {
namespace B2 {


Subscribe::Subscribe():
	_consumerReference(),
	_filter(),
	_initialTerminationTime(),
	_subscriptionPolicy()
{
}


Subscribe::Subscribe(const W3::Addressing::EndpointReference& consumerReference, const Poco::SharedPtr<Filter>& filter, const Poco::Nullable<Poco::DateTime>& initialTerminationTime, const Poco::SharedPtr<SubscriptionPolicy>& subscriptionPolicy):
	_consumerReference(consumerReference),
	_filter(filter),
	_initialTerminationTime(initialTerminationTime),
	_subscriptionPolicy(subscriptionPolicy)
{
}


Subscribe::~Subscribe()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/SubscribeCreationFailedFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


SubscribeCreationFailedFault::SubscribeCreationFailedFault()
{
}


SubscribeCreationFailedFault::SubscribeCreationFailedFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


SubscribeCreationFailedFault::~SubscribeCreationFailedFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/SubscribeResponse.h"


namespace OASIS {
namespace WSN {
namespace B2 {


SubscribeResponse::SubscribeResponse():
	_subscriptionReference(),
	_currentTime(),
	_terminationTime()
{
}


SubscribeResponse::SubscribeResponse(const W3::Addressing::EndpointReference& subscriptionReference, const Poco::Optional<Poco::DateTime>& currentTime, const Poco::Nullable<Poco::DateTime>& terminationTime):
	_subscriptionReference(subscriptionReference),
	_currentTime(currentTime),
	_terminationTime(terminationTime)
{
}


SubscribeResponse::~SubscribeResponse()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/SubscriptionManagerRP.h"


namespace OASIS {
namespace WSN {
namespace B2 {


SubscriptionManagerRP::SubscriptionManagerRP():
	_consumerReference(),
	_filter(),
	_subscriptionPolicy(),
	_creationTime()
{
}


SubscriptionManagerRP::SubscriptionManagerRP(const W3::Addressing::EndpointReference& consumerReference, const Poco::SharedPtr<Filter>& filter, const Poco::SharedPtr<SubscriptionPolicy>& subscriptionPolicy, const Poco::Optional<Poco::DateTime>& creationTime):
	_consumerReference(consumerReference),
	_filter(filter),
	_subscriptionPolicy(subscriptionPolicy),
	_creationTime(creationTime)
{
}


SubscriptionManagerRP::~SubscriptionManagerRP()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/SubscriptionPolicy.h"


namespace OASIS {
namespace WSN {
namespace B2 {


SubscriptionPolicy::SubscriptionPolicy()
{
}


SubscriptionPolicy::~SubscriptionPolicy()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/TopicExpression.h"


namespace OASIS {
namespace WSN {
namespace B2 {


TopicExpression::TopicExpression():
	_dialect()
{
}


TopicExpression::TopicExpression(const Poco::URI& dialect):
	_dialect(dialect)
{
}


TopicExpression::~TopicExpression()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/TopicExpressionDialectUnknownFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


TopicExpressionDialectUnknownFault::TopicExpressionDialectUnknownFault()
{
}


TopicExpressionDialectUnknownFault::TopicExpressionDialectUnknownFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


TopicExpressionDialectUnknownFault::~TopicExpressionDialectUnknownFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/TopicNotSupportedFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


TopicNotSupportedFault::TopicNotSupportedFault()
{
}


TopicNotSupportedFault::TopicNotSupportedFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


TopicNotSupportedFault::~TopicNotSupportedFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/UnableToCreatePullPointFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


UnableToCreatePullPointFault::UnableToCreatePullPointFault()
{
}


UnableToCreatePullPointFault::UnableToCreatePullPointFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


UnableToCreatePullPointFault::~UnableToCreatePullPointFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/UnableToDestroyPullPointFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


UnableToDestroyPullPointFault::UnableToDestroyPullPointFault()
{
}


UnableToDestroyPullPointFault::UnableToDestroyPullPointFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


UnableToDestroyPullPointFault::~UnableToDestroyPullPointFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/UnableToDestroySubscriptionFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


UnableToDestroySubscriptionFault::UnableToDestroySubscriptionFault()
{
}


UnableToDestroySubscriptionFault::UnableToDestroySubscriptionFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


UnableToDestroySubscriptionFault::~UnableToDestroySubscriptionFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/UnableToGetMessagesFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


UnableToGetMessagesFault::UnableToGetMessagesFault()
{
}


UnableToGetMessagesFault::UnableToGetMessagesFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause)
{
}


UnableToGetMessagesFault::~UnableToGetMessagesFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/UnacceptableInitialTerminationTimeFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


UnacceptableInitialTerminationTimeFault::UnacceptableInitialTerminationTimeFault():
	_minimumTime(),
	_maximumTime()
{
}


UnacceptableInitialTerminationTimeFault::UnacceptableInitialTerminationTimeFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause, const Poco::DateTime& minimumTime, const Poco::Optional<Poco::DateTime>& maximumTime):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause),
	_minimumTime(minimumTime),
	_maximumTime(maximumTime)
{
}


UnacceptableInitialTerminationTimeFault::~UnacceptableInitialTerminationTimeFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/UnacceptableTerminationTimeFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


UnacceptableTerminationTimeFault::UnacceptableTerminationTimeFault():
	_minimumTime(),
	_maximumTime()
{
}


UnacceptableTerminationTimeFault::UnacceptableTerminationTimeFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause, const Poco::DateTime& minimumTime, const Poco::Optional<Poco::DateTime>& maximumTime):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause),
	_minimumTime(minimumTime),
	_maximumTime(maximumTime)
{
}


UnacceptableTerminationTimeFault::~UnacceptableTerminationTimeFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/UnrecognizedPolicyRequestFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


UnrecognizedPolicyRequestFault::UnrecognizedPolicyRequestFault():
	_unrecognizedPolicy()
{
}


UnrecognizedPolicyRequestFault::UnrecognizedPolicyRequestFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause, const std::vector<std::string>& unrecognizedPolicy):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause),
	_unrecognizedPolicy(unrecognizedPolicy)
{
}


UnrecognizedPolicyRequestFault::~UnrecognizedPolicyRequestFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/Unsubscribe.h"


namespace OASIS {
namespace WSN {
namespace B2 {


Unsubscribe::Unsubscribe()
{
}


Unsubscribe::~Unsubscribe()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/UnsubscribeResponse.h"


namespace OASIS {
namespace WSN {
namespace B2 {


UnsubscribeResponse::UnsubscribeResponse()
{
}


UnsubscribeResponse::~UnsubscribeResponse()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/UnsupportedPolicyRequestFault.h"


namespace OASIS {
namespace WSN {
namespace B2 {


UnsupportedPolicyRequestFault::UnsupportedPolicyRequestFault():
	_unsupportedPolicy()
{
}


UnsupportedPolicyRequestFault::UnsupportedPolicyRequestFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, const std::vector<OASIS::WSRF::BF2::Description>& description, const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause, const std::vector<std::string>& unsupportedPolicy):
	OASIS::WSRF::BF2::BaseFault(timestamp, originator, errorCode, description, faultCause),
	_unsupportedPolicy(unsupportedPolicy)
{
}


UnsupportedPolicyRequestFault::~UnsupportedPolicyRequestFault()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/B2/UseRaw.h"


namespace OASIS {
namespace WSN {
namespace B2 {


UseRaw::UseRaw()
{
}


UseRaw::~UseRaw()
{
}


} } } // OASIS::WSN::B2


#include "OASIS/WSN/T1/Documentation.h"


namespace OASIS {
namespace WSN {
namespace T1 {


Documentation::Documentation()
{
}


Documentation::~Documentation()
{
}


} } } // OASIS::WSN::T1


#include "OASIS/WSN/T1/ExtensibleDocumented.h"


namespace OASIS {
namespace WSN {
namespace T1 {


ExtensibleDocumented::ExtensibleDocumented():
	_documentation()
{
}


ExtensibleDocumented::ExtensibleDocumented(const Poco::SharedPtr<Documentation>& documentation):
	_documentation(documentation)
{
}


ExtensibleDocumented::~ExtensibleDocumented()
{
}


} } } // OASIS::WSN::T1


#include "OASIS/WSN/T1/QueryExpression.h"


namespace OASIS {
namespace WSN {
namespace T1 {


QueryExpression::QueryExpression():
	_dialect()
{
}


QueryExpression::QueryExpression(const Poco::URI& dialect):
	_dialect(dialect)
{
}


QueryExpression::~QueryExpression()
{
}


} } } // OASIS::WSN::T1


#include "OASIS/WSN/T1/Topic.h"


namespace OASIS {
namespace WSN {
namespace T1 {


Topic::Topic():
	_parent()
{
}


Topic::Topic(const Poco::SharedPtr<Documentation>& documentation, const Poco::Optional<std::string>& parent):
	ExtensibleDocumented(documentation),
	_parent(parent)
{
}


Topic::~Topic()
{
}


} } } // OASIS::WSN::T1


#include "OASIS/WSN/T1/TopicNamespace.h"


namespace OASIS {
namespace WSN {
namespace T1 {


TopicNamespace::TopicNamespace():
	_name(),
	_targetNamespace(),
	_final(false),
	_topic()
{
}


TopicNamespace::TopicNamespace(const Poco::SharedPtr<Documentation>& documentation, const Poco::Optional<std::string>& name, const Poco::URI& targetNamespace, Poco::Optional<bool> final, const std::vector<Topic>& topic):
	ExtensibleDocumented(documentation),
	_name(name),
	_targetNamespace(targetNamespace),
	_final(final),
	_topic(topic)
{
}


TopicNamespace::~TopicNamespace()
{
}


} } } // OASIS::WSN::T1


#include "OASIS/WSN/T1/TopicSet.h"


namespace OASIS {
namespace WSN {
namespace T1 {


TopicSet::TopicSet()
{
}


TopicSet::TopicSet(const Poco::SharedPtr<Documentation>& documentation):
	ExtensibleDocumented(documentation)
{
}


TopicSet::~TopicSet()
{
}


} } } // OASIS::WSN::T1


#include "OASIS/WSRF/BF2/BaseFault.h"


namespace OASIS {
namespace WSRF {
namespace BF2 {


BaseFault::BaseFault():
	_timestamp(),
	_originator(),
	_errorCode(),
	_description(),
	_faultCause()
{
}


BaseFault::BaseFault(const Poco::DateTime& timestamp, const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, const Poco::SharedPtr<ErrorCode>& errorCode, const std::vector<Description>& description, const Poco::SharedPtr<FaultCause>& faultCause):
	_timestamp(timestamp),
	_originator(originator),
	_errorCode(errorCode),
	_description(description),
	_faultCause(faultCause)
{
}


BaseFault::~BaseFault()
{
}


} } } // OASIS::WSRF::BF2


#include "OASIS/WSRF/BF2/Description.h"


namespace OASIS {
namespace WSRF {
namespace BF2 {


Description::Description():
	_value()
{
}


Description::Description(const Poco::Optional<std::string>& value):
	_value(value)
{
}


Description::~Description()
{
}


} } } // OASIS::WSRF::BF2


#include "OASIS/WSRF/BF2/ErrorCode.h"


namespace OASIS {
namespace WSRF {
namespace BF2 {


ErrorCode::ErrorCode():
	_value()
{
}


ErrorCode::ErrorCode(const Poco::Optional<std::string>& value):
	_value(value)
{
}


ErrorCode::~ErrorCode()
{
}


} } } // OASIS::WSRF::BF2


#include "OASIS/WSRF/BF2/FaultCause.h"


namespace OASIS {
namespace WSRF {
namespace BF2 {


FaultCause::FaultCause()
{
}


FaultCause::~FaultCause()
{
}


} } } // OASIS::WSRF::BF2


#include "ONVIF/Device/AddIPAddressFilter.h"


namespace ONVIF {
namespace Device {


AddIPAddressFilter::AddIPAddressFilter():
	_iPAddressFilter()
{
}


AddIPAddressFilter::AddIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter):
	_iPAddressFilter(iPAddressFilter)
{
}


AddIPAddressFilter::~AddIPAddressFilter()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/AddIPAddressFilterResponse.h"


namespace ONVIF {
namespace Device {


AddIPAddressFilterResponse::AddIPAddressFilterResponse()
{
}


AddIPAddressFilterResponse::~AddIPAddressFilterResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/AddScopes.h"


namespace ONVIF {
namespace Device {


AddScopes::AddScopes():
	_scopeItem()
{
}


AddScopes::AddScopes(const std::vector<Poco::URI>& scopeItem):
	_scopeItem(scopeItem)
{
}


AddScopes::~AddScopes()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/AddScopesResponse.h"


namespace ONVIF {
namespace Device {


AddScopesResponse::AddScopesResponse()
{
}


AddScopesResponse::~AddScopesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/Capabilities.h"


namespace ONVIF {
namespace Device {


Capabilities::Capabilities()
{
}


Capabilities::~Capabilities()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/CreateCertificate.h"


namespace ONVIF {
namespace Device {


CreateCertificate::CreateCertificate():
	_certificateID(),
	_subject(),
	_validNotBefore(),
	_validNotAfter()
{
}


CreateCertificate::CreateCertificate(const Poco::Optional<std::string>& certificateID, const Poco::Optional<std::string>& subject, const Poco::Optional<Poco::DateTime>& validNotBefore, const Poco::Optional<Poco::DateTime>& validNotAfter):
	_certificateID(certificateID),
	_subject(subject),
	_validNotBefore(validNotBefore),
	_validNotAfter(validNotAfter)
{
}


CreateCertificate::~CreateCertificate()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/CreateCertificateResponse.h"


namespace ONVIF {
namespace Device {


CreateCertificateResponse::CreateCertificateResponse():
	_nvtCertificate()
{
}


CreateCertificateResponse::CreateCertificateResponse(const ONVIF::Certificate& nvtCertificate):
	_nvtCertificate(nvtCertificate)
{
}


CreateCertificateResponse::~CreateCertificateResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/CreateDot1XConfiguration.h"


namespace ONVIF {
namespace Device {


CreateDot1XConfiguration::CreateDot1XConfiguration():
	_dot1XConfiguration()
{
}


CreateDot1XConfiguration::CreateDot1XConfiguration(const ONVIF::Dot1XConfiguration& dot1XConfiguration):
	_dot1XConfiguration(dot1XConfiguration)
{
}


CreateDot1XConfiguration::~CreateDot1XConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/CreateDot1XConfigurationResponse.h"


namespace ONVIF {
namespace Device {


CreateDot1XConfigurationResponse::CreateDot1XConfigurationResponse()
{
}


CreateDot1XConfigurationResponse::~CreateDot1XConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/CreateStorageConfiguration.h"


namespace ONVIF {
namespace Device {


CreateStorageConfiguration::CreateStorageConfiguration():
	_storageConfiguration()
{
}


CreateStorageConfiguration::CreateStorageConfiguration(const StorageConfigurationData& storageConfiguration):
	_storageConfiguration(storageConfiguration)
{
}


CreateStorageConfiguration::~CreateStorageConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/CreateStorageConfigurationResponse.h"


namespace ONVIF {
namespace Device {


CreateStorageConfigurationResponse::CreateStorageConfigurationResponse():
	_token()
{
}


CreateStorageConfigurationResponse::CreateStorageConfigurationResponse(const std::string& token):
	_token(token)
{
}


CreateStorageConfigurationResponse::~CreateStorageConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/CreateUsers.h"


namespace ONVIF {
namespace Device {


CreateUsers::CreateUsers():
	_user()
{
}


CreateUsers::CreateUsers(const std::vector<ONVIF::User>& user):
	_user(user)
{
}


CreateUsers::~CreateUsers()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/CreateUsersResponse.h"


namespace ONVIF {
namespace Device {


CreateUsersResponse::CreateUsersResponse()
{
}


CreateUsersResponse::~CreateUsersResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeleteCertificates.h"


namespace ONVIF {
namespace Device {


DeleteCertificates::DeleteCertificates():
	_certificateID()
{
}


DeleteCertificates::DeleteCertificates(const std::vector<std::string>& certificateID):
	_certificateID(certificateID)
{
}


DeleteCertificates::~DeleteCertificates()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeleteCertificatesResponse.h"


namespace ONVIF {
namespace Device {


DeleteCertificatesResponse::DeleteCertificatesResponse()
{
}


DeleteCertificatesResponse::~DeleteCertificatesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeleteDot1XConfiguration.h"


namespace ONVIF {
namespace Device {


DeleteDot1XConfiguration::DeleteDot1XConfiguration():
	_dot1XConfigurationToken()
{
}


DeleteDot1XConfiguration::DeleteDot1XConfiguration(const std::vector<std::string>& dot1XConfigurationToken):
	_dot1XConfigurationToken(dot1XConfigurationToken)
{
}


DeleteDot1XConfiguration::~DeleteDot1XConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeleteDot1XConfigurationResponse.h"


namespace ONVIF {
namespace Device {


DeleteDot1XConfigurationResponse::DeleteDot1XConfigurationResponse()
{
}


DeleteDot1XConfigurationResponse::~DeleteDot1XConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeleteGeoLocation.h"


namespace ONVIF {
namespace Device {


DeleteGeoLocation::DeleteGeoLocation():
	_location()
{
}


DeleteGeoLocation::DeleteGeoLocation(const std::vector<ONVIF::LocationEntity>& location):
	_location(location)
{
}


DeleteGeoLocation::~DeleteGeoLocation()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeleteGeoLocationResponse.h"


namespace ONVIF {
namespace Device {


DeleteGeoLocationResponse::DeleteGeoLocationResponse()
{
}


DeleteGeoLocationResponse::~DeleteGeoLocationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeleteStorageConfiguration.h"


namespace ONVIF {
namespace Device {


DeleteStorageConfiguration::DeleteStorageConfiguration():
	_token()
{
}


DeleteStorageConfiguration::DeleteStorageConfiguration(const std::string& token):
	_token(token)
{
}


DeleteStorageConfiguration::~DeleteStorageConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeleteStorageConfigurationResponse.h"


namespace ONVIF {
namespace Device {


DeleteStorageConfigurationResponse::DeleteStorageConfigurationResponse()
{
}


DeleteStorageConfigurationResponse::~DeleteStorageConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeleteUsers.h"


namespace ONVIF {
namespace Device {


DeleteUsers::DeleteUsers():
	_username()
{
}


DeleteUsers::DeleteUsers(const std::vector<std::string>& username):
	_username(username)
{
}


DeleteUsers::~DeleteUsers()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeleteUsersResponse.h"


namespace ONVIF {
namespace Device {


DeleteUsersResponse::DeleteUsersResponse()
{
}


DeleteUsersResponse::~DeleteUsersResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeviceBinding.h"


namespace ONVIF {
namespace Device {


DeviceBinding::~DeviceBinding()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/DeviceServiceCapabilities.h"


namespace ONVIF {
namespace Device {


DeviceServiceCapabilities::DeviceServiceCapabilities():
	_network(),
	_security(),
	_system(),
	_misc()
{
}


DeviceServiceCapabilities::DeviceServiceCapabilities(const NetworkCapabilities& network, const SecurityCapabilities& security, const SystemCapabilities& system, const Poco::SharedPtr<MiscCapabilities>& misc):
	_network(network),
	_security(security),
	_system(system),
	_misc(misc)
{
}


DeviceServiceCapabilities::~DeviceServiceCapabilities()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/Extension.h"


namespace ONVIF {
namespace Device {


Extension::Extension()
{
}


Extension::~Extension()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/Extension_1.h"


namespace ONVIF {
namespace Device {


Extension_1::Extension_1()
{
}


Extension_1::~Extension_1()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/Extension_2.h"


namespace ONVIF {
namespace Device {


Extension_2::Extension_2()
{
}


Extension_2::~Extension_2()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetAccessPolicy.h"


namespace ONVIF {
namespace Device {


GetAccessPolicy::GetAccessPolicy()
{
}


GetAccessPolicy::~GetAccessPolicy()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetAccessPolicyResponse.h"


namespace ONVIF {
namespace Device {


GetAccessPolicyResponse::GetAccessPolicyResponse():
	_policyFile()
{
}


GetAccessPolicyResponse::GetAccessPolicyResponse(const ONVIF::BinaryData& policyFile):
	_policyFile(policyFile)
{
}


GetAccessPolicyResponse::~GetAccessPolicyResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetAuthFailureWarningConfiguration.h"


namespace ONVIF {
namespace Device {


GetAuthFailureWarningConfiguration::GetAuthFailureWarningConfiguration()
{
}


GetAuthFailureWarningConfiguration::~GetAuthFailureWarningConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetAuthFailureWarningConfigurationResponse.h"


namespace ONVIF {
namespace Device {


GetAuthFailureWarningConfigurationResponse::GetAuthFailureWarningConfigurationResponse():
	_enabled(),
	_monitorPeriod(),
	_maxAuthFailures()
{
}


GetAuthFailureWarningConfigurationResponse::GetAuthFailureWarningConfigurationResponse(bool enabled, int monitorPeriod, int maxAuthFailures):
	_enabled(enabled),
	_monitorPeriod(monitorPeriod),
	_maxAuthFailures(maxAuthFailures)
{
}


GetAuthFailureWarningConfigurationResponse::~GetAuthFailureWarningConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetAuthFailureWarningOptions.h"


namespace ONVIF {
namespace Device {


GetAuthFailureWarningOptions::GetAuthFailureWarningOptions()
{
}


GetAuthFailureWarningOptions::~GetAuthFailureWarningOptions()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetAuthFailureWarningOptionsResponse.h"


namespace ONVIF {
namespace Device {


GetAuthFailureWarningOptionsResponse::GetAuthFailureWarningOptionsResponse():
	_monitorPeriodRange(),
	_authFailureRange()
{
}


GetAuthFailureWarningOptionsResponse::GetAuthFailureWarningOptionsResponse(const ONVIF::IntRange& monitorPeriodRange, const ONVIF::IntRange& authFailureRange):
	_monitorPeriodRange(monitorPeriodRange),
	_authFailureRange(authFailureRange)
{
}


GetAuthFailureWarningOptionsResponse::~GetAuthFailureWarningOptionsResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetCACertificates.h"


namespace ONVIF {
namespace Device {


GetCACertificates::GetCACertificates()
{
}


GetCACertificates::~GetCACertificates()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetCACertificatesResponse.h"


namespace ONVIF {
namespace Device {


GetCACertificatesResponse::GetCACertificatesResponse():
	_cACertificate()
{
}


GetCACertificatesResponse::GetCACertificatesResponse(const std::vector<ONVIF::Certificate>& cACertificate):
	_cACertificate(cACertificate)
{
}


GetCACertificatesResponse::~GetCACertificatesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetCapabilities.h"


namespace ONVIF {
namespace Device {


GetCapabilities::GetCapabilities():
	_category()
{
}


GetCapabilities::GetCapabilities(const std::vector<std::string>& category):
	_category(category)
{
}


GetCapabilities::~GetCapabilities()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetCapabilitiesResponse.h"


namespace ONVIF {
namespace Device {


GetCapabilitiesResponse::GetCapabilitiesResponse():
	_capabilities()
{
}


GetCapabilitiesResponse::GetCapabilitiesResponse(const ONVIF::Capabilities& capabilities):
	_capabilities(capabilities)
{
}


GetCapabilitiesResponse::~GetCapabilitiesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetCertificateInformation.h"


namespace ONVIF {
namespace Device {


GetCertificateInformation::GetCertificateInformation():
	_certificateID()
{
}


GetCertificateInformation::GetCertificateInformation(const std::string& certificateID):
	_certificateID(certificateID)
{
}


GetCertificateInformation::~GetCertificateInformation()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetCertificateInformationResponse.h"


namespace ONVIF {
namespace Device {


GetCertificateInformationResponse::GetCertificateInformationResponse():
	_certificateInformation()
{
}


GetCertificateInformationResponse::GetCertificateInformationResponse(const ONVIF::CertificateInformation& certificateInformation):
	_certificateInformation(certificateInformation)
{
}


GetCertificateInformationResponse::~GetCertificateInformationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetCertificates.h"


namespace ONVIF {
namespace Device {


GetCertificates::GetCertificates()
{
}


GetCertificates::~GetCertificates()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetCertificatesResponse.h"


namespace ONVIF {
namespace Device {


GetCertificatesResponse::GetCertificatesResponse():
	_nvtCertificate()
{
}


GetCertificatesResponse::GetCertificatesResponse(const std::vector<ONVIF::Certificate>& nvtCertificate):
	_nvtCertificate(nvtCertificate)
{
}


GetCertificatesResponse::~GetCertificatesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetCertificatesStatus.h"


namespace ONVIF {
namespace Device {


GetCertificatesStatus::GetCertificatesStatus()
{
}


GetCertificatesStatus::~GetCertificatesStatus()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetCertificatesStatusResponse.h"


namespace ONVIF {
namespace Device {


GetCertificatesStatusResponse::GetCertificatesStatusResponse():
	_certificateStatus()
{
}


GetCertificatesStatusResponse::GetCertificatesStatusResponse(const std::vector<ONVIF::CertificateStatus>& certificateStatus):
	_certificateStatus(certificateStatus)
{
}


GetCertificatesStatusResponse::~GetCertificatesStatusResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetClientCertificateMode.h"


namespace ONVIF {
namespace Device {


GetClientCertificateMode::GetClientCertificateMode()
{
}


GetClientCertificateMode::~GetClientCertificateMode()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetClientCertificateModeResponse.h"


namespace ONVIF {
namespace Device {


GetClientCertificateModeResponse::GetClientCertificateModeResponse():
	_enabled()
{
}


GetClientCertificateModeResponse::GetClientCertificateModeResponse(bool enabled):
	_enabled(enabled)
{
}


GetClientCertificateModeResponse::~GetClientCertificateModeResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDNS.h"


namespace ONVIF {
namespace Device {


GetDNS::GetDNS()
{
}


GetDNS::~GetDNS()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDNSResponse.h"


namespace ONVIF {
namespace Device {


GetDNSResponse::GetDNSResponse():
	_dNSInformation()
{
}


GetDNSResponse::GetDNSResponse(const ONVIF::DNSInformation& dNSInformation):
	_dNSInformation(dNSInformation)
{
}


GetDNSResponse::~GetDNSResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDPAddresses.h"


namespace ONVIF {
namespace Device {


GetDPAddresses::GetDPAddresses()
{
}


GetDPAddresses::~GetDPAddresses()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDPAddressesResponse.h"


namespace ONVIF {
namespace Device {


GetDPAddressesResponse::GetDPAddressesResponse():
	_dPAddress()
{
}


GetDPAddressesResponse::GetDPAddressesResponse(const std::vector<ONVIF::NetworkHost>& dPAddress):
	_dPAddress(dPAddress)
{
}


GetDPAddressesResponse::~GetDPAddressesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDeviceInformation.h"


namespace ONVIF {
namespace Device {


GetDeviceInformation::GetDeviceInformation()
{
}


GetDeviceInformation::~GetDeviceInformation()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDeviceInformationResponse.h"


namespace ONVIF {
namespace Device {


GetDeviceInformationResponse::GetDeviceInformationResponse():
	_manufacturer(),
	_model(),
	_firmwareVersion(),
	_serialNumber(),
	_hardwareId()
{
}


GetDeviceInformationResponse::GetDeviceInformationResponse(const std::string& manufacturer, const std::string& model, const std::string& firmwareVersion, const std::string& serialNumber, const std::string& hardwareId):
	_manufacturer(manufacturer),
	_model(model),
	_firmwareVersion(firmwareVersion),
	_serialNumber(serialNumber),
	_hardwareId(hardwareId)
{
}


GetDeviceInformationResponse::~GetDeviceInformationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDiscoveryMode.h"


namespace ONVIF {
namespace Device {


GetDiscoveryMode::GetDiscoveryMode()
{
}


GetDiscoveryMode::~GetDiscoveryMode()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDiscoveryModeResponse.h"


namespace ONVIF {
namespace Device {


GetDiscoveryModeResponse::GetDiscoveryModeResponse():
	_discoveryMode()
{
}


GetDiscoveryModeResponse::GetDiscoveryModeResponse(const std::string& discoveryMode):
	_discoveryMode(discoveryMode)
{
}


GetDiscoveryModeResponse::~GetDiscoveryModeResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDot11Capabilities.h"


namespace ONVIF {
namespace Device {


GetDot11Capabilities::GetDot11Capabilities()
{
}


GetDot11Capabilities::~GetDot11Capabilities()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDot11CapabilitiesResponse.h"


namespace ONVIF {
namespace Device {


GetDot11CapabilitiesResponse::GetDot11CapabilitiesResponse():
	_capabilities()
{
}


GetDot11CapabilitiesResponse::GetDot11CapabilitiesResponse(const ONVIF::Dot11Capabilities& capabilities):
	_capabilities(capabilities)
{
}


GetDot11CapabilitiesResponse::~GetDot11CapabilitiesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDot11Status.h"


namespace ONVIF {
namespace Device {


GetDot11Status::GetDot11Status():
	_interfaceToken()
{
}


GetDot11Status::GetDot11Status(const std::string& interfaceToken):
	_interfaceToken(interfaceToken)
{
}


GetDot11Status::~GetDot11Status()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDot11StatusResponse.h"


namespace ONVIF {
namespace Device {


GetDot11StatusResponse::GetDot11StatusResponse():
	_status()
{
}


GetDot11StatusResponse::GetDot11StatusResponse(const ONVIF::Dot11Status& status):
	_status(status)
{
}


GetDot11StatusResponse::~GetDot11StatusResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDot1XConfiguration.h"


namespace ONVIF {
namespace Device {


GetDot1XConfiguration::GetDot1XConfiguration():
	_dot1XConfigurationToken()
{
}


GetDot1XConfiguration::GetDot1XConfiguration(const std::string& dot1XConfigurationToken):
	_dot1XConfigurationToken(dot1XConfigurationToken)
{
}


GetDot1XConfiguration::~GetDot1XConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDot1XConfigurationResponse.h"


namespace ONVIF {
namespace Device {


GetDot1XConfigurationResponse::GetDot1XConfigurationResponse():
	_dot1XConfiguration()
{
}


GetDot1XConfigurationResponse::GetDot1XConfigurationResponse(const ONVIF::Dot1XConfiguration& dot1XConfiguration):
	_dot1XConfiguration(dot1XConfiguration)
{
}


GetDot1XConfigurationResponse::~GetDot1XConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDot1XConfigurations.h"


namespace ONVIF {
namespace Device {


GetDot1XConfigurations::GetDot1XConfigurations()
{
}


GetDot1XConfigurations::~GetDot1XConfigurations()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDot1XConfigurationsResponse.h"


namespace ONVIF {
namespace Device {


GetDot1XConfigurationsResponse::GetDot1XConfigurationsResponse():
	_dot1XConfiguration()
{
}


GetDot1XConfigurationsResponse::GetDot1XConfigurationsResponse(const std::vector<ONVIF::Dot1XConfiguration>& dot1XConfiguration):
	_dot1XConfiguration(dot1XConfiguration)
{
}


GetDot1XConfigurationsResponse::~GetDot1XConfigurationsResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDynamicDNS.h"


namespace ONVIF {
namespace Device {


GetDynamicDNS::GetDynamicDNS()
{
}


GetDynamicDNS::~GetDynamicDNS()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetDynamicDNSResponse.h"


namespace ONVIF {
namespace Device {


GetDynamicDNSResponse::GetDynamicDNSResponse():
	_dynamicDNSInformation()
{
}


GetDynamicDNSResponse::GetDynamicDNSResponse(const ONVIF::DynamicDNSInformation& dynamicDNSInformation):
	_dynamicDNSInformation(dynamicDNSInformation)
{
}


GetDynamicDNSResponse::~GetDynamicDNSResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetEndpointReference.h"


namespace ONVIF {
namespace Device {


GetEndpointReference::GetEndpointReference()
{
}


GetEndpointReference::~GetEndpointReference()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetEndpointReferenceResponse.h"


namespace ONVIF {
namespace Device {


GetEndpointReferenceResponse::GetEndpointReferenceResponse():
	_gUID()
{
}


GetEndpointReferenceResponse::GetEndpointReferenceResponse(const std::string& gUID):
	_gUID(gUID)
{
}


GetEndpointReferenceResponse::~GetEndpointReferenceResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetGeoLocation.h"


namespace ONVIF {
namespace Device {


GetGeoLocation::GetGeoLocation()
{
}


GetGeoLocation::~GetGeoLocation()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetGeoLocationResponse.h"


namespace ONVIF {
namespace Device {


GetGeoLocationResponse::GetGeoLocationResponse():
	_location()
{
}


GetGeoLocationResponse::GetGeoLocationResponse(const std::vector<ONVIF::LocationEntity>& location):
	_location(location)
{
}


GetGeoLocationResponse::~GetGeoLocationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetHostname.h"


namespace ONVIF {
namespace Device {


GetHostname::GetHostname()
{
}


GetHostname::~GetHostname()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetHostnameResponse.h"


namespace ONVIF {
namespace Device {


GetHostnameResponse::GetHostnameResponse():
	_hostnameInformation()
{
}


GetHostnameResponse::GetHostnameResponse(const ONVIF::HostnameInformation& hostnameInformation):
	_hostnameInformation(hostnameInformation)
{
}


GetHostnameResponse::~GetHostnameResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetIPAddressFilter.h"


namespace ONVIF {
namespace Device {


GetIPAddressFilter::GetIPAddressFilter()
{
}


GetIPAddressFilter::~GetIPAddressFilter()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetIPAddressFilterResponse.h"


namespace ONVIF {
namespace Device {


GetIPAddressFilterResponse::GetIPAddressFilterResponse():
	_iPAddressFilter()
{
}


GetIPAddressFilterResponse::GetIPAddressFilterResponse(const ONVIF::IPAddressFilter& iPAddressFilter):
	_iPAddressFilter(iPAddressFilter)
{
}


GetIPAddressFilterResponse::~GetIPAddressFilterResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetNTP.h"


namespace ONVIF {
namespace Device {


GetNTP::GetNTP()
{
}


GetNTP::~GetNTP()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetNTPResponse.h"


namespace ONVIF {
namespace Device {


GetNTPResponse::GetNTPResponse():
	_nTPInformation()
{
}


GetNTPResponse::GetNTPResponse(const ONVIF::NTPInformation& nTPInformation):
	_nTPInformation(nTPInformation)
{
}


GetNTPResponse::~GetNTPResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetNetworkDefaultGateway.h"


namespace ONVIF {
namespace Device {


GetNetworkDefaultGateway::GetNetworkDefaultGateway()
{
}


GetNetworkDefaultGateway::~GetNetworkDefaultGateway()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetNetworkDefaultGatewayResponse.h"


namespace ONVIF {
namespace Device {


GetNetworkDefaultGatewayResponse::GetNetworkDefaultGatewayResponse():
	_networkGateway()
{
}


GetNetworkDefaultGatewayResponse::GetNetworkDefaultGatewayResponse(const ONVIF::NetworkGateway& networkGateway):
	_networkGateway(networkGateway)
{
}


GetNetworkDefaultGatewayResponse::~GetNetworkDefaultGatewayResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetNetworkInterfaces.h"


namespace ONVIF {
namespace Device {


GetNetworkInterfaces::GetNetworkInterfaces()
{
}


GetNetworkInterfaces::~GetNetworkInterfaces()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetNetworkInterfacesResponse.h"


namespace ONVIF {
namespace Device {


GetNetworkInterfacesResponse::GetNetworkInterfacesResponse():
	_networkInterfaces()
{
}


GetNetworkInterfacesResponse::GetNetworkInterfacesResponse(const std::vector<ONVIF::NetworkInterface>& networkInterfaces):
	_networkInterfaces(networkInterfaces)
{
}


GetNetworkInterfacesResponse::~GetNetworkInterfacesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetNetworkProtocols.h"


namespace ONVIF {
namespace Device {


GetNetworkProtocols::GetNetworkProtocols()
{
}


GetNetworkProtocols::~GetNetworkProtocols()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetNetworkProtocolsResponse.h"


namespace ONVIF {
namespace Device {


GetNetworkProtocolsResponse::GetNetworkProtocolsResponse():
	_networkProtocols()
{
}


GetNetworkProtocolsResponse::GetNetworkProtocolsResponse(const std::vector<ONVIF::NetworkProtocol>& networkProtocols):
	_networkProtocols(networkProtocols)
{
}


GetNetworkProtocolsResponse::~GetNetworkProtocolsResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetPasswordComplexityConfiguration.h"


namespace ONVIF {
namespace Device {


GetPasswordComplexityConfiguration::GetPasswordComplexityConfiguration()
{
}


GetPasswordComplexityConfiguration::~GetPasswordComplexityConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetPasswordComplexityConfigurationResponse.h"


namespace ONVIF {
namespace Device {


GetPasswordComplexityConfigurationResponse::GetPasswordComplexityConfigurationResponse():
	_minLen(),
	_uppercase(),
	_number(),
	_specialChars(),
	_blockUsernameOccurrence(),
	_policyConfigurationLocked()
{
}


GetPasswordComplexityConfigurationResponse::GetPasswordComplexityConfigurationResponse(Poco::Optional<int> minLen, Poco::Optional<int> uppercase, Poco::Optional<int> number, Poco::Optional<int> specialChars, Poco::Optional<bool> blockUsernameOccurrence, Poco::Optional<bool> policyConfigurationLocked):
	_minLen(minLen),
	_uppercase(uppercase),
	_number(number),
	_specialChars(specialChars),
	_blockUsernameOccurrence(blockUsernameOccurrence),
	_policyConfigurationLocked(policyConfigurationLocked)
{
}


GetPasswordComplexityConfigurationResponse::~GetPasswordComplexityConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetPasswordComplexityOptions.h"


namespace ONVIF {
namespace Device {


GetPasswordComplexityOptions::GetPasswordComplexityOptions()
{
}


GetPasswordComplexityOptions::~GetPasswordComplexityOptions()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetPasswordComplexityOptionsResponse.h"


namespace ONVIF {
namespace Device {


GetPasswordComplexityOptionsResponse::GetPasswordComplexityOptionsResponse():
	_minLenRange(),
	_uppercaseRange(),
	_numberRange(),
	_specialCharsRange(),
	_blockUsernameOccurrenceSupported(),
	_policyConfigurationLockSupported()
{
}


GetPasswordComplexityOptionsResponse::GetPasswordComplexityOptionsResponse(const Poco::SharedPtr<ONVIF::IntRange>& minLenRange, const Poco::SharedPtr<ONVIF::IntRange>& uppercaseRange, const Poco::SharedPtr<ONVIF::IntRange>& numberRange, const Poco::SharedPtr<ONVIF::IntRange>& specialCharsRange, Poco::Optional<bool> blockUsernameOccurrenceSupported, Poco::Optional<bool> policyConfigurationLockSupported):
	_minLenRange(minLenRange),
	_uppercaseRange(uppercaseRange),
	_numberRange(numberRange),
	_specialCharsRange(specialCharsRange),
	_blockUsernameOccurrenceSupported(blockUsernameOccurrenceSupported),
	_policyConfigurationLockSupported(policyConfigurationLockSupported)
{
}


GetPasswordComplexityOptionsResponse::~GetPasswordComplexityOptionsResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetPasswordHistoryConfiguration.h"


namespace ONVIF {
namespace Device {


GetPasswordHistoryConfiguration::GetPasswordHistoryConfiguration()
{
}


GetPasswordHistoryConfiguration::~GetPasswordHistoryConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetPasswordHistoryConfigurationResponse.h"


namespace ONVIF {
namespace Device {


GetPasswordHistoryConfigurationResponse::GetPasswordHistoryConfigurationResponse():
	_enabled(),
	_length()
{
}


GetPasswordHistoryConfigurationResponse::GetPasswordHistoryConfigurationResponse(bool enabled, int length):
	_enabled(enabled),
	_length(length)
{
}


GetPasswordHistoryConfigurationResponse::~GetPasswordHistoryConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetPkcs10Request.h"


namespace ONVIF {
namespace Device {


GetPkcs10Request::GetPkcs10Request():
	_certificateID(),
	_subject(),
	_attributes()
{
}


GetPkcs10Request::GetPkcs10Request(const std::string& certificateID, const Poco::Optional<std::string>& subject, const Poco::SharedPtr<ONVIF::BinaryData>& attributes):
	_certificateID(certificateID),
	_subject(subject),
	_attributes(attributes)
{
}


GetPkcs10Request::~GetPkcs10Request()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetPkcs10RequestResponse.h"


namespace ONVIF {
namespace Device {


GetPkcs10RequestResponse::GetPkcs10RequestResponse():
	_pkcs10Request()
{
}


GetPkcs10RequestResponse::GetPkcs10RequestResponse(const ONVIF::BinaryData& pkcs10Request):
	_pkcs10Request(pkcs10Request)
{
}


GetPkcs10RequestResponse::~GetPkcs10RequestResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetRelayOutputs.h"


namespace ONVIF {
namespace Device {


GetRelayOutputs::GetRelayOutputs()
{
}


GetRelayOutputs::~GetRelayOutputs()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetRelayOutputsResponse.h"


namespace ONVIF {
namespace Device {


GetRelayOutputsResponse::GetRelayOutputsResponse():
	_relayOutputs()
{
}


GetRelayOutputsResponse::GetRelayOutputsResponse(const std::vector<ONVIF::RelayOutput>& relayOutputs):
	_relayOutputs(relayOutputs)
{
}


GetRelayOutputsResponse::~GetRelayOutputsResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetRemoteDiscoveryMode.h"


namespace ONVIF {
namespace Device {


GetRemoteDiscoveryMode::GetRemoteDiscoveryMode()
{
}


GetRemoteDiscoveryMode::~GetRemoteDiscoveryMode()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetRemoteDiscoveryModeResponse.h"


namespace ONVIF {
namespace Device {


GetRemoteDiscoveryModeResponse::GetRemoteDiscoveryModeResponse():
	_remoteDiscoveryMode()
{
}


GetRemoteDiscoveryModeResponse::GetRemoteDiscoveryModeResponse(const std::string& remoteDiscoveryMode):
	_remoteDiscoveryMode(remoteDiscoveryMode)
{
}


GetRemoteDiscoveryModeResponse::~GetRemoteDiscoveryModeResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetRemoteUser.h"


namespace ONVIF {
namespace Device {


GetRemoteUser::GetRemoteUser()
{
}


GetRemoteUser::~GetRemoteUser()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetRemoteUserResponse.h"


namespace ONVIF {
namespace Device {


GetRemoteUserResponse::GetRemoteUserResponse():
	_remoteUser()
{
}


GetRemoteUserResponse::GetRemoteUserResponse(const Poco::SharedPtr<ONVIF::RemoteUser>& remoteUser):
	_remoteUser(remoteUser)
{
}


GetRemoteUserResponse::~GetRemoteUserResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetScopes.h"


namespace ONVIF {
namespace Device {


GetScopes::GetScopes()
{
}


GetScopes::~GetScopes()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetScopesResponse.h"


namespace ONVIF {
namespace Device {


GetScopesResponse::GetScopesResponse():
	_scopes()
{
}


GetScopesResponse::GetScopesResponse(const std::vector<ONVIF::Scope>& scopes):
	_scopes(scopes)
{
}


GetScopesResponse::~GetScopesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetServiceCapabilities.h"


namespace ONVIF {
namespace Device {


GetServiceCapabilities::GetServiceCapabilities()
{
}


GetServiceCapabilities::~GetServiceCapabilities()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetServiceCapabilitiesResponse.h"


namespace ONVIF {
namespace Device {


GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse():
	_capabilities()
{
}


GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse(const DeviceServiceCapabilities& capabilities):
	_capabilities(capabilities)
{
}


GetServiceCapabilitiesResponse::~GetServiceCapabilitiesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetServices.h"


namespace ONVIF {
namespace Device {


GetServices::GetServices():
	_includeCapability()
{
}


GetServices::GetServices(bool includeCapability):
	_includeCapability(includeCapability)
{
}


GetServices::~GetServices()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetServicesResponse.h"


namespace ONVIF {
namespace Device {


GetServicesResponse::GetServicesResponse():
	_service()
{
}


GetServicesResponse::GetServicesResponse(const std::vector<Service>& service):
	_service(service)
{
}


GetServicesResponse::~GetServicesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetStorageConfiguration.h"


namespace ONVIF {
namespace Device {


GetStorageConfiguration::GetStorageConfiguration():
	_token()
{
}


GetStorageConfiguration::GetStorageConfiguration(const std::string& token):
	_token(token)
{
}


GetStorageConfiguration::~GetStorageConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetStorageConfigurationResponse.h"


namespace ONVIF {
namespace Device {


GetStorageConfigurationResponse::GetStorageConfigurationResponse():
	_storageConfiguration()
{
}


GetStorageConfigurationResponse::GetStorageConfigurationResponse(const StorageConfiguration& storageConfiguration):
	_storageConfiguration(storageConfiguration)
{
}


GetStorageConfigurationResponse::~GetStorageConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetStorageConfigurations.h"


namespace ONVIF {
namespace Device {


GetStorageConfigurations::GetStorageConfigurations()
{
}


GetStorageConfigurations::~GetStorageConfigurations()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetStorageConfigurationsResponse.h"


namespace ONVIF {
namespace Device {


GetStorageConfigurationsResponse::GetStorageConfigurationsResponse():
	_storageConfigurations()
{
}


GetStorageConfigurationsResponse::GetStorageConfigurationsResponse(const std::vector<StorageConfiguration>& storageConfigurations):
	_storageConfigurations(storageConfigurations)
{
}


GetStorageConfigurationsResponse::~GetStorageConfigurationsResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetSystemBackup.h"


namespace ONVIF {
namespace Device {


GetSystemBackup::GetSystemBackup()
{
}


GetSystemBackup::~GetSystemBackup()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetSystemBackupResponse.h"


namespace ONVIF {
namespace Device {


GetSystemBackupResponse::GetSystemBackupResponse():
	_backupFiles()
{
}


GetSystemBackupResponse::GetSystemBackupResponse(const std::vector<ONVIF::BackupFile>& backupFiles):
	_backupFiles(backupFiles)
{
}


GetSystemBackupResponse::~GetSystemBackupResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetSystemDateAndTime.h"


namespace ONVIF {
namespace Device {


GetSystemDateAndTime::GetSystemDateAndTime()
{
}


GetSystemDateAndTime::~GetSystemDateAndTime()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetSystemDateAndTimeResponse.h"


namespace ONVIF {
namespace Device {


GetSystemDateAndTimeResponse::GetSystemDateAndTimeResponse():
	_systemDateAndTime()
{
}


GetSystemDateAndTimeResponse::GetSystemDateAndTimeResponse(const ONVIF::SystemDateTime& systemDateAndTime):
	_systemDateAndTime(systemDateAndTime)
{
}


GetSystemDateAndTimeResponse::~GetSystemDateAndTimeResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetSystemLog.h"


namespace ONVIF {
namespace Device {


GetSystemLog::GetSystemLog():
	_logType()
{
}


GetSystemLog::GetSystemLog(const ONVIF::SystemLog& logType):
	_logType(logType)
{
}


GetSystemLog::~GetSystemLog()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetSystemLogResponse.h"


namespace ONVIF {
namespace Device {


GetSystemLogResponse::GetSystemLogResponse():
	_systemLog()
{
}


GetSystemLogResponse::GetSystemLogResponse(const ONVIF::SystemLog& systemLog):
	_systemLog(systemLog)
{
}


GetSystemLogResponse::~GetSystemLogResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetSystemSupportInformation.h"


namespace ONVIF {
namespace Device {


GetSystemSupportInformation::GetSystemSupportInformation()
{
}


GetSystemSupportInformation::~GetSystemSupportInformation()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetSystemSupportInformationResponse.h"


namespace ONVIF {
namespace Device {


GetSystemSupportInformationResponse::GetSystemSupportInformationResponse():
	_supportInformation()
{
}


GetSystemSupportInformationResponse::GetSystemSupportInformationResponse(const ONVIF::SupportInformation& supportInformation):
	_supportInformation(supportInformation)
{
}


GetSystemSupportInformationResponse::~GetSystemSupportInformationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetSystemUris.h"


namespace ONVIF {
namespace Device {


GetSystemUris::GetSystemUris()
{
}


GetSystemUris::~GetSystemUris()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetSystemUrisResponse.h"


namespace ONVIF {
namespace Device {


GetSystemUrisResponse::GetSystemUrisResponse():
	_systemLogUris(),
	_supportInfoUri(),
	_systemBackupUri(),
	_extension()
{
}


GetSystemUrisResponse::GetSystemUrisResponse(const Poco::SharedPtr<ONVIF::SystemLogUriList>& systemLogUris, const Poco::Optional<Poco::URI>& supportInfoUri, const Poco::Optional<Poco::URI>& systemBackupUri, const Poco::SharedPtr<Extension>& extension):
	_systemLogUris(systemLogUris),
	_supportInfoUri(supportInfoUri),
	_systemBackupUri(systemBackupUri),
	_extension(extension)
{
}


GetSystemUrisResponse::~GetSystemUrisResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetUsers.h"


namespace ONVIF {
namespace Device {


GetUsers::GetUsers()
{
}


GetUsers::~GetUsers()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetUsersResponse.h"


namespace ONVIF {
namespace Device {


GetUsersResponse::GetUsersResponse():
	_user()
{
}


GetUsersResponse::GetUsersResponse(const std::vector<ONVIF::User>& user):
	_user(user)
{
}


GetUsersResponse::~GetUsersResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetWsdlUrl.h"


namespace ONVIF {
namespace Device {


GetWsdlUrl::GetWsdlUrl()
{
}


GetWsdlUrl::~GetWsdlUrl()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetWsdlUrlResponse.h"


namespace ONVIF {
namespace Device {


GetWsdlUrlResponse::GetWsdlUrlResponse():
	_wsdlUrl()
{
}


GetWsdlUrlResponse::GetWsdlUrlResponse(const Poco::URI& wsdlUrl):
	_wsdlUrl(wsdlUrl)
{
}


GetWsdlUrlResponse::~GetWsdlUrlResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetZeroConfiguration.h"


namespace ONVIF {
namespace Device {


GetZeroConfiguration::GetZeroConfiguration()
{
}


GetZeroConfiguration::~GetZeroConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/GetZeroConfigurationResponse.h"


namespace ONVIF {
namespace Device {


GetZeroConfigurationResponse::GetZeroConfigurationResponse():
	_zeroConfiguration()
{
}


GetZeroConfigurationResponse::GetZeroConfigurationResponse(const ONVIF::NetworkZeroConfiguration& zeroConfiguration):
	_zeroConfiguration(zeroConfiguration)
{
}


GetZeroConfigurationResponse::~GetZeroConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/LoadCACertificates.h"


namespace ONVIF {
namespace Device {


LoadCACertificates::LoadCACertificates():
	_cACertificate()
{
}


LoadCACertificates::LoadCACertificates(const std::vector<ONVIF::Certificate>& cACertificate):
	_cACertificate(cACertificate)
{
}


LoadCACertificates::~LoadCACertificates()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/LoadCACertificatesResponse.h"


namespace ONVIF {
namespace Device {


LoadCACertificatesResponse::LoadCACertificatesResponse()
{
}


LoadCACertificatesResponse::~LoadCACertificatesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/LoadCertificateWithPrivateKey.h"


namespace ONVIF {
namespace Device {


LoadCertificateWithPrivateKey::LoadCertificateWithPrivateKey():
	_certificateWithPrivateKey()
{
}


LoadCertificateWithPrivateKey::LoadCertificateWithPrivateKey(const std::vector<ONVIF::CertificateWithPrivateKey>& certificateWithPrivateKey):
	_certificateWithPrivateKey(certificateWithPrivateKey)
{
}


LoadCertificateWithPrivateKey::~LoadCertificateWithPrivateKey()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/LoadCertificateWithPrivateKeyResponse.h"


namespace ONVIF {
namespace Device {


LoadCertificateWithPrivateKeyResponse::LoadCertificateWithPrivateKeyResponse()
{
}


LoadCertificateWithPrivateKeyResponse::~LoadCertificateWithPrivateKeyResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/LoadCertificates.h"


namespace ONVIF {
namespace Device {


LoadCertificates::LoadCertificates():
	_nVTCertificate()
{
}


LoadCertificates::LoadCertificates(const std::vector<ONVIF::Certificate>& nVTCertificate):
	_nVTCertificate(nVTCertificate)
{
}


LoadCertificates::~LoadCertificates()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/LoadCertificatesResponse.h"


namespace ONVIF {
namespace Device {


LoadCertificatesResponse::LoadCertificatesResponse()
{
}


LoadCertificatesResponse::~LoadCertificatesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/MiscCapabilities.h"


namespace ONVIF {
namespace Device {


MiscCapabilities::MiscCapabilities():
	_auxiliaryCommands()
{
}


MiscCapabilities::MiscCapabilities(const std::vector<std::vector<std::string>>& auxiliaryCommands):
	_auxiliaryCommands(auxiliaryCommands)
{
}


MiscCapabilities::~MiscCapabilities()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/NetworkCapabilities.h"


namespace ONVIF {
namespace Device {


NetworkCapabilities::NetworkCapabilities():
	_iPFilter(),
	_zeroConfiguration(),
	_iPVersion6(),
	_dynDNS(),
	_dot11Configuration(),
	_dot1XConfigurations(),
	_hostnameFromDHCP(),
	_nTP(),
	_dHCPv6()
{
}


NetworkCapabilities::NetworkCapabilities(Poco::Optional<bool> iPFilter, Poco::Optional<bool> zeroConfiguration, Poco::Optional<bool> iPVersion6, Poco::Optional<bool> dynDNS, Poco::Optional<bool> dot11Configuration, Poco::Optional<int> dot1XConfigurations, Poco::Optional<bool> hostnameFromDHCP, Poco::Optional<int> nTP, Poco::Optional<bool> dHCPv6):
	_iPFilter(iPFilter),
	_zeroConfiguration(zeroConfiguration),
	_iPVersion6(iPVersion6),
	_dynDNS(dynDNS),
	_dot11Configuration(dot11Configuration),
	_dot1XConfigurations(dot1XConfigurations),
	_hostnameFromDHCP(hostnameFromDHCP),
	_nTP(nTP),
	_dHCPv6(dHCPv6)
{
}


NetworkCapabilities::~NetworkCapabilities()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/RemoveIPAddressFilter.h"


namespace ONVIF {
namespace Device {


RemoveIPAddressFilter::RemoveIPAddressFilter():
	_iPAddressFilter()
{
}


RemoveIPAddressFilter::RemoveIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter):
	_iPAddressFilter(iPAddressFilter)
{
}


RemoveIPAddressFilter::~RemoveIPAddressFilter()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/RemoveIPAddressFilterResponse.h"


namespace ONVIF {
namespace Device {


RemoveIPAddressFilterResponse::RemoveIPAddressFilterResponse()
{
}


RemoveIPAddressFilterResponse::~RemoveIPAddressFilterResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/RemoveScopes.h"


namespace ONVIF {
namespace Device {


RemoveScopes::RemoveScopes():
	_scopeItem()
{
}


RemoveScopes::RemoveScopes(const std::vector<Poco::URI>& scopeItem):
	_scopeItem(scopeItem)
{
}


RemoveScopes::~RemoveScopes()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/RemoveScopesResponse.h"


namespace ONVIF {
namespace Device {


RemoveScopesResponse::RemoveScopesResponse():
	_scopeItem()
{
}


RemoveScopesResponse::RemoveScopesResponse(const std::vector<Poco::URI>& scopeItem):
	_scopeItem(scopeItem)
{
}


RemoveScopesResponse::~RemoveScopesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/RestoreSystem.h"


namespace ONVIF {
namespace Device {


RestoreSystem::RestoreSystem():
	_backupFiles()
{
}


RestoreSystem::RestoreSystem(const std::vector<ONVIF::BackupFile>& backupFiles):
	_backupFiles(backupFiles)
{
}


RestoreSystem::~RestoreSystem()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/RestoreSystemResponse.h"


namespace ONVIF {
namespace Device {


RestoreSystemResponse::RestoreSystemResponse()
{
}


RestoreSystemResponse::~RestoreSystemResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/ScanAvailableDot11Networks.h"


namespace ONVIF {
namespace Device {


ScanAvailableDot11Networks::ScanAvailableDot11Networks():
	_interfaceToken()
{
}


ScanAvailableDot11Networks::ScanAvailableDot11Networks(const std::string& interfaceToken):
	_interfaceToken(interfaceToken)
{
}


ScanAvailableDot11Networks::~ScanAvailableDot11Networks()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/ScanAvailableDot11NetworksResponse.h"


namespace ONVIF {
namespace Device {


ScanAvailableDot11NetworksResponse::ScanAvailableDot11NetworksResponse():
	_networks()
{
}


ScanAvailableDot11NetworksResponse::ScanAvailableDot11NetworksResponse(const std::vector<ONVIF::Dot11AvailableNetworks>& networks):
	_networks(networks)
{
}


ScanAvailableDot11NetworksResponse::~ScanAvailableDot11NetworksResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SecurityCapabilities.h"


namespace ONVIF {
namespace Device {


SecurityCapabilities::SecurityCapabilities():
	_tLS1_0(),
	_tLS1_1(),
	_tLS1_2(),
	_onboardKeyGeneration(),
	_accessPolicyConfig(),
	_defaultAccessPolicy(),
	_dot1X(),
	_remoteUserHandling(),
	_x_509Token(),
	_sAMLToken(),
	_kerberosToken(),
	_usernameToken(),
	_httpDigest(),
	_rELToken(),
	_supportedEAPMethods(),
	_maxUsers(),
	_maxUserNameLength(),
	_maxPasswordLength(),
	_securityPolicies(),
	_maxPasswordHistory(),
	_hashingAlgorithms()
{
}


SecurityCapabilities::SecurityCapabilities(Poco::Optional<bool> tLS1_0, Poco::Optional<bool> tLS1_1, Poco::Optional<bool> tLS1_2, Poco::Optional<bool> onboardKeyGeneration, Poco::Optional<bool> accessPolicyConfig, Poco::Optional<bool> defaultAccessPolicy, Poco::Optional<bool> dot1X, Poco::Optional<bool> remoteUserHandling, Poco::Optional<bool> x_509Token, Poco::Optional<bool> sAMLToken, Poco::Optional<bool> kerberosToken, Poco::Optional<bool> usernameToken, Poco::Optional<bool> httpDigest, Poco::Optional<bool> rELToken, const std::vector<std::vector<int>>& supportedEAPMethods, Poco::Optional<int> maxUsers, Poco::Optional<int> maxUserNameLength, Poco::Optional<int> maxPasswordLength, const std::vector<std::vector<std::string>>& securityPolicies, Poco::Optional<int> maxPasswordHistory, const std::vector<std::vector<std::string>>& hashingAlgorithms):
	_tLS1_0(tLS1_0),
	_tLS1_1(tLS1_1),
	_tLS1_2(tLS1_2),
	_onboardKeyGeneration(onboardKeyGeneration),
	_accessPolicyConfig(accessPolicyConfig),
	_defaultAccessPolicy(defaultAccessPolicy),
	_dot1X(dot1X),
	_remoteUserHandling(remoteUserHandling),
	_x_509Token(x_509Token),
	_sAMLToken(sAMLToken),
	_kerberosToken(kerberosToken),
	_usernameToken(usernameToken),
	_httpDigest(httpDigest),
	_rELToken(rELToken),
	_supportedEAPMethods(supportedEAPMethods),
	_maxUsers(maxUsers),
	_maxUserNameLength(maxUserNameLength),
	_maxPasswordLength(maxPasswordLength),
	_securityPolicies(securityPolicies),
	_maxPasswordHistory(maxPasswordHistory),
	_hashingAlgorithms(hashingAlgorithms)
{
}


SecurityCapabilities::~SecurityCapabilities()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SendAuxiliaryCommand.h"


namespace ONVIF {
namespace Device {


SendAuxiliaryCommand::SendAuxiliaryCommand():
	_auxiliaryCommand()
{
}


SendAuxiliaryCommand::SendAuxiliaryCommand(const std::string& auxiliaryCommand):
	_auxiliaryCommand(auxiliaryCommand)
{
}


SendAuxiliaryCommand::~SendAuxiliaryCommand()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SendAuxiliaryCommandResponse.h"


namespace ONVIF {
namespace Device {


SendAuxiliaryCommandResponse::SendAuxiliaryCommandResponse():
	_auxiliaryCommandResponse()
{
}


SendAuxiliaryCommandResponse::SendAuxiliaryCommandResponse(const Poco::Optional<std::string>& auxiliaryCommandResponse):
	_auxiliaryCommandResponse(auxiliaryCommandResponse)
{
}


SendAuxiliaryCommandResponse::~SendAuxiliaryCommandResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/Service.h"


namespace ONVIF {
namespace Device {


Service::Service():
	_namespace(),
	_xAddr(),
	_capabilities(),
	_version()
{
}


Service::Service(const Poco::URI& namespace_, const Poco::URI& xAddr, const Poco::SharedPtr<Capabilities>& capabilities, const ONVIF::OnvifVersion& version):
	_namespace(namespace_),
	_xAddr(xAddr),
	_capabilities(capabilities),
	_version(version)
{
}


Service::~Service()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetAccessPolicy.h"


namespace ONVIF {
namespace Device {


SetAccessPolicy::SetAccessPolicy():
	_policyFile()
{
}


SetAccessPolicy::SetAccessPolicy(const ONVIF::BinaryData& policyFile):
	_policyFile(policyFile)
{
}


SetAccessPolicy::~SetAccessPolicy()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetAccessPolicyResponse.h"


namespace ONVIF {
namespace Device {


SetAccessPolicyResponse::SetAccessPolicyResponse()
{
}


SetAccessPolicyResponse::~SetAccessPolicyResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetAuthFailureWarningConfiguration.h"


namespace ONVIF {
namespace Device {


SetAuthFailureWarningConfiguration::SetAuthFailureWarningConfiguration():
	_enabled(),
	_monitorPeriod(),
	_maxAuthFailures()
{
}


SetAuthFailureWarningConfiguration::SetAuthFailureWarningConfiguration(bool enabled, int monitorPeriod, int maxAuthFailures):
	_enabled(enabled),
	_monitorPeriod(monitorPeriod),
	_maxAuthFailures(maxAuthFailures)
{
}


SetAuthFailureWarningConfiguration::~SetAuthFailureWarningConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetAuthFailureWarningConfigurationResponse.h"


namespace ONVIF {
namespace Device {


SetAuthFailureWarningConfigurationResponse::SetAuthFailureWarningConfigurationResponse()
{
}


SetAuthFailureWarningConfigurationResponse::~SetAuthFailureWarningConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetCertificatesStatus.h"


namespace ONVIF {
namespace Device {


SetCertificatesStatus::SetCertificatesStatus():
	_certificateStatus()
{
}


SetCertificatesStatus::SetCertificatesStatus(const std::vector<ONVIF::CertificateStatus>& certificateStatus):
	_certificateStatus(certificateStatus)
{
}


SetCertificatesStatus::~SetCertificatesStatus()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetCertificatesStatusResponse.h"


namespace ONVIF {
namespace Device {


SetCertificatesStatusResponse::SetCertificatesStatusResponse()
{
}


SetCertificatesStatusResponse::~SetCertificatesStatusResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetClientCertificateMode.h"


namespace ONVIF {
namespace Device {


SetClientCertificateMode::SetClientCertificateMode():
	_enabled()
{
}


SetClientCertificateMode::SetClientCertificateMode(bool enabled):
	_enabled(enabled)
{
}


SetClientCertificateMode::~SetClientCertificateMode()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetClientCertificateModeResponse.h"


namespace ONVIF {
namespace Device {


SetClientCertificateModeResponse::SetClientCertificateModeResponse()
{
}


SetClientCertificateModeResponse::~SetClientCertificateModeResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetDNS.h"


namespace ONVIF {
namespace Device {


SetDNS::SetDNS():
	_fromDHCP(),
	_searchDomain(),
	_dNSManual()
{
}


SetDNS::SetDNS(bool fromDHCP, const std::vector<std::string>& searchDomain, const std::vector<ONVIF::IPAddress>& dNSManual):
	_fromDHCP(fromDHCP),
	_searchDomain(searchDomain),
	_dNSManual(dNSManual)
{
}


SetDNS::~SetDNS()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetDNSResponse.h"


namespace ONVIF {
namespace Device {


SetDNSResponse::SetDNSResponse()
{
}


SetDNSResponse::~SetDNSResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetDPAddresses.h"


namespace ONVIF {
namespace Device {


SetDPAddresses::SetDPAddresses():
	_dPAddress()
{
}


SetDPAddresses::SetDPAddresses(const std::vector<ONVIF::NetworkHost>& dPAddress):
	_dPAddress(dPAddress)
{
}


SetDPAddresses::~SetDPAddresses()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetDPAddressesResponse.h"


namespace ONVIF {
namespace Device {


SetDPAddressesResponse::SetDPAddressesResponse()
{
}


SetDPAddressesResponse::~SetDPAddressesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetDiscoveryMode.h"


namespace ONVIF {
namespace Device {


SetDiscoveryMode::SetDiscoveryMode():
	_discoveryMode()
{
}


SetDiscoveryMode::SetDiscoveryMode(const std::string& discoveryMode):
	_discoveryMode(discoveryMode)
{
}


SetDiscoveryMode::~SetDiscoveryMode()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetDiscoveryModeResponse.h"


namespace ONVIF {
namespace Device {


SetDiscoveryModeResponse::SetDiscoveryModeResponse()
{
}


SetDiscoveryModeResponse::~SetDiscoveryModeResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetDot1XConfiguration.h"


namespace ONVIF {
namespace Device {


SetDot1XConfiguration::SetDot1XConfiguration():
	_dot1XConfiguration()
{
}


SetDot1XConfiguration::SetDot1XConfiguration(const ONVIF::Dot1XConfiguration& dot1XConfiguration):
	_dot1XConfiguration(dot1XConfiguration)
{
}


SetDot1XConfiguration::~SetDot1XConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetDot1XConfigurationResponse.h"


namespace ONVIF {
namespace Device {


SetDot1XConfigurationResponse::SetDot1XConfigurationResponse()
{
}


SetDot1XConfigurationResponse::~SetDot1XConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetDynamicDNS.h"


namespace ONVIF {
namespace Device {


SetDynamicDNS::SetDynamicDNS():
	_type(),
	_name(),
	_tTL()
{
}


SetDynamicDNS::SetDynamicDNS(const std::string& type, const Poco::Optional<std::string>& name, const Poco::Optional<std::string>& tTL):
	_type(type),
	_name(name),
	_tTL(tTL)
{
}


SetDynamicDNS::~SetDynamicDNS()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetDynamicDNSResponse.h"


namespace ONVIF {
namespace Device {


SetDynamicDNSResponse::SetDynamicDNSResponse()
{
}


SetDynamicDNSResponse::~SetDynamicDNSResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetGeoLocation.h"


namespace ONVIF {
namespace Device {


SetGeoLocation::SetGeoLocation():
	_location()
{
}


SetGeoLocation::SetGeoLocation(const std::vector<ONVIF::LocationEntity>& location):
	_location(location)
{
}


SetGeoLocation::~SetGeoLocation()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetGeoLocationResponse.h"


namespace ONVIF {
namespace Device {


SetGeoLocationResponse::SetGeoLocationResponse()
{
}


SetGeoLocationResponse::~SetGeoLocationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetHashingAlgorithm.h"


namespace ONVIF {
namespace Device {


SetHashingAlgorithm::SetHashingAlgorithm():
	_algorithm()
{
}


SetHashingAlgorithm::SetHashingAlgorithm(const std::vector<std::string>& algorithm):
	_algorithm(algorithm)
{
}


SetHashingAlgorithm::~SetHashingAlgorithm()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetHashingAlgorithmResponse.h"


namespace ONVIF {
namespace Device {


SetHashingAlgorithmResponse::SetHashingAlgorithmResponse()
{
}


SetHashingAlgorithmResponse::~SetHashingAlgorithmResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetHostname.h"


namespace ONVIF {
namespace Device {


SetHostname::SetHostname():
	_name()
{
}


SetHostname::SetHostname(const std::string& name):
	_name(name)
{
}


SetHostname::~SetHostname()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetHostnameFromDHCP.h"


namespace ONVIF {
namespace Device {


SetHostnameFromDHCP::SetHostnameFromDHCP():
	_fromDHCP()
{
}


SetHostnameFromDHCP::SetHostnameFromDHCP(bool fromDHCP):
	_fromDHCP(fromDHCP)
{
}


SetHostnameFromDHCP::~SetHostnameFromDHCP()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetHostnameFromDHCPResponse.h"


namespace ONVIF {
namespace Device {


SetHostnameFromDHCPResponse::SetHostnameFromDHCPResponse():
	_rebootNeeded()
{
}


SetHostnameFromDHCPResponse::SetHostnameFromDHCPResponse(bool rebootNeeded):
	_rebootNeeded(rebootNeeded)
{
}


SetHostnameFromDHCPResponse::~SetHostnameFromDHCPResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetHostnameResponse.h"


namespace ONVIF {
namespace Device {


SetHostnameResponse::SetHostnameResponse()
{
}


SetHostnameResponse::~SetHostnameResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetIPAddressFilter.h"


namespace ONVIF {
namespace Device {


SetIPAddressFilter::SetIPAddressFilter():
	_iPAddressFilter()
{
}


SetIPAddressFilter::SetIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter):
	_iPAddressFilter(iPAddressFilter)
{
}


SetIPAddressFilter::~SetIPAddressFilter()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetIPAddressFilterResponse.h"


namespace ONVIF {
namespace Device {


SetIPAddressFilterResponse::SetIPAddressFilterResponse()
{
}


SetIPAddressFilterResponse::~SetIPAddressFilterResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetNTP.h"


namespace ONVIF {
namespace Device {


SetNTP::SetNTP():
	_fromDHCP(),
	_nTPManual()
{
}


SetNTP::SetNTP(bool fromDHCP, const std::vector<ONVIF::NetworkHost>& nTPManual):
	_fromDHCP(fromDHCP),
	_nTPManual(nTPManual)
{
}


SetNTP::~SetNTP()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetNTPResponse.h"


namespace ONVIF {
namespace Device {


SetNTPResponse::SetNTPResponse()
{
}


SetNTPResponse::~SetNTPResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetNetworkDefaultGateway.h"


namespace ONVIF {
namespace Device {


SetNetworkDefaultGateway::SetNetworkDefaultGateway():
	_iPv4Address(),
	_iPv6Address()
{
}


SetNetworkDefaultGateway::SetNetworkDefaultGateway(const std::vector<std::string>& iPv4Address, const std::vector<std::string>& iPv6Address):
	_iPv4Address(iPv4Address),
	_iPv6Address(iPv6Address)
{
}


SetNetworkDefaultGateway::~SetNetworkDefaultGateway()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetNetworkDefaultGatewayResponse.h"


namespace ONVIF {
namespace Device {


SetNetworkDefaultGatewayResponse::SetNetworkDefaultGatewayResponse()
{
}


SetNetworkDefaultGatewayResponse::~SetNetworkDefaultGatewayResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetNetworkInterfaces.h"


namespace ONVIF {
namespace Device {


SetNetworkInterfaces::SetNetworkInterfaces():
	_interfaceToken(),
	_networkInterface()
{
}


SetNetworkInterfaces::SetNetworkInterfaces(const std::string& interfaceToken, const ONVIF::NetworkInterfaceSetConfiguration& networkInterface):
	_interfaceToken(interfaceToken),
	_networkInterface(networkInterface)
{
}


SetNetworkInterfaces::~SetNetworkInterfaces()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetNetworkInterfacesResponse.h"


namespace ONVIF {
namespace Device {


SetNetworkInterfacesResponse::SetNetworkInterfacesResponse():
	_rebootNeeded()
{
}


SetNetworkInterfacesResponse::SetNetworkInterfacesResponse(bool rebootNeeded):
	_rebootNeeded(rebootNeeded)
{
}


SetNetworkInterfacesResponse::~SetNetworkInterfacesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetNetworkProtocols.h"


namespace ONVIF {
namespace Device {


SetNetworkProtocols::SetNetworkProtocols():
	_networkProtocols()
{
}


SetNetworkProtocols::SetNetworkProtocols(const std::vector<ONVIF::NetworkProtocol>& networkProtocols):
	_networkProtocols(networkProtocols)
{
}


SetNetworkProtocols::~SetNetworkProtocols()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetNetworkProtocolsResponse.h"


namespace ONVIF {
namespace Device {


SetNetworkProtocolsResponse::SetNetworkProtocolsResponse()
{
}


SetNetworkProtocolsResponse::~SetNetworkProtocolsResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetPasswordComplexityConfiguration.h"


namespace ONVIF {
namespace Device {


SetPasswordComplexityConfiguration::SetPasswordComplexityConfiguration():
	_minLen(),
	_uppercase(),
	_number(),
	_specialChars(),
	_blockUsernameOccurrence(),
	_policyConfigurationLocked()
{
}


SetPasswordComplexityConfiguration::SetPasswordComplexityConfiguration(Poco::Optional<int> minLen, Poco::Optional<int> uppercase, Poco::Optional<int> number, Poco::Optional<int> specialChars, Poco::Optional<bool> blockUsernameOccurrence, Poco::Optional<bool> policyConfigurationLocked):
	_minLen(minLen),
	_uppercase(uppercase),
	_number(number),
	_specialChars(specialChars),
	_blockUsernameOccurrence(blockUsernameOccurrence),
	_policyConfigurationLocked(policyConfigurationLocked)
{
}


SetPasswordComplexityConfiguration::~SetPasswordComplexityConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetPasswordComplexityConfigurationResponse.h"


namespace ONVIF {
namespace Device {


SetPasswordComplexityConfigurationResponse::SetPasswordComplexityConfigurationResponse()
{
}


SetPasswordComplexityConfigurationResponse::~SetPasswordComplexityConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetPasswordHistoryConfiguration.h"


namespace ONVIF {
namespace Device {


SetPasswordHistoryConfiguration::SetPasswordHistoryConfiguration():
	_enabled(),
	_length()
{
}


SetPasswordHistoryConfiguration::SetPasswordHistoryConfiguration(bool enabled, int length):
	_enabled(enabled),
	_length(length)
{
}


SetPasswordHistoryConfiguration::~SetPasswordHistoryConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetPasswordHistoryConfigurationResponse.h"


namespace ONVIF {
namespace Device {


SetPasswordHistoryConfigurationResponse::SetPasswordHistoryConfigurationResponse()
{
}


SetPasswordHistoryConfigurationResponse::~SetPasswordHistoryConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetRelayOutputSettings.h"


namespace ONVIF {
namespace Device {


SetRelayOutputSettings::SetRelayOutputSettings():
	_relayOutputToken(),
	_properties()
{
}


SetRelayOutputSettings::SetRelayOutputSettings(const std::string& relayOutputToken, const ONVIF::RelayOutputSettings& properties):
	_relayOutputToken(relayOutputToken),
	_properties(properties)
{
}


SetRelayOutputSettings::~SetRelayOutputSettings()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetRelayOutputSettingsResponse.h"


namespace ONVIF {
namespace Device {


SetRelayOutputSettingsResponse::SetRelayOutputSettingsResponse()
{
}


SetRelayOutputSettingsResponse::~SetRelayOutputSettingsResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetRelayOutputState.h"


namespace ONVIF {
namespace Device {


SetRelayOutputState::SetRelayOutputState():
	_relayOutputToken(),
	_logicalState()
{
}


SetRelayOutputState::SetRelayOutputState(const std::string& relayOutputToken, const std::string& logicalState):
	_relayOutputToken(relayOutputToken),
	_logicalState(logicalState)
{
}


SetRelayOutputState::~SetRelayOutputState()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetRelayOutputStateResponse.h"


namespace ONVIF {
namespace Device {


SetRelayOutputStateResponse::SetRelayOutputStateResponse()
{
}


SetRelayOutputStateResponse::~SetRelayOutputStateResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetRemoteDiscoveryMode.h"


namespace ONVIF {
namespace Device {


SetRemoteDiscoveryMode::SetRemoteDiscoveryMode():
	_remoteDiscoveryMode()
{
}


SetRemoteDiscoveryMode::SetRemoteDiscoveryMode(const std::string& remoteDiscoveryMode):
	_remoteDiscoveryMode(remoteDiscoveryMode)
{
}


SetRemoteDiscoveryMode::~SetRemoteDiscoveryMode()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetRemoteDiscoveryModeResponse.h"


namespace ONVIF {
namespace Device {


SetRemoteDiscoveryModeResponse::SetRemoteDiscoveryModeResponse()
{
}


SetRemoteDiscoveryModeResponse::~SetRemoteDiscoveryModeResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetRemoteUser.h"


namespace ONVIF {
namespace Device {


SetRemoteUser::SetRemoteUser():
	_remoteUser()
{
}


SetRemoteUser::SetRemoteUser(const Poco::SharedPtr<ONVIF::RemoteUser>& remoteUser):
	_remoteUser(remoteUser)
{
}


SetRemoteUser::~SetRemoteUser()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetRemoteUserResponse.h"


namespace ONVIF {
namespace Device {


SetRemoteUserResponse::SetRemoteUserResponse()
{
}


SetRemoteUserResponse::~SetRemoteUserResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetScopes.h"


namespace ONVIF {
namespace Device {


SetScopes::SetScopes():
	_scopes()
{
}


SetScopes::SetScopes(const std::vector<Poco::URI>& scopes):
	_scopes(scopes)
{
}


SetScopes::~SetScopes()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetScopesResponse.h"


namespace ONVIF {
namespace Device {


SetScopesResponse::SetScopesResponse()
{
}


SetScopesResponse::~SetScopesResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetStorageConfiguration.h"


namespace ONVIF {
namespace Device {


SetStorageConfiguration::SetStorageConfiguration():
	_storageConfiguration()
{
}


SetStorageConfiguration::SetStorageConfiguration(const StorageConfiguration& storageConfiguration):
	_storageConfiguration(storageConfiguration)
{
}


SetStorageConfiguration::~SetStorageConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetStorageConfigurationResponse.h"


namespace ONVIF {
namespace Device {


SetStorageConfigurationResponse::SetStorageConfigurationResponse()
{
}


SetStorageConfigurationResponse::~SetStorageConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetSystemDateAndTime.h"


namespace ONVIF {
namespace Device {


SetSystemDateAndTime::SetSystemDateAndTime():
	_dateTimeType(),
	_daylightSavings(),
	_timeZone(),
	_uTCDateTime()
{
}


SetSystemDateAndTime::SetSystemDateAndTime(const std::string& dateTimeType, bool daylightSavings, const Poco::SharedPtr<ONVIF::TimeZone>& timeZone, const Poco::SharedPtr<ONVIF::DateTime>& uTCDateTime):
	_dateTimeType(dateTimeType),
	_daylightSavings(daylightSavings),
	_timeZone(timeZone),
	_uTCDateTime(uTCDateTime)
{
}


SetSystemDateAndTime::~SetSystemDateAndTime()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetSystemDateAndTimeResponse.h"


namespace ONVIF {
namespace Device {


SetSystemDateAndTimeResponse::SetSystemDateAndTimeResponse()
{
}


SetSystemDateAndTimeResponse::~SetSystemDateAndTimeResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetSystemFactoryDefault.h"


namespace ONVIF {
namespace Device {


SetSystemFactoryDefault::SetSystemFactoryDefault():
	_factoryDefault()
{
}


SetSystemFactoryDefault::SetSystemFactoryDefault(const std::string& factoryDefault):
	_factoryDefault(factoryDefault)
{
}


SetSystemFactoryDefault::~SetSystemFactoryDefault()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetSystemFactoryDefaultResponse.h"


namespace ONVIF {
namespace Device {


SetSystemFactoryDefaultResponse::SetSystemFactoryDefaultResponse()
{
}


SetSystemFactoryDefaultResponse::~SetSystemFactoryDefaultResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetUser.h"


namespace ONVIF {
namespace Device {


SetUser::SetUser():
	_user()
{
}


SetUser::SetUser(const std::vector<ONVIF::User>& user):
	_user(user)
{
}


SetUser::~SetUser()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetUserResponse.h"


namespace ONVIF {
namespace Device {


SetUserResponse::SetUserResponse()
{
}


SetUserResponse::~SetUserResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetZeroConfiguration.h"


namespace ONVIF {
namespace Device {


SetZeroConfiguration::SetZeroConfiguration():
	_interfaceToken(),
	_enabled()
{
}


SetZeroConfiguration::SetZeroConfiguration(const std::string& interfaceToken, bool enabled):
	_interfaceToken(interfaceToken),
	_enabled(enabled)
{
}


SetZeroConfiguration::~SetZeroConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SetZeroConfigurationResponse.h"


namespace ONVIF {
namespace Device {


SetZeroConfigurationResponse::SetZeroConfigurationResponse()
{
}


SetZeroConfigurationResponse::~SetZeroConfigurationResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/StartFirmwareUpgrade.h"


namespace ONVIF {
namespace Device {


StartFirmwareUpgrade::StartFirmwareUpgrade()
{
}


StartFirmwareUpgrade::~StartFirmwareUpgrade()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/StartFirmwareUpgradeResponse.h"


namespace ONVIF {
namespace Device {


StartFirmwareUpgradeResponse::StartFirmwareUpgradeResponse():
	_uploadUri(),
	_uploadDelay(),
	_expectedDownTime()
{
}


StartFirmwareUpgradeResponse::StartFirmwareUpgradeResponse(const Poco::URI& uploadUri, const std::string& uploadDelay, const std::string& expectedDownTime):
	_uploadUri(uploadUri),
	_uploadDelay(uploadDelay),
	_expectedDownTime(expectedDownTime)
{
}


StartFirmwareUpgradeResponse::~StartFirmwareUpgradeResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/StartSystemRestore.h"


namespace ONVIF {
namespace Device {


StartSystemRestore::StartSystemRestore()
{
}


StartSystemRestore::~StartSystemRestore()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/StartSystemRestoreResponse.h"


namespace ONVIF {
namespace Device {


StartSystemRestoreResponse::StartSystemRestoreResponse():
	_uploadUri(),
	_expectedDownTime()
{
}


StartSystemRestoreResponse::StartSystemRestoreResponse(const Poco::URI& uploadUri, const std::string& expectedDownTime):
	_uploadUri(uploadUri),
	_expectedDownTime(expectedDownTime)
{
}


StartSystemRestoreResponse::~StartSystemRestoreResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/StorageConfiguration.h"


namespace ONVIF {
namespace Device {


StorageConfiguration::StorageConfiguration():
	_data()
{
}


StorageConfiguration::StorageConfiguration(const std::string& token, const StorageConfigurationData& data):
	ONVIF::DeviceEntity(token),
	_data(data)
{
}


StorageConfiguration::~StorageConfiguration()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/StorageConfigurationData.h"


namespace ONVIF {
namespace Device {


StorageConfigurationData::StorageConfigurationData():
	_type(),
	_region(),
	_localPath(),
	_storageUri(),
	_user(),
	_extension()
{
}


StorageConfigurationData::StorageConfigurationData(const std::string& type, const Poco::Optional<std::string>& region, const Poco::Optional<Poco::URI>& localPath, const Poco::Optional<Poco::URI>& storageUri, const Poco::SharedPtr<UserCredential>& user, const Poco::SharedPtr<Extension_2>& extension):
	_type(type),
	_region(region),
	_localPath(localPath),
	_storageUri(storageUri),
	_user(user),
	_extension(extension)
{
}


StorageConfigurationData::~StorageConfigurationData()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SystemCapabilities.h"


namespace ONVIF {
namespace Device {


SystemCapabilities::SystemCapabilities():
	_discoveryResolve(),
	_discoveryBye(),
	_remoteDiscovery(),
	_systemBackup(),
	_systemLogging(),
	_firmwareUpgrade(),
	_httpFirmwareUpgrade(),
	_httpSystemBackup(),
	_httpSystemLogging(),
	_httpSupportInformation(),
	_storageConfiguration(),
	_maxStorageConfigurations(),
	_geoLocationEntries(),
	_autoGeo(),
	_storageTypesSupported(),
	_discoveryNotSupported(),
	_networkConfigNotSupported(),
	_userConfigNotSupported(),
	_addons()
{
}


SystemCapabilities::SystemCapabilities(Poco::Optional<bool> discoveryResolve, Poco::Optional<bool> discoveryBye, Poco::Optional<bool> remoteDiscovery, Poco::Optional<bool> systemBackup, Poco::Optional<bool> systemLogging, Poco::Optional<bool> firmwareUpgrade, Poco::Optional<bool> httpFirmwareUpgrade, Poco::Optional<bool> httpSystemBackup, Poco::Optional<bool> httpSystemLogging, Poco::Optional<bool> httpSupportInformation, Poco::Optional<bool> storageConfiguration, Poco::Optional<int> maxStorageConfigurations, Poco::Optional<int> geoLocationEntries, const std::vector<std::vector<std::string>>& autoGeo, const std::vector<std::vector<std::string>>& storageTypesSupported, Poco::Optional<bool> discoveryNotSupported, Poco::Optional<bool> networkConfigNotSupported, Poco::Optional<bool> userConfigNotSupported, const std::vector<std::vector<std::string>>& addons):
	_discoveryResolve(discoveryResolve),
	_discoveryBye(discoveryBye),
	_remoteDiscovery(remoteDiscovery),
	_systemBackup(systemBackup),
	_systemLogging(systemLogging),
	_firmwareUpgrade(firmwareUpgrade),
	_httpFirmwareUpgrade(httpFirmwareUpgrade),
	_httpSystemBackup(httpSystemBackup),
	_httpSystemLogging(httpSystemLogging),
	_httpSupportInformation(httpSupportInformation),
	_storageConfiguration(storageConfiguration),
	_maxStorageConfigurations(maxStorageConfigurations),
	_geoLocationEntries(geoLocationEntries),
	_autoGeo(autoGeo),
	_storageTypesSupported(storageTypesSupported),
	_discoveryNotSupported(discoveryNotSupported),
	_networkConfigNotSupported(networkConfigNotSupported),
	_userConfigNotSupported(userConfigNotSupported),
	_addons(addons)
{
}


SystemCapabilities::~SystemCapabilities()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SystemReboot.h"


namespace ONVIF {
namespace Device {


SystemReboot::SystemReboot()
{
}


SystemReboot::~SystemReboot()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/SystemRebootResponse.h"


namespace ONVIF {
namespace Device {


SystemRebootResponse::SystemRebootResponse():
	_message()
{
}


SystemRebootResponse::SystemRebootResponse(const std::string& message):
	_message(message)
{
}


SystemRebootResponse::~SystemRebootResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/UpgradeSystemFirmware.h"


namespace ONVIF {
namespace Device {


UpgradeSystemFirmware::UpgradeSystemFirmware():
	_firmware()
{
}


UpgradeSystemFirmware::UpgradeSystemFirmware(const ONVIF::AttachmentData& firmware):
	_firmware(firmware)
{
}


UpgradeSystemFirmware::~UpgradeSystemFirmware()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/UpgradeSystemFirmwareResponse.h"


namespace ONVIF {
namespace Device {


UpgradeSystemFirmwareResponse::UpgradeSystemFirmwareResponse():
	_message()
{
}


UpgradeSystemFirmwareResponse::UpgradeSystemFirmwareResponse(const Poco::Optional<std::string>& message):
	_message(message)
{
}


UpgradeSystemFirmwareResponse::~UpgradeSystemFirmwareResponse()
{
}


} } // ONVIF::Device


#include "ONVIF/Device/UserCredential.h"


namespace ONVIF {
namespace Device {


UserCredential::UserCredential():
	_userName(),
	_password(),
	_extension()
{
}


UserCredential::UserCredential(const std::string& userName, const Poco::Optional<std::string>& password, const Poco::SharedPtr<Extension_1>& extension):
	_userName(userName),
	_password(password),
	_extension(extension)
{
}


UserCredential::~UserCredential()
{
}


} } // ONVIF::Device


#include "ONVIF/DeviceIO/Capabilities.h"


namespace ONVIF {
namespace DeviceIO {


Capabilities::Capabilities():
	_videoSources(0),
	_videoOutputs(0),
	_audioSources(0),
	_audioOutputs(0),
	_relayOutputs(0),
	_serialPorts(0),
	_digitalInputs(0),
	_digitalInputOptions(false)
{
}


Capabilities::Capabilities(Poco::Optional<int> videoSources, Poco::Optional<int> videoOutputs, Poco::Optional<int> audioSources, Poco::Optional<int> audioOutputs, Poco::Optional<int> relayOutputs, Poco::Optional<int> serialPorts, Poco::Optional<int> digitalInputs, Poco::Optional<bool> digitalInputOptions):
	_videoSources(videoSources),
	_videoOutputs(videoOutputs),
	_audioSources(audioSources),
	_audioOutputs(audioOutputs),
	_relayOutputs(relayOutputs),
	_serialPorts(serialPorts),
	_digitalInputs(digitalInputs),
	_digitalInputOptions(digitalInputOptions)
{
}


Capabilities::~Capabilities()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/DeviceIOBinding.h"


namespace ONVIF {
namespace DeviceIO {


DeviceIOBinding::~DeviceIOBinding()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/DigitalInputConfigurationOptions.h"


namespace ONVIF {
namespace DeviceIO {


DigitalInputConfigurationOptions::DigitalInputConfigurationOptions():
	_idleState()
{
}


DigitalInputConfigurationOptions::DigitalInputConfigurationOptions(const std::vector<std::string>& idleState):
	_idleState(idleState)
{
}


DigitalInputConfigurationOptions::~DigitalInputConfigurationOptions()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/Get.h"


namespace ONVIF {
namespace DeviceIO {


Get::Get()
{
}


Get::~Get()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetAudioOutputConfiguration.h"


namespace ONVIF {
namespace DeviceIO {


GetAudioOutputConfiguration::GetAudioOutputConfiguration():
	_audioOutputToken()
{
}


GetAudioOutputConfiguration::GetAudioOutputConfiguration(const std::string& audioOutputToken):
	_audioOutputToken(audioOutputToken)
{
}


GetAudioOutputConfiguration::~GetAudioOutputConfiguration()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetAudioOutputConfigurationOptions.h"


namespace ONVIF {
namespace DeviceIO {


GetAudioOutputConfigurationOptions::GetAudioOutputConfigurationOptions():
	_audioOutputToken()
{
}


GetAudioOutputConfigurationOptions::GetAudioOutputConfigurationOptions(const std::string& audioOutputToken):
	_audioOutputToken(audioOutputToken)
{
}


GetAudioOutputConfigurationOptions::~GetAudioOutputConfigurationOptions()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetAudioOutputConfigurationOptionsResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetAudioOutputConfigurationOptionsResponse::GetAudioOutputConfigurationOptionsResponse():
	_audioOutputOptions()
{
}


GetAudioOutputConfigurationOptionsResponse::GetAudioOutputConfigurationOptionsResponse(const ONVIF::AudioOutputConfigurationOptions& audioOutputOptions):
	_audioOutputOptions(audioOutputOptions)
{
}


GetAudioOutputConfigurationOptionsResponse::~GetAudioOutputConfigurationOptionsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetAudioOutputConfigurationResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetAudioOutputConfigurationResponse::GetAudioOutputConfigurationResponse():
	_audioOutputConfiguration()
{
}


GetAudioOutputConfigurationResponse::GetAudioOutputConfigurationResponse(const ONVIF::AudioOutputConfiguration& audioOutputConfiguration):
	_audioOutputConfiguration(audioOutputConfiguration)
{
}


GetAudioOutputConfigurationResponse::~GetAudioOutputConfigurationResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetAudioSourceConfiguration.h"


namespace ONVIF {
namespace DeviceIO {


GetAudioSourceConfiguration::GetAudioSourceConfiguration():
	_audioSourceToken()
{
}


GetAudioSourceConfiguration::GetAudioSourceConfiguration(const std::string& audioSourceToken):
	_audioSourceToken(audioSourceToken)
{
}


GetAudioSourceConfiguration::~GetAudioSourceConfiguration()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetAudioSourceConfigurationOptions.h"


namespace ONVIF {
namespace DeviceIO {


GetAudioSourceConfigurationOptions::GetAudioSourceConfigurationOptions():
	_audioSourceToken()
{
}


GetAudioSourceConfigurationOptions::GetAudioSourceConfigurationOptions(const std::string& audioSourceToken):
	_audioSourceToken(audioSourceToken)
{
}


GetAudioSourceConfigurationOptions::~GetAudioSourceConfigurationOptions()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetAudioSourceConfigurationOptionsResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetAudioSourceConfigurationOptionsResponse::GetAudioSourceConfigurationOptionsResponse():
	_audioSourceOptions()
{
}


GetAudioSourceConfigurationOptionsResponse::GetAudioSourceConfigurationOptionsResponse(const ONVIF::AudioSourceConfigurationOptions& audioSourceOptions):
	_audioSourceOptions(audioSourceOptions)
{
}


GetAudioSourceConfigurationOptionsResponse::~GetAudioSourceConfigurationOptionsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetAudioSourceConfigurationResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetAudioSourceConfigurationResponse::GetAudioSourceConfigurationResponse():
	_audioSourceConfiguration()
{
}


GetAudioSourceConfigurationResponse::GetAudioSourceConfigurationResponse(const ONVIF::AudioSourceConfiguration& audioSourceConfiguration):
	_audioSourceConfiguration(audioSourceConfiguration)
{
}


GetAudioSourceConfigurationResponse::~GetAudioSourceConfigurationResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetDigitalInputConfigurationOptions.h"


namespace ONVIF {
namespace DeviceIO {


GetDigitalInputConfigurationOptions::GetDigitalInputConfigurationOptions():
	_token()
{
}


GetDigitalInputConfigurationOptions::GetDigitalInputConfigurationOptions(const Poco::Optional<std::string>& token):
	_token(token)
{
}


GetDigitalInputConfigurationOptions::~GetDigitalInputConfigurationOptions()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetDigitalInputConfigurationOptionsResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetDigitalInputConfigurationOptionsResponse::GetDigitalInputConfigurationOptionsResponse():
	_digitalInputOptions()
{
}


GetDigitalInputConfigurationOptionsResponse::GetDigitalInputConfigurationOptionsResponse(const DigitalInputConfigurationOptions& digitalInputOptions):
	_digitalInputOptions(digitalInputOptions)
{
}


GetDigitalInputConfigurationOptionsResponse::~GetDigitalInputConfigurationOptionsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetDigitalInputs.h"


namespace ONVIF {
namespace DeviceIO {


GetDigitalInputs::GetDigitalInputs()
{
}


GetDigitalInputs::~GetDigitalInputs()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetDigitalInputsResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetDigitalInputsResponse::GetDigitalInputsResponse():
	_digitalInputs()
{
}


GetDigitalInputsResponse::GetDigitalInputsResponse(const std::vector<ONVIF::DigitalInput>& digitalInputs):
	_digitalInputs(digitalInputs)
{
}


GetDigitalInputsResponse::~GetDigitalInputsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetRelayOutputOptions.h"


namespace ONVIF {
namespace DeviceIO {


GetRelayOutputOptions::GetRelayOutputOptions():
	_relayOutputToken()
{
}


GetRelayOutputOptions::GetRelayOutputOptions(const Poco::Optional<std::string>& relayOutputToken):
	_relayOutputToken(relayOutputToken)
{
}


GetRelayOutputOptions::~GetRelayOutputOptions()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetRelayOutputOptionsResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetRelayOutputOptionsResponse::GetRelayOutputOptionsResponse():
	_relayOutputOptions()
{
}


GetRelayOutputOptionsResponse::GetRelayOutputOptionsResponse(const std::vector<RelayOutputOptions>& relayOutputOptions):
	_relayOutputOptions(relayOutputOptions)
{
}


GetRelayOutputOptionsResponse::~GetRelayOutputOptionsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetResponse::GetResponse():
	_token()
{
}


GetResponse::GetResponse(const std::vector<std::string>& token):
	_token(token)
{
}


GetResponse::~GetResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetSerialPortConfiguration.h"


namespace ONVIF {
namespace DeviceIO {


GetSerialPortConfiguration::GetSerialPortConfiguration():
	_serialPortToken()
{
}


GetSerialPortConfiguration::GetSerialPortConfiguration(const std::string& serialPortToken):
	_serialPortToken(serialPortToken)
{
}


GetSerialPortConfiguration::~GetSerialPortConfiguration()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetSerialPortConfigurationOptions.h"


namespace ONVIF {
namespace DeviceIO {


GetSerialPortConfigurationOptions::GetSerialPortConfigurationOptions():
	_serialPortToken()
{
}


GetSerialPortConfigurationOptions::GetSerialPortConfigurationOptions(const std::string& serialPortToken):
	_serialPortToken(serialPortToken)
{
}


GetSerialPortConfigurationOptions::~GetSerialPortConfigurationOptions()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetSerialPortConfigurationOptionsResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetSerialPortConfigurationOptionsResponse::GetSerialPortConfigurationOptionsResponse():
	_serialPortOptions()
{
}


GetSerialPortConfigurationOptionsResponse::GetSerialPortConfigurationOptionsResponse(const SerialPortConfigurationOptions& serialPortOptions):
	_serialPortOptions(serialPortOptions)
{
}


GetSerialPortConfigurationOptionsResponse::~GetSerialPortConfigurationOptionsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetSerialPortConfigurationResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetSerialPortConfigurationResponse::GetSerialPortConfigurationResponse():
	_serialPortConfiguration()
{
}


GetSerialPortConfigurationResponse::GetSerialPortConfigurationResponse(const SerialPortConfiguration& serialPortConfiguration):
	_serialPortConfiguration(serialPortConfiguration)
{
}


GetSerialPortConfigurationResponse::~GetSerialPortConfigurationResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetSerialPorts.h"


namespace ONVIF {
namespace DeviceIO {


GetSerialPorts::GetSerialPorts()
{
}


GetSerialPorts::~GetSerialPorts()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetSerialPortsResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetSerialPortsResponse::GetSerialPortsResponse():
	_serialPort()
{
}


GetSerialPortsResponse::GetSerialPortsResponse(const std::vector<SerialPort>& serialPort):
	_serialPort(serialPort)
{
}


GetSerialPortsResponse::~GetSerialPortsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetServiceCapabilities.h"


namespace ONVIF {
namespace DeviceIO {


GetServiceCapabilities::GetServiceCapabilities()
{
}


GetServiceCapabilities::~GetServiceCapabilities()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetServiceCapabilitiesResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse():
	_capabilities()
{
}


GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse(const Capabilities& capabilities):
	_capabilities(capabilities)
{
}


GetServiceCapabilitiesResponse::~GetServiceCapabilitiesResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetVideoOutputConfiguration.h"


namespace ONVIF {
namespace DeviceIO {


GetVideoOutputConfiguration::GetVideoOutputConfiguration():
	_videoOutputToken()
{
}


GetVideoOutputConfiguration::GetVideoOutputConfiguration(const std::string& videoOutputToken):
	_videoOutputToken(videoOutputToken)
{
}


GetVideoOutputConfiguration::~GetVideoOutputConfiguration()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetVideoOutputConfigurationOptions.h"


namespace ONVIF {
namespace DeviceIO {


GetVideoOutputConfigurationOptions::GetVideoOutputConfigurationOptions():
	_videoOutputToken()
{
}


GetVideoOutputConfigurationOptions::GetVideoOutputConfigurationOptions(const std::string& videoOutputToken):
	_videoOutputToken(videoOutputToken)
{
}


GetVideoOutputConfigurationOptions::~GetVideoOutputConfigurationOptions()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetVideoOutputConfigurationOptionsResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetVideoOutputConfigurationOptionsResponse::GetVideoOutputConfigurationOptionsResponse():
	_videoOutputConfigurationOptions()
{
}


GetVideoOutputConfigurationOptionsResponse::GetVideoOutputConfigurationOptionsResponse(const ONVIF::VideoOutputConfigurationOptions& videoOutputConfigurationOptions):
	_videoOutputConfigurationOptions(videoOutputConfigurationOptions)
{
}


GetVideoOutputConfigurationOptionsResponse::~GetVideoOutputConfigurationOptionsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetVideoOutputConfigurationResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetVideoOutputConfigurationResponse::GetVideoOutputConfigurationResponse():
	_videoOutputConfiguration()
{
}


GetVideoOutputConfigurationResponse::GetVideoOutputConfigurationResponse(const ONVIF::VideoOutputConfiguration& videoOutputConfiguration):
	_videoOutputConfiguration(videoOutputConfiguration)
{
}


GetVideoOutputConfigurationResponse::~GetVideoOutputConfigurationResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetVideoOutputs.h"


namespace ONVIF {
namespace DeviceIO {


GetVideoOutputs::GetVideoOutputs()
{
}


GetVideoOutputs::~GetVideoOutputs()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetVideoOutputsResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetVideoOutputsResponse::GetVideoOutputsResponse():
	_videoOutputs()
{
}


GetVideoOutputsResponse::GetVideoOutputsResponse(const std::vector<ONVIF::VideoOutput>& videoOutputs):
	_videoOutputs(videoOutputs)
{
}


GetVideoOutputsResponse::~GetVideoOutputsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetVideoSourceConfiguration.h"


namespace ONVIF {
namespace DeviceIO {


GetVideoSourceConfiguration::GetVideoSourceConfiguration():
	_videoSourceToken()
{
}


GetVideoSourceConfiguration::GetVideoSourceConfiguration(const std::string& videoSourceToken):
	_videoSourceToken(videoSourceToken)
{
}


GetVideoSourceConfiguration::~GetVideoSourceConfiguration()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetVideoSourceConfigurationOptions.h"


namespace ONVIF {
namespace DeviceIO {


GetVideoSourceConfigurationOptions::GetVideoSourceConfigurationOptions():
	_videoSourceToken()
{
}


GetVideoSourceConfigurationOptions::GetVideoSourceConfigurationOptions(const std::string& videoSourceToken):
	_videoSourceToken(videoSourceToken)
{
}


GetVideoSourceConfigurationOptions::~GetVideoSourceConfigurationOptions()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetVideoSourceConfigurationOptionsResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetVideoSourceConfigurationOptionsResponse::GetVideoSourceConfigurationOptionsResponse():
	_videoSourceConfigurationOptions()
{
}


GetVideoSourceConfigurationOptionsResponse::GetVideoSourceConfigurationOptionsResponse(const ONVIF::VideoSourceConfigurationOptions& videoSourceConfigurationOptions):
	_videoSourceConfigurationOptions(videoSourceConfigurationOptions)
{
}


GetVideoSourceConfigurationOptionsResponse::~GetVideoSourceConfigurationOptionsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/GetVideoSourceConfigurationResponse.h"


namespace ONVIF {
namespace DeviceIO {


GetVideoSourceConfigurationResponse::GetVideoSourceConfigurationResponse():
	_videoSourceConfiguration()
{
}


GetVideoSourceConfigurationResponse::GetVideoSourceConfigurationResponse(const ONVIF::VideoSourceConfiguration& videoSourceConfiguration):
	_videoSourceConfiguration(videoSourceConfiguration)
{
}


GetVideoSourceConfigurationResponse::~GetVideoSourceConfigurationResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/ParityBitList.h"


namespace ONVIF {
namespace DeviceIO {


ParityBitList::ParityBitList():
	_items()
{
}


ParityBitList::ParityBitList(const std::vector<std::string>& items):
	_items(items)
{
}


ParityBitList::~ParityBitList()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/RelayOutputOptions.h"


namespace ONVIF {
namespace DeviceIO {


RelayOutputOptions::RelayOutputOptions():
	_token(),
	_mode(),
	_delayTimes(),
	_discrete(),
	_extension()
{
}


RelayOutputOptions::RelayOutputOptions(const std::string& token, const std::vector<std::string>& mode, const Poco::Optional<std::vector<float>>& delayTimes, Poco::Optional<bool> discrete, const Poco::SharedPtr<RelayOutputOptionsExtension>& extension):
	_token(token),
	_mode(mode),
	_delayTimes(delayTimes),
	_discrete(discrete),
	_extension(extension)
{
}


RelayOutputOptions::~RelayOutputOptions()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/RelayOutputOptionsExtension.h"


namespace ONVIF {
namespace DeviceIO {


RelayOutputOptionsExtension::RelayOutputOptionsExtension()
{
}


RelayOutputOptionsExtension::~RelayOutputOptionsExtension()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SendReceiveSerialCommand.h"


namespace ONVIF {
namespace DeviceIO {


SendReceiveSerialCommand::SendReceiveSerialCommand():
	_token(),
	_serialData(),
	_timeOut(),
	_dataLength(),
	_delimiter()
{
}


SendReceiveSerialCommand::SendReceiveSerialCommand(const Poco::Optional<std::string>& token, const Poco::SharedPtr<SerialData>& serialData, const Poco::Optional<std::string>& timeOut, Poco::Optional<int> dataLength, const Poco::Optional<std::string>& delimiter):
	_token(token),
	_serialData(serialData),
	_timeOut(timeOut),
	_dataLength(dataLength),
	_delimiter(delimiter)
{
}


SendReceiveSerialCommand::~SendReceiveSerialCommand()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SendReceiveSerialCommandResponse.h"


namespace ONVIF {
namespace DeviceIO {


SendReceiveSerialCommandResponse::SendReceiveSerialCommandResponse():
	_serialData()
{
}


SendReceiveSerialCommandResponse::SendReceiveSerialCommandResponse(const Poco::SharedPtr<SerialData>& serialData):
	_serialData(serialData)
{
}


SendReceiveSerialCommandResponse::~SendReceiveSerialCommandResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SerialData.h"


namespace ONVIF {
namespace DeviceIO {


SerialData::SerialData():
	_binary(),
	_string()
{
}


SerialData::SerialData(const Poco::Optional<std::vector<char>>& binary, const Poco::Optional<std::string>& string):
	_binary(binary),
	_string(string)
{
}


SerialData::~SerialData()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SerialPort.h"


namespace ONVIF {
namespace DeviceIO {


SerialPort::SerialPort()
{
}


SerialPort::SerialPort(const std::string& token):
	ONVIF::DeviceEntity(token)
{
}


SerialPort::~SerialPort()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SerialPortConfiguration.h"


namespace ONVIF {
namespace DeviceIO {


SerialPortConfiguration::SerialPortConfiguration():
	_token(),
	_type(),
	_baudRate(),
	_parityBit(),
	_characterLength(),
	_stopBit()
{
}


SerialPortConfiguration::SerialPortConfiguration(const std::string& token, const std::string& type, int baudRate, const std::string& parityBit, int characterLength, float stopBit):
	_token(token),
	_type(type),
	_baudRate(baudRate),
	_parityBit(parityBit),
	_characterLength(characterLength),
	_stopBit(stopBit)
{
}


SerialPortConfiguration::~SerialPortConfiguration()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SerialPortConfigurationOptions.h"


namespace ONVIF {
namespace DeviceIO {


SerialPortConfigurationOptions::SerialPortConfigurationOptions():
	_token(),
	_baudRateList(),
	_parityBitList(),
	_characterLengthList(),
	_stopBitList()
{
}


SerialPortConfigurationOptions::SerialPortConfigurationOptions(const std::string& token, const ONVIF::IntItems& baudRateList, const ParityBitList& parityBitList, const ONVIF::IntItems& characterLengthList, const ONVIF::FloatItems& stopBitList):
	_token(token),
	_baudRateList(baudRateList),
	_parityBitList(parityBitList),
	_characterLengthList(characterLengthList),
	_stopBitList(stopBitList)
{
}


SerialPortConfigurationOptions::~SerialPortConfigurationOptions()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetAudioOutputConfiguration.h"


namespace ONVIF {
namespace DeviceIO {


SetAudioOutputConfiguration::SetAudioOutputConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetAudioOutputConfiguration::SetAudioOutputConfiguration(const ONVIF::AudioOutputConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetAudioOutputConfiguration::~SetAudioOutputConfiguration()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetAudioOutputConfigurationResponse.h"


namespace ONVIF {
namespace DeviceIO {


SetAudioOutputConfigurationResponse::SetAudioOutputConfigurationResponse()
{
}


SetAudioOutputConfigurationResponse::~SetAudioOutputConfigurationResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetAudioSourceConfiguration.h"


namespace ONVIF {
namespace DeviceIO {


SetAudioSourceConfiguration::SetAudioSourceConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetAudioSourceConfiguration::SetAudioSourceConfiguration(const ONVIF::AudioSourceConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetAudioSourceConfiguration::~SetAudioSourceConfiguration()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetAudioSourceConfigurationResponse.h"


namespace ONVIF {
namespace DeviceIO {


SetAudioSourceConfigurationResponse::SetAudioSourceConfigurationResponse()
{
}


SetAudioSourceConfigurationResponse::~SetAudioSourceConfigurationResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetDigitalInputConfigurations.h"


namespace ONVIF {
namespace DeviceIO {


SetDigitalInputConfigurations::SetDigitalInputConfigurations():
	_digitalInputs()
{
}


SetDigitalInputConfigurations::SetDigitalInputConfigurations(const std::vector<ONVIF::DigitalInput>& digitalInputs):
	_digitalInputs(digitalInputs)
{
}


SetDigitalInputConfigurations::~SetDigitalInputConfigurations()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetDigitalInputConfigurationsResponse.h"


namespace ONVIF {
namespace DeviceIO {


SetDigitalInputConfigurationsResponse::SetDigitalInputConfigurationsResponse()
{
}


SetDigitalInputConfigurationsResponse::~SetDigitalInputConfigurationsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetRelayOutputSettings.h"


namespace ONVIF {
namespace DeviceIO {


SetRelayOutputSettings::SetRelayOutputSettings():
	_relayOutput()
{
}


SetRelayOutputSettings::SetRelayOutputSettings(const ONVIF::RelayOutput& relayOutput):
	_relayOutput(relayOutput)
{
}


SetRelayOutputSettings::~SetRelayOutputSettings()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetRelayOutputSettingsResponse.h"


namespace ONVIF {
namespace DeviceIO {


SetRelayOutputSettingsResponse::SetRelayOutputSettingsResponse()
{
}


SetRelayOutputSettingsResponse::~SetRelayOutputSettingsResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetSerialPortConfiguration.h"


namespace ONVIF {
namespace DeviceIO {


SetSerialPortConfiguration::SetSerialPortConfiguration():
	_serialPortConfiguration(),
	_forcePersistance()
{
}


SetSerialPortConfiguration::SetSerialPortConfiguration(const SerialPortConfiguration& serialPortConfiguration, bool forcePersistance):
	_serialPortConfiguration(serialPortConfiguration),
	_forcePersistance(forcePersistance)
{
}


SetSerialPortConfiguration::~SetSerialPortConfiguration()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetSerialPortConfigurationResponse.h"


namespace ONVIF {
namespace DeviceIO {


SetSerialPortConfigurationResponse::SetSerialPortConfigurationResponse()
{
}


SetSerialPortConfigurationResponse::~SetSerialPortConfigurationResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetVideoOutputConfiguration.h"


namespace ONVIF {
namespace DeviceIO {


SetVideoOutputConfiguration::SetVideoOutputConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetVideoOutputConfiguration::SetVideoOutputConfiguration(const ONVIF::VideoOutputConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetVideoOutputConfiguration::~SetVideoOutputConfiguration()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetVideoOutputConfigurationResponse.h"


namespace ONVIF {
namespace DeviceIO {


SetVideoOutputConfigurationResponse::SetVideoOutputConfigurationResponse()
{
}


SetVideoOutputConfigurationResponse::~SetVideoOutputConfigurationResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetVideoSourceConfiguration.h"


namespace ONVIF {
namespace DeviceIO {


SetVideoSourceConfiguration::SetVideoSourceConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetVideoSourceConfiguration::SetVideoSourceConfiguration(const ONVIF::VideoSourceConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetVideoSourceConfiguration::~SetVideoSourceConfiguration()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/DeviceIO/SetVideoSourceConfigurationResponse.h"


namespace ONVIF {
namespace DeviceIO {


SetVideoSourceConfigurationResponse::SetVideoSourceConfigurationResponse()
{
}


SetVideoSourceConfigurationResponse::~SetVideoSourceConfigurationResponse()
{
}


} } // ONVIF::DeviceIO


#include "ONVIF/Media/AddAudioDecoderConfiguration.h"


namespace ONVIF {
namespace Media {


AddAudioDecoderConfiguration::AddAudioDecoderConfiguration():
	_profileToken(),
	_configurationToken()
{
}


AddAudioDecoderConfiguration::AddAudioDecoderConfiguration(const std::string& profileToken, const std::string& configurationToken):
	_profileToken(profileToken),
	_configurationToken(configurationToken)
{
}


AddAudioDecoderConfiguration::~AddAudioDecoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddAudioDecoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


AddAudioDecoderConfigurationResponse::AddAudioDecoderConfigurationResponse()
{
}


AddAudioDecoderConfigurationResponse::~AddAudioDecoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddAudioEncoderConfiguration.h"


namespace ONVIF {
namespace Media {


AddAudioEncoderConfiguration::AddAudioEncoderConfiguration():
	_profileToken(),
	_configurationToken()
{
}


AddAudioEncoderConfiguration::AddAudioEncoderConfiguration(const std::string& profileToken, const std::string& configurationToken):
	_profileToken(profileToken),
	_configurationToken(configurationToken)
{
}


AddAudioEncoderConfiguration::~AddAudioEncoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddAudioEncoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


AddAudioEncoderConfigurationResponse::AddAudioEncoderConfigurationResponse()
{
}


AddAudioEncoderConfigurationResponse::~AddAudioEncoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddAudioOutputConfiguration.h"


namespace ONVIF {
namespace Media {


AddAudioOutputConfiguration::AddAudioOutputConfiguration():
	_profileToken(),
	_configurationToken()
{
}


AddAudioOutputConfiguration::AddAudioOutputConfiguration(const std::string& profileToken, const std::string& configurationToken):
	_profileToken(profileToken),
	_configurationToken(configurationToken)
{
}


AddAudioOutputConfiguration::~AddAudioOutputConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddAudioOutputConfigurationResponse.h"


namespace ONVIF {
namespace Media {


AddAudioOutputConfigurationResponse::AddAudioOutputConfigurationResponse()
{
}


AddAudioOutputConfigurationResponse::~AddAudioOutputConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddAudioSourceConfiguration.h"


namespace ONVIF {
namespace Media {


AddAudioSourceConfiguration::AddAudioSourceConfiguration():
	_profileToken(),
	_configurationToken()
{
}


AddAudioSourceConfiguration::AddAudioSourceConfiguration(const std::string& profileToken, const std::string& configurationToken):
	_profileToken(profileToken),
	_configurationToken(configurationToken)
{
}


AddAudioSourceConfiguration::~AddAudioSourceConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddAudioSourceConfigurationResponse.h"


namespace ONVIF {
namespace Media {


AddAudioSourceConfigurationResponse::AddAudioSourceConfigurationResponse()
{
}


AddAudioSourceConfigurationResponse::~AddAudioSourceConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddMetadataConfiguration.h"


namespace ONVIF {
namespace Media {


AddMetadataConfiguration::AddMetadataConfiguration():
	_profileToken(),
	_configurationToken()
{
}


AddMetadataConfiguration::AddMetadataConfiguration(const std::string& profileToken, const std::string& configurationToken):
	_profileToken(profileToken),
	_configurationToken(configurationToken)
{
}


AddMetadataConfiguration::~AddMetadataConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddMetadataConfigurationResponse.h"


namespace ONVIF {
namespace Media {


AddMetadataConfigurationResponse::AddMetadataConfigurationResponse()
{
}


AddMetadataConfigurationResponse::~AddMetadataConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddPTZConfiguration.h"


namespace ONVIF {
namespace Media {


AddPTZConfiguration::AddPTZConfiguration():
	_profileToken(),
	_configurationToken()
{
}


AddPTZConfiguration::AddPTZConfiguration(const std::string& profileToken, const std::string& configurationToken):
	_profileToken(profileToken),
	_configurationToken(configurationToken)
{
}


AddPTZConfiguration::~AddPTZConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddPTZConfigurationResponse.h"


namespace ONVIF {
namespace Media {


AddPTZConfigurationResponse::AddPTZConfigurationResponse()
{
}


AddPTZConfigurationResponse::~AddPTZConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddVideoAnalyticsConfiguration.h"


namespace ONVIF {
namespace Media {


AddVideoAnalyticsConfiguration::AddVideoAnalyticsConfiguration():
	_profileToken(),
	_configurationToken()
{
}


AddVideoAnalyticsConfiguration::AddVideoAnalyticsConfiguration(const std::string& profileToken, const std::string& configurationToken):
	_profileToken(profileToken),
	_configurationToken(configurationToken)
{
}


AddVideoAnalyticsConfiguration::~AddVideoAnalyticsConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddVideoAnalyticsConfigurationResponse.h"


namespace ONVIF {
namespace Media {


AddVideoAnalyticsConfigurationResponse::AddVideoAnalyticsConfigurationResponse()
{
}


AddVideoAnalyticsConfigurationResponse::~AddVideoAnalyticsConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddVideoEncoderConfiguration.h"


namespace ONVIF {
namespace Media {


AddVideoEncoderConfiguration::AddVideoEncoderConfiguration():
	_profileToken(),
	_configurationToken()
{
}


AddVideoEncoderConfiguration::AddVideoEncoderConfiguration(const std::string& profileToken, const std::string& configurationToken):
	_profileToken(profileToken),
	_configurationToken(configurationToken)
{
}


AddVideoEncoderConfiguration::~AddVideoEncoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddVideoEncoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


AddVideoEncoderConfigurationResponse::AddVideoEncoderConfigurationResponse()
{
}


AddVideoEncoderConfigurationResponse::~AddVideoEncoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddVideoSourceConfiguration.h"


namespace ONVIF {
namespace Media {


AddVideoSourceConfiguration::AddVideoSourceConfiguration():
	_profileToken(),
	_configurationToken()
{
}


AddVideoSourceConfiguration::AddVideoSourceConfiguration(const std::string& profileToken, const std::string& configurationToken):
	_profileToken(profileToken),
	_configurationToken(configurationToken)
{
}


AddVideoSourceConfiguration::~AddVideoSourceConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/AddVideoSourceConfigurationResponse.h"


namespace ONVIF {
namespace Media {


AddVideoSourceConfigurationResponse::AddVideoSourceConfigurationResponse()
{
}


AddVideoSourceConfigurationResponse::~AddVideoSourceConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/Capabilities.h"


namespace ONVIF {
namespace Media {


Capabilities::Capabilities():
	_snapshotUri(),
	_rotation(),
	_videoSourceMode(),
	_oSD(),
	_temporaryOSDText(),
	_eXICompression(),
	_profileCapabilities(),
	_streamingCapabilities()
{
}


Capabilities::Capabilities(Poco::Optional<bool> snapshotUri, Poco::Optional<bool> rotation, Poco::Optional<bool> videoSourceMode, Poco::Optional<bool> oSD, Poco::Optional<bool> temporaryOSDText, Poco::Optional<bool> eXICompression, const ProfileCapabilities& profileCapabilities, const StreamingCapabilities& streamingCapabilities):
	_snapshotUri(snapshotUri),
	_rotation(rotation),
	_videoSourceMode(videoSourceMode),
	_oSD(oSD),
	_temporaryOSDText(temporaryOSDText),
	_eXICompression(eXICompression),
	_profileCapabilities(profileCapabilities),
	_streamingCapabilities(streamingCapabilities)
{
}


Capabilities::~Capabilities()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/CreateOSD.h"


namespace ONVIF {
namespace Media {


CreateOSD::CreateOSD():
	_oSD()
{
}


CreateOSD::CreateOSD(const ONVIF::OSDConfiguration& oSD):
	_oSD(oSD)
{
}


CreateOSD::~CreateOSD()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/CreateOSDResponse.h"


namespace ONVIF {
namespace Media {


CreateOSDResponse::CreateOSDResponse():
	_oSDToken()
{
}


CreateOSDResponse::CreateOSDResponse(const std::string& oSDToken):
	_oSDToken(oSDToken)
{
}


CreateOSDResponse::~CreateOSDResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/CreateProfile.h"


namespace ONVIF {
namespace Media {


CreateProfile::CreateProfile():
	_name(),
	_token()
{
}


CreateProfile::CreateProfile(const std::string& name, const Poco::Optional<std::string>& token):
	_name(name),
	_token(token)
{
}


CreateProfile::~CreateProfile()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/CreateProfileResponse.h"


namespace ONVIF {
namespace Media {


CreateProfileResponse::CreateProfileResponse():
	_profile()
{
}


CreateProfileResponse::CreateProfileResponse(const ONVIF::Profile& profile):
	_profile(profile)
{
}


CreateProfileResponse::~CreateProfileResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/DeleteOSD.h"


namespace ONVIF {
namespace Media {


DeleteOSD::DeleteOSD():
	_oSDToken()
{
}


DeleteOSD::DeleteOSD(const std::string& oSDToken):
	_oSDToken(oSDToken)
{
}


DeleteOSD::~DeleteOSD()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/DeleteOSDResponse.h"


namespace ONVIF {
namespace Media {


DeleteOSDResponse::DeleteOSDResponse()
{
}


DeleteOSDResponse::~DeleteOSDResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/DeleteProfile.h"


namespace ONVIF {
namespace Media {


DeleteProfile::DeleteProfile():
	_profileToken()
{
}


DeleteProfile::DeleteProfile(const std::string& profileToken):
	_profileToken(profileToken)
{
}


DeleteProfile::~DeleteProfile()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/DeleteProfileResponse.h"


namespace ONVIF {
namespace Media {


DeleteProfileResponse::DeleteProfileResponse()
{
}


DeleteProfileResponse::~DeleteProfileResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioDecoderConfiguration.h"


namespace ONVIF {
namespace Media {


GetAudioDecoderConfiguration::GetAudioDecoderConfiguration():
	_configurationToken()
{
}


GetAudioDecoderConfiguration::GetAudioDecoderConfiguration(const std::string& configurationToken):
	_configurationToken(configurationToken)
{
}


GetAudioDecoderConfiguration::~GetAudioDecoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioDecoderConfigurationOptions.h"


namespace ONVIF {
namespace Media {


GetAudioDecoderConfigurationOptions::GetAudioDecoderConfigurationOptions():
	_configurationToken(),
	_profileToken()
{
}


GetAudioDecoderConfigurationOptions::GetAudioDecoderConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken):
	_configurationToken(configurationToken),
	_profileToken(profileToken)
{
}


GetAudioDecoderConfigurationOptions::~GetAudioDecoderConfigurationOptions()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioDecoderConfigurationOptionsResponse.h"


namespace ONVIF {
namespace Media {


GetAudioDecoderConfigurationOptionsResponse::GetAudioDecoderConfigurationOptionsResponse():
	_options()
{
}


GetAudioDecoderConfigurationOptionsResponse::GetAudioDecoderConfigurationOptionsResponse(const ONVIF::AudioDecoderConfigurationOptions& options):
	_options(options)
{
}


GetAudioDecoderConfigurationOptionsResponse::~GetAudioDecoderConfigurationOptionsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioDecoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


GetAudioDecoderConfigurationResponse::GetAudioDecoderConfigurationResponse():
	_configuration()
{
}


GetAudioDecoderConfigurationResponse::GetAudioDecoderConfigurationResponse(const ONVIF::AudioDecoderConfiguration& configuration):
	_configuration(configuration)
{
}


GetAudioDecoderConfigurationResponse::~GetAudioDecoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioDecoderConfigurations.h"


namespace ONVIF {
namespace Media {


GetAudioDecoderConfigurations::GetAudioDecoderConfigurations()
{
}


GetAudioDecoderConfigurations::~GetAudioDecoderConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioDecoderConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetAudioDecoderConfigurationsResponse::GetAudioDecoderConfigurationsResponse():
	_configurations()
{
}


GetAudioDecoderConfigurationsResponse::GetAudioDecoderConfigurationsResponse(const std::vector<ONVIF::AudioDecoderConfiguration>& configurations):
	_configurations(configurations)
{
}


GetAudioDecoderConfigurationsResponse::~GetAudioDecoderConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioEncoderConfiguration.h"


namespace ONVIF {
namespace Media {


GetAudioEncoderConfiguration::GetAudioEncoderConfiguration():
	_configurationToken()
{
}


GetAudioEncoderConfiguration::GetAudioEncoderConfiguration(const std::string& configurationToken):
	_configurationToken(configurationToken)
{
}


GetAudioEncoderConfiguration::~GetAudioEncoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioEncoderConfigurationOptions.h"


namespace ONVIF {
namespace Media {


GetAudioEncoderConfigurationOptions::GetAudioEncoderConfigurationOptions():
	_configurationToken(),
	_profileToken()
{
}


GetAudioEncoderConfigurationOptions::GetAudioEncoderConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken):
	_configurationToken(configurationToken),
	_profileToken(profileToken)
{
}


GetAudioEncoderConfigurationOptions::~GetAudioEncoderConfigurationOptions()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioEncoderConfigurationOptionsResponse.h"


namespace ONVIF {
namespace Media {


GetAudioEncoderConfigurationOptionsResponse::GetAudioEncoderConfigurationOptionsResponse():
	_options()
{
}


GetAudioEncoderConfigurationOptionsResponse::GetAudioEncoderConfigurationOptionsResponse(const ONVIF::AudioEncoderConfigurationOptions& options):
	_options(options)
{
}


GetAudioEncoderConfigurationOptionsResponse::~GetAudioEncoderConfigurationOptionsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioEncoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


GetAudioEncoderConfigurationResponse::GetAudioEncoderConfigurationResponse():
	_configuration()
{
}


GetAudioEncoderConfigurationResponse::GetAudioEncoderConfigurationResponse(const ONVIF::AudioEncoderConfiguration& configuration):
	_configuration(configuration)
{
}


GetAudioEncoderConfigurationResponse::~GetAudioEncoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioEncoderConfigurations.h"


namespace ONVIF {
namespace Media {


GetAudioEncoderConfigurations::GetAudioEncoderConfigurations()
{
}


GetAudioEncoderConfigurations::~GetAudioEncoderConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioEncoderConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetAudioEncoderConfigurationsResponse::GetAudioEncoderConfigurationsResponse():
	_configurations()
{
}


GetAudioEncoderConfigurationsResponse::GetAudioEncoderConfigurationsResponse(const std::vector<ONVIF::AudioEncoderConfiguration>& configurations):
	_configurations(configurations)
{
}


GetAudioEncoderConfigurationsResponse::~GetAudioEncoderConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioOutputConfiguration.h"


namespace ONVIF {
namespace Media {


GetAudioOutputConfiguration::GetAudioOutputConfiguration():
	_configurationToken()
{
}


GetAudioOutputConfiguration::GetAudioOutputConfiguration(const std::string& configurationToken):
	_configurationToken(configurationToken)
{
}


GetAudioOutputConfiguration::~GetAudioOutputConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioOutputConfigurationOptions.h"


namespace ONVIF {
namespace Media {


GetAudioOutputConfigurationOptions::GetAudioOutputConfigurationOptions():
	_configurationToken(),
	_profileToken()
{
}


GetAudioOutputConfigurationOptions::GetAudioOutputConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken):
	_configurationToken(configurationToken),
	_profileToken(profileToken)
{
}


GetAudioOutputConfigurationOptions::~GetAudioOutputConfigurationOptions()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioOutputConfigurationOptionsResponse.h"


namespace ONVIF {
namespace Media {


GetAudioOutputConfigurationOptionsResponse::GetAudioOutputConfigurationOptionsResponse():
	_options()
{
}


GetAudioOutputConfigurationOptionsResponse::GetAudioOutputConfigurationOptionsResponse(const ONVIF::AudioOutputConfigurationOptions& options):
	_options(options)
{
}


GetAudioOutputConfigurationOptionsResponse::~GetAudioOutputConfigurationOptionsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioOutputConfigurationResponse.h"


namespace ONVIF {
namespace Media {


GetAudioOutputConfigurationResponse::GetAudioOutputConfigurationResponse():
	_configuration()
{
}


GetAudioOutputConfigurationResponse::GetAudioOutputConfigurationResponse(const ONVIF::AudioOutputConfiguration& configuration):
	_configuration(configuration)
{
}


GetAudioOutputConfigurationResponse::~GetAudioOutputConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioOutputConfigurations.h"


namespace ONVIF {
namespace Media {


GetAudioOutputConfigurations::GetAudioOutputConfigurations()
{
}


GetAudioOutputConfigurations::~GetAudioOutputConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioOutputConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetAudioOutputConfigurationsResponse::GetAudioOutputConfigurationsResponse():
	_configurations()
{
}


GetAudioOutputConfigurationsResponse::GetAudioOutputConfigurationsResponse(const std::vector<ONVIF::AudioOutputConfiguration>& configurations):
	_configurations(configurations)
{
}


GetAudioOutputConfigurationsResponse::~GetAudioOutputConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioOutputs.h"


namespace ONVIF {
namespace Media {


GetAudioOutputs::GetAudioOutputs()
{
}


GetAudioOutputs::~GetAudioOutputs()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioOutputsResponse.h"


namespace ONVIF {
namespace Media {


GetAudioOutputsResponse::GetAudioOutputsResponse():
	_audioOutputs()
{
}


GetAudioOutputsResponse::GetAudioOutputsResponse(const std::vector<ONVIF::AudioOutput>& audioOutputs):
	_audioOutputs(audioOutputs)
{
}


GetAudioOutputsResponse::~GetAudioOutputsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioSourceConfiguration.h"


namespace ONVIF {
namespace Media {


GetAudioSourceConfiguration::GetAudioSourceConfiguration():
	_configurationToken()
{
}


GetAudioSourceConfiguration::GetAudioSourceConfiguration(const std::string& configurationToken):
	_configurationToken(configurationToken)
{
}


GetAudioSourceConfiguration::~GetAudioSourceConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioSourceConfigurationOptions.h"


namespace ONVIF {
namespace Media {


GetAudioSourceConfigurationOptions::GetAudioSourceConfigurationOptions():
	_configurationToken(),
	_profileToken()
{
}


GetAudioSourceConfigurationOptions::GetAudioSourceConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken):
	_configurationToken(configurationToken),
	_profileToken(profileToken)
{
}


GetAudioSourceConfigurationOptions::~GetAudioSourceConfigurationOptions()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioSourceConfigurationOptionsResponse.h"


namespace ONVIF {
namespace Media {


GetAudioSourceConfigurationOptionsResponse::GetAudioSourceConfigurationOptionsResponse():
	_options()
{
}


GetAudioSourceConfigurationOptionsResponse::GetAudioSourceConfigurationOptionsResponse(const ONVIF::AudioSourceConfigurationOptions& options):
	_options(options)
{
}


GetAudioSourceConfigurationOptionsResponse::~GetAudioSourceConfigurationOptionsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioSourceConfigurationResponse.h"


namespace ONVIF {
namespace Media {


GetAudioSourceConfigurationResponse::GetAudioSourceConfigurationResponse():
	_configuration()
{
}


GetAudioSourceConfigurationResponse::GetAudioSourceConfigurationResponse(const ONVIF::AudioSourceConfiguration& configuration):
	_configuration(configuration)
{
}


GetAudioSourceConfigurationResponse::~GetAudioSourceConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioSourceConfigurations.h"


namespace ONVIF {
namespace Media {


GetAudioSourceConfigurations::GetAudioSourceConfigurations()
{
}


GetAudioSourceConfigurations::~GetAudioSourceConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioSourceConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetAudioSourceConfigurationsResponse::GetAudioSourceConfigurationsResponse():
	_configurations()
{
}


GetAudioSourceConfigurationsResponse::GetAudioSourceConfigurationsResponse(const std::vector<ONVIF::AudioSourceConfiguration>& configurations):
	_configurations(configurations)
{
}


GetAudioSourceConfigurationsResponse::~GetAudioSourceConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioSources.h"


namespace ONVIF {
namespace Media {


GetAudioSources::GetAudioSources()
{
}


GetAudioSources::~GetAudioSources()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetAudioSourcesResponse.h"


namespace ONVIF {
namespace Media {


GetAudioSourcesResponse::GetAudioSourcesResponse():
	_audioSources()
{
}


GetAudioSourcesResponse::GetAudioSourcesResponse(const std::vector<ONVIF::AudioSource>& audioSources):
	_audioSources(audioSources)
{
}


GetAudioSourcesResponse::~GetAudioSourcesResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleAudioDecoderConfigurations.h"


namespace ONVIF {
namespace Media {


GetCompatibleAudioDecoderConfigurations::GetCompatibleAudioDecoderConfigurations():
	_profileToken()
{
}


GetCompatibleAudioDecoderConfigurations::GetCompatibleAudioDecoderConfigurations(const std::string& profileToken):
	_profileToken(profileToken)
{
}


GetCompatibleAudioDecoderConfigurations::~GetCompatibleAudioDecoderConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleAudioDecoderConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetCompatibleAudioDecoderConfigurationsResponse::GetCompatibleAudioDecoderConfigurationsResponse():
	_configurations()
{
}


GetCompatibleAudioDecoderConfigurationsResponse::GetCompatibleAudioDecoderConfigurationsResponse(const std::vector<ONVIF::AudioDecoderConfiguration>& configurations):
	_configurations(configurations)
{
}


GetCompatibleAudioDecoderConfigurationsResponse::~GetCompatibleAudioDecoderConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleAudioEncoderConfigurations.h"


namespace ONVIF {
namespace Media {


GetCompatibleAudioEncoderConfigurations::GetCompatibleAudioEncoderConfigurations():
	_profileToken()
{
}


GetCompatibleAudioEncoderConfigurations::GetCompatibleAudioEncoderConfigurations(const std::string& profileToken):
	_profileToken(profileToken)
{
}


GetCompatibleAudioEncoderConfigurations::~GetCompatibleAudioEncoderConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleAudioEncoderConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetCompatibleAudioEncoderConfigurationsResponse::GetCompatibleAudioEncoderConfigurationsResponse():
	_configurations()
{
}


GetCompatibleAudioEncoderConfigurationsResponse::GetCompatibleAudioEncoderConfigurationsResponse(const std::vector<ONVIF::AudioEncoderConfiguration>& configurations):
	_configurations(configurations)
{
}


GetCompatibleAudioEncoderConfigurationsResponse::~GetCompatibleAudioEncoderConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleAudioOutputConfigurations.h"


namespace ONVIF {
namespace Media {


GetCompatibleAudioOutputConfigurations::GetCompatibleAudioOutputConfigurations():
	_profileToken()
{
}


GetCompatibleAudioOutputConfigurations::GetCompatibleAudioOutputConfigurations(const std::string& profileToken):
	_profileToken(profileToken)
{
}


GetCompatibleAudioOutputConfigurations::~GetCompatibleAudioOutputConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleAudioOutputConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetCompatibleAudioOutputConfigurationsResponse::GetCompatibleAudioOutputConfigurationsResponse():
	_configurations()
{
}


GetCompatibleAudioOutputConfigurationsResponse::GetCompatibleAudioOutputConfigurationsResponse(const std::vector<ONVIF::AudioOutputConfiguration>& configurations):
	_configurations(configurations)
{
}


GetCompatibleAudioOutputConfigurationsResponse::~GetCompatibleAudioOutputConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleAudioSourceConfigurations.h"


namespace ONVIF {
namespace Media {


GetCompatibleAudioSourceConfigurations::GetCompatibleAudioSourceConfigurations():
	_profileToken()
{
}


GetCompatibleAudioSourceConfigurations::GetCompatibleAudioSourceConfigurations(const std::string& profileToken):
	_profileToken(profileToken)
{
}


GetCompatibleAudioSourceConfigurations::~GetCompatibleAudioSourceConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleAudioSourceConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetCompatibleAudioSourceConfigurationsResponse::GetCompatibleAudioSourceConfigurationsResponse():
	_configurations()
{
}


GetCompatibleAudioSourceConfigurationsResponse::GetCompatibleAudioSourceConfigurationsResponse(const std::vector<ONVIF::AudioSourceConfiguration>& configurations):
	_configurations(configurations)
{
}


GetCompatibleAudioSourceConfigurationsResponse::~GetCompatibleAudioSourceConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleMetadataConfigurations.h"


namespace ONVIF {
namespace Media {


GetCompatibleMetadataConfigurations::GetCompatibleMetadataConfigurations():
	_profileToken()
{
}


GetCompatibleMetadataConfigurations::GetCompatibleMetadataConfigurations(const std::string& profileToken):
	_profileToken(profileToken)
{
}


GetCompatibleMetadataConfigurations::~GetCompatibleMetadataConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleMetadataConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetCompatibleMetadataConfigurationsResponse::GetCompatibleMetadataConfigurationsResponse():
	_configurations()
{
}


GetCompatibleMetadataConfigurationsResponse::GetCompatibleMetadataConfigurationsResponse(const std::vector<ONVIF::MetadataConfiguration>& configurations):
	_configurations(configurations)
{
}


GetCompatibleMetadataConfigurationsResponse::~GetCompatibleMetadataConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleVideoAnalyticsConfigurations.h"


namespace ONVIF {
namespace Media {


GetCompatibleVideoAnalyticsConfigurations::GetCompatibleVideoAnalyticsConfigurations():
	_profileToken()
{
}


GetCompatibleVideoAnalyticsConfigurations::GetCompatibleVideoAnalyticsConfigurations(const std::string& profileToken):
	_profileToken(profileToken)
{
}


GetCompatibleVideoAnalyticsConfigurations::~GetCompatibleVideoAnalyticsConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleVideoAnalyticsConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetCompatibleVideoAnalyticsConfigurationsResponse::GetCompatibleVideoAnalyticsConfigurationsResponse():
	_configurations()
{
}


GetCompatibleVideoAnalyticsConfigurationsResponse::GetCompatibleVideoAnalyticsConfigurationsResponse(const std::vector<ONVIF::VideoAnalyticsConfiguration>& configurations):
	_configurations(configurations)
{
}


GetCompatibleVideoAnalyticsConfigurationsResponse::~GetCompatibleVideoAnalyticsConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleVideoEncoderConfigurations.h"


namespace ONVIF {
namespace Media {


GetCompatibleVideoEncoderConfigurations::GetCompatibleVideoEncoderConfigurations():
	_profileToken()
{
}


GetCompatibleVideoEncoderConfigurations::GetCompatibleVideoEncoderConfigurations(const std::string& profileToken):
	_profileToken(profileToken)
{
}


GetCompatibleVideoEncoderConfigurations::~GetCompatibleVideoEncoderConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleVideoEncoderConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetCompatibleVideoEncoderConfigurationsResponse::GetCompatibleVideoEncoderConfigurationsResponse():
	_configurations()
{
}


GetCompatibleVideoEncoderConfigurationsResponse::GetCompatibleVideoEncoderConfigurationsResponse(const std::vector<ONVIF::VideoEncoderConfiguration>& configurations):
	_configurations(configurations)
{
}


GetCompatibleVideoEncoderConfigurationsResponse::~GetCompatibleVideoEncoderConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleVideoSourceConfigurations.h"


namespace ONVIF {
namespace Media {


GetCompatibleVideoSourceConfigurations::GetCompatibleVideoSourceConfigurations():
	_profileToken()
{
}


GetCompatibleVideoSourceConfigurations::GetCompatibleVideoSourceConfigurations(const std::string& profileToken):
	_profileToken(profileToken)
{
}


GetCompatibleVideoSourceConfigurations::~GetCompatibleVideoSourceConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetCompatibleVideoSourceConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetCompatibleVideoSourceConfigurationsResponse::GetCompatibleVideoSourceConfigurationsResponse():
	_configurations()
{
}


GetCompatibleVideoSourceConfigurationsResponse::GetCompatibleVideoSourceConfigurationsResponse(const std::vector<ONVIF::VideoSourceConfiguration>& configurations):
	_configurations(configurations)
{
}


GetCompatibleVideoSourceConfigurationsResponse::~GetCompatibleVideoSourceConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetGuaranteedNumberOfVideoEncoderInstances.h"


namespace ONVIF {
namespace Media {


GetGuaranteedNumberOfVideoEncoderInstances::GetGuaranteedNumberOfVideoEncoderInstances():
	_configurationToken()
{
}


GetGuaranteedNumberOfVideoEncoderInstances::GetGuaranteedNumberOfVideoEncoderInstances(const std::string& configurationToken):
	_configurationToken(configurationToken)
{
}


GetGuaranteedNumberOfVideoEncoderInstances::~GetGuaranteedNumberOfVideoEncoderInstances()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetGuaranteedNumberOfVideoEncoderInstancesResponse.h"


namespace ONVIF {
namespace Media {


GetGuaranteedNumberOfVideoEncoderInstancesResponse::GetGuaranteedNumberOfVideoEncoderInstancesResponse():
	_totalNumber(),
	_jPEG(),
	_h264(),
	_mPEG4()
{
}


GetGuaranteedNumberOfVideoEncoderInstancesResponse::GetGuaranteedNumberOfVideoEncoderInstancesResponse(int totalNumber, Poco::Optional<int> jPEG, Poco::Optional<int> h264, Poco::Optional<int> mPEG4):
	_totalNumber(totalNumber),
	_jPEG(jPEG),
	_h264(h264),
	_mPEG4(mPEG4)
{
}


GetGuaranteedNumberOfVideoEncoderInstancesResponse::~GetGuaranteedNumberOfVideoEncoderInstancesResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetMetadataConfiguration.h"


namespace ONVIF {
namespace Media {


GetMetadataConfiguration::GetMetadataConfiguration():
	_configurationToken()
{
}


GetMetadataConfiguration::GetMetadataConfiguration(const std::string& configurationToken):
	_configurationToken(configurationToken)
{
}


GetMetadataConfiguration::~GetMetadataConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetMetadataConfigurationOptions.h"


namespace ONVIF {
namespace Media {


GetMetadataConfigurationOptions::GetMetadataConfigurationOptions():
	_configurationToken(),
	_profileToken()
{
}


GetMetadataConfigurationOptions::GetMetadataConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken):
	_configurationToken(configurationToken),
	_profileToken(profileToken)
{
}


GetMetadataConfigurationOptions::~GetMetadataConfigurationOptions()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetMetadataConfigurationOptionsResponse.h"


namespace ONVIF {
namespace Media {


GetMetadataConfigurationOptionsResponse::GetMetadataConfigurationOptionsResponse():
	_options()
{
}


GetMetadataConfigurationOptionsResponse::GetMetadataConfigurationOptionsResponse(const ONVIF::MetadataConfigurationOptions& options):
	_options(options)
{
}


GetMetadataConfigurationOptionsResponse::~GetMetadataConfigurationOptionsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetMetadataConfigurationResponse.h"


namespace ONVIF {
namespace Media {


GetMetadataConfigurationResponse::GetMetadataConfigurationResponse():
	_configuration()
{
}


GetMetadataConfigurationResponse::GetMetadataConfigurationResponse(const ONVIF::MetadataConfiguration& configuration):
	_configuration(configuration)
{
}


GetMetadataConfigurationResponse::~GetMetadataConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetMetadataConfigurations.h"


namespace ONVIF {
namespace Media {


GetMetadataConfigurations::GetMetadataConfigurations()
{
}


GetMetadataConfigurations::~GetMetadataConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetMetadataConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetMetadataConfigurationsResponse::GetMetadataConfigurationsResponse():
	_configurations()
{
}


GetMetadataConfigurationsResponse::GetMetadataConfigurationsResponse(const std::vector<ONVIF::MetadataConfiguration>& configurations):
	_configurations(configurations)
{
}


GetMetadataConfigurationsResponse::~GetMetadataConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetOSD.h"


namespace ONVIF {
namespace Media {


GetOSD::GetOSD():
	_oSDToken()
{
}


GetOSD::GetOSD(const std::string& oSDToken):
	_oSDToken(oSDToken)
{
}


GetOSD::~GetOSD()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetOSDOptions.h"


namespace ONVIF {
namespace Media {


GetOSDOptions::GetOSDOptions():
	_configurationToken()
{
}


GetOSDOptions::GetOSDOptions(const std::string& configurationToken):
	_configurationToken(configurationToken)
{
}


GetOSDOptions::~GetOSDOptions()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetOSDOptionsResponse.h"


namespace ONVIF {
namespace Media {


GetOSDOptionsResponse::GetOSDOptionsResponse():
	_oSDOptions()
{
}


GetOSDOptionsResponse::GetOSDOptionsResponse(const ONVIF::OSDConfigurationOptions& oSDOptions):
	_oSDOptions(oSDOptions)
{
}


GetOSDOptionsResponse::~GetOSDOptionsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetOSDResponse.h"


namespace ONVIF {
namespace Media {


GetOSDResponse::GetOSDResponse():
	_oSD()
{
}


GetOSDResponse::GetOSDResponse(const ONVIF::OSDConfiguration& oSD):
	_oSD(oSD)
{
}


GetOSDResponse::~GetOSDResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetOSDs.h"


namespace ONVIF {
namespace Media {


GetOSDs::GetOSDs():
	_configurationToken()
{
}


GetOSDs::GetOSDs(const Poco::Optional<std::string>& configurationToken):
	_configurationToken(configurationToken)
{
}


GetOSDs::~GetOSDs()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetOSDsResponse.h"


namespace ONVIF {
namespace Media {


GetOSDsResponse::GetOSDsResponse():
	_oSDs()
{
}


GetOSDsResponse::GetOSDsResponse(const std::vector<ONVIF::OSDConfiguration>& oSDs):
	_oSDs(oSDs)
{
}


GetOSDsResponse::~GetOSDsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetProfile.h"


namespace ONVIF {
namespace Media {


GetProfile::GetProfile():
	_profileToken()
{
}


GetProfile::GetProfile(const std::string& profileToken):
	_profileToken(profileToken)
{
}


GetProfile::~GetProfile()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetProfileResponse.h"


namespace ONVIF {
namespace Media {


GetProfileResponse::GetProfileResponse():
	_profile()
{
}


GetProfileResponse::GetProfileResponse(const ONVIF::Profile& profile):
	_profile(profile)
{
}


GetProfileResponse::~GetProfileResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetProfiles.h"


namespace ONVIF {
namespace Media {


GetProfiles::GetProfiles()
{
}


GetProfiles::~GetProfiles()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetProfilesResponse.h"


namespace ONVIF {
namespace Media {


GetProfilesResponse::GetProfilesResponse():
	_profiles()
{
}


GetProfilesResponse::GetProfilesResponse(const std::vector<ONVIF::Profile>& profiles):
	_profiles(profiles)
{
}


GetProfilesResponse::~GetProfilesResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetServiceCapabilities.h"


namespace ONVIF {
namespace Media {


GetServiceCapabilities::GetServiceCapabilities()
{
}


GetServiceCapabilities::~GetServiceCapabilities()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetServiceCapabilitiesResponse.h"


namespace ONVIF {
namespace Media {


GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse():
	_capabilities()
{
}


GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse(const Capabilities& capabilities):
	_capabilities(capabilities)
{
}


GetServiceCapabilitiesResponse::~GetServiceCapabilitiesResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetSnapshotUri.h"


namespace ONVIF {
namespace Media {


GetSnapshotUri::GetSnapshotUri():
	_profileToken()
{
}


GetSnapshotUri::GetSnapshotUri(const std::string& profileToken):
	_profileToken(profileToken)
{
}


GetSnapshotUri::~GetSnapshotUri()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetSnapshotUriResponse.h"


namespace ONVIF {
namespace Media {


GetSnapshotUriResponse::GetSnapshotUriResponse():
	_mediaUri()
{
}


GetSnapshotUriResponse::GetSnapshotUriResponse(const ONVIF::MediaUri& mediaUri):
	_mediaUri(mediaUri)
{
}


GetSnapshotUriResponse::~GetSnapshotUriResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetStreamUri.h"


namespace ONVIF {
namespace Media {


GetStreamUri::GetStreamUri():
	_streamSetup(),
	_profileToken()
{
}


GetStreamUri::GetStreamUri(const ONVIF::StreamSetup& streamSetup, const std::string& profileToken):
	_streamSetup(streamSetup),
	_profileToken(profileToken)
{
}


GetStreamUri::~GetStreamUri()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetStreamUriResponse.h"


namespace ONVIF {
namespace Media {


GetStreamUriResponse::GetStreamUriResponse():
	_mediaUri()
{
}


GetStreamUriResponse::GetStreamUriResponse(const ONVIF::MediaUri& mediaUri):
	_mediaUri(mediaUri)
{
}


GetStreamUriResponse::~GetStreamUriResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoAnalyticsConfiguration.h"


namespace ONVIF {
namespace Media {


GetVideoAnalyticsConfiguration::GetVideoAnalyticsConfiguration():
	_configurationToken()
{
}


GetVideoAnalyticsConfiguration::GetVideoAnalyticsConfiguration(const std::string& configurationToken):
	_configurationToken(configurationToken)
{
}


GetVideoAnalyticsConfiguration::~GetVideoAnalyticsConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoAnalyticsConfigurationResponse.h"


namespace ONVIF {
namespace Media {


GetVideoAnalyticsConfigurationResponse::GetVideoAnalyticsConfigurationResponse():
	_configuration()
{
}


GetVideoAnalyticsConfigurationResponse::GetVideoAnalyticsConfigurationResponse(const ONVIF::VideoAnalyticsConfiguration& configuration):
	_configuration(configuration)
{
}


GetVideoAnalyticsConfigurationResponse::~GetVideoAnalyticsConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoAnalyticsConfigurations.h"


namespace ONVIF {
namespace Media {


GetVideoAnalyticsConfigurations::GetVideoAnalyticsConfigurations()
{
}


GetVideoAnalyticsConfigurations::~GetVideoAnalyticsConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoAnalyticsConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetVideoAnalyticsConfigurationsResponse::GetVideoAnalyticsConfigurationsResponse():
	_configurations()
{
}


GetVideoAnalyticsConfigurationsResponse::GetVideoAnalyticsConfigurationsResponse(const std::vector<ONVIF::VideoAnalyticsConfiguration>& configurations):
	_configurations(configurations)
{
}


GetVideoAnalyticsConfigurationsResponse::~GetVideoAnalyticsConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoEncoderConfiguration.h"


namespace ONVIF {
namespace Media {


GetVideoEncoderConfiguration::GetVideoEncoderConfiguration():
	_configurationToken()
{
}


GetVideoEncoderConfiguration::GetVideoEncoderConfiguration(const std::string& configurationToken):
	_configurationToken(configurationToken)
{
}


GetVideoEncoderConfiguration::~GetVideoEncoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoEncoderConfigurationOptions.h"


namespace ONVIF {
namespace Media {


GetVideoEncoderConfigurationOptions::GetVideoEncoderConfigurationOptions():
	_configurationToken(),
	_profileToken()
{
}


GetVideoEncoderConfigurationOptions::GetVideoEncoderConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken):
	_configurationToken(configurationToken),
	_profileToken(profileToken)
{
}


GetVideoEncoderConfigurationOptions::~GetVideoEncoderConfigurationOptions()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoEncoderConfigurationOptionsResponse.h"


namespace ONVIF {
namespace Media {


GetVideoEncoderConfigurationOptionsResponse::GetVideoEncoderConfigurationOptionsResponse():
	_options()
{
}


GetVideoEncoderConfigurationOptionsResponse::GetVideoEncoderConfigurationOptionsResponse(const ONVIF::VideoEncoderConfigurationOptions& options):
	_options(options)
{
}


GetVideoEncoderConfigurationOptionsResponse::~GetVideoEncoderConfigurationOptionsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoEncoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


GetVideoEncoderConfigurationResponse::GetVideoEncoderConfigurationResponse():
	_configuration()
{
}


GetVideoEncoderConfigurationResponse::GetVideoEncoderConfigurationResponse(const ONVIF::VideoEncoderConfiguration& configuration):
	_configuration(configuration)
{
}


GetVideoEncoderConfigurationResponse::~GetVideoEncoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoEncoderConfigurations.h"


namespace ONVIF {
namespace Media {


GetVideoEncoderConfigurations::GetVideoEncoderConfigurations()
{
}


GetVideoEncoderConfigurations::~GetVideoEncoderConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoEncoderConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetVideoEncoderConfigurationsResponse::GetVideoEncoderConfigurationsResponse():
	_configurations()
{
}


GetVideoEncoderConfigurationsResponse::GetVideoEncoderConfigurationsResponse(const std::vector<ONVIF::VideoEncoderConfiguration>& configurations):
	_configurations(configurations)
{
}


GetVideoEncoderConfigurationsResponse::~GetVideoEncoderConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoSourceConfiguration.h"


namespace ONVIF {
namespace Media {


GetVideoSourceConfiguration::GetVideoSourceConfiguration():
	_configurationToken()
{
}


GetVideoSourceConfiguration::GetVideoSourceConfiguration(const std::string& configurationToken):
	_configurationToken(configurationToken)
{
}


GetVideoSourceConfiguration::~GetVideoSourceConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoSourceConfigurationOptions.h"


namespace ONVIF {
namespace Media {


GetVideoSourceConfigurationOptions::GetVideoSourceConfigurationOptions():
	_configurationToken(),
	_profileToken()
{
}


GetVideoSourceConfigurationOptions::GetVideoSourceConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken):
	_configurationToken(configurationToken),
	_profileToken(profileToken)
{
}


GetVideoSourceConfigurationOptions::~GetVideoSourceConfigurationOptions()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoSourceConfigurationOptionsResponse.h"


namespace ONVIF {
namespace Media {


GetVideoSourceConfigurationOptionsResponse::GetVideoSourceConfigurationOptionsResponse():
	_options()
{
}


GetVideoSourceConfigurationOptionsResponse::GetVideoSourceConfigurationOptionsResponse(const ONVIF::VideoSourceConfigurationOptions& options):
	_options(options)
{
}


GetVideoSourceConfigurationOptionsResponse::~GetVideoSourceConfigurationOptionsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoSourceConfigurationResponse.h"


namespace ONVIF {
namespace Media {


GetVideoSourceConfigurationResponse::GetVideoSourceConfigurationResponse():
	_configuration()
{
}


GetVideoSourceConfigurationResponse::GetVideoSourceConfigurationResponse(const ONVIF::VideoSourceConfiguration& configuration):
	_configuration(configuration)
{
}


GetVideoSourceConfigurationResponse::~GetVideoSourceConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoSourceConfigurations.h"


namespace ONVIF {
namespace Media {


GetVideoSourceConfigurations::GetVideoSourceConfigurations()
{
}


GetVideoSourceConfigurations::~GetVideoSourceConfigurations()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoSourceConfigurationsResponse.h"


namespace ONVIF {
namespace Media {


GetVideoSourceConfigurationsResponse::GetVideoSourceConfigurationsResponse():
	_configurations()
{
}


GetVideoSourceConfigurationsResponse::GetVideoSourceConfigurationsResponse(const std::vector<ONVIF::VideoSourceConfiguration>& configurations):
	_configurations(configurations)
{
}


GetVideoSourceConfigurationsResponse::~GetVideoSourceConfigurationsResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoSourceModes.h"


namespace ONVIF {
namespace Media {


GetVideoSourceModes::GetVideoSourceModes():
	_videoSourceToken()
{
}


GetVideoSourceModes::GetVideoSourceModes(const std::string& videoSourceToken):
	_videoSourceToken(videoSourceToken)
{
}


GetVideoSourceModes::~GetVideoSourceModes()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoSourceModesResponse.h"


namespace ONVIF {
namespace Media {


GetVideoSourceModesResponse::GetVideoSourceModesResponse():
	_videoSourceModes()
{
}


GetVideoSourceModesResponse::GetVideoSourceModesResponse(const std::vector<VideoSourceMode>& videoSourceModes):
	_videoSourceModes(videoSourceModes)
{
}


GetVideoSourceModesResponse::~GetVideoSourceModesResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoSources.h"


namespace ONVIF {
namespace Media {


GetVideoSources::GetVideoSources()
{
}


GetVideoSources::~GetVideoSources()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/GetVideoSourcesResponse.h"


namespace ONVIF {
namespace Media {


GetVideoSourcesResponse::GetVideoSourcesResponse():
	_videoSources()
{
}


GetVideoSourcesResponse::GetVideoSourcesResponse(const std::vector<ONVIF::VideoSource>& videoSources):
	_videoSources(videoSources)
{
}


GetVideoSourcesResponse::~GetVideoSourcesResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/MediaBinding.h"


namespace ONVIF {
namespace Media {


MediaBinding::~MediaBinding()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/ProfileCapabilities.h"


namespace ONVIF {
namespace Media {


ProfileCapabilities::ProfileCapabilities():
	_maximumNumberOfProfiles()
{
}


ProfileCapabilities::ProfileCapabilities(Poco::Optional<int> maximumNumberOfProfiles):
	_maximumNumberOfProfiles(maximumNumberOfProfiles)
{
}


ProfileCapabilities::~ProfileCapabilities()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveAudioDecoderConfiguration.h"


namespace ONVIF {
namespace Media {


RemoveAudioDecoderConfiguration::RemoveAudioDecoderConfiguration():
	_profileToken()
{
}


RemoveAudioDecoderConfiguration::RemoveAudioDecoderConfiguration(const std::string& profileToken):
	_profileToken(profileToken)
{
}


RemoveAudioDecoderConfiguration::~RemoveAudioDecoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveAudioDecoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


RemoveAudioDecoderConfigurationResponse::RemoveAudioDecoderConfigurationResponse()
{
}


RemoveAudioDecoderConfigurationResponse::~RemoveAudioDecoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveAudioEncoderConfiguration.h"


namespace ONVIF {
namespace Media {


RemoveAudioEncoderConfiguration::RemoveAudioEncoderConfiguration():
	_profileToken()
{
}


RemoveAudioEncoderConfiguration::RemoveAudioEncoderConfiguration(const std::string& profileToken):
	_profileToken(profileToken)
{
}


RemoveAudioEncoderConfiguration::~RemoveAudioEncoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveAudioEncoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


RemoveAudioEncoderConfigurationResponse::RemoveAudioEncoderConfigurationResponse()
{
}


RemoveAudioEncoderConfigurationResponse::~RemoveAudioEncoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveAudioOutputConfiguration.h"


namespace ONVIF {
namespace Media {


RemoveAudioOutputConfiguration::RemoveAudioOutputConfiguration():
	_profileToken()
{
}


RemoveAudioOutputConfiguration::RemoveAudioOutputConfiguration(const std::string& profileToken):
	_profileToken(profileToken)
{
}


RemoveAudioOutputConfiguration::~RemoveAudioOutputConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveAudioOutputConfigurationResponse.h"


namespace ONVIF {
namespace Media {


RemoveAudioOutputConfigurationResponse::RemoveAudioOutputConfigurationResponse()
{
}


RemoveAudioOutputConfigurationResponse::~RemoveAudioOutputConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveAudioSourceConfiguration.h"


namespace ONVIF {
namespace Media {


RemoveAudioSourceConfiguration::RemoveAudioSourceConfiguration():
	_profileToken()
{
}


RemoveAudioSourceConfiguration::RemoveAudioSourceConfiguration(const std::string& profileToken):
	_profileToken(profileToken)
{
}


RemoveAudioSourceConfiguration::~RemoveAudioSourceConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveAudioSourceConfigurationResponse.h"


namespace ONVIF {
namespace Media {


RemoveAudioSourceConfigurationResponse::RemoveAudioSourceConfigurationResponse()
{
}


RemoveAudioSourceConfigurationResponse::~RemoveAudioSourceConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveMetadataConfiguration.h"


namespace ONVIF {
namespace Media {


RemoveMetadataConfiguration::RemoveMetadataConfiguration():
	_profileToken()
{
}


RemoveMetadataConfiguration::RemoveMetadataConfiguration(const std::string& profileToken):
	_profileToken(profileToken)
{
}


RemoveMetadataConfiguration::~RemoveMetadataConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveMetadataConfigurationResponse.h"


namespace ONVIF {
namespace Media {


RemoveMetadataConfigurationResponse::RemoveMetadataConfigurationResponse()
{
}


RemoveMetadataConfigurationResponse::~RemoveMetadataConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemovePTZConfiguration.h"


namespace ONVIF {
namespace Media {


RemovePTZConfiguration::RemovePTZConfiguration():
	_profileToken()
{
}


RemovePTZConfiguration::RemovePTZConfiguration(const std::string& profileToken):
	_profileToken(profileToken)
{
}


RemovePTZConfiguration::~RemovePTZConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemovePTZConfigurationResponse.h"


namespace ONVIF {
namespace Media {


RemovePTZConfigurationResponse::RemovePTZConfigurationResponse()
{
}


RemovePTZConfigurationResponse::~RemovePTZConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveVideoAnalyticsConfiguration.h"


namespace ONVIF {
namespace Media {


RemoveVideoAnalyticsConfiguration::RemoveVideoAnalyticsConfiguration():
	_profileToken()
{
}


RemoveVideoAnalyticsConfiguration::RemoveVideoAnalyticsConfiguration(const std::string& profileToken):
	_profileToken(profileToken)
{
}


RemoveVideoAnalyticsConfiguration::~RemoveVideoAnalyticsConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveVideoAnalyticsConfigurationResponse.h"


namespace ONVIF {
namespace Media {


RemoveVideoAnalyticsConfigurationResponse::RemoveVideoAnalyticsConfigurationResponse()
{
}


RemoveVideoAnalyticsConfigurationResponse::~RemoveVideoAnalyticsConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveVideoEncoderConfiguration.h"


namespace ONVIF {
namespace Media {


RemoveVideoEncoderConfiguration::RemoveVideoEncoderConfiguration():
	_profileToken()
{
}


RemoveVideoEncoderConfiguration::RemoveVideoEncoderConfiguration(const std::string& profileToken):
	_profileToken(profileToken)
{
}


RemoveVideoEncoderConfiguration::~RemoveVideoEncoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveVideoEncoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


RemoveVideoEncoderConfigurationResponse::RemoveVideoEncoderConfigurationResponse()
{
}


RemoveVideoEncoderConfigurationResponse::~RemoveVideoEncoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveVideoSourceConfiguration.h"


namespace ONVIF {
namespace Media {


RemoveVideoSourceConfiguration::RemoveVideoSourceConfiguration():
	_profileToken()
{
}


RemoveVideoSourceConfiguration::RemoveVideoSourceConfiguration(const std::string& profileToken):
	_profileToken(profileToken)
{
}


RemoveVideoSourceConfiguration::~RemoveVideoSourceConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/RemoveVideoSourceConfigurationResponse.h"


namespace ONVIF {
namespace Media {


RemoveVideoSourceConfigurationResponse::RemoveVideoSourceConfigurationResponse()
{
}


RemoveVideoSourceConfigurationResponse::~RemoveVideoSourceConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetAudioDecoderConfiguration.h"


namespace ONVIF {
namespace Media {


SetAudioDecoderConfiguration::SetAudioDecoderConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetAudioDecoderConfiguration::SetAudioDecoderConfiguration(const ONVIF::AudioDecoderConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetAudioDecoderConfiguration::~SetAudioDecoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetAudioDecoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


SetAudioDecoderConfigurationResponse::SetAudioDecoderConfigurationResponse()
{
}


SetAudioDecoderConfigurationResponse::~SetAudioDecoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetAudioEncoderConfiguration.h"


namespace ONVIF {
namespace Media {


SetAudioEncoderConfiguration::SetAudioEncoderConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetAudioEncoderConfiguration::SetAudioEncoderConfiguration(const ONVIF::AudioEncoderConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetAudioEncoderConfiguration::~SetAudioEncoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetAudioEncoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


SetAudioEncoderConfigurationResponse::SetAudioEncoderConfigurationResponse()
{
}


SetAudioEncoderConfigurationResponse::~SetAudioEncoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetAudioOutputConfiguration.h"


namespace ONVIF {
namespace Media {


SetAudioOutputConfiguration::SetAudioOutputConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetAudioOutputConfiguration::SetAudioOutputConfiguration(const ONVIF::AudioOutputConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetAudioOutputConfiguration::~SetAudioOutputConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetAudioOutputConfigurationResponse.h"


namespace ONVIF {
namespace Media {


SetAudioOutputConfigurationResponse::SetAudioOutputConfigurationResponse()
{
}


SetAudioOutputConfigurationResponse::~SetAudioOutputConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetAudioSourceConfiguration.h"


namespace ONVIF {
namespace Media {


SetAudioSourceConfiguration::SetAudioSourceConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetAudioSourceConfiguration::SetAudioSourceConfiguration(const ONVIF::AudioSourceConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetAudioSourceConfiguration::~SetAudioSourceConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetAudioSourceConfigurationResponse.h"


namespace ONVIF {
namespace Media {


SetAudioSourceConfigurationResponse::SetAudioSourceConfigurationResponse()
{
}


SetAudioSourceConfigurationResponse::~SetAudioSourceConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetMetadataConfiguration.h"


namespace ONVIF {
namespace Media {


SetMetadataConfiguration::SetMetadataConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetMetadataConfiguration::SetMetadataConfiguration(const ONVIF::MetadataConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetMetadataConfiguration::~SetMetadataConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetMetadataConfigurationResponse.h"


namespace ONVIF {
namespace Media {


SetMetadataConfigurationResponse::SetMetadataConfigurationResponse()
{
}


SetMetadataConfigurationResponse::~SetMetadataConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetOSD.h"


namespace ONVIF {
namespace Media {


SetOSD::SetOSD():
	_oSD()
{
}


SetOSD::SetOSD(const ONVIF::OSDConfiguration& oSD):
	_oSD(oSD)
{
}


SetOSD::~SetOSD()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetOSDResponse.h"


namespace ONVIF {
namespace Media {


SetOSDResponse::SetOSDResponse()
{
}


SetOSDResponse::~SetOSDResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetSynchronizationPoint.h"


namespace ONVIF {
namespace Media {


SetSynchronizationPoint::SetSynchronizationPoint():
	_profileToken()
{
}


SetSynchronizationPoint::SetSynchronizationPoint(const std::string& profileToken):
	_profileToken(profileToken)
{
}


SetSynchronizationPoint::~SetSynchronizationPoint()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetSynchronizationPointResponse.h"


namespace ONVIF {
namespace Media {


SetSynchronizationPointResponse::SetSynchronizationPointResponse()
{
}


SetSynchronizationPointResponse::~SetSynchronizationPointResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetVideoAnalyticsConfiguration.h"


namespace ONVIF {
namespace Media {


SetVideoAnalyticsConfiguration::SetVideoAnalyticsConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetVideoAnalyticsConfiguration::SetVideoAnalyticsConfiguration(const ONVIF::VideoAnalyticsConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetVideoAnalyticsConfiguration::~SetVideoAnalyticsConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetVideoAnalyticsConfigurationResponse.h"


namespace ONVIF {
namespace Media {


SetVideoAnalyticsConfigurationResponse::SetVideoAnalyticsConfigurationResponse()
{
}


SetVideoAnalyticsConfigurationResponse::~SetVideoAnalyticsConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetVideoEncoderConfiguration.h"


namespace ONVIF {
namespace Media {


SetVideoEncoderConfiguration::SetVideoEncoderConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetVideoEncoderConfiguration::SetVideoEncoderConfiguration(const ONVIF::VideoEncoderConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetVideoEncoderConfiguration::~SetVideoEncoderConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetVideoEncoderConfigurationResponse.h"


namespace ONVIF {
namespace Media {


SetVideoEncoderConfigurationResponse::SetVideoEncoderConfigurationResponse()
{
}


SetVideoEncoderConfigurationResponse::~SetVideoEncoderConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetVideoSourceConfiguration.h"


namespace ONVIF {
namespace Media {


SetVideoSourceConfiguration::SetVideoSourceConfiguration():
	_configuration(),
	_forcePersistence()
{
}


SetVideoSourceConfiguration::SetVideoSourceConfiguration(const ONVIF::VideoSourceConfiguration& configuration, bool forcePersistence):
	_configuration(configuration),
	_forcePersistence(forcePersistence)
{
}


SetVideoSourceConfiguration::~SetVideoSourceConfiguration()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetVideoSourceConfigurationResponse.h"


namespace ONVIF {
namespace Media {


SetVideoSourceConfigurationResponse::SetVideoSourceConfigurationResponse()
{
}


SetVideoSourceConfigurationResponse::~SetVideoSourceConfigurationResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetVideoSourceMode.h"


namespace ONVIF {
namespace Media {


SetVideoSourceMode::SetVideoSourceMode():
	_videoSourceToken(),
	_videoSourceModeToken()
{
}


SetVideoSourceMode::SetVideoSourceMode(const std::string& videoSourceToken, const std::string& videoSourceModeToken):
	_videoSourceToken(videoSourceToken),
	_videoSourceModeToken(videoSourceModeToken)
{
}


SetVideoSourceMode::~SetVideoSourceMode()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/SetVideoSourceModeResponse.h"


namespace ONVIF {
namespace Media {


SetVideoSourceModeResponse::SetVideoSourceModeResponse():
	_reboot()
{
}


SetVideoSourceModeResponse::SetVideoSourceModeResponse(bool reboot):
	_reboot(reboot)
{
}


SetVideoSourceModeResponse::~SetVideoSourceModeResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/StartMulticastStreaming.h"


namespace ONVIF {
namespace Media {


StartMulticastStreaming::StartMulticastStreaming():
	_profileToken()
{
}


StartMulticastStreaming::StartMulticastStreaming(const std::string& profileToken):
	_profileToken(profileToken)
{
}


StartMulticastStreaming::~StartMulticastStreaming()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/StartMulticastStreamingResponse.h"


namespace ONVIF {
namespace Media {


StartMulticastStreamingResponse::StartMulticastStreamingResponse()
{
}


StartMulticastStreamingResponse::~StartMulticastStreamingResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/StopMulticastStreaming.h"


namespace ONVIF {
namespace Media {


StopMulticastStreaming::StopMulticastStreaming():
	_profileToken()
{
}


StopMulticastStreaming::StopMulticastStreaming(const std::string& profileToken):
	_profileToken(profileToken)
{
}


StopMulticastStreaming::~StopMulticastStreaming()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/StopMulticastStreamingResponse.h"


namespace ONVIF {
namespace Media {


StopMulticastStreamingResponse::StopMulticastStreamingResponse()
{
}


StopMulticastStreamingResponse::~StopMulticastStreamingResponse()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/StreamingCapabilities.h"


namespace ONVIF {
namespace Media {


StreamingCapabilities::StreamingCapabilities():
	_rTPMulticast(),
	_rTP_TCP(),
	_rTP_RTSP_TCP(),
	_nonAggregateControl(),
	_noRTSPStreaming()
{
}


StreamingCapabilities::StreamingCapabilities(Poco::Optional<bool> rTPMulticast, Poco::Optional<bool> rTP_TCP, Poco::Optional<bool> rTP_RTSP_TCP, Poco::Optional<bool> nonAggregateControl, Poco::Optional<bool> noRTSPStreaming):
	_rTPMulticast(rTPMulticast),
	_rTP_TCP(rTP_TCP),
	_rTP_RTSP_TCP(rTP_RTSP_TCP),
	_nonAggregateControl(nonAggregateControl),
	_noRTSPStreaming(noRTSPStreaming)
{
}


StreamingCapabilities::~StreamingCapabilities()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/VideoSourceMode.h"


namespace ONVIF {
namespace Media {


VideoSourceMode::VideoSourceMode():
	_token(),
	_enabled(),
	_maxFramerate(),
	_maxResolution(),
	_encodings(),
	_reboot(),
	_description(),
	_extension()
{
}


VideoSourceMode::VideoSourceMode(const std::string& token, Poco::Optional<bool> enabled, float maxFramerate, const ONVIF::VideoResolution& maxResolution, const std::vector<std::string>& encodings, bool reboot, const Poco::Optional<std::string>& description, const Poco::SharedPtr<VideoSourceModeExtension>& extension):
	_token(token),
	_enabled(enabled),
	_maxFramerate(maxFramerate),
	_maxResolution(maxResolution),
	_encodings(encodings),
	_reboot(reboot),
	_description(description),
	_extension(extension)
{
}


VideoSourceMode::~VideoSourceMode()
{
}


} } // ONVIF::Media


#include "ONVIF/Media/VideoSourceModeExtension.h"


namespace ONVIF {
namespace Media {


VideoSourceModeExtension::VideoSourceModeExtension()
{
}


VideoSourceModeExtension::~VideoSourceModeExtension()
{
}


} } // ONVIF::Media


#include "ONVIF/AACDecOptions.h"


namespace ONVIF {


AACDecOptions::AACDecOptions():
	_bitrate(),
	_sampleRateRange()
{
}


AACDecOptions::AACDecOptions(const IntItems& bitrate, const IntItems& sampleRateRange):
	_bitrate(bitrate),
	_sampleRateRange(sampleRateRange)
{
}


AACDecOptions::~AACDecOptions()
{
}


} // ONVIF


#include "ONVIF/AbsoluteFocus.h"


namespace ONVIF {


AbsoluteFocus::AbsoluteFocus():
	_position(),
	_speed()
{
}


AbsoluteFocus::AbsoluteFocus(float position, Poco::Optional<float> speed):
	_position(position),
	_speed(speed)
{
}


AbsoluteFocus::~AbsoluteFocus()
{
}


} // ONVIF


#include "ONVIF/AbsoluteFocusOptions.h"


namespace ONVIF {


AbsoluteFocusOptions::AbsoluteFocusOptions():
	_position(),
	_speed()
{
}


AbsoluteFocusOptions::AbsoluteFocusOptions(const FloatRange& position, const Poco::SharedPtr<FloatRange>& speed):
	_position(position),
	_speed(speed)
{
}


AbsoluteFocusOptions::~AbsoluteFocusOptions()
{
}


} // ONVIF


#include "ONVIF/ActionEngineEventPayload.h"


namespace ONVIF {


ActionEngineEventPayload::ActionEngineEventPayload():
	_requestInfo(),
	_responseInfo(),
	_fault(),
	_extension()
{
}


ActionEngineEventPayload::ActionEngineEventPayload(const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& requestInfo, const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& responseInfo, const Poco::SharedPtr<W3::SOAP::Envelope::Fault>& fault, const Poco::SharedPtr<ActionEngineEventPayloadExtension>& extension):
	_requestInfo(requestInfo),
	_responseInfo(responseInfo),
	_fault(fault),
	_extension(extension)
{
}


ActionEngineEventPayload::~ActionEngineEventPayload()
{
}


} // ONVIF


#include "ONVIF/ActionEngineEventPayloadExtension.h"


namespace ONVIF {


ActionEngineEventPayloadExtension::ActionEngineEventPayloadExtension()
{
}


ActionEngineEventPayloadExtension::~ActionEngineEventPayloadExtension()
{
}


} // ONVIF


#include "ONVIF/ActiveConnection.h"


namespace ONVIF {


ActiveConnection::ActiveConnection():
	_currentBitrate(),
	_currentFps()
{
}


ActiveConnection::ActiveConnection(float currentBitrate, float currentFps):
	_currentBitrate(currentBitrate),
	_currentFps(currentFps)
{
}


ActiveConnection::~ActiveConnection()
{
}


} // ONVIF


#include "ONVIF/AnalyticsCapabilities.h"


namespace ONVIF {


AnalyticsCapabilities::AnalyticsCapabilities():
	_xAddr(),
	_ruleSupport(),
	_analyticsModuleSupport()
{
}


AnalyticsCapabilities::AnalyticsCapabilities(const Poco::URI& xAddr, bool ruleSupport, bool analyticsModuleSupport):
	_xAddr(xAddr),
	_ruleSupport(ruleSupport),
	_analyticsModuleSupport(analyticsModuleSupport)
{
}


AnalyticsCapabilities::~AnalyticsCapabilities()
{
}


} // ONVIF


#include "ONVIF/AnalyticsDeviceCapabilities.h"


namespace ONVIF {


AnalyticsDeviceCapabilities::AnalyticsDeviceCapabilities():
	_xAddr(),
	_ruleSupport(),
	_extension()
{
}


AnalyticsDeviceCapabilities::AnalyticsDeviceCapabilities(const Poco::URI& xAddr, Poco::Optional<bool> ruleSupport, const Poco::SharedPtr<AnalyticsDeviceExtension>& extension):
	_xAddr(xAddr),
	_ruleSupport(ruleSupport),
	_extension(extension)
{
}


AnalyticsDeviceCapabilities::~AnalyticsDeviceCapabilities()
{
}


} // ONVIF


#include "ONVIF/AnalyticsDeviceEngineConfiguration.h"


namespace ONVIF {


AnalyticsDeviceEngineConfiguration::AnalyticsDeviceEngineConfiguration():
	_engineConfiguration(),
	_extension()
{
}


AnalyticsDeviceEngineConfiguration::AnalyticsDeviceEngineConfiguration(const std::vector<EngineConfiguration>& engineConfiguration, const Poco::SharedPtr<AnalyticsDeviceEngineConfigurationExtension>& extension):
	_engineConfiguration(engineConfiguration),
	_extension(extension)
{
}


AnalyticsDeviceEngineConfiguration::~AnalyticsDeviceEngineConfiguration()
{
}


} // ONVIF


#include "ONVIF/AnalyticsDeviceEngineConfigurationExtension.h"


namespace ONVIF {


AnalyticsDeviceEngineConfigurationExtension::AnalyticsDeviceEngineConfigurationExtension()
{
}


AnalyticsDeviceEngineConfigurationExtension::~AnalyticsDeviceEngineConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/AnalyticsDeviceExtension.h"


namespace ONVIF {


AnalyticsDeviceExtension::AnalyticsDeviceExtension()
{
}


AnalyticsDeviceExtension::~AnalyticsDeviceExtension()
{
}


} // ONVIF


#include "ONVIF/AnalyticsEngine.h"


namespace ONVIF {


AnalyticsEngine::AnalyticsEngine():
	_analyticsEngineConfiguration()
{
}


AnalyticsEngine::AnalyticsEngine(const std::string& token, const std::string& name, int useCount, const AnalyticsDeviceEngineConfiguration& analyticsEngineConfiguration):
	ConfigurationEntity(token, name, useCount),
	_analyticsEngineConfiguration(analyticsEngineConfiguration)
{
}


AnalyticsEngine::~AnalyticsEngine()
{
}


} // ONVIF


#include "ONVIF/AnalyticsEngineConfiguration.h"


namespace ONVIF {


AnalyticsEngineConfiguration::AnalyticsEngineConfiguration():
	_analyticsModule(),
	_extension()
{
}


AnalyticsEngineConfiguration::AnalyticsEngineConfiguration(const std::vector<Config>& analyticsModule, const Poco::SharedPtr<AnalyticsEngineConfigurationExtension>& extension):
	_analyticsModule(analyticsModule),
	_extension(extension)
{
}


AnalyticsEngineConfiguration::~AnalyticsEngineConfiguration()
{
}


} // ONVIF


#include "ONVIF/AnalyticsEngineConfigurationExtension.h"


namespace ONVIF {


AnalyticsEngineConfigurationExtension::AnalyticsEngineConfigurationExtension()
{
}


AnalyticsEngineConfigurationExtension::~AnalyticsEngineConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/AnalyticsEngineControl.h"


namespace ONVIF {


AnalyticsEngineControl::AnalyticsEngineControl():
	_engineToken(),
	_engineConfigToken(),
	_inputToken(),
	_receiverToken(),
	_multicast(),
	_subscription(),
	_mode()
{
}


AnalyticsEngineControl::AnalyticsEngineControl(const std::string& token, const std::string& name, int useCount, const std::string& engineToken, const std::string& engineConfigToken, const std::vector<std::string>& inputToken, const std::vector<std::string>& receiverToken, const Poco::SharedPtr<MulticastConfiguration>& multicast, const Config& subscription, const std::string& mode):
	ConfigurationEntity(token, name, useCount),
	_engineToken(engineToken),
	_engineConfigToken(engineConfigToken),
	_inputToken(inputToken),
	_receiverToken(receiverToken),
	_multicast(multicast),
	_subscription(subscription),
	_mode(mode)
{
}


AnalyticsEngineControl::~AnalyticsEngineControl()
{
}


} // ONVIF


#include "ONVIF/AnalyticsEngineInput.h"


namespace ONVIF {


AnalyticsEngineInput::AnalyticsEngineInput():
	_sourceIdentification(),
	_videoInput(),
	_metadataInput()
{
}


AnalyticsEngineInput::AnalyticsEngineInput(const std::string& token, const std::string& name, int useCount, const SourceIdentification& sourceIdentification, const VideoEncoderConfiguration& videoInput, const MetadataInput& metadataInput):
	ConfigurationEntity(token, name, useCount),
	_sourceIdentification(sourceIdentification),
	_videoInput(videoInput),
	_metadataInput(metadataInput)
{
}


AnalyticsEngineInput::~AnalyticsEngineInput()
{
}


} // ONVIF


#include "ONVIF/AnalyticsEngineInputInfo.h"


namespace ONVIF {


AnalyticsEngineInputInfo::AnalyticsEngineInputInfo():
	_inputInfo(),
	_extension()
{
}


AnalyticsEngineInputInfo::AnalyticsEngineInputInfo(const Poco::SharedPtr<Config>& inputInfo, const Poco::SharedPtr<AnalyticsEngineInputInfoExtension>& extension):
	_inputInfo(inputInfo),
	_extension(extension)
{
}


AnalyticsEngineInputInfo::~AnalyticsEngineInputInfo()
{
}


} // ONVIF


#include "ONVIF/AnalyticsEngineInputInfoExtension.h"


namespace ONVIF {


AnalyticsEngineInputInfoExtension::AnalyticsEngineInputInfoExtension()
{
}


AnalyticsEngineInputInfoExtension::~AnalyticsEngineInputInfoExtension()
{
}


} // ONVIF


#include "ONVIF/AnalyticsState.h"


namespace ONVIF {


AnalyticsState::AnalyticsState():
	_error(),
	_state()
{
}


AnalyticsState::AnalyticsState(const Poco::Optional<std::string>& error, const std::string& state):
	_error(error),
	_state(state)
{
}


AnalyticsState::~AnalyticsState()
{
}


} // ONVIF


#include "ONVIF/AnalyticsStateInformation.h"


namespace ONVIF {


AnalyticsStateInformation::AnalyticsStateInformation():
	_analyticsEngineControlToken(),
	_state()
{
}


AnalyticsStateInformation::AnalyticsStateInformation(const std::string& analyticsEngineControlToken, const AnalyticsState& state):
	_analyticsEngineControlToken(analyticsEngineControlToken),
	_state(state)
{
}


AnalyticsStateInformation::~AnalyticsStateInformation()
{
}


} // ONVIF


#include "ONVIF/AnyHolder.h"


namespace ONVIF {


AnyHolder::AnyHolder()
{
}


AnyHolder::~AnyHolder()
{
}


} // ONVIF


#include "ONVIF/ArrayOfFileProgress.h"


namespace ONVIF {


ArrayOfFileProgress::ArrayOfFileProgress():
	_fileProgress(),
	_extension()
{
}


ArrayOfFileProgress::ArrayOfFileProgress(const std::vector<FileProgress>& fileProgress, const Poco::SharedPtr<ArrayOfFileProgressExtension>& extension):
	_fileProgress(fileProgress),
	_extension(extension)
{
}


ArrayOfFileProgress::~ArrayOfFileProgress()
{
}


} // ONVIF


#include "ONVIF/ArrayOfFileProgressExtension.h"


namespace ONVIF {


ArrayOfFileProgressExtension::ArrayOfFileProgressExtension()
{
}


ArrayOfFileProgressExtension::~ArrayOfFileProgressExtension()
{
}


} // ONVIF


#include "ONVIF/AttachmentData.h"


namespace ONVIF {


AttachmentData::AttachmentData():
	_contentType(),
	_include()
{
}


AttachmentData::AttachmentData(const Poco::Optional<std::string>& contentType, const W3::XOP::Include& include):
	_contentType(contentType),
	_include(include)
{
}


AttachmentData::~AttachmentData()
{
}


} // ONVIF


#include "ONVIF/AudioAttributes.h"


namespace ONVIF {


AudioAttributes::AudioAttributes():
	_bitrate(),
	_encoding(),
	_samplerate()
{
}


AudioAttributes::AudioAttributes(Poco::Optional<int> bitrate, const std::string& encoding, int samplerate):
	_bitrate(bitrate),
	_encoding(encoding),
	_samplerate(samplerate)
{
}


AudioAttributes::~AudioAttributes()
{
}


} // ONVIF


#include "ONVIF/AudioClassCandidate.h"


namespace ONVIF {


AudioClassCandidate::AudioClassCandidate():
	_type(),
	_likelihood()
{
}


AudioClassCandidate::AudioClassCandidate(const std::string& type, float likelihood):
	_type(type),
	_likelihood(likelihood)
{
}


AudioClassCandidate::~AudioClassCandidate()
{
}


} // ONVIF


#include "ONVIF/AudioClassDescriptor.h"


namespace ONVIF {


AudioClassDescriptor::AudioClassDescriptor():
	_classCandidate(),
	_extension()
{
}


AudioClassDescriptor::AudioClassDescriptor(const std::vector<AudioClassCandidate>& classCandidate, const Poco::SharedPtr<AudioClassDescriptorExtension>& extension):
	_classCandidate(classCandidate),
	_extension(extension)
{
}


AudioClassDescriptor::~AudioClassDescriptor()
{
}


} // ONVIF


#include "ONVIF/AudioClassDescriptorExtension.h"


namespace ONVIF {


AudioClassDescriptorExtension::AudioClassDescriptorExtension()
{
}


AudioClassDescriptorExtension::~AudioClassDescriptorExtension()
{
}


} // ONVIF


#include "ONVIF/AudioDecoderConfiguration.h"


namespace ONVIF {


AudioDecoderConfiguration::AudioDecoderConfiguration()
{
}


AudioDecoderConfiguration::AudioDecoderConfiguration(const std::string& token, const std::string& name, int useCount):
	ConfigurationEntity(token, name, useCount)
{
}


AudioDecoderConfiguration::~AudioDecoderConfiguration()
{
}


} // ONVIF


#include "ONVIF/AudioDecoderConfigurationOptions.h"


namespace ONVIF {


AudioDecoderConfigurationOptions::AudioDecoderConfigurationOptions():
	_aACDecOptions(),
	_g711DecOptions(),
	_g726DecOptions(),
	_extension()
{
}


AudioDecoderConfigurationOptions::AudioDecoderConfigurationOptions(const Poco::SharedPtr<AACDecOptions>& aACDecOptions, const Poco::SharedPtr<G711DecOptions>& g711DecOptions, const Poco::SharedPtr<G726DecOptions>& g726DecOptions, const Poco::SharedPtr<AudioDecoderConfigurationOptionsExtension>& extension):
	_aACDecOptions(aACDecOptions),
	_g711DecOptions(g711DecOptions),
	_g726DecOptions(g726DecOptions),
	_extension(extension)
{
}


AudioDecoderConfigurationOptions::~AudioDecoderConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/AudioDecoderConfigurationOptionsExtension.h"


namespace ONVIF {


AudioDecoderConfigurationOptionsExtension::AudioDecoderConfigurationOptionsExtension()
{
}


AudioDecoderConfigurationOptionsExtension::~AudioDecoderConfigurationOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/AudioEncoder2Configuration.h"


namespace ONVIF {


AudioEncoder2Configuration::AudioEncoder2Configuration():
	_encoding(),
	_multicast(),
	_bitrate(),
	_sampleRate()
{
}


AudioEncoder2Configuration::AudioEncoder2Configuration(const std::string& token, const std::string& name, int useCount, const std::string& encoding, const Poco::SharedPtr<MulticastConfiguration>& multicast, int bitrate, int sampleRate):
	ConfigurationEntity(token, name, useCount),
	_encoding(encoding),
	_multicast(multicast),
	_bitrate(bitrate),
	_sampleRate(sampleRate)
{
}


AudioEncoder2Configuration::~AudioEncoder2Configuration()
{
}


} // ONVIF


#include "ONVIF/AudioEncoder2ConfigurationOptions.h"


namespace ONVIF {


AudioEncoder2ConfigurationOptions::AudioEncoder2ConfigurationOptions():
	_encoding(),
	_bitrateList(),
	_sampleRateList()
{
}


AudioEncoder2ConfigurationOptions::AudioEncoder2ConfigurationOptions(const std::string& encoding, const IntItems& bitrateList, const IntItems& sampleRateList):
	_encoding(encoding),
	_bitrateList(bitrateList),
	_sampleRateList(sampleRateList)
{
}


AudioEncoder2ConfigurationOptions::~AudioEncoder2ConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/AudioEncoderConfiguration.h"


namespace ONVIF {


AudioEncoderConfiguration::AudioEncoderConfiguration():
	_encoding(),
	_bitrate(),
	_sampleRate(),
	_multicast(),
	_sessionTimeout()
{
}


AudioEncoderConfiguration::AudioEncoderConfiguration(const std::string& token, const std::string& name, int useCount, const std::string& encoding, int bitrate, int sampleRate, const MulticastConfiguration& multicast, const std::string& sessionTimeout):
	ConfigurationEntity(token, name, useCount),
	_encoding(encoding),
	_bitrate(bitrate),
	_sampleRate(sampleRate),
	_multicast(multicast),
	_sessionTimeout(sessionTimeout)
{
}


AudioEncoderConfiguration::~AudioEncoderConfiguration()
{
}


} // ONVIF


#include "ONVIF/AudioEncoderConfigurationOption.h"


namespace ONVIF {


AudioEncoderConfigurationOption::AudioEncoderConfigurationOption():
	_encoding(),
	_bitrateList(),
	_sampleRateList()
{
}


AudioEncoderConfigurationOption::AudioEncoderConfigurationOption(const std::string& encoding, const IntItems& bitrateList, const IntItems& sampleRateList):
	_encoding(encoding),
	_bitrateList(bitrateList),
	_sampleRateList(sampleRateList)
{
}


AudioEncoderConfigurationOption::~AudioEncoderConfigurationOption()
{
}


} // ONVIF


#include "ONVIF/AudioEncoderConfigurationOptions.h"


namespace ONVIF {


AudioEncoderConfigurationOptions::AudioEncoderConfigurationOptions():
	_options()
{
}


AudioEncoderConfigurationOptions::AudioEncoderConfigurationOptions(const std::vector<AudioEncoderConfigurationOption>& options):
	_options(options)
{
}


AudioEncoderConfigurationOptions::~AudioEncoderConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/AudioOutput.h"


namespace ONVIF {


AudioOutput::AudioOutput()
{
}


AudioOutput::AudioOutput(const std::string& token):
	DeviceEntity(token)
{
}


AudioOutput::~AudioOutput()
{
}


} // ONVIF


#include "ONVIF/AudioOutputConfiguration.h"


namespace ONVIF {


AudioOutputConfiguration::AudioOutputConfiguration():
	_outputToken(),
	_sendPrimacy(),
	_outputLevel()
{
}


AudioOutputConfiguration::AudioOutputConfiguration(const std::string& token, const std::string& name, int useCount, const std::string& outputToken, const Poco::Optional<Poco::URI>& sendPrimacy, int outputLevel):
	ConfigurationEntity(token, name, useCount),
	_outputToken(outputToken),
	_sendPrimacy(sendPrimacy),
	_outputLevel(outputLevel)
{
}


AudioOutputConfiguration::~AudioOutputConfiguration()
{
}


} // ONVIF


#include "ONVIF/AudioOutputConfigurationOptions.h"


namespace ONVIF {


AudioOutputConfigurationOptions::AudioOutputConfigurationOptions():
	_outputTokensAvailable(),
	_sendPrimacyOptions(),
	_outputLevelRange()
{
}


AudioOutputConfigurationOptions::AudioOutputConfigurationOptions(const std::vector<std::string>& outputTokensAvailable, const std::vector<Poco::URI>& sendPrimacyOptions, const IntRange& outputLevelRange):
	_outputTokensAvailable(outputTokensAvailable),
	_sendPrimacyOptions(sendPrimacyOptions),
	_outputLevelRange(outputLevelRange)
{
}


AudioOutputConfigurationOptions::~AudioOutputConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/AudioSource.h"


namespace ONVIF {


AudioSource::AudioSource():
	_channels()
{
}


AudioSource::AudioSource(const std::string& token, int channels):
	DeviceEntity(token),
	_channels(channels)
{
}


AudioSource::~AudioSource()
{
}


} // ONVIF


#include "ONVIF/AudioSourceConfiguration.h"


namespace ONVIF {


AudioSourceConfiguration::AudioSourceConfiguration():
	_sourceToken()
{
}


AudioSourceConfiguration::AudioSourceConfiguration(const std::string& token, const std::string& name, int useCount, const std::string& sourceToken):
	ConfigurationEntity(token, name, useCount),
	_sourceToken(sourceToken)
{
}


AudioSourceConfiguration::~AudioSourceConfiguration()
{
}


} // ONVIF


#include "ONVIF/AudioSourceConfigurationOptions.h"


namespace ONVIF {


AudioSourceConfigurationOptions::AudioSourceConfigurationOptions():
	_inputTokensAvailable(),
	_extension()
{
}


AudioSourceConfigurationOptions::AudioSourceConfigurationOptions(const std::vector<std::string>& inputTokensAvailable, const Poco::SharedPtr<AudioSourceOptionsExtension>& extension):
	_inputTokensAvailable(inputTokensAvailable),
	_extension(extension)
{
}


AudioSourceConfigurationOptions::~AudioSourceConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/AudioSourceOptionsExtension.h"


namespace ONVIF {


AudioSourceOptionsExtension::AudioSourceOptionsExtension()
{
}


AudioSourceOptionsExtension::~AudioSourceOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/BacklightCompensation.h"


namespace ONVIF {


BacklightCompensation::BacklightCompensation():
	_mode(),
	_level()
{
}


BacklightCompensation::BacklightCompensation(const std::string& mode, float level):
	_mode(mode),
	_level(level)
{
}


BacklightCompensation::~BacklightCompensation()
{
}


} // ONVIF


#include "ONVIF/BacklightCompensation20.h"


namespace ONVIF {


BacklightCompensation20::BacklightCompensation20():
	_mode(),
	_level()
{
}


BacklightCompensation20::BacklightCompensation20(const std::string& mode, Poco::Optional<float> level):
	_mode(mode),
	_level(level)
{
}


BacklightCompensation20::~BacklightCompensation20()
{
}


} // ONVIF


#include "ONVIF/BacklightCompensationOptions.h"


namespace ONVIF {


BacklightCompensationOptions::BacklightCompensationOptions():
	_mode(),
	_level()
{
}


BacklightCompensationOptions::BacklightCompensationOptions(const std::vector<std::string>& mode, const FloatRange& level):
	_mode(mode),
	_level(level)
{
}


BacklightCompensationOptions::~BacklightCompensationOptions()
{
}


} // ONVIF


#include "ONVIF/BacklightCompensationOptions20.h"


namespace ONVIF {


BacklightCompensationOptions20::BacklightCompensationOptions20():
	_mode(),
	_level()
{
}


BacklightCompensationOptions20::BacklightCompensationOptions20(const std::vector<std::string>& mode, const Poco::SharedPtr<FloatRange>& level):
	_mode(mode),
	_level(level)
{
}


BacklightCompensationOptions20::~BacklightCompensationOptions20()
{
}


} // ONVIF


#include "ONVIF/BackupFile.h"


namespace ONVIF {


BackupFile::BackupFile():
	_name(),
	_data()
{
}


BackupFile::BackupFile(const std::string& name, const AttachmentData& data):
	_name(name),
	_data(data)
{
}


BackupFile::~BackupFile()
{
}


} // ONVIF


#include "ONVIF/BinaryData.h"


namespace ONVIF {


BinaryData::BinaryData():
	_contentType(),
	_data()
{
}


BinaryData::BinaryData(const Poco::Optional<std::string>& contentType, const std::vector<char>& data):
	_contentType(contentType),
	_data(data)
{
}


BinaryData::~BinaryData()
{
}


} // ONVIF


#include "ONVIF/Capabilities.h"


namespace ONVIF {


Capabilities::Capabilities():
	_analytics(),
	_device(),
	_events(),
	_imaging(),
	_media(),
	_pTZ(),
	_extension()
{
}


Capabilities::Capabilities(const Poco::SharedPtr<AnalyticsCapabilities>& analytics, const Poco::SharedPtr<DeviceCapabilities>& device, const Poco::SharedPtr<EventCapabilities>& events, const Poco::SharedPtr<ImagingCapabilities>& imaging, const Poco::SharedPtr<MediaCapabilities>& media, const Poco::SharedPtr<PTZCapabilities>& pTZ, const Poco::SharedPtr<CapabilitiesExtension>& extension):
	_analytics(analytics),
	_device(device),
	_events(events),
	_imaging(imaging),
	_media(media),
	_pTZ(pTZ),
	_extension(extension)
{
}


Capabilities::~Capabilities()
{
}


} // ONVIF


#include "ONVIF/CapabilitiesExtension.h"


namespace ONVIF {


CapabilitiesExtension::CapabilitiesExtension():
	_deviceIO(),
	_display(),
	_recording(),
	_search(),
	_replay(),
	_receiver(),
	_analyticsDevice(),
	_extensions()
{
}


CapabilitiesExtension::CapabilitiesExtension(const Poco::SharedPtr<DeviceIOCapabilities>& deviceIO, const Poco::SharedPtr<DisplayCapabilities>& display, const Poco::SharedPtr<RecordingCapabilities>& recording, const Poco::SharedPtr<SearchCapabilities>& search, const Poco::SharedPtr<ReplayCapabilities>& replay, const Poco::SharedPtr<ReceiverCapabilities>& receiver, const Poco::SharedPtr<AnalyticsDeviceCapabilities>& analyticsDevice, const Poco::SharedPtr<CapabilitiesExtension2>& extensions):
	_deviceIO(deviceIO),
	_display(display),
	_recording(recording),
	_search(search),
	_replay(replay),
	_receiver(receiver),
	_analyticsDevice(analyticsDevice),
	_extensions(extensions)
{
}


CapabilitiesExtension::~CapabilitiesExtension()
{
}


} // ONVIF


#include "ONVIF/CapabilitiesExtension2.h"


namespace ONVIF {


CapabilitiesExtension2::CapabilitiesExtension2()
{
}


CapabilitiesExtension2::~CapabilitiesExtension2()
{
}


} // ONVIF


#include "ONVIF/CellLayout.h"


namespace ONVIF {


CellLayout::CellLayout():
	_columns(),
	_rows(),
	_transformation()
{
}


CellLayout::CellLayout(int columns, int rows, const Transformation& transformation):
	_columns(columns),
	_rows(rows),
	_transformation(transformation)
{
}


CellLayout::~CellLayout()
{
}


} // ONVIF


#include "ONVIF/Certificate.h"


namespace ONVIF {


Certificate::Certificate():
	_certificateID(),
	_certificate()
{
}


Certificate::Certificate(const std::string& certificateID, const BinaryData& certificate):
	_certificateID(certificateID),
	_certificate(certificate)
{
}


Certificate::~Certificate()
{
}


} // ONVIF


#include "ONVIF/CertificateGenerationParameters.h"


namespace ONVIF {


CertificateGenerationParameters::CertificateGenerationParameters():
	_certificateID(),
	_subject(),
	_validNotBefore(),
	_validNotAfter(),
	_extension()
{
}


CertificateGenerationParameters::CertificateGenerationParameters(const Poco::Optional<std::string>& certificateID, const Poco::Optional<std::string>& subject, const Poco::Optional<std::string>& validNotBefore, const Poco::Optional<std::string>& validNotAfter, const Poco::SharedPtr<CertificateGenerationParametersExtension>& extension):
	_certificateID(certificateID),
	_subject(subject),
	_validNotBefore(validNotBefore),
	_validNotAfter(validNotAfter),
	_extension(extension)
{
}


CertificateGenerationParameters::~CertificateGenerationParameters()
{
}


} // ONVIF


#include "ONVIF/CertificateGenerationParametersExtension.h"


namespace ONVIF {


CertificateGenerationParametersExtension::CertificateGenerationParametersExtension()
{
}


CertificateGenerationParametersExtension::~CertificateGenerationParametersExtension()
{
}


} // ONVIF


#include "ONVIF/CertificateInformation.h"


namespace ONVIF {


CertificateInformation::CertificateInformation():
	_certificateID(),
	_issuerDN(),
	_subjectDN(),
	_keyUsage(),
	_extendedKeyUsage(),
	_keyLength(),
	_version(),
	_serialNum(),
	_signatureAlgorithm(),
	_validity(),
	_extension()
{
}


CertificateInformation::CertificateInformation(const std::string& certificateID, const Poco::Optional<std::string>& issuerDN, const Poco::Optional<std::string>& subjectDN, const Poco::SharedPtr<CertificateUsage>& keyUsage, const Poco::SharedPtr<CertificateUsage>& extendedKeyUsage, Poco::Optional<int> keyLength, const Poco::Optional<std::string>& version, const Poco::Optional<std::string>& serialNum, const Poco::Optional<std::string>& signatureAlgorithm, const Poco::SharedPtr<DateTimeRange>& validity, const Poco::SharedPtr<CertificateInformationExtension>& extension):
	_certificateID(certificateID),
	_issuerDN(issuerDN),
	_subjectDN(subjectDN),
	_keyUsage(keyUsage),
	_extendedKeyUsage(extendedKeyUsage),
	_keyLength(keyLength),
	_version(version),
	_serialNum(serialNum),
	_signatureAlgorithm(signatureAlgorithm),
	_validity(validity),
	_extension(extension)
{
}


CertificateInformation::~CertificateInformation()
{
}


} // ONVIF


#include "ONVIF/CertificateInformationExtension.h"


namespace ONVIF {


CertificateInformationExtension::CertificateInformationExtension()
{
}


CertificateInformationExtension::~CertificateInformationExtension()
{
}


} // ONVIF


#include "ONVIF/CertificateStatus.h"


namespace ONVIF {


CertificateStatus::CertificateStatus():
	_certificateID(),
	_status()
{
}


CertificateStatus::CertificateStatus(const std::string& certificateID, bool status):
	_certificateID(certificateID),
	_status(status)
{
}


CertificateStatus::~CertificateStatus()
{
}


} // ONVIF


#include "ONVIF/CertificateUsage.h"


namespace ONVIF {


CertificateUsage::CertificateUsage():
	_value()
{
}


CertificateUsage::CertificateUsage(const Poco::Optional<std::string>& value):
	_value(value)
{
}


CertificateUsage::~CertificateUsage()
{
}


} // ONVIF


#include "ONVIF/CertificateWithPrivateKey.h"


namespace ONVIF {


CertificateWithPrivateKey::CertificateWithPrivateKey():
	_certificateID(),
	_certificate(),
	_privateKey()
{
}


CertificateWithPrivateKey::CertificateWithPrivateKey(const Poco::Optional<std::string>& certificateID, const BinaryData& certificate, const BinaryData& privateKey):
	_certificateID(certificateID),
	_certificate(certificate),
	_privateKey(privateKey)
{
}


CertificateWithPrivateKey::~CertificateWithPrivateKey()
{
}


} // ONVIF


#include "ONVIF/CodingCapabilities.h"


namespace ONVIF {


CodingCapabilities::CodingCapabilities():
	_audioEncodingCapabilities(),
	_audioDecodingCapabilities(),
	_videoDecodingCapabilities()
{
}


CodingCapabilities::CodingCapabilities(const Poco::SharedPtr<AudioEncoderConfigurationOptions>& audioEncodingCapabilities, const Poco::SharedPtr<AudioDecoderConfigurationOptions>& audioDecodingCapabilities, const VideoDecoderConfigurationOptions& videoDecodingCapabilities):
	_audioEncodingCapabilities(audioEncodingCapabilities),
	_audioDecodingCapabilities(audioDecodingCapabilities),
	_videoDecodingCapabilities(videoDecodingCapabilities)
{
}


CodingCapabilities::~CodingCapabilities()
{
}


} // ONVIF


#include "ONVIF/Color.h"


namespace ONVIF {


Color::Color():
	_x(),
	_y(),
	_z(),
	_colorspace(),
	_likelihood()
{
}


Color::Color(float x, float y, float z, const Poco::Optional<Poco::URI>& colorspace, Poco::Optional<float> likelihood):
	_x(x),
	_y(y),
	_z(z),
	_colorspace(colorspace),
	_likelihood(likelihood)
{
}


Color::~Color()
{
}


} // ONVIF


#include "ONVIF/ColorCluster.h"


namespace ONVIF {


ColorCluster::ColorCluster():
	_color(),
	_weight(),
	_covariance()
{
}


ColorCluster::ColorCluster(const Color& color, Poco::Optional<float> weight, const Poco::SharedPtr<ColorCovariance>& covariance):
	_color(color),
	_weight(weight),
	_covariance(covariance)
{
}


ColorCluster::~ColorCluster()
{
}


} // ONVIF


#include "ONVIF/ColorCovariance.h"


namespace ONVIF {


ColorCovariance::ColorCovariance():
	_xX(),
	_yY(),
	_zZ(),
	_xY(),
	_xZ(),
	_yZ(),
	_colorspace()
{
}


ColorCovariance::ColorCovariance(float xX, float yY, float zZ, Poco::Optional<float> xY, Poco::Optional<float> xZ, Poco::Optional<float> yZ, const Poco::Optional<Poco::URI>& colorspace):
	_xX(xX),
	_yY(yY),
	_zZ(zZ),
	_xY(xY),
	_xZ(xZ),
	_yZ(yZ),
	_colorspace(colorspace)
{
}


ColorCovariance::~ColorCovariance()
{
}


} // ONVIF


#include "ONVIF/ColorDescriptor.h"


namespace ONVIF {


ColorDescriptor::ColorDescriptor():
	_colorCluster(),
	_extension()
{
}


ColorDescriptor::ColorDescriptor(const std::vector<ColorCluster>& colorCluster, const Poco::Optional<std::string>& extension):
	_colorCluster(colorCluster),
	_extension(extension)
{
}


ColorDescriptor::~ColorDescriptor()
{
}


} // ONVIF


#include "ONVIF/ColorOptions.h"


namespace ONVIF {


ColorOptions::ColorOptions():
	_colorList(),
	_colorspaceRange()
{
}


ColorOptions::ColorOptions(const std::vector<Color>& colorList, const std::vector<ColorspaceRange>& colorspaceRange):
	_colorList(colorList),
	_colorspaceRange(colorspaceRange)
{
}


ColorOptions::~ColorOptions()
{
}


} // ONVIF


#include "ONVIF/ColorspaceRange.h"


namespace ONVIF {


ColorspaceRange::ColorspaceRange():
	_x(),
	_y(),
	_z(),
	_colorspace()
{
}


ColorspaceRange::ColorspaceRange(const FloatRange& x, const FloatRange& y, const FloatRange& z, const Poco::URI& colorspace):
	_x(x),
	_y(y),
	_z(z),
	_colorspace(colorspace)
{
}


ColorspaceRange::~ColorspaceRange()
{
}


} // ONVIF


#include "ONVIF/Config.h"


namespace ONVIF {


Config::Config():
	_name(),
	_type(),
	_parameters()
{
}


Config::Config(const std::string& name, const std::string& type, const ItemList& parameters):
	_name(name),
	_type(type),
	_parameters(parameters)
{
}


Config::~Config()
{
}


} // ONVIF


#include "ONVIF/ConfigDescription.h"


namespace ONVIF {


ConfigDescription::ConfigDescription():
	_name(),
	_fixed(),
	_maxInstances(),
	_parameters(),
	_messages(),
	_extension()
{
}


ConfigDescription::ConfigDescription(const std::string& name, Poco::Optional<bool> fixed, Poco::Optional<int> maxInstances, const ItemListDescription& parameters, const std::vector<Messages>& messages, const Poco::SharedPtr<ConfigDescriptionExtension>& extension):
	_name(name),
	_fixed(fixed),
	_maxInstances(maxInstances),
	_parameters(parameters),
	_messages(messages),
	_extension(extension)
{
}


ConfigDescription::~ConfigDescription()
{
}


} // ONVIF


#include "ONVIF/ConfigDescriptionExtension.h"


namespace ONVIF {


ConfigDescriptionExtension::ConfigDescriptionExtension()
{
}


ConfigDescriptionExtension::~ConfigDescriptionExtension()
{
}


} // ONVIF


#include "ONVIF/ConfigurationEntity.h"


namespace ONVIF {


ConfigurationEntity::ConfigurationEntity():
	_token(),
	_name(),
	_useCount()
{
}


ConfigurationEntity::ConfigurationEntity(const std::string& token, const std::string& name, int useCount):
	_token(token),
	_name(name),
	_useCount(useCount)
{
}


ConfigurationEntity::~ConfigurationEntity()
{
}


} // ONVIF


#include "ONVIF/ContinuousFocus.h"


namespace ONVIF {


ContinuousFocus::ContinuousFocus():
	_speed()
{
}


ContinuousFocus::ContinuousFocus(float speed):
	_speed(speed)
{
}


ContinuousFocus::~ContinuousFocus()
{
}


} // ONVIF


#include "ONVIF/ContinuousFocusOptions.h"


namespace ONVIF {


ContinuousFocusOptions::ContinuousFocusOptions():
	_speed()
{
}


ContinuousFocusOptions::ContinuousFocusOptions(const FloatRange& speed):
	_speed(speed)
{
}


ContinuousFocusOptions::~ContinuousFocusOptions()
{
}


} // ONVIF


#include "ONVIF/DNSInformation.h"


namespace ONVIF {


DNSInformation::DNSInformation():
	_fromDHCP(),
	_searchDomain(),
	_dNSFromDHCP(),
	_dNSManual(),
	_extension()
{
}


DNSInformation::DNSInformation(bool fromDHCP, const std::vector<std::string>& searchDomain, const std::vector<IPAddress>& dNSFromDHCP, const std::vector<IPAddress>& dNSManual, const Poco::SharedPtr<DNSInformationExtension>& extension):
	_fromDHCP(fromDHCP),
	_searchDomain(searchDomain),
	_dNSFromDHCP(dNSFromDHCP),
	_dNSManual(dNSManual),
	_extension(extension)
{
}


DNSInformation::~DNSInformation()
{
}


} // ONVIF


#include "ONVIF/DNSInformationExtension.h"


namespace ONVIF {


DNSInformationExtension::DNSInformationExtension()
{
}


DNSInformationExtension::~DNSInformationExtension()
{
}


} // ONVIF


#include "ONVIF/Date.h"


namespace ONVIF {


Date::Date():
	_year(),
	_month(),
	_day()
{
}


Date::Date(int year, int month, int day):
	_year(year),
	_month(month),
	_day(day)
{
}


Date::~Date()
{
}


} // ONVIF


#include "ONVIF/DateTime.h"


namespace ONVIF {


DateTime::DateTime():
	_time(),
	_date()
{
}


DateTime::DateTime(const Time& time, const Date& date):
	_time(time),
	_date(date)
{
}


DateTime::~DateTime()
{
}


} // ONVIF


#include "ONVIF/DateTimeRange.h"


namespace ONVIF {


DateTimeRange::DateTimeRange():
	_from(),
	_until()
{
}


DateTimeRange::DateTimeRange(const Poco::DateTime& from, const Poco::DateTime& until):
	_from(from),
	_until(until)
{
}


DateTimeRange::~DateTimeRange()
{
}


} // ONVIF


#include "ONVIF/Defogging.h"


namespace ONVIF {


Defogging::Defogging():
	_mode(),
	_level(),
	_extension()
{
}


Defogging::Defogging(const std::string& mode, Poco::Optional<float> level, const Poco::SharedPtr<DefoggingExtension>& extension):
	_mode(mode),
	_level(level),
	_extension(extension)
{
}


Defogging::~Defogging()
{
}


} // ONVIF


#include "ONVIF/DefoggingExtension.h"


namespace ONVIF {


DefoggingExtension::DefoggingExtension()
{
}


DefoggingExtension::~DefoggingExtension()
{
}


} // ONVIF


#include "ONVIF/DefoggingOptions.h"


namespace ONVIF {


DefoggingOptions::DefoggingOptions():
	_mode(),
	_level()
{
}


DefoggingOptions::DefoggingOptions(const std::vector<std::string>& mode, bool level):
	_mode(mode),
	_level(level)
{
}


DefoggingOptions::~DefoggingOptions()
{
}


} // ONVIF


#include "ONVIF/DeviceCapabilities.h"


namespace ONVIF {


DeviceCapabilities::DeviceCapabilities():
	_xAddr(),
	_network(),
	_system(),
	_iO(),
	_security(),
	_extension()
{
}


DeviceCapabilities::DeviceCapabilities(const Poco::URI& xAddr, const Poco::SharedPtr<NetworkCapabilities>& network, const Poco::SharedPtr<SystemCapabilities>& system, const Poco::SharedPtr<IOCapabilities>& iO, const Poco::SharedPtr<SecurityCapabilities>& security, const Poco::SharedPtr<DeviceCapabilitiesExtension>& extension):
	_xAddr(xAddr),
	_network(network),
	_system(system),
	_iO(iO),
	_security(security),
	_extension(extension)
{
}


DeviceCapabilities::~DeviceCapabilities()
{
}


} // ONVIF


#include "ONVIF/DeviceCapabilitiesExtension.h"


namespace ONVIF {


DeviceCapabilitiesExtension::DeviceCapabilitiesExtension()
{
}


DeviceCapabilitiesExtension::~DeviceCapabilitiesExtension()
{
}


} // ONVIF


#include "ONVIF/DeviceEntity.h"


namespace ONVIF {


DeviceEntity::DeviceEntity():
	_token()
{
}


DeviceEntity::DeviceEntity(const std::string& token):
	_token(token)
{
}


DeviceEntity::~DeviceEntity()
{
}


} // ONVIF


#include "ONVIF/DeviceIOCapabilities.h"


namespace ONVIF {


DeviceIOCapabilities::DeviceIOCapabilities():
	_xAddr(),
	_videoSources(),
	_videoOutputs(),
	_audioSources(),
	_audioOutputs(),
	_relayOutputs()
{
}


DeviceIOCapabilities::DeviceIOCapabilities(const Poco::URI& xAddr, int videoSources, int videoOutputs, int audioSources, int audioOutputs, int relayOutputs):
	_xAddr(xAddr),
	_videoSources(videoSources),
	_videoOutputs(videoOutputs),
	_audioSources(audioSources),
	_audioOutputs(audioOutputs),
	_relayOutputs(relayOutputs)
{
}


DeviceIOCapabilities::~DeviceIOCapabilities()
{
}


} // ONVIF


#include "ONVIF/DigitalInput.h"


namespace ONVIF {


DigitalInput::DigitalInput():
	_idleState()
{
}


DigitalInput::DigitalInput(const std::string& token, const Poco::Optional<std::string>& idleState):
	DeviceEntity(token),
	_idleState(idleState)
{
}


DigitalInput::~DigitalInput()
{
}


} // ONVIF


#include "ONVIF/DisplayCapabilities.h"


namespace ONVIF {


DisplayCapabilities::DisplayCapabilities():
	_xAddr(),
	_fixedLayout()
{
}


DisplayCapabilities::DisplayCapabilities(const Poco::URI& xAddr, bool fixedLayout):
	_xAddr(xAddr),
	_fixedLayout(fixedLayout)
{
}


DisplayCapabilities::~DisplayCapabilities()
{
}


} // ONVIF


#include "ONVIF/Dot11AvailableNetworks.h"


namespace ONVIF {


Dot11AvailableNetworks::Dot11AvailableNetworks():
	_sSID(),
	_bSSID(),
	_authAndMangementSuite(),
	_pairCipher(),
	_groupCipher(),
	_signalStrength(),
	_extension()
{
}


Dot11AvailableNetworks::Dot11AvailableNetworks(const std::string& sSID, const Poco::Optional<std::string>& bSSID, const std::vector<std::string>& authAndMangementSuite, const std::vector<std::string>& pairCipher, const std::vector<std::string>& groupCipher, const Poco::Optional<std::string>& signalStrength, const Poco::SharedPtr<Dot11AvailableNetworksExtension>& extension):
	_sSID(sSID),
	_bSSID(bSSID),
	_authAndMangementSuite(authAndMangementSuite),
	_pairCipher(pairCipher),
	_groupCipher(groupCipher),
	_signalStrength(signalStrength),
	_extension(extension)
{
}


Dot11AvailableNetworks::~Dot11AvailableNetworks()
{
}


} // ONVIF


#include "ONVIF/Dot11AvailableNetworksExtension.h"


namespace ONVIF {


Dot11AvailableNetworksExtension::Dot11AvailableNetworksExtension()
{
}


Dot11AvailableNetworksExtension::~Dot11AvailableNetworksExtension()
{
}


} // ONVIF


#include "ONVIF/Dot11Capabilities.h"


namespace ONVIF {


Dot11Capabilities::Dot11Capabilities():
	_tKIP(),
	_scanAvailableNetworks(),
	_multipleConfiguration(),
	_adHocStationMode(),
	_wEP()
{
}


Dot11Capabilities::Dot11Capabilities(bool tKIP, bool scanAvailableNetworks, bool multipleConfiguration, bool adHocStationMode, bool wEP):
	_tKIP(tKIP),
	_scanAvailableNetworks(scanAvailableNetworks),
	_multipleConfiguration(multipleConfiguration),
	_adHocStationMode(adHocStationMode),
	_wEP(wEP)
{
}


Dot11Capabilities::~Dot11Capabilities()
{
}


} // ONVIF


#include "ONVIF/Dot11Configuration.h"


namespace ONVIF {


Dot11Configuration::Dot11Configuration():
	_sSID(),
	_mode(),
	_alias(),
	_priority(),
	_security()
{
}


Dot11Configuration::Dot11Configuration(const std::string& sSID, const std::string& mode, const std::string& alias, const int& priority, const Dot11SecurityConfiguration& security):
	_sSID(sSID),
	_mode(mode),
	_alias(alias),
	_priority(priority),
	_security(security)
{
}


Dot11Configuration::~Dot11Configuration()
{
}


} // ONVIF


#include "ONVIF/Dot11PSKSet.h"


namespace ONVIF {


Dot11PSKSet::Dot11PSKSet():
	_key(),
	_passphrase(),
	_extension()
{
}


Dot11PSKSet::Dot11PSKSet(const Poco::Optional<std::string>& key, const Poco::Optional<std::string>& passphrase, const Poco::SharedPtr<Dot11PSKSetExtension>& extension):
	_key(key),
	_passphrase(passphrase),
	_extension(extension)
{
}


Dot11PSKSet::~Dot11PSKSet()
{
}


} // ONVIF


#include "ONVIF/Dot11PSKSetExtension.h"


namespace ONVIF {


Dot11PSKSetExtension::Dot11PSKSetExtension()
{
}


Dot11PSKSetExtension::~Dot11PSKSetExtension()
{
}


} // ONVIF


#include "ONVIF/Dot11SecurityConfiguration.h"


namespace ONVIF {


Dot11SecurityConfiguration::Dot11SecurityConfiguration():
	_mode(),
	_algorithm(),
	_pSK(),
	_dot1X(),
	_extension()
{
}


Dot11SecurityConfiguration::Dot11SecurityConfiguration(const std::string& mode, const Poco::Optional<std::string>& algorithm, const Poco::SharedPtr<Dot11PSKSet>& pSK, const Poco::Optional<std::string>& dot1X, const Poco::SharedPtr<Dot11SecurityConfigurationExtension>& extension):
	_mode(mode),
	_algorithm(algorithm),
	_pSK(pSK),
	_dot1X(dot1X),
	_extension(extension)
{
}


Dot11SecurityConfiguration::~Dot11SecurityConfiguration()
{
}


} // ONVIF


#include "ONVIF/Dot11SecurityConfigurationExtension.h"


namespace ONVIF {


Dot11SecurityConfigurationExtension::Dot11SecurityConfigurationExtension()
{
}


Dot11SecurityConfigurationExtension::~Dot11SecurityConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/Dot11Status.h"


namespace ONVIF {


Dot11Status::Dot11Status():
	_sSID(),
	_bSSID(),
	_pairCipher(),
	_groupCipher(),
	_signalStrength(),
	_activeConfigAlias()
{
}


Dot11Status::Dot11Status(const std::string& sSID, const Poco::Optional<std::string>& bSSID, const Poco::Optional<std::string>& pairCipher, const Poco::Optional<std::string>& groupCipher, const Poco::Optional<std::string>& signalStrength, const std::string& activeConfigAlias):
	_sSID(sSID),
	_bSSID(bSSID),
	_pairCipher(pairCipher),
	_groupCipher(groupCipher),
	_signalStrength(signalStrength),
	_activeConfigAlias(activeConfigAlias)
{
}


Dot11Status::~Dot11Status()
{
}


} // ONVIF


#include "ONVIF/Dot1XConfiguration.h"


namespace ONVIF {


Dot1XConfiguration::Dot1XConfiguration():
	_dot1XConfigurationToken(),
	_identity(),
	_anonymousID(),
	_eAPMethod(),
	_cACertificateID(),
	_eAPMethodConfiguration(),
	_extension()
{
}


Dot1XConfiguration::Dot1XConfiguration(const std::string& dot1XConfigurationToken, const std::string& identity, const Poco::Optional<std::string>& anonymousID, int eAPMethod, const std::vector<std::string>& cACertificateID, const Poco::SharedPtr<EAPMethodConfiguration>& eAPMethodConfiguration, const Poco::SharedPtr<Dot1XConfigurationExtension>& extension):
	_dot1XConfigurationToken(dot1XConfigurationToken),
	_identity(identity),
	_anonymousID(anonymousID),
	_eAPMethod(eAPMethod),
	_cACertificateID(cACertificateID),
	_eAPMethodConfiguration(eAPMethodConfiguration),
	_extension(extension)
{
}


Dot1XConfiguration::~Dot1XConfiguration()
{
}


} // ONVIF


#include "ONVIF/Dot1XConfigurationExtension.h"


namespace ONVIF {


Dot1XConfigurationExtension::Dot1XConfigurationExtension()
{
}


Dot1XConfigurationExtension::~Dot1XConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/Dot3Configuration.h"


namespace ONVIF {


Dot3Configuration::Dot3Configuration()
{
}


Dot3Configuration::~Dot3Configuration()
{
}


} // ONVIF


#include "ONVIF/DurationRange.h"


namespace ONVIF {


DurationRange::DurationRange():
	_min(),
	_max()
{
}


DurationRange::DurationRange(const std::string& min, const std::string& max):
	_min(min),
	_max(max)
{
}


DurationRange::~DurationRange()
{
}


} // ONVIF


#include "ONVIF/DynamicDNSInformation.h"


namespace ONVIF {


DynamicDNSInformation::DynamicDNSInformation():
	_type(),
	_name(),
	_tTL(),
	_extension()
{
}


DynamicDNSInformation::DynamicDNSInformation(const std::string& type, const Poco::Optional<std::string>& name, const Poco::Optional<std::string>& tTL, const Poco::SharedPtr<DynamicDNSInformationExtension>& extension):
	_type(type),
	_name(name),
	_tTL(tTL),
	_extension(extension)
{
}


DynamicDNSInformation::~DynamicDNSInformation()
{
}


} // ONVIF


#include "ONVIF/DynamicDNSInformationExtension.h"


namespace ONVIF {


DynamicDNSInformationExtension::DynamicDNSInformationExtension()
{
}


DynamicDNSInformationExtension::~DynamicDNSInformationExtension()
{
}


} // ONVIF


#include "ONVIF/EAPMethodConfiguration.h"


namespace ONVIF {


EAPMethodConfiguration::EAPMethodConfiguration():
	_tLSConfiguration(),
	_password(),
	_extension()
{
}


EAPMethodConfiguration::EAPMethodConfiguration(const Poco::SharedPtr<TLSConfiguration>& tLSConfiguration, const Poco::Optional<std::string>& password, const Poco::SharedPtr<EapMethodExtension>& extension):
	_tLSConfiguration(tLSConfiguration),
	_password(password),
	_extension(extension)
{
}


EAPMethodConfiguration::~EAPMethodConfiguration()
{
}


} // ONVIF


#include "ONVIF/EFlip.h"


namespace ONVIF {


EFlip::EFlip():
	_mode()
{
}


EFlip::EFlip(const std::string& mode):
	_mode(mode)
{
}


EFlip::~EFlip()
{
}


} // ONVIF


#include "ONVIF/EFlipOptions.h"


namespace ONVIF {


EFlipOptions::EFlipOptions():
	_mode(),
	_extension()
{
}


EFlipOptions::EFlipOptions(const std::vector<std::string>& mode, const Poco::SharedPtr<EFlipOptionsExtension>& extension):
	_mode(mode),
	_extension(extension)
{
}


EFlipOptions::~EFlipOptions()
{
}


} // ONVIF


#include "ONVIF/EFlipOptionsExtension.h"


namespace ONVIF {


EFlipOptionsExtension::EFlipOptionsExtension()
{
}


EFlipOptionsExtension::~EFlipOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/EapMethodExtension.h"


namespace ONVIF {


EapMethodExtension::EapMethodExtension()
{
}


EapMethodExtension::~EapMethodExtension()
{
}


} // ONVIF


#include "ONVIF/ElementItem.h"


namespace ONVIF {


ElementItem::ElementItem():
	_name()
{
}


ElementItem::ElementItem(const std::string& name):
	_name(name)
{
}


ElementItem::~ElementItem()
{
}


} // ONVIF


#include "ONVIF/ElementItemDescription.h"


namespace ONVIF {


ElementItemDescription::ElementItemDescription():
	_name(),
	_type()
{
}


ElementItemDescription::ElementItemDescription(const std::string& name, const std::string& type):
	_name(name),
	_type(type)
{
}


ElementItemDescription::~ElementItemDescription()
{
}


} // ONVIF


#include "ONVIF/EngineConfiguration.h"


namespace ONVIF {


EngineConfiguration::EngineConfiguration():
	_videoAnalyticsConfiguration(),
	_analyticsEngineInputInfo()
{
}


EngineConfiguration::EngineConfiguration(const VideoAnalyticsConfiguration& videoAnalyticsConfiguration, const AnalyticsEngineInputInfo& analyticsEngineInputInfo):
	_videoAnalyticsConfiguration(videoAnalyticsConfiguration),
	_analyticsEngineInputInfo(analyticsEngineInputInfo)
{
}


EngineConfiguration::~EngineConfiguration()
{
}


} // ONVIF


#include "ONVIF/EventCapabilities.h"


namespace ONVIF {


EventCapabilities::EventCapabilities():
	_xAddr(),
	_wSSubscriptionPolicySupport(),
	_wSPullPointSupport(),
	_wSPausableSubscriptionManagerInterfaceSupport()
{
}


EventCapabilities::EventCapabilities(const Poco::URI& xAddr, bool wSSubscriptionPolicySupport, bool wSPullPointSupport, bool wSPausableSubscriptionManagerInterfaceSupport):
	_xAddr(xAddr),
	_wSSubscriptionPolicySupport(wSSubscriptionPolicySupport),
	_wSPullPointSupport(wSPullPointSupport),
	_wSPausableSubscriptionManagerInterfaceSupport(wSPausableSubscriptionManagerInterfaceSupport)
{
}


EventCapabilities::~EventCapabilities()
{
}


} // ONVIF


#include "ONVIF/EventFilter.h"


namespace ONVIF {


EventFilter::EventFilter()
{
}


EventFilter::~EventFilter()
{
}


} // ONVIF


#include "ONVIF/EventSubscription.h"


namespace ONVIF {


EventSubscription::EventSubscription():
	_filter(),
	_subscriptionPolicy()
{
}


EventSubscription::EventSubscription(const Poco::SharedPtr<OASIS::WSN::B2::Filter>& filter, const Poco::SharedPtr<SubscriptionPolicy>& subscriptionPolicy):
	_filter(filter),
	_subscriptionPolicy(subscriptionPolicy)
{
}


EventSubscription::~EventSubscription()
{
}


} // ONVIF


#include "ONVIF/Exposure.h"


namespace ONVIF {


Exposure::Exposure():
	_mode(),
	_priority(),
	_window(),
	_minExposureTime(),
	_maxExposureTime(),
	_minGain(),
	_maxGain(),
	_minIris(),
	_maxIris(),
	_exposureTime(),
	_gain(),
	_iris()
{
}


Exposure::Exposure(const std::string& mode, const std::string& priority, const Rectangle& window, float minExposureTime, float maxExposureTime, float minGain, float maxGain, float minIris, float maxIris, float exposureTime, float gain, float iris):
	_mode(mode),
	_priority(priority),
	_window(window),
	_minExposureTime(minExposureTime),
	_maxExposureTime(maxExposureTime),
	_minGain(minGain),
	_maxGain(maxGain),
	_minIris(minIris),
	_maxIris(maxIris),
	_exposureTime(exposureTime),
	_gain(gain),
	_iris(iris)
{
}


Exposure::~Exposure()
{
}


} // ONVIF


#include "ONVIF/Exposure20.h"


namespace ONVIF {


Exposure20::Exposure20():
	_mode(),
	_priority(),
	_window(),
	_minExposureTime(),
	_maxExposureTime(),
	_minGain(),
	_maxGain(),
	_minIris(),
	_maxIris(),
	_exposureTime(),
	_gain(),
	_iris()
{
}


Exposure20::Exposure20(const std::string& mode, const Poco::Optional<std::string>& priority, const Poco::SharedPtr<Rectangle>& window, Poco::Optional<float> minExposureTime, Poco::Optional<float> maxExposureTime, Poco::Optional<float> minGain, Poco::Optional<float> maxGain, Poco::Optional<float> minIris, Poco::Optional<float> maxIris, Poco::Optional<float> exposureTime, Poco::Optional<float> gain, Poco::Optional<float> iris):
	_mode(mode),
	_priority(priority),
	_window(window),
	_minExposureTime(minExposureTime),
	_maxExposureTime(maxExposureTime),
	_minGain(minGain),
	_maxGain(maxGain),
	_minIris(minIris),
	_maxIris(maxIris),
	_exposureTime(exposureTime),
	_gain(gain),
	_iris(iris)
{
}


Exposure20::~Exposure20()
{
}


} // ONVIF


#include "ONVIF/ExposureOptions.h"


namespace ONVIF {


ExposureOptions::ExposureOptions():
	_mode(),
	_priority(),
	_minExposureTime(),
	_maxExposureTime(),
	_minGain(),
	_maxGain(),
	_minIris(),
	_maxIris(),
	_exposureTime(),
	_gain(),
	_iris()
{
}


ExposureOptions::ExposureOptions(const std::vector<std::string>& mode, const std::vector<std::string>& priority, const FloatRange& minExposureTime, const FloatRange& maxExposureTime, const FloatRange& minGain, const FloatRange& maxGain, const FloatRange& minIris, const FloatRange& maxIris, const FloatRange& exposureTime, const FloatRange& gain, const FloatRange& iris):
	_mode(mode),
	_priority(priority),
	_minExposureTime(minExposureTime),
	_maxExposureTime(maxExposureTime),
	_minGain(minGain),
	_maxGain(maxGain),
	_minIris(minIris),
	_maxIris(maxIris),
	_exposureTime(exposureTime),
	_gain(gain),
	_iris(iris)
{
}


ExposureOptions::~ExposureOptions()
{
}


} // ONVIF


#include "ONVIF/ExposureOptions20.h"


namespace ONVIF {


ExposureOptions20::ExposureOptions20():
	_mode(),
	_priority(),
	_minExposureTime(),
	_maxExposureTime(),
	_minGain(),
	_maxGain(),
	_minIris(),
	_maxIris(),
	_exposureTime(),
	_gain(),
	_iris()
{
}


ExposureOptions20::ExposureOptions20(const std::vector<std::string>& mode, const std::vector<std::string>& priority, const Poco::SharedPtr<FloatRange>& minExposureTime, const Poco::SharedPtr<FloatRange>& maxExposureTime, const Poco::SharedPtr<FloatRange>& minGain, const Poco::SharedPtr<FloatRange>& maxGain, const Poco::SharedPtr<FloatRange>& minIris, const Poco::SharedPtr<FloatRange>& maxIris, const Poco::SharedPtr<FloatRange>& exposureTime, const Poco::SharedPtr<FloatRange>& gain, const Poco::SharedPtr<FloatRange>& iris):
	_mode(mode),
	_priority(priority),
	_minExposureTime(minExposureTime),
	_maxExposureTime(maxExposureTime),
	_minGain(minGain),
	_maxGain(maxGain),
	_minIris(minIris),
	_maxIris(maxIris),
	_exposureTime(exposureTime),
	_gain(gain),
	_iris(iris)
{
}


ExposureOptions20::~ExposureOptions20()
{
}


} // ONVIF


#include "ONVIF/FileProgress.h"


namespace ONVIF {


FileProgress::FileProgress():
	_fileName(),
	_progress()
{
}


FileProgress::FileProgress(const std::string& fileName, float progress):
	_fileName(fileName),
	_progress(progress)
{
}


FileProgress::~FileProgress()
{
}


} // ONVIF


#include "ONVIF/Filter.h"


namespace ONVIF {


Filter::Filter():
	_topic(),
	_source()
{
}


Filter::Filter(const std::string& topic, const Poco::Optional<std::string>& source):
	_topic(topic),
	_source(source)
{
}


Filter::~Filter()
{
}


} // ONVIF


#include "ONVIF/FindEventResult.h"


namespace ONVIF {


FindEventResult::FindEventResult():
	_recordingToken(),
	_trackToken(),
	_time(),
	_event(),
	_startStateEvent()
{
}


FindEventResult::FindEventResult(const std::string& recordingToken, const std::string& trackToken, const Poco::DateTime& time, const OASIS::WSN::B2::NotificationMessageHolder& event, bool startStateEvent):
	_recordingToken(recordingToken),
	_trackToken(trackToken),
	_time(time),
	_event(event),
	_startStateEvent(startStateEvent)
{
}


FindEventResult::~FindEventResult()
{
}


} // ONVIF


#include "ONVIF/FindEventResultList.h"


namespace ONVIF {


FindEventResultList::FindEventResultList():
	_searchState(),
	_result()
{
}


FindEventResultList::FindEventResultList(const std::string& searchState, const std::vector<FindEventResult>& result):
	_searchState(searchState),
	_result(result)
{
}


FindEventResultList::~FindEventResultList()
{
}


} // ONVIF


#include "ONVIF/FindMetadataResult.h"


namespace ONVIF {


FindMetadataResult::FindMetadataResult():
	_recordingToken(),
	_trackToken(),
	_time()
{
}


FindMetadataResult::FindMetadataResult(const std::string& recordingToken, const std::string& trackToken, const Poco::DateTime& time):
	_recordingToken(recordingToken),
	_trackToken(trackToken),
	_time(time)
{
}


FindMetadataResult::~FindMetadataResult()
{
}


} // ONVIF


#include "ONVIF/FindMetadataResultList.h"


namespace ONVIF {


FindMetadataResultList::FindMetadataResultList():
	_searchState(),
	_result()
{
}


FindMetadataResultList::FindMetadataResultList(const std::string& searchState, const std::vector<FindMetadataResult>& result):
	_searchState(searchState),
	_result(result)
{
}


FindMetadataResultList::~FindMetadataResultList()
{
}


} // ONVIF


#include "ONVIF/FindPTZPositionResult.h"


namespace ONVIF {


FindPTZPositionResult::FindPTZPositionResult():
	_recordingToken(),
	_trackToken(),
	_time(),
	_position()
{
}


FindPTZPositionResult::FindPTZPositionResult(const std::string& recordingToken, const std::string& trackToken, const Poco::DateTime& time, const PTZVector& position):
	_recordingToken(recordingToken),
	_trackToken(trackToken),
	_time(time),
	_position(position)
{
}


FindPTZPositionResult::~FindPTZPositionResult()
{
}


} // ONVIF


#include "ONVIF/FindPTZPositionResultList.h"


namespace ONVIF {


FindPTZPositionResultList::FindPTZPositionResultList():
	_searchState(),
	_result()
{
}


FindPTZPositionResultList::FindPTZPositionResultList(const std::string& searchState, const std::vector<FindPTZPositionResult>& result):
	_searchState(searchState),
	_result(result)
{
}


FindPTZPositionResultList::~FindPTZPositionResultList()
{
}


} // ONVIF


#include "ONVIF/FindRecordingResultList.h"


namespace ONVIF {


FindRecordingResultList::FindRecordingResultList():
	_searchState(),
	_recordingInformation()
{
}


FindRecordingResultList::FindRecordingResultList(const std::string& searchState, const std::vector<RecordingInformation>& recordingInformation):
	_searchState(searchState),
	_recordingInformation(recordingInformation)
{
}


FindRecordingResultList::~FindRecordingResultList()
{
}


} // ONVIF


#include "ONVIF/FloatItems.h"


namespace ONVIF {


FloatItems::FloatItems():
	_items()
{
}


FloatItems::FloatItems(const std::vector<float>& items):
	_items(items)
{
}


FloatItems::~FloatItems()
{
}


} // ONVIF


#include "ONVIF/FloatRange.h"


namespace ONVIF {


FloatRange::FloatRange():
	_min(),
	_max()
{
}


FloatRange::FloatRange(float min, float max):
	_min(min),
	_max(max)
{
}


FloatRange::~FloatRange()
{
}


} // ONVIF


#include "ONVIF/FocusConfiguration.h"


namespace ONVIF {


FocusConfiguration::FocusConfiguration():
	_autoFocusMode(),
	_defaultSpeed(),
	_nearLimit(),
	_farLimit()
{
}


FocusConfiguration::FocusConfiguration(const std::string& autoFocusMode, float defaultSpeed, float nearLimit, float farLimit):
	_autoFocusMode(autoFocusMode),
	_defaultSpeed(defaultSpeed),
	_nearLimit(nearLimit),
	_farLimit(farLimit)
{
}


FocusConfiguration::~FocusConfiguration()
{
}


} // ONVIF


#include "ONVIF/FocusConfiguration20.h"


namespace ONVIF {


FocusConfiguration20::FocusConfiguration20():
	_aFMode(),
	_autoFocusMode(),
	_defaultSpeed(),
	_nearLimit(),
	_farLimit(),
	_extension()
{
}


FocusConfiguration20::FocusConfiguration20(const std::vector<std::vector<std::string>>& aFMode, const std::string& autoFocusMode, Poco::Optional<float> defaultSpeed, Poco::Optional<float> nearLimit, Poco::Optional<float> farLimit, const Poco::SharedPtr<FocusConfiguration20Extension>& extension):
	_aFMode(aFMode),
	_autoFocusMode(autoFocusMode),
	_defaultSpeed(defaultSpeed),
	_nearLimit(nearLimit),
	_farLimit(farLimit),
	_extension(extension)
{
}


FocusConfiguration20::~FocusConfiguration20()
{
}


} // ONVIF


#include "ONVIF/FocusConfiguration20Extension.h"


namespace ONVIF {


FocusConfiguration20Extension::FocusConfiguration20Extension()
{
}


FocusConfiguration20Extension::~FocusConfiguration20Extension()
{
}


} // ONVIF


#include "ONVIF/FocusMove.h"


namespace ONVIF {


FocusMove::FocusMove():
	_absolute(),
	_relative(),
	_continuous()
{
}


FocusMove::FocusMove(const Poco::SharedPtr<AbsoluteFocus>& absolute, const Poco::SharedPtr<RelativeFocus>& relative, const Poco::SharedPtr<ContinuousFocus>& continuous):
	_absolute(absolute),
	_relative(relative),
	_continuous(continuous)
{
}


FocusMove::~FocusMove()
{
}


} // ONVIF


#include "ONVIF/FocusOptions.h"


namespace ONVIF {


FocusOptions::FocusOptions():
	_autoFocusModes(),
	_defaultSpeed(),
	_nearLimit(),
	_farLimit()
{
}


FocusOptions::FocusOptions(const std::vector<std::string>& autoFocusModes, const FloatRange& defaultSpeed, const FloatRange& nearLimit, const FloatRange& farLimit):
	_autoFocusModes(autoFocusModes),
	_defaultSpeed(defaultSpeed),
	_nearLimit(nearLimit),
	_farLimit(farLimit)
{
}


FocusOptions::~FocusOptions()
{
}


} // ONVIF


#include "ONVIF/FocusOptions20.h"


namespace ONVIF {


FocusOptions20::FocusOptions20():
	_autoFocusModes(),
	_defaultSpeed(),
	_nearLimit(),
	_farLimit(),
	_extension()
{
}


FocusOptions20::FocusOptions20(const std::vector<std::string>& autoFocusModes, const Poco::SharedPtr<FloatRange>& defaultSpeed, const Poco::SharedPtr<FloatRange>& nearLimit, const Poco::SharedPtr<FloatRange>& farLimit, const Poco::SharedPtr<FocusOptions20Extension>& extension):
	_autoFocusModes(autoFocusModes),
	_defaultSpeed(defaultSpeed),
	_nearLimit(nearLimit),
	_farLimit(farLimit),
	_extension(extension)
{
}


FocusOptions20::~FocusOptions20()
{
}


} // ONVIF


#include "ONVIF/FocusOptions20Extension.h"


namespace ONVIF {


FocusOptions20Extension::FocusOptions20Extension():
	_aFModes()
{
}


FocusOptions20Extension::FocusOptions20Extension(const Poco::Optional<std::vector<std::string>>& aFModes):
	_aFModes(aFModes)
{
}


FocusOptions20Extension::~FocusOptions20Extension()
{
}


} // ONVIF


#include "ONVIF/FocusStatus.h"


namespace ONVIF {


FocusStatus::FocusStatus():
	_position(),
	_moveStatus(),
	_error()
{
}


FocusStatus::FocusStatus(float position, const std::string& moveStatus, const std::string& error):
	_position(position),
	_moveStatus(moveStatus),
	_error(error)
{
}


FocusStatus::~FocusStatus()
{
}


} // ONVIF


#include "ONVIF/FocusStatus20.h"


namespace ONVIF {


FocusStatus20::FocusStatus20():
	_position(),
	_moveStatus(),
	_error(),
	_extension()
{
}


FocusStatus20::FocusStatus20(float position, const std::string& moveStatus, const Poco::Optional<std::string>& error, const Poco::SharedPtr<FocusStatus20Extension>& extension):
	_position(position),
	_moveStatus(moveStatus),
	_error(error),
	_extension(extension)
{
}


FocusStatus20::~FocusStatus20()
{
}


} // ONVIF


#include "ONVIF/FocusStatus20Extension.h"


namespace ONVIF {


FocusStatus20Extension::FocusStatus20Extension()
{
}


FocusStatus20Extension::~FocusStatus20Extension()
{
}


} // ONVIF


#include "ONVIF/G711DecOptions.h"


namespace ONVIF {


G711DecOptions::G711DecOptions():
	_bitrate(),
	_sampleRateRange()
{
}


G711DecOptions::G711DecOptions(const IntItems& bitrate, const IntItems& sampleRateRange):
	_bitrate(bitrate),
	_sampleRateRange(sampleRateRange)
{
}


G711DecOptions::~G711DecOptions()
{
}


} // ONVIF


#include "ONVIF/G726DecOptions.h"


namespace ONVIF {


G726DecOptions::G726DecOptions():
	_bitrate(),
	_sampleRateRange()
{
}


G726DecOptions::G726DecOptions(const IntItems& bitrate, const IntItems& sampleRateRange):
	_bitrate(bitrate),
	_sampleRateRange(sampleRateRange)
{
}


G726DecOptions::~G726DecOptions()
{
}


} // ONVIF


#include "ONVIF/GenericEapPwdConfigurationExtension.h"


namespace ONVIF {


GenericEapPwdConfigurationExtension::GenericEapPwdConfigurationExtension()
{
}


GenericEapPwdConfigurationExtension::~GenericEapPwdConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/GeoLocation.h"


namespace ONVIF {


GeoLocation::GeoLocation():
	_lon(),
	_lat(),
	_elevation()
{
}


GeoLocation::GeoLocation(Poco::Optional<double> lon, Poco::Optional<double> lat, Poco::Optional<float> elevation):
	_lon(lon),
	_lat(lat),
	_elevation(elevation)
{
}


GeoLocation::~GeoLocation()
{
}


} // ONVIF


#include "ONVIF/GeoOrientation.h"


namespace ONVIF {


GeoOrientation::GeoOrientation():
	_roll(),
	_pitch(),
	_yaw()
{
}


GeoOrientation::GeoOrientation(Poco::Optional<float> roll, Poco::Optional<float> pitch, Poco::Optional<float> yaw):
	_roll(roll),
	_pitch(pitch),
	_yaw(yaw)
{
}


GeoOrientation::~GeoOrientation()
{
}


} // ONVIF


#include "ONVIF/GetRecordingJobsResponseItem.h"


namespace ONVIF {


GetRecordingJobsResponseItem::GetRecordingJobsResponseItem():
	_jobToken(),
	_jobConfiguration()
{
}


GetRecordingJobsResponseItem::GetRecordingJobsResponseItem(const std::string& jobToken, const RecordingJobConfiguration& jobConfiguration):
	_jobToken(jobToken),
	_jobConfiguration(jobConfiguration)
{
}


GetRecordingJobsResponseItem::~GetRecordingJobsResponseItem()
{
}


} // ONVIF


#include "ONVIF/GetRecordingsResponseItem.h"


namespace ONVIF {


GetRecordingsResponseItem::GetRecordingsResponseItem():
	_recordingToken(),
	_configuration(),
	_tracks()
{
}


GetRecordingsResponseItem::GetRecordingsResponseItem(const std::string& recordingToken, const RecordingConfiguration& configuration, const GetTracksResponseList& tracks):
	_recordingToken(recordingToken),
	_configuration(configuration),
	_tracks(tracks)
{
}


GetRecordingsResponseItem::~GetRecordingsResponseItem()
{
}


} // ONVIF


#include "ONVIF/GetTracksResponseItem.h"


namespace ONVIF {


GetTracksResponseItem::GetTracksResponseItem():
	_trackToken(),
	_configuration()
{
}


GetTracksResponseItem::GetTracksResponseItem(const std::string& trackToken, const TrackConfiguration& configuration):
	_trackToken(trackToken),
	_configuration(configuration)
{
}


GetTracksResponseItem::~GetTracksResponseItem()
{
}


} // ONVIF


#include "ONVIF/GetTracksResponseList.h"


namespace ONVIF {


GetTracksResponseList::GetTracksResponseList():
	_track()
{
}


GetTracksResponseList::GetTracksResponseList(const std::vector<GetTracksResponseItem>& track):
	_track(track)
{
}


GetTracksResponseList::~GetTracksResponseList()
{
}


} // ONVIF


#include "ONVIF/H264Configuration.h"


namespace ONVIF {


H264Configuration::H264Configuration():
	_govLength(),
	_h264Profile()
{
}


H264Configuration::H264Configuration(int govLength, const std::string& h264Profile):
	_govLength(govLength),
	_h264Profile(h264Profile)
{
}


H264Configuration::~H264Configuration()
{
}


} // ONVIF


#include "ONVIF/H264DecOptions.h"


namespace ONVIF {


H264DecOptions::H264DecOptions():
	_resolutionsAvailable(),
	_supportedH264Profiles(),
	_supportedInputBitrate(),
	_supportedFrameRate()
{
}


H264DecOptions::H264DecOptions(const std::vector<VideoResolution>& resolutionsAvailable, const std::vector<std::string>& supportedH264Profiles, const IntRange& supportedInputBitrate, const IntRange& supportedFrameRate):
	_resolutionsAvailable(resolutionsAvailable),
	_supportedH264Profiles(supportedH264Profiles),
	_supportedInputBitrate(supportedInputBitrate),
	_supportedFrameRate(supportedFrameRate)
{
}


H264DecOptions::~H264DecOptions()
{
}


} // ONVIF


#include "ONVIF/H264Options.h"


namespace ONVIF {


H264Options::H264Options():
	_resolutionsAvailable(),
	_govLengthRange(),
	_frameRateRange(),
	_encodingIntervalRange(),
	_h264ProfilesSupported()
{
}


H264Options::H264Options(const std::vector<VideoResolution>& resolutionsAvailable, const IntRange& govLengthRange, const IntRange& frameRateRange, const IntRange& encodingIntervalRange, const std::vector<std::string>& h264ProfilesSupported):
	_resolutionsAvailable(resolutionsAvailable),
	_govLengthRange(govLengthRange),
	_frameRateRange(frameRateRange),
	_encodingIntervalRange(encodingIntervalRange),
	_h264ProfilesSupported(h264ProfilesSupported)
{
}


H264Options::~H264Options()
{
}


} // ONVIF


#include "ONVIF/H264Options2.h"


namespace ONVIF {


H264Options2::H264Options2():
	_bitrateRange()
{
}


H264Options2::H264Options2(const std::vector<VideoResolution>& resolutionsAvailable, const IntRange& govLengthRange, const IntRange& frameRateRange, const IntRange& encodingIntervalRange, const std::vector<std::string>& h264ProfilesSupported, const IntRange& bitrateRange):
	H264Options(resolutionsAvailable, govLengthRange, frameRateRange, encodingIntervalRange, h264ProfilesSupported),
	_bitrateRange(bitrateRange)
{
}


H264Options2::~H264Options2()
{
}


} // ONVIF


#include "ONVIF/HostnameInformation.h"


namespace ONVIF {


HostnameInformation::HostnameInformation():
	_fromDHCP(),
	_name(),
	_extension()
{
}


HostnameInformation::HostnameInformation(bool fromDHCP, const Poco::Optional<std::string>& name, const Poco::SharedPtr<HostnameInformationExtension>& extension):
	_fromDHCP(fromDHCP),
	_name(name),
	_extension(extension)
{
}


HostnameInformation::~HostnameInformation()
{
}


} // ONVIF


#include "ONVIF/HostnameInformationExtension.h"


namespace ONVIF {


HostnameInformationExtension::HostnameInformationExtension()
{
}


HostnameInformationExtension::~HostnameInformationExtension()
{
}


} // ONVIF


#include "ONVIF/IOCapabilities.h"


namespace ONVIF {


IOCapabilities::IOCapabilities():
	_inputConnectors(),
	_relayOutputs(),
	_extension()
{
}


IOCapabilities::IOCapabilities(Poco::Optional<int> inputConnectors, Poco::Optional<int> relayOutputs, const Poco::SharedPtr<IOCapabilitiesExtension>& extension):
	_inputConnectors(inputConnectors),
	_relayOutputs(relayOutputs),
	_extension(extension)
{
}


IOCapabilities::~IOCapabilities()
{
}


} // ONVIF


#include "ONVIF/IOCapabilitiesExtension.h"


namespace ONVIF {


IOCapabilitiesExtension::IOCapabilitiesExtension():
	_auxiliary(),
	_auxiliaryCommands(),
	_extension()
{
}


IOCapabilitiesExtension::IOCapabilitiesExtension(Poco::Optional<bool> auxiliary, const std::vector<std::string>& auxiliaryCommands, const IOCapabilitiesExtension2& extension):
	_auxiliary(auxiliary),
	_auxiliaryCommands(auxiliaryCommands),
	_extension(extension)
{
}


IOCapabilitiesExtension::~IOCapabilitiesExtension()
{
}


} // ONVIF


#include "ONVIF/IOCapabilitiesExtension2.h"


namespace ONVIF {


IOCapabilitiesExtension2::IOCapabilitiesExtension2()
{
}


IOCapabilitiesExtension2::~IOCapabilitiesExtension2()
{
}


} // ONVIF


#include "ONVIF/IPAddress.h"


namespace ONVIF {


IPAddress::IPAddress():
	_type(),
	_iPv4Address(),
	_iPv6Address()
{
}


IPAddress::IPAddress(const std::string& type, const Poco::Optional<std::string>& iPv4Address, const Poco::Optional<std::string>& iPv6Address):
	_type(type),
	_iPv4Address(iPv4Address),
	_iPv6Address(iPv6Address)
{
}


IPAddress::~IPAddress()
{
}


} // ONVIF


#include "ONVIF/IPAddressFilter.h"


namespace ONVIF {


IPAddressFilter::IPAddressFilter():
	_type(),
	_iPv4Address(),
	_iPv6Address(),
	_extension()
{
}


IPAddressFilter::IPAddressFilter(const Poco::SharedPtr<IPAddressFilter>& type, const std::vector<PrefixedIPv4Address>& iPv4Address, const std::vector<PrefixedIPv6Address>& iPv6Address, const Poco::SharedPtr<IPAddressFilterExtension>& extension):
	_type(type),
	_iPv4Address(iPv4Address),
	_iPv6Address(iPv6Address),
	_extension(extension)
{
}


IPAddressFilter::~IPAddressFilter()
{
}


} // ONVIF


#include "ONVIF/IPAddressFilterExtension.h"


namespace ONVIF {


IPAddressFilterExtension::IPAddressFilterExtension()
{
}


IPAddressFilterExtension::~IPAddressFilterExtension()
{
}


} // ONVIF


#include "ONVIF/IPv4Configuration.h"


namespace ONVIF {


IPv4Configuration::IPv4Configuration():
	_manual(),
	_linkLocal(),
	_fromDHCP(),
	_dHCP()
{
}


IPv4Configuration::IPv4Configuration(const std::vector<PrefixedIPv4Address>& manual, const Poco::SharedPtr<PrefixedIPv4Address>& linkLocal, const Poco::SharedPtr<PrefixedIPv4Address>& fromDHCP, bool dHCP):
	_manual(manual),
	_linkLocal(linkLocal),
	_fromDHCP(fromDHCP),
	_dHCP(dHCP)
{
}


IPv4Configuration::~IPv4Configuration()
{
}


} // ONVIF


#include "ONVIF/IPv4NetworkInterface.h"


namespace ONVIF {


IPv4NetworkInterface::IPv4NetworkInterface():
	_enabled(),
	_config()
{
}


IPv4NetworkInterface::IPv4NetworkInterface(bool enabled, const IPv4Configuration& config):
	_enabled(enabled),
	_config(config)
{
}


IPv4NetworkInterface::~IPv4NetworkInterface()
{
}


} // ONVIF


#include "ONVIF/IPv4NetworkInterfaceSetConfiguration.h"


namespace ONVIF {


IPv4NetworkInterfaceSetConfiguration::IPv4NetworkInterfaceSetConfiguration():
	_enabled(),
	_manual(),
	_dHCP()
{
}


IPv4NetworkInterfaceSetConfiguration::IPv4NetworkInterfaceSetConfiguration(Poco::Optional<bool> enabled, const std::vector<PrefixedIPv4Address>& manual, Poco::Optional<bool> dHCP):
	_enabled(enabled),
	_manual(manual),
	_dHCP(dHCP)
{
}


IPv4NetworkInterfaceSetConfiguration::~IPv4NetworkInterfaceSetConfiguration()
{
}


} // ONVIF


#include "ONVIF/IPv6Configuration.h"


namespace ONVIF {


IPv6Configuration::IPv6Configuration():
	_acceptRouterAdvert(),
	_dHCP(),
	_manual(),
	_linkLocal(),
	_fromDHCP(),
	_fromRA(),
	_extension()
{
}


IPv6Configuration::IPv6Configuration(Poco::Optional<bool> acceptRouterAdvert, const std::string& dHCP, const std::vector<PrefixedIPv6Address>& manual, const std::vector<PrefixedIPv6Address>& linkLocal, const std::vector<PrefixedIPv6Address>& fromDHCP, const std::vector<PrefixedIPv6Address>& fromRA, const Poco::SharedPtr<IPv6ConfigurationExtension>& extension):
	_acceptRouterAdvert(acceptRouterAdvert),
	_dHCP(dHCP),
	_manual(manual),
	_linkLocal(linkLocal),
	_fromDHCP(fromDHCP),
	_fromRA(fromRA),
	_extension(extension)
{
}


IPv6Configuration::~IPv6Configuration()
{
}


} // ONVIF


#include "ONVIF/IPv6ConfigurationExtension.h"


namespace ONVIF {


IPv6ConfigurationExtension::IPv6ConfigurationExtension()
{
}


IPv6ConfigurationExtension::~IPv6ConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/IPv6NetworkInterface.h"


namespace ONVIF {


IPv6NetworkInterface::IPv6NetworkInterface():
	_enabled(),
	_config()
{
}


IPv6NetworkInterface::IPv6NetworkInterface(bool enabled, const Poco::SharedPtr<IPv6Configuration>& config):
	_enabled(enabled),
	_config(config)
{
}


IPv6NetworkInterface::~IPv6NetworkInterface()
{
}


} // ONVIF


#include "ONVIF/IPv6NetworkInterfaceSetConfiguration.h"


namespace ONVIF {


IPv6NetworkInterfaceSetConfiguration::IPv6NetworkInterfaceSetConfiguration():
	_enabled(),
	_acceptRouterAdvert(),
	_manual(),
	_dHCP()
{
}


IPv6NetworkInterfaceSetConfiguration::IPv6NetworkInterfaceSetConfiguration(Poco::Optional<bool> enabled, Poco::Optional<bool> acceptRouterAdvert, const std::vector<PrefixedIPv6Address>& manual, const Poco::Optional<std::string>& dHCP):
	_enabled(enabled),
	_acceptRouterAdvert(acceptRouterAdvert),
	_manual(manual),
	_dHCP(dHCP)
{
}


IPv6NetworkInterfaceSetConfiguration::~IPv6NetworkInterfaceSetConfiguration()
{
}


} // ONVIF


#include "ONVIF/ImageStabilization.h"


namespace ONVIF {


ImageStabilization::ImageStabilization():
	_mode(),
	_level(),
	_extension()
{
}


ImageStabilization::ImageStabilization(const std::string& mode, Poco::Optional<float> level, const Poco::SharedPtr<ImageStabilizationExtension>& extension):
	_mode(mode),
	_level(level),
	_extension(extension)
{
}


ImageStabilization::~ImageStabilization()
{
}


} // ONVIF


#include "ONVIF/ImageStabilizationExtension.h"


namespace ONVIF {


ImageStabilizationExtension::ImageStabilizationExtension()
{
}


ImageStabilizationExtension::~ImageStabilizationExtension()
{
}


} // ONVIF


#include "ONVIF/ImageStabilizationOptions.h"


namespace ONVIF {


ImageStabilizationOptions::ImageStabilizationOptions():
	_mode(),
	_level(),
	_extension()
{
}


ImageStabilizationOptions::ImageStabilizationOptions(const std::vector<std::string>& mode, const Poco::SharedPtr<FloatRange>& level, const Poco::SharedPtr<ImageStabilizationOptionsExtension>& extension):
	_mode(mode),
	_level(level),
	_extension(extension)
{
}


ImageStabilizationOptions::~ImageStabilizationOptions()
{
}


} // ONVIF


#include "ONVIF/ImageStabilizationOptionsExtension.h"


namespace ONVIF {


ImageStabilizationOptionsExtension::ImageStabilizationOptionsExtension()
{
}


ImageStabilizationOptionsExtension::~ImageStabilizationOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/ImagingCapabilities.h"


namespace ONVIF {


ImagingCapabilities::ImagingCapabilities():
	_xAddr()
{
}


ImagingCapabilities::ImagingCapabilities(const Poco::URI& xAddr):
	_xAddr(xAddr)
{
}


ImagingCapabilities::~ImagingCapabilities()
{
}


} // ONVIF


#include "ONVIF/ImagingOptions.h"


namespace ONVIF {


ImagingOptions::ImagingOptions():
	_backlightCompensation(),
	_brightness(),
	_colorSaturation(),
	_contrast(),
	_exposure(),
	_focus(),
	_irCutFilterModes(),
	_sharpness(),
	_wideDynamicRange(),
	_whiteBalance()
{
}


ImagingOptions::ImagingOptions(const BacklightCompensationOptions& backlightCompensation, const FloatRange& brightness, const FloatRange& colorSaturation, const FloatRange& contrast, const ExposureOptions& exposure, const FocusOptions& focus, const std::vector<std::string>& irCutFilterModes, const FloatRange& sharpness, const WideDynamicRangeOptions& wideDynamicRange, const WhiteBalanceOptions& whiteBalance):
	_backlightCompensation(backlightCompensation),
	_brightness(brightness),
	_colorSaturation(colorSaturation),
	_contrast(contrast),
	_exposure(exposure),
	_focus(focus),
	_irCutFilterModes(irCutFilterModes),
	_sharpness(sharpness),
	_wideDynamicRange(wideDynamicRange),
	_whiteBalance(whiteBalance)
{
}


ImagingOptions::~ImagingOptions()
{
}


} // ONVIF


#include "ONVIF/ImagingOptions20.h"


namespace ONVIF {


ImagingOptions20::ImagingOptions20():
	_backlightCompensation(),
	_brightness(),
	_colorSaturation(),
	_contrast(),
	_exposure(),
	_focus(),
	_irCutFilterModes(),
	_sharpness(),
	_wideDynamicRange(),
	_whiteBalance(),
	_extension()
{
}


ImagingOptions20::ImagingOptions20(const Poco::SharedPtr<BacklightCompensationOptions20>& backlightCompensation, const Poco::SharedPtr<FloatRange>& brightness, const Poco::SharedPtr<FloatRange>& colorSaturation, const Poco::SharedPtr<FloatRange>& contrast, const Poco::SharedPtr<ExposureOptions20>& exposure, const Poco::SharedPtr<FocusOptions20>& focus, const std::vector<std::string>& irCutFilterModes, const Poco::SharedPtr<FloatRange>& sharpness, const Poco::SharedPtr<WideDynamicRangeOptions20>& wideDynamicRange, const Poco::SharedPtr<WhiteBalanceOptions20>& whiteBalance, const Poco::SharedPtr<ImagingOptions20Extension>& extension):
	_backlightCompensation(backlightCompensation),
	_brightness(brightness),
	_colorSaturation(colorSaturation),
	_contrast(contrast),
	_exposure(exposure),
	_focus(focus),
	_irCutFilterModes(irCutFilterModes),
	_sharpness(sharpness),
	_wideDynamicRange(wideDynamicRange),
	_whiteBalance(whiteBalance),
	_extension(extension)
{
}


ImagingOptions20::~ImagingOptions20()
{
}


} // ONVIF


#include "ONVIF/ImagingOptions20Extension.h"


namespace ONVIF {


ImagingOptions20Extension::ImagingOptions20Extension():
	_imageStabilization(),
	_extension()
{
}


ImagingOptions20Extension::ImagingOptions20Extension(const Poco::SharedPtr<ImageStabilizationOptions>& imageStabilization, const Poco::SharedPtr<ImagingOptions20Extension2>& extension):
	_imageStabilization(imageStabilization),
	_extension(extension)
{
}


ImagingOptions20Extension::~ImagingOptions20Extension()
{
}


} // ONVIF


#include "ONVIF/ImagingOptions20Extension2.h"


namespace ONVIF {


ImagingOptions20Extension2::ImagingOptions20Extension2():
	_irCutFilterAutoAdjustment(),
	_extension()
{
}


ImagingOptions20Extension2::ImagingOptions20Extension2(const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptions>& irCutFilterAutoAdjustment, const Poco::SharedPtr<ImagingOptions20Extension3>& extension):
	_irCutFilterAutoAdjustment(irCutFilterAutoAdjustment),
	_extension(extension)
{
}


ImagingOptions20Extension2::~ImagingOptions20Extension2()
{
}


} // ONVIF


#include "ONVIF/ImagingOptions20Extension3.h"


namespace ONVIF {


ImagingOptions20Extension3::ImagingOptions20Extension3():
	_toneCompensationOptions(),
	_defoggingOptions(),
	_noiseReductionOptions(),
	_extension()
{
}


ImagingOptions20Extension3::ImagingOptions20Extension3(const Poco::SharedPtr<ToneCompensationOptions>& toneCompensationOptions, const Poco::SharedPtr<DefoggingOptions>& defoggingOptions, const Poco::SharedPtr<NoiseReductionOptions>& noiseReductionOptions, const Poco::SharedPtr<ImagingOptions20Extension4>& extension):
	_toneCompensationOptions(toneCompensationOptions),
	_defoggingOptions(defoggingOptions),
	_noiseReductionOptions(noiseReductionOptions),
	_extension(extension)
{
}


ImagingOptions20Extension3::~ImagingOptions20Extension3()
{
}


} // ONVIF


#include "ONVIF/ImagingOptions20Extension4.h"


namespace ONVIF {


ImagingOptions20Extension4::ImagingOptions20Extension4()
{
}


ImagingOptions20Extension4::~ImagingOptions20Extension4()
{
}


} // ONVIF


#include "ONVIF/ImagingSettings.h"


namespace ONVIF {


ImagingSettings::ImagingSettings():
	_backlightCompensation(),
	_brightness(),
	_colorSaturation(),
	_contrast(),
	_exposure(),
	_focus(),
	_irCutFilter(),
	_sharpness(),
	_wideDynamicRange(),
	_whiteBalance(),
	_extension()
{
}


ImagingSettings::ImagingSettings(const Poco::SharedPtr<BacklightCompensation>& backlightCompensation, Poco::Optional<float> brightness, Poco::Optional<float> colorSaturation, Poco::Optional<float> contrast, const Poco::SharedPtr<Exposure>& exposure, const Poco::SharedPtr<FocusConfiguration>& focus, const Poco::Optional<std::string>& irCutFilter, Poco::Optional<float> sharpness, const Poco::SharedPtr<WideDynamicRange>& wideDynamicRange, const Poco::SharedPtr<WhiteBalance>& whiteBalance, const Poco::SharedPtr<ImagingSettingsExtension>& extension):
	_backlightCompensation(backlightCompensation),
	_brightness(brightness),
	_colorSaturation(colorSaturation),
	_contrast(contrast),
	_exposure(exposure),
	_focus(focus),
	_irCutFilter(irCutFilter),
	_sharpness(sharpness),
	_wideDynamicRange(wideDynamicRange),
	_whiteBalance(whiteBalance),
	_extension(extension)
{
}


ImagingSettings::~ImagingSettings()
{
}


} // ONVIF


#include "ONVIF/ImagingSettings20.h"


namespace ONVIF {


ImagingSettings20::ImagingSettings20():
	_backlightCompensation(),
	_brightness(),
	_colorSaturation(),
	_contrast(),
	_exposure(),
	_focus(),
	_irCutFilter(),
	_sharpness(),
	_wideDynamicRange(),
	_whiteBalance(),
	_extension()
{
}


ImagingSettings20::ImagingSettings20(const Poco::SharedPtr<BacklightCompensation20>& backlightCompensation, Poco::Optional<float> brightness, Poco::Optional<float> colorSaturation, Poco::Optional<float> contrast, const Poco::SharedPtr<Exposure20>& exposure, const Poco::SharedPtr<FocusConfiguration20>& focus, const Poco::Optional<std::string>& irCutFilter, Poco::Optional<float> sharpness, const Poco::SharedPtr<WideDynamicRange20>& wideDynamicRange, const Poco::SharedPtr<WhiteBalance20>& whiteBalance, const Poco::SharedPtr<ImagingSettingsExtension20>& extension):
	_backlightCompensation(backlightCompensation),
	_brightness(brightness),
	_colorSaturation(colorSaturation),
	_contrast(contrast),
	_exposure(exposure),
	_focus(focus),
	_irCutFilter(irCutFilter),
	_sharpness(sharpness),
	_wideDynamicRange(wideDynamicRange),
	_whiteBalance(whiteBalance),
	_extension(extension)
{
}


ImagingSettings20::~ImagingSettings20()
{
}


} // ONVIF


#include "ONVIF/ImagingSettingsExtension.h"


namespace ONVIF {


ImagingSettingsExtension::ImagingSettingsExtension()
{
}


ImagingSettingsExtension::~ImagingSettingsExtension()
{
}


} // ONVIF


#include "ONVIF/ImagingSettingsExtension20.h"


namespace ONVIF {


ImagingSettingsExtension20::ImagingSettingsExtension20():
	_imageStabilization(),
	_extension()
{
}


ImagingSettingsExtension20::ImagingSettingsExtension20(const Poco::SharedPtr<ImageStabilization>& imageStabilization, const Poco::SharedPtr<ImagingSettingsExtension202>& extension):
	_imageStabilization(imageStabilization),
	_extension(extension)
{
}


ImagingSettingsExtension20::~ImagingSettingsExtension20()
{
}


} // ONVIF


#include "ONVIF/ImagingSettingsExtension202.h"


namespace ONVIF {


ImagingSettingsExtension202::ImagingSettingsExtension202():
	_irCutFilterAutoAdjustment(),
	_extension()
{
}


ImagingSettingsExtension202::ImagingSettingsExtension202(const std::vector<IrCutFilterAutoAdjustment>& irCutFilterAutoAdjustment, const Poco::SharedPtr<ImagingSettingsExtension203>& extension):
	_irCutFilterAutoAdjustment(irCutFilterAutoAdjustment),
	_extension(extension)
{
}


ImagingSettingsExtension202::~ImagingSettingsExtension202()
{
}


} // ONVIF


#include "ONVIF/ImagingSettingsExtension203.h"


namespace ONVIF {


ImagingSettingsExtension203::ImagingSettingsExtension203():
	_toneCompensation(),
	_defogging(),
	_noiseReduction(),
	_extension()
{
}


ImagingSettingsExtension203::ImagingSettingsExtension203(const Poco::SharedPtr<ToneCompensation>& toneCompensation, const Poco::SharedPtr<Defogging>& defogging, const Poco::SharedPtr<NoiseReduction>& noiseReduction, const Poco::SharedPtr<ImagingSettingsExtension204>& extension):
	_toneCompensation(toneCompensation),
	_defogging(defogging),
	_noiseReduction(noiseReduction),
	_extension(extension)
{
}


ImagingSettingsExtension203::~ImagingSettingsExtension203()
{
}


} // ONVIF


#include "ONVIF/ImagingSettingsExtension204.h"


namespace ONVIF {


ImagingSettingsExtension204::ImagingSettingsExtension204()
{
}


ImagingSettingsExtension204::~ImagingSettingsExtension204()
{
}


} // ONVIF


#include "ONVIF/ImagingStatus.h"


namespace ONVIF {


ImagingStatus::ImagingStatus():
	_focusStatus()
{
}


ImagingStatus::ImagingStatus(const FocusStatus& focusStatus):
	_focusStatus(focusStatus)
{
}


ImagingStatus::~ImagingStatus()
{
}


} // ONVIF


#include "ONVIF/ImagingStatus20.h"


namespace ONVIF {


ImagingStatus20::ImagingStatus20():
	_focusStatus20(),
	_extension()
{
}


ImagingStatus20::ImagingStatus20(const Poco::SharedPtr<FocusStatus20>& focusStatus20, const Poco::SharedPtr<ImagingStatus20Extension>& extension):
	_focusStatus20(focusStatus20),
	_extension(extension)
{
}


ImagingStatus20::~ImagingStatus20()
{
}


} // ONVIF


#include "ONVIF/ImagingStatus20Extension.h"


namespace ONVIF {


ImagingStatus20Extension::ImagingStatus20Extension()
{
}


ImagingStatus20Extension::~ImagingStatus20Extension()
{
}


} // ONVIF


#include "ONVIF/IntItems.h"


namespace ONVIF {


IntItems::IntItems():
	_items()
{
}


IntItems::IntItems(const std::vector<int>& items):
	_items(items)
{
}


IntItems::~IntItems()
{
}


} // ONVIF


#include "ONVIF/IntRange.h"


namespace ONVIF {


IntRange::IntRange():
	_min(),
	_max()
{
}


IntRange::IntRange(int min, int max):
	_min(min),
	_max(max)
{
}


IntRange::~IntRange()
{
}


} // ONVIF


#include "ONVIF/IntRectangle.h"


namespace ONVIF {


IntRectangle::IntRectangle():
	_x(),
	_y(),
	_width(),
	_height()
{
}


IntRectangle::IntRectangle(int x, int y, int width, int height):
	_x(x),
	_y(y),
	_width(width),
	_height(height)
{
}


IntRectangle::~IntRectangle()
{
}


} // ONVIF


#include "ONVIF/IntRectangleRange.h"


namespace ONVIF {


IntRectangleRange::IntRectangleRange():
	_xRange(),
	_yRange(),
	_widthRange(),
	_heightRange()
{
}


IntRectangleRange::IntRectangleRange(const IntRange& xRange, const IntRange& yRange, const IntRange& widthRange, const IntRange& heightRange):
	_xRange(xRange),
	_yRange(yRange),
	_widthRange(widthRange),
	_heightRange(heightRange)
{
}


IntRectangleRange::~IntRectangleRange()
{
}


} // ONVIF


#include "ONVIF/IrCutFilterAutoAdjustment.h"


namespace ONVIF {


IrCutFilterAutoAdjustment::IrCutFilterAutoAdjustment():
	_boundaryType(),
	_boundaryOffset(),
	_responseTime(),
	_extension()
{
}


IrCutFilterAutoAdjustment::IrCutFilterAutoAdjustment(const std::string& boundaryType, Poco::Optional<float> boundaryOffset, const Poco::Optional<std::string>& responseTime, const Poco::SharedPtr<IrCutFilterAutoAdjustmentExtension>& extension):
	_boundaryType(boundaryType),
	_boundaryOffset(boundaryOffset),
	_responseTime(responseTime),
	_extension(extension)
{
}


IrCutFilterAutoAdjustment::~IrCutFilterAutoAdjustment()
{
}


} // ONVIF


#include "ONVIF/IrCutFilterAutoAdjustmentExtension.h"


namespace ONVIF {


IrCutFilterAutoAdjustmentExtension::IrCutFilterAutoAdjustmentExtension()
{
}


IrCutFilterAutoAdjustmentExtension::~IrCutFilterAutoAdjustmentExtension()
{
}


} // ONVIF


#include "ONVIF/IrCutFilterAutoAdjustmentOptions.h"


namespace ONVIF {


IrCutFilterAutoAdjustmentOptions::IrCutFilterAutoAdjustmentOptions():
	_boundaryType(),
	_boundaryOffset(),
	_responseTimeRange(),
	_extension()
{
}


IrCutFilterAutoAdjustmentOptions::IrCutFilterAutoAdjustmentOptions(const std::vector<std::string>& boundaryType, Poco::Optional<bool> boundaryOffset, const Poco::SharedPtr<DurationRange>& responseTimeRange, const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptionsExtension>& extension):
	_boundaryType(boundaryType),
	_boundaryOffset(boundaryOffset),
	_responseTimeRange(responseTimeRange),
	_extension(extension)
{
}


IrCutFilterAutoAdjustmentOptions::~IrCutFilterAutoAdjustmentOptions()
{
}


} // ONVIF


#include "ONVIF/IrCutFilterAutoAdjustmentOptionsExtension.h"


namespace ONVIF {


IrCutFilterAutoAdjustmentOptionsExtension::IrCutFilterAutoAdjustmentOptionsExtension()
{
}


IrCutFilterAutoAdjustmentOptionsExtension::~IrCutFilterAutoAdjustmentOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/ItemList.h"


namespace ONVIF {


ItemList::ItemList():
	_simpleItem(),
	_elementItem(),
	_extension()
{
}


ItemList::ItemList(const std::vector<SimpleItem>& simpleItem, const std::vector<ElementItem>& elementItem, const Poco::SharedPtr<ItemListExtension>& extension):
	_simpleItem(simpleItem),
	_elementItem(elementItem),
	_extension(extension)
{
}


ItemList::~ItemList()
{
}


} // ONVIF


#include "ONVIF/ItemListDescription.h"


namespace ONVIF {


ItemListDescription::ItemListDescription():
	_simpleItemDescription(),
	_elementItemDescription(),
	_extension()
{
}


ItemListDescription::ItemListDescription(const std::vector<SimpleItemDescription>& simpleItemDescription, const std::vector<ElementItemDescription>& elementItemDescription, const Poco::SharedPtr<ItemListDescriptionExtension>& extension):
	_simpleItemDescription(simpleItemDescription),
	_elementItemDescription(elementItemDescription),
	_extension(extension)
{
}


ItemListDescription::~ItemListDescription()
{
}


} // ONVIF


#include "ONVIF/ItemListDescriptionExtension.h"


namespace ONVIF {


ItemListDescriptionExtension::ItemListDescriptionExtension()
{
}


ItemListDescriptionExtension::~ItemListDescriptionExtension()
{
}


} // ONVIF


#include "ONVIF/ItemListExtension.h"


namespace ONVIF {


ItemListExtension::ItemListExtension()
{
}


ItemListExtension::~ItemListExtension()
{
}


} // ONVIF


#include "ONVIF/JpegDecOptions.h"


namespace ONVIF {


JpegDecOptions::JpegDecOptions():
	_resolutionsAvailable(),
	_supportedInputBitrate(),
	_supportedFrameRate()
{
}


JpegDecOptions::JpegDecOptions(const std::vector<VideoResolution>& resolutionsAvailable, const IntRange& supportedInputBitrate, const IntRange& supportedFrameRate):
	_resolutionsAvailable(resolutionsAvailable),
	_supportedInputBitrate(supportedInputBitrate),
	_supportedFrameRate(supportedFrameRate)
{
}


JpegDecOptions::~JpegDecOptions()
{
}


} // ONVIF


#include "ONVIF/JpegOptions.h"


namespace ONVIF {


JpegOptions::JpegOptions():
	_resolutionsAvailable(),
	_frameRateRange(),
	_encodingIntervalRange()
{
}


JpegOptions::JpegOptions(const std::vector<VideoResolution>& resolutionsAvailable, const IntRange& frameRateRange, const IntRange& encodingIntervalRange):
	_resolutionsAvailable(resolutionsAvailable),
	_frameRateRange(frameRateRange),
	_encodingIntervalRange(encodingIntervalRange)
{
}


JpegOptions::~JpegOptions()
{
}


} // ONVIF


#include "ONVIF/JpegOptions2.h"


namespace ONVIF {


JpegOptions2::JpegOptions2():
	_bitrateRange()
{
}


JpegOptions2::JpegOptions2(const std::vector<VideoResolution>& resolutionsAvailable, const IntRange& frameRateRange, const IntRange& encodingIntervalRange, const IntRange& bitrateRange):
	JpegOptions(resolutionsAvailable, frameRateRange, encodingIntervalRange),
	_bitrateRange(bitrateRange)
{
}


JpegOptions2::~JpegOptions2()
{
}


} // ONVIF


#include "ONVIF/Layout.h"


namespace ONVIF {


Layout::Layout():
	_paneLayout(),
	_extension()
{
}


Layout::Layout(const std::vector<PaneLayout>& paneLayout, const Poco::SharedPtr<LayoutExtension>& extension):
	_paneLayout(paneLayout),
	_extension(extension)
{
}


Layout::~Layout()
{
}


} // ONVIF


#include "ONVIF/LayoutExtension.h"


namespace ONVIF {


LayoutExtension::LayoutExtension()
{
}


LayoutExtension::~LayoutExtension()
{
}


} // ONVIF


#include "ONVIF/LayoutOptions.h"


namespace ONVIF {


LayoutOptions::LayoutOptions():
	_paneLayoutOptions(),
	_extension()
{
}


LayoutOptions::LayoutOptions(const std::vector<PaneLayoutOptions>& paneLayoutOptions, const Poco::SharedPtr<LayoutOptionsExtension>& extension):
	_paneLayoutOptions(paneLayoutOptions),
	_extension(extension)
{
}


LayoutOptions::~LayoutOptions()
{
}


} // ONVIF


#include "ONVIF/LayoutOptionsExtension.h"


namespace ONVIF {


LayoutOptionsExtension::LayoutOptionsExtension()
{
}


LayoutOptionsExtension::~LayoutOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/LensDescription.h"


namespace ONVIF {


LensDescription::LensDescription():
	_focalLength(),
	_offset(),
	_projection(),
	_xFactor()
{
}


LensDescription::LensDescription(Poco::Optional<float> focalLength, const LensOffset& offset, const std::vector<LensProjection>& projection, float xFactor):
	_focalLength(focalLength),
	_offset(offset),
	_projection(projection),
	_xFactor(xFactor)
{
}


LensDescription::~LensDescription()
{
}


} // ONVIF


#include "ONVIF/LensOffset.h"


namespace ONVIF {


LensOffset::LensOffset():
	_x(),
	_y()
{
}


LensOffset::LensOffset(Poco::Optional<float> x, Poco::Optional<float> y):
	_x(x),
	_y(y)
{
}


LensOffset::~LensOffset()
{
}


} // ONVIF


#include "ONVIF/LensProjection.h"


namespace ONVIF {


LensProjection::LensProjection():
	_angle(),
	_radius(),
	_transmittance()
{
}


LensProjection::LensProjection(float angle, float radius, Poco::Optional<float> transmittance):
	_angle(angle),
	_radius(radius),
	_transmittance(transmittance)
{
}


LensProjection::~LensProjection()
{
}


} // ONVIF


#include "ONVIF/LocalLocation.h"


namespace ONVIF {


LocalLocation::LocalLocation():
	_x(),
	_y(),
	_z()
{
}


LocalLocation::LocalLocation(Poco::Optional<float> x, Poco::Optional<float> y, Poco::Optional<float> z):
	_x(x),
	_y(y),
	_z(z)
{
}


LocalLocation::~LocalLocation()
{
}


} // ONVIF


#include "ONVIF/LocalOrientation.h"


namespace ONVIF {


LocalOrientation::LocalOrientation():
	_pan(),
	_tilt(),
	_roll()
{
}


LocalOrientation::LocalOrientation(Poco::Optional<float> pan, Poco::Optional<float> tilt, Poco::Optional<float> roll):
	_pan(pan),
	_tilt(tilt),
	_roll(roll)
{
}


LocalOrientation::~LocalOrientation()
{
}


} // ONVIF


#include "ONVIF/LocationEntity.h"


namespace ONVIF {


LocationEntity::LocationEntity():
	_entity(),
	_token(),
	_fixed(),
	_geoSource(),
	_autoGeo(),
	_geoLocation(),
	_geoOrientation(),
	_localLocation(),
	_localOrientation()
{
}


LocationEntity::LocationEntity(const Poco::Optional<std::string>& entity, const Poco::Optional<std::string>& token, Poco::Optional<bool> fixed, const Poco::Optional<Poco::URI>& geoSource, Poco::Optional<bool> autoGeo, const Poco::SharedPtr<GeoLocation>& geoLocation, const Poco::SharedPtr<GeoOrientation>& geoOrientation, const Poco::SharedPtr<LocalLocation>& localLocation, const Poco::SharedPtr<LocalOrientation>& localOrientation):
	_entity(entity),
	_token(token),
	_fixed(fixed),
	_geoSource(geoSource),
	_autoGeo(autoGeo),
	_geoLocation(geoLocation),
	_geoOrientation(geoOrientation),
	_localLocation(localLocation),
	_localOrientation(localOrientation)
{
}


LocationEntity::~LocationEntity()
{
}


} // ONVIF


#include "ONVIF/MaximumNumberOfOSDs.h"


namespace ONVIF {


MaximumNumberOfOSDs::MaximumNumberOfOSDs():
	_total(),
	_image(),
	_plainText(),
	_date(),
	_time(),
	_dateAndTime()
{
}


MaximumNumberOfOSDs::MaximumNumberOfOSDs(int total, Poco::Optional<int> image, Poco::Optional<int> plainText, Poco::Optional<int> date, Poco::Optional<int> time, Poco::Optional<int> dateAndTime):
	_total(total),
	_image(image),
	_plainText(plainText),
	_date(date),
	_time(time),
	_dateAndTime(dateAndTime)
{
}


MaximumNumberOfOSDs::~MaximumNumberOfOSDs()
{
}


} // ONVIF


#include "ONVIF/MediaAttributes.h"


namespace ONVIF {


MediaAttributes::MediaAttributes():
	_recordingToken(),
	_trackAttributes(),
	_from(),
	_until()
{
}


MediaAttributes::MediaAttributes(const std::string& recordingToken, const std::vector<TrackAttributes>& trackAttributes, const Poco::DateTime& from, const Poco::DateTime& until):
	_recordingToken(recordingToken),
	_trackAttributes(trackAttributes),
	_from(from),
	_until(until)
{
}


MediaAttributes::~MediaAttributes()
{
}


} // ONVIF


#include "ONVIF/MediaCapabilities.h"


namespace ONVIF {


MediaCapabilities::MediaCapabilities():
	_xAddr(),
	_streamingCapabilities(),
	_extension()
{
}


MediaCapabilities::MediaCapabilities(const Poco::URI& xAddr, const RealTimeStreamingCapabilities& streamingCapabilities, const Poco::SharedPtr<MediaCapabilitiesExtension>& extension):
	_xAddr(xAddr),
	_streamingCapabilities(streamingCapabilities),
	_extension(extension)
{
}


MediaCapabilities::~MediaCapabilities()
{
}


} // ONVIF


#include "ONVIF/MediaCapabilitiesExtension.h"


namespace ONVIF {


MediaCapabilitiesExtension::MediaCapabilitiesExtension():
	_profileCapabilities()
{
}


MediaCapabilitiesExtension::MediaCapabilitiesExtension(const ProfileCapabilities& profileCapabilities):
	_profileCapabilities(profileCapabilities)
{
}


MediaCapabilitiesExtension::~MediaCapabilitiesExtension()
{
}


} // ONVIF


#include "ONVIF/MediaUri.h"


namespace ONVIF {


MediaUri::MediaUri():
	_uri(),
	_invalidAfterConnect(),
	_invalidAfterReboot(),
	_timeout()
{
}


MediaUri::MediaUri(const Poco::URI& uri, bool invalidAfterConnect, bool invalidAfterReboot, const std::string& timeout):
	_uri(uri),
	_invalidAfterConnect(invalidAfterConnect),
	_invalidAfterReboot(invalidAfterReboot),
	_timeout(timeout)
{
}


MediaUri::~MediaUri()
{
}


} // ONVIF


#include "ONVIF/Message.h"


namespace ONVIF {


Message::Message():
	_utcTime(),
	_propertyOperation(),
	_source(),
	_key(),
	_data(),
	_extension()
{
}


Message::Message(const Poco::DateTime& utcTime, const Poco::Optional<std::string>& propertyOperation, const Poco::SharedPtr<ItemList>& source, const Poco::SharedPtr<ItemList>& key, const Poco::SharedPtr<ItemList>& data, const Poco::SharedPtr<MessageExtension>& extension):
	_utcTime(utcTime),
	_propertyOperation(propertyOperation),
	_source(source),
	_key(key),
	_data(data),
	_extension(extension)
{
}


Message::~Message()
{
}


} // ONVIF


#include "ONVIF/MessageDescription.h"


namespace ONVIF {


MessageDescription::MessageDescription():
	_isProperty(),
	_source(),
	_key(),
	_data(),
	_extension()
{
}


MessageDescription::MessageDescription(Poco::Optional<bool> isProperty, const Poco::SharedPtr<ItemListDescription>& source, const Poco::SharedPtr<ItemListDescription>& key, const Poco::SharedPtr<ItemListDescription>& data, const Poco::SharedPtr<MessageDescriptionExtension>& extension):
	_isProperty(isProperty),
	_source(source),
	_key(key),
	_data(data),
	_extension(extension)
{
}


MessageDescription::~MessageDescription()
{
}


} // ONVIF


#include "ONVIF/MessageDescriptionExtension.h"


namespace ONVIF {


MessageDescriptionExtension::MessageDescriptionExtension()
{
}


MessageDescriptionExtension::~MessageDescriptionExtension()
{
}


} // ONVIF


#include "ONVIF/MessageExtension.h"


namespace ONVIF {


MessageExtension::MessageExtension()
{
}


MessageExtension::~MessageExtension()
{
}


} // ONVIF


#include "ONVIF/Messages.h"


namespace ONVIF {


Messages::Messages():
	_parentTopic()
{
}


Messages::Messages(Poco::Optional<bool> isProperty, const Poco::SharedPtr<ItemListDescription>& source, const Poco::SharedPtr<ItemListDescription>& key, const Poco::SharedPtr<ItemListDescription>& data, const Poco::SharedPtr<MessageDescriptionExtension>& extension, const std::string& parentTopic):
	MessageDescription(isProperty, source, key, data, extension),
	_parentTopic(parentTopic)
{
}


Messages::~Messages()
{
}


} // ONVIF


#include "ONVIF/MetadataAttributes.h"


namespace ONVIF {


MetadataAttributes::MetadataAttributes():
	_ptzSpaces(),
	_canContainPTZ(),
	_canContainAnalytics(),
	_canContainNotifications()
{
}


MetadataAttributes::MetadataAttributes(const std::vector<std::vector<std::string>>& ptzSpaces, bool canContainPTZ, bool canContainAnalytics, bool canContainNotifications):
	_ptzSpaces(ptzSpaces),
	_canContainPTZ(canContainPTZ),
	_canContainAnalytics(canContainAnalytics),
	_canContainNotifications(canContainNotifications)
{
}


MetadataAttributes::~MetadataAttributes()
{
}


} // ONVIF


#include "ONVIF/MetadataConfiguration.h"


namespace ONVIF {


MetadataConfiguration::MetadataConfiguration():
	_compressionType(),
	_geoLocation(),
	_shapePolygon(),
	_pTZStatus(),
	_events(),
	_analytics(),
	_multicast(),
	_sessionTimeout(),
	_analyticsEngineConfiguration(),
	_extension()
{
}


MetadataConfiguration::MetadataConfiguration(const std::string& token, const std::string& name, int useCount, const Poco::Optional<std::string>& compressionType, Poco::Optional<bool> geoLocation, Poco::Optional<bool> shapePolygon, const Poco::SharedPtr<PTZFilter>& pTZStatus, const Poco::SharedPtr<EventSubscription>& events, Poco::Optional<bool> analytics, const MulticastConfiguration& multicast, const std::string& sessionTimeout, const Poco::SharedPtr<AnalyticsEngineConfiguration>& analyticsEngineConfiguration, const Poco::SharedPtr<MetadataConfigurationExtension>& extension):
	ConfigurationEntity(token, name, useCount),
	_compressionType(compressionType),
	_geoLocation(geoLocation),
	_shapePolygon(shapePolygon),
	_pTZStatus(pTZStatus),
	_events(events),
	_analytics(analytics),
	_multicast(multicast),
	_sessionTimeout(sessionTimeout),
	_analyticsEngineConfiguration(analyticsEngineConfiguration),
	_extension(extension)
{
}


MetadataConfiguration::~MetadataConfiguration()
{
}


} // ONVIF


#include "ONVIF/MetadataConfigurationExtension.h"


namespace ONVIF {


MetadataConfigurationExtension::MetadataConfigurationExtension()
{
}


MetadataConfigurationExtension::~MetadataConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/MetadataConfigurationOptions.h"


namespace ONVIF {


MetadataConfigurationOptions::MetadataConfigurationOptions():
	_geoLocation(),
	_maxContentFilterSize(),
	_pTZStatusFilterOptions(),
	_extension()
{
}


MetadataConfigurationOptions::MetadataConfigurationOptions(Poco::Optional<bool> geoLocation, Poco::Optional<int> maxContentFilterSize, const PTZStatusFilterOptions& pTZStatusFilterOptions, const Poco::SharedPtr<MetadataConfigurationOptionsExtension>& extension):
	_geoLocation(geoLocation),
	_maxContentFilterSize(maxContentFilterSize),
	_pTZStatusFilterOptions(pTZStatusFilterOptions),
	_extension(extension)
{
}


MetadataConfigurationOptions::~MetadataConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/MetadataConfigurationOptionsExtension.h"


namespace ONVIF {


MetadataConfigurationOptionsExtension::MetadataConfigurationOptionsExtension():
	_compressionType(),
	_extension()
{
}


MetadataConfigurationOptionsExtension::MetadataConfigurationOptionsExtension(const std::vector<std::string>& compressionType, const Poco::SharedPtr<MetadataConfigurationOptionsExtension2>& extension):
	_compressionType(compressionType),
	_extension(extension)
{
}


MetadataConfigurationOptionsExtension::~MetadataConfigurationOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/MetadataConfigurationOptionsExtension2.h"


namespace ONVIF {


MetadataConfigurationOptionsExtension2::MetadataConfigurationOptionsExtension2()
{
}


MetadataConfigurationOptionsExtension2::~MetadataConfigurationOptionsExtension2()
{
}


} // ONVIF


#include "ONVIF/MetadataFilter.h"


namespace ONVIF {


MetadataFilter::MetadataFilter():
	_metadataStreamFilter()
{
}


MetadataFilter::MetadataFilter(const std::string& metadataStreamFilter):
	_metadataStreamFilter(metadataStreamFilter)
{
}


MetadataFilter::~MetadataFilter()
{
}


} // ONVIF


#include "ONVIF/MetadataInput.h"


namespace ONVIF {


MetadataInput::MetadataInput():
	_metadataConfig(),
	_extension()
{
}


MetadataInput::MetadataInput(const std::vector<Config>& metadataConfig, const Poco::SharedPtr<MetadataInputExtension>& extension):
	_metadataConfig(metadataConfig),
	_extension(extension)
{
}


MetadataInput::~MetadataInput()
{
}


} // ONVIF


#include "ONVIF/MetadataInputExtension.h"


namespace ONVIF {


MetadataInputExtension::MetadataInputExtension()
{
}


MetadataInputExtension::~MetadataInputExtension()
{
}


} // ONVIF


#include "ONVIF/MotionExpression.h"


namespace ONVIF {


MotionExpression::MotionExpression():
	_type(),
	_expression()
{
}


MotionExpression::MotionExpression(const Poco::Optional<std::string>& type, const std::string& expression):
	_type(type),
	_expression(expression)
{
}


MotionExpression::~MotionExpression()
{
}


} // ONVIF


#include "ONVIF/MotionExpressionConfiguration.h"


namespace ONVIF {


MotionExpressionConfiguration::MotionExpressionConfiguration():
	_motionExpression()
{
}


MotionExpressionConfiguration::MotionExpressionConfiguration(const MotionExpression& motionExpression):
	_motionExpression(motionExpression)
{
}


MotionExpressionConfiguration::~MotionExpressionConfiguration()
{
}


} // ONVIF


#include "ONVIF/MoveOptions.h"


namespace ONVIF {


MoveOptions::MoveOptions():
	_absolute(),
	_relative(),
	_continuous()
{
}


MoveOptions::MoveOptions(const Poco::SharedPtr<AbsoluteFocusOptions>& absolute, const Poco::SharedPtr<RelativeFocusOptions>& relative, const Poco::SharedPtr<ContinuousFocusOptions>& continuous):
	_absolute(absolute),
	_relative(relative),
	_continuous(continuous)
{
}


MoveOptions::~MoveOptions()
{
}


} // ONVIF


#include "ONVIF/MoveOptions20.h"


namespace ONVIF {


MoveOptions20::MoveOptions20():
	_absolute(),
	_relative(),
	_continuous()
{
}


MoveOptions20::MoveOptions20(const Poco::SharedPtr<AbsoluteFocusOptions>& absolute, const Poco::SharedPtr<RelativeFocusOptions20>& relative, const Poco::SharedPtr<ContinuousFocusOptions>& continuous):
	_absolute(absolute),
	_relative(relative),
	_continuous(continuous)
{
}


MoveOptions20::~MoveOptions20()
{
}


} // ONVIF


#include "ONVIF/Mpeg4Configuration.h"


namespace ONVIF {


Mpeg4Configuration::Mpeg4Configuration():
	_govLength(),
	_mpeg4Profile()
{
}


Mpeg4Configuration::Mpeg4Configuration(int govLength, const std::string& mpeg4Profile):
	_govLength(govLength),
	_mpeg4Profile(mpeg4Profile)
{
}


Mpeg4Configuration::~Mpeg4Configuration()
{
}


} // ONVIF


#include "ONVIF/Mpeg4DecOptions.h"


namespace ONVIF {


Mpeg4DecOptions::Mpeg4DecOptions():
	_resolutionsAvailable(),
	_supportedMpeg4Profiles(),
	_supportedInputBitrate(),
	_supportedFrameRate()
{
}


Mpeg4DecOptions::Mpeg4DecOptions(const std::vector<VideoResolution>& resolutionsAvailable, const std::vector<std::string>& supportedMpeg4Profiles, const IntRange& supportedInputBitrate, const IntRange& supportedFrameRate):
	_resolutionsAvailable(resolutionsAvailable),
	_supportedMpeg4Profiles(supportedMpeg4Profiles),
	_supportedInputBitrate(supportedInputBitrate),
	_supportedFrameRate(supportedFrameRate)
{
}


Mpeg4DecOptions::~Mpeg4DecOptions()
{
}


} // ONVIF


#include "ONVIF/Mpeg4Options.h"


namespace ONVIF {


Mpeg4Options::Mpeg4Options():
	_resolutionsAvailable(),
	_govLengthRange(),
	_frameRateRange(),
	_encodingIntervalRange(),
	_mpeg4ProfilesSupported()
{
}


Mpeg4Options::Mpeg4Options(const std::vector<VideoResolution>& resolutionsAvailable, const IntRange& govLengthRange, const IntRange& frameRateRange, const IntRange& encodingIntervalRange, const std::vector<std::string>& mpeg4ProfilesSupported):
	_resolutionsAvailable(resolutionsAvailable),
	_govLengthRange(govLengthRange),
	_frameRateRange(frameRateRange),
	_encodingIntervalRange(encodingIntervalRange),
	_mpeg4ProfilesSupported(mpeg4ProfilesSupported)
{
}


Mpeg4Options::~Mpeg4Options()
{
}


} // ONVIF


#include "ONVIF/Mpeg4Options2.h"


namespace ONVIF {


Mpeg4Options2::Mpeg4Options2():
	_bitrateRange()
{
}


Mpeg4Options2::Mpeg4Options2(const std::vector<VideoResolution>& resolutionsAvailable, const IntRange& govLengthRange, const IntRange& frameRateRange, const IntRange& encodingIntervalRange, const std::vector<std::string>& mpeg4ProfilesSupported, const IntRange& bitrateRange):
	Mpeg4Options(resolutionsAvailable, govLengthRange, frameRateRange, encodingIntervalRange, mpeg4ProfilesSupported),
	_bitrateRange(bitrateRange)
{
}


Mpeg4Options2::~Mpeg4Options2()
{
}


} // ONVIF


#include "ONVIF/MulticastConfiguration.h"


namespace ONVIF {


MulticastConfiguration::MulticastConfiguration():
	_address(),
	_port(),
	_tTL(),
	_autoStart()
{
}


MulticastConfiguration::MulticastConfiguration(const IPAddress& address, int port, int tTL, bool autoStart):
	_address(address),
	_port(port),
	_tTL(tTL),
	_autoStart(autoStart)
{
}


MulticastConfiguration::~MulticastConfiguration()
{
}


} // ONVIF


#include "ONVIF/NTPInformation.h"


namespace ONVIF {


NTPInformation::NTPInformation():
	_fromDHCP(),
	_nTPFromDHCP(),
	_nTPManual(),
	_extension()
{
}


NTPInformation::NTPInformation(bool fromDHCP, const std::vector<NetworkHost>& nTPFromDHCP, const std::vector<NetworkHost>& nTPManual, const Poco::SharedPtr<NTPInformationExtension>& extension):
	_fromDHCP(fromDHCP),
	_nTPFromDHCP(nTPFromDHCP),
	_nTPManual(nTPManual),
	_extension(extension)
{
}


NTPInformation::~NTPInformation()
{
}


} // ONVIF


#include "ONVIF/NTPInformationExtension.h"


namespace ONVIF {


NTPInformationExtension::NTPInformationExtension()
{
}


NTPInformationExtension::~NTPInformationExtension()
{
}


} // ONVIF


#include "ONVIF/NetworkCapabilities.h"


namespace ONVIF {


NetworkCapabilities::NetworkCapabilities():
	_iPFilter(),
	_zeroConfiguration(),
	_iPVersion6(),
	_dynDNS(),
	_extension()
{
}


NetworkCapabilities::NetworkCapabilities(Poco::Optional<bool> iPFilter, Poco::Optional<bool> zeroConfiguration, Poco::Optional<bool> iPVersion6, Poco::Optional<bool> dynDNS, const Poco::SharedPtr<NetworkCapabilitiesExtension>& extension):
	_iPFilter(iPFilter),
	_zeroConfiguration(zeroConfiguration),
	_iPVersion6(iPVersion6),
	_dynDNS(dynDNS),
	_extension(extension)
{
}


NetworkCapabilities::~NetworkCapabilities()
{
}


} // ONVIF


#include "ONVIF/NetworkCapabilitiesExtension.h"


namespace ONVIF {


NetworkCapabilitiesExtension::NetworkCapabilitiesExtension():
	_dot11Configuration(),
	_extension()
{
}


NetworkCapabilitiesExtension::NetworkCapabilitiesExtension(Poco::Optional<bool> dot11Configuration, const Poco::SharedPtr<NetworkCapabilitiesExtension2>& extension):
	_dot11Configuration(dot11Configuration),
	_extension(extension)
{
}


NetworkCapabilitiesExtension::~NetworkCapabilitiesExtension()
{
}


} // ONVIF


#include "ONVIF/NetworkCapabilitiesExtension2.h"


namespace ONVIF {


NetworkCapabilitiesExtension2::NetworkCapabilitiesExtension2()
{
}


NetworkCapabilitiesExtension2::~NetworkCapabilitiesExtension2()
{
}


} // ONVIF


#include "ONVIF/NetworkGateway.h"


namespace ONVIF {


NetworkGateway::NetworkGateway():
	_iPv4Address(),
	_iPv6Address()
{
}


NetworkGateway::NetworkGateway(const std::vector<std::string>& iPv4Address, const std::vector<std::string>& iPv6Address):
	_iPv4Address(iPv4Address),
	_iPv6Address(iPv6Address)
{
}


NetworkGateway::~NetworkGateway()
{
}


} // ONVIF


#include "ONVIF/NetworkHost.h"


namespace ONVIF {


NetworkHost::NetworkHost():
	_type(),
	_iPv4Address(),
	_iPv6Address(),
	_dNSname(),
	_extension()
{
}


NetworkHost::NetworkHost(const Poco::SharedPtr<NetworkHost>& type, const Poco::Optional<std::string>& iPv4Address, const Poco::Optional<std::string>& iPv6Address, const Poco::Optional<std::string>& dNSname, const Poco::SharedPtr<NetworkHostExtension>& extension):
	_type(type),
	_iPv4Address(iPv4Address),
	_iPv6Address(iPv6Address),
	_dNSname(dNSname),
	_extension(extension)
{
}


NetworkHost::~NetworkHost()
{
}


} // ONVIF


#include "ONVIF/NetworkHostExtension.h"


namespace ONVIF {


NetworkHostExtension::NetworkHostExtension()
{
}


NetworkHostExtension::~NetworkHostExtension()
{
}


} // ONVIF


#include "ONVIF/NetworkInterface.h"


namespace ONVIF {


NetworkInterface::NetworkInterface():
	_enabled(),
	_info(),
	_link(),
	_iPv4(),
	_iPv6(),
	_extension()
{
}


NetworkInterface::NetworkInterface(const std::string& token, bool enabled, const Poco::SharedPtr<NetworkInterfaceInfo>& info, const Poco::SharedPtr<NetworkInterfaceLink>& link, const Poco::SharedPtr<IPv4NetworkInterface>& iPv4, const Poco::SharedPtr<IPv6NetworkInterface>& iPv6, const Poco::SharedPtr<NetworkInterfaceExtension>& extension):
	DeviceEntity(token),
	_enabled(enabled),
	_info(info),
	_link(link),
	_iPv4(iPv4),
	_iPv6(iPv6),
	_extension(extension)
{
}


NetworkInterface::~NetworkInterface()
{
}


} // ONVIF


#include "ONVIF/NetworkInterfaceConnectionSetting.h"


namespace ONVIF {


NetworkInterfaceConnectionSetting::NetworkInterfaceConnectionSetting():
	_autoNegotiation(),
	_speed(),
	_duplex()
{
}


NetworkInterfaceConnectionSetting::NetworkInterfaceConnectionSetting(bool autoNegotiation, int speed, const std::string& duplex):
	_autoNegotiation(autoNegotiation),
	_speed(speed),
	_duplex(duplex)
{
}


NetworkInterfaceConnectionSetting::~NetworkInterfaceConnectionSetting()
{
}


} // ONVIF


#include "ONVIF/NetworkInterfaceExtension.h"


namespace ONVIF {


NetworkInterfaceExtension::NetworkInterfaceExtension():
	_interfaceType(),
	_dot3(),
	_dot11(),
	_extension()
{
}


NetworkInterfaceExtension::NetworkInterfaceExtension(const int& interfaceType, const std::vector<Dot3Configuration>& dot3, const std::vector<Dot11Configuration>& dot11, const Poco::SharedPtr<NetworkInterfaceExtension2>& extension):
	_interfaceType(interfaceType),
	_dot3(dot3),
	_dot11(dot11),
	_extension(extension)
{
}


NetworkInterfaceExtension::~NetworkInterfaceExtension()
{
}


} // ONVIF


#include "ONVIF/NetworkInterfaceExtension2.h"


namespace ONVIF {


NetworkInterfaceExtension2::NetworkInterfaceExtension2()
{
}


NetworkInterfaceExtension2::~NetworkInterfaceExtension2()
{
}


} // ONVIF


#include "ONVIF/NetworkInterfaceInfo.h"


namespace ONVIF {


NetworkInterfaceInfo::NetworkInterfaceInfo():
	_name(),
	_hwAddress(),
	_mTU()
{
}


NetworkInterfaceInfo::NetworkInterfaceInfo(const Poco::Optional<std::string>& name, const std::string& hwAddress, Poco::Optional<int> mTU):
	_name(name),
	_hwAddress(hwAddress),
	_mTU(mTU)
{
}


NetworkInterfaceInfo::~NetworkInterfaceInfo()
{
}


} // ONVIF


#include "ONVIF/NetworkInterfaceLink.h"


namespace ONVIF {


NetworkInterfaceLink::NetworkInterfaceLink():
	_adminSettings(),
	_operSettings(),
	_interfaceType()
{
}


NetworkInterfaceLink::NetworkInterfaceLink(const NetworkInterfaceConnectionSetting& adminSettings, const NetworkInterfaceConnectionSetting& operSettings, const int& interfaceType):
	_adminSettings(adminSettings),
	_operSettings(operSettings),
	_interfaceType(interfaceType)
{
}


NetworkInterfaceLink::~NetworkInterfaceLink()
{
}


} // ONVIF


#include "ONVIF/NetworkInterfaceSetConfiguration.h"


namespace ONVIF {


NetworkInterfaceSetConfiguration::NetworkInterfaceSetConfiguration():
	_enabled(),
	_link(),
	_mTU(),
	_iPv4(),
	_iPv6(),
	_extension()
{
}


NetworkInterfaceSetConfiguration::NetworkInterfaceSetConfiguration(Poco::Optional<bool> enabled, const Poco::SharedPtr<NetworkInterfaceConnectionSetting>& link, Poco::Optional<int> mTU, const Poco::SharedPtr<IPv4NetworkInterfaceSetConfiguration>& iPv4, const Poco::SharedPtr<IPv6NetworkInterfaceSetConfiguration>& iPv6, const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension>& extension):
	_enabled(enabled),
	_link(link),
	_mTU(mTU),
	_iPv4(iPv4),
	_iPv6(iPv6),
	_extension(extension)
{
}


NetworkInterfaceSetConfiguration::~NetworkInterfaceSetConfiguration()
{
}


} // ONVIF


#include "ONVIF/NetworkInterfaceSetConfigurationExtension.h"


namespace ONVIF {


NetworkInterfaceSetConfigurationExtension::NetworkInterfaceSetConfigurationExtension():
	_dot3(),
	_dot11(),
	_extension()
{
}


NetworkInterfaceSetConfigurationExtension::NetworkInterfaceSetConfigurationExtension(const std::vector<Dot3Configuration>& dot3, const std::vector<Dot11Configuration>& dot11, const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension2>& extension):
	_dot3(dot3),
	_dot11(dot11),
	_extension(extension)
{
}


NetworkInterfaceSetConfigurationExtension::~NetworkInterfaceSetConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/NetworkInterfaceSetConfigurationExtension2.h"


namespace ONVIF {


NetworkInterfaceSetConfigurationExtension2::NetworkInterfaceSetConfigurationExtension2()
{
}


NetworkInterfaceSetConfigurationExtension2::~NetworkInterfaceSetConfigurationExtension2()
{
}


} // ONVIF


#include "ONVIF/NetworkProtocol.h"


namespace ONVIF {


NetworkProtocol::NetworkProtocol():
	_name(),
	_enabled(),
	_port(),
	_extension()
{
}


NetworkProtocol::NetworkProtocol(const Poco::SharedPtr<NetworkProtocol>& name, bool enabled, const std::vector<int>& port, const Poco::SharedPtr<NetworkProtocolExtension>& extension):
	_name(name),
	_enabled(enabled),
	_port(port),
	_extension(extension)
{
}


NetworkProtocol::~NetworkProtocol()
{
}


} // ONVIF


#include "ONVIF/NetworkProtocolExtension.h"


namespace ONVIF {


NetworkProtocolExtension::NetworkProtocolExtension()
{
}


NetworkProtocolExtension::~NetworkProtocolExtension()
{
}


} // ONVIF


#include "ONVIF/NetworkZeroConfiguration.h"


namespace ONVIF {


NetworkZeroConfiguration::NetworkZeroConfiguration():
	_interfaceToken(),
	_enabled(),
	_addresses(),
	_extension()
{
}


NetworkZeroConfiguration::NetworkZeroConfiguration(const std::string& interfaceToken, bool enabled, const std::vector<std::string>& addresses, const Poco::SharedPtr<NetworkZeroConfigurationExtension>& extension):
	_interfaceToken(interfaceToken),
	_enabled(enabled),
	_addresses(addresses),
	_extension(extension)
{
}


NetworkZeroConfiguration::~NetworkZeroConfiguration()
{
}


} // ONVIF


#include "ONVIF/NetworkZeroConfigurationExtension.h"


namespace ONVIF {


NetworkZeroConfigurationExtension::NetworkZeroConfigurationExtension():
	_additional(),
	_extension()
{
}


NetworkZeroConfigurationExtension::NetworkZeroConfigurationExtension(const std::vector<NetworkZeroConfiguration>& additional, const Poco::SharedPtr<NetworkZeroConfigurationExtension2>& extension):
	_additional(additional),
	_extension(extension)
{
}


NetworkZeroConfigurationExtension::~NetworkZeroConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/NetworkZeroConfigurationExtension2.h"


namespace ONVIF {


NetworkZeroConfigurationExtension2::NetworkZeroConfigurationExtension2()
{
}


NetworkZeroConfigurationExtension2::~NetworkZeroConfigurationExtension2()
{
}


} // ONVIF


#include "ONVIF/NoiseReduction.h"


namespace ONVIF {


NoiseReduction::NoiseReduction():
	_level()
{
}


NoiseReduction::NoiseReduction(float level):
	_level(level)
{
}


NoiseReduction::~NoiseReduction()
{
}


} // ONVIF


#include "ONVIF/NoiseReductionOptions.h"


namespace ONVIF {


NoiseReductionOptions::NoiseReductionOptions():
	_level()
{
}


NoiseReductionOptions::NoiseReductionOptions(bool level):
	_level(level)
{
}


NoiseReductionOptions::~NoiseReductionOptions()
{
}


} // ONVIF


#include "ONVIF/OSDColor.h"


namespace ONVIF {


OSDColor::OSDColor():
	_transparent(),
	_color()
{
}


OSDColor::OSDColor(Poco::Optional<int> transparent, const Color& color):
	_transparent(transparent),
	_color(color)
{
}


OSDColor::~OSDColor()
{
}


} // ONVIF


#include "ONVIF/OSDColorOptions.h"


namespace ONVIF {


OSDColorOptions::OSDColorOptions():
	_color(),
	_transparent(),
	_extension()
{
}


OSDColorOptions::OSDColorOptions(const Poco::SharedPtr<ColorOptions>& color, const Poco::SharedPtr<IntRange>& transparent, const Poco::SharedPtr<OSDColorOptionsExtension>& extension):
	_color(color),
	_transparent(transparent),
	_extension(extension)
{
}


OSDColorOptions::~OSDColorOptions()
{
}


} // ONVIF


#include "ONVIF/OSDColorOptionsExtension.h"


namespace ONVIF {


OSDColorOptionsExtension::OSDColorOptionsExtension()
{
}


OSDColorOptionsExtension::~OSDColorOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/OSDConfiguration.h"


namespace ONVIF {


OSDConfiguration::OSDConfiguration():
	_videoSourceConfigurationToken(),
	_type(),
	_position(),
	_textString(),
	_image(),
	_extension()
{
}


OSDConfiguration::OSDConfiguration(const std::string& token, const OSDReference& videoSourceConfigurationToken, const std::string& type, const OSDPosConfiguration& position, const Poco::SharedPtr<OSDTextConfiguration>& textString, const Poco::SharedPtr<OSDImgConfiguration>& image, const Poco::SharedPtr<OSDConfigurationExtension>& extension):
	DeviceEntity(token),
	_videoSourceConfigurationToken(videoSourceConfigurationToken),
	_type(type),
	_position(position),
	_textString(textString),
	_image(image),
	_extension(extension)
{
}


OSDConfiguration::~OSDConfiguration()
{
}


} // ONVIF


#include "ONVIF/OSDConfigurationExtension.h"


namespace ONVIF {


OSDConfigurationExtension::OSDConfigurationExtension()
{
}


OSDConfigurationExtension::~OSDConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/OSDConfigurationOptions.h"


namespace ONVIF {


OSDConfigurationOptions::OSDConfigurationOptions():
	_maximumNumberOfOSDs(),
	_type(),
	_positionOption(),
	_textOption(),
	_imageOption(),
	_extension()
{
}


OSDConfigurationOptions::OSDConfigurationOptions(const MaximumNumberOfOSDs& maximumNumberOfOSDs, const std::vector<std::string>& type, const std::vector<std::string>& positionOption, const Poco::SharedPtr<OSDTextOptions>& textOption, const Poco::SharedPtr<OSDImgOptions>& imageOption, const Poco::SharedPtr<OSDConfigurationOptionsExtension>& extension):
	_maximumNumberOfOSDs(maximumNumberOfOSDs),
	_type(type),
	_positionOption(positionOption),
	_textOption(textOption),
	_imageOption(imageOption),
	_extension(extension)
{
}


OSDConfigurationOptions::~OSDConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/OSDConfigurationOptionsExtension.h"


namespace ONVIF {


OSDConfigurationOptionsExtension::OSDConfigurationOptionsExtension()
{
}


OSDConfigurationOptionsExtension::~OSDConfigurationOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/OSDImgConfiguration.h"


namespace ONVIF {


OSDImgConfiguration::OSDImgConfiguration():
	_imgPath(),
	_extension()
{
}


OSDImgConfiguration::OSDImgConfiguration(const Poco::URI& imgPath, const Poco::SharedPtr<OSDImgConfigurationExtension>& extension):
	_imgPath(imgPath),
	_extension(extension)
{
}


OSDImgConfiguration::~OSDImgConfiguration()
{
}


} // ONVIF


#include "ONVIF/OSDImgConfigurationExtension.h"


namespace ONVIF {


OSDImgConfigurationExtension::OSDImgConfigurationExtension()
{
}


OSDImgConfigurationExtension::~OSDImgConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/OSDImgOptions.h"


namespace ONVIF {


OSDImgOptions::OSDImgOptions():
	_formatsSupported(),
	_maxSize(),
	_maxWidth(),
	_maxHeight(),
	_imagePath(),
	_extension()
{
}


OSDImgOptions::OSDImgOptions(const std::vector<std::vector<std::string>>& formatsSupported, Poco::Optional<int> maxSize, Poco::Optional<int> maxWidth, Poco::Optional<int> maxHeight, const std::vector<Poco::URI>& imagePath, const Poco::SharedPtr<OSDImgOptionsExtension>& extension):
	_formatsSupported(formatsSupported),
	_maxSize(maxSize),
	_maxWidth(maxWidth),
	_maxHeight(maxHeight),
	_imagePath(imagePath),
	_extension(extension)
{
}


OSDImgOptions::~OSDImgOptions()
{
}


} // ONVIF


#include "ONVIF/OSDImgOptionsExtension.h"


namespace ONVIF {


OSDImgOptionsExtension::OSDImgOptionsExtension()
{
}


OSDImgOptionsExtension::~OSDImgOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/OSDPosConfiguration.h"


namespace ONVIF {


OSDPosConfiguration::OSDPosConfiguration():
	_type(),
	_pos(),
	_extension()
{
}


OSDPosConfiguration::OSDPosConfiguration(const std::string& type, const Poco::SharedPtr<Vector>& pos, const Poco::SharedPtr<OSDPosConfigurationExtension>& extension):
	_type(type),
	_pos(pos),
	_extension(extension)
{
}


OSDPosConfiguration::~OSDPosConfiguration()
{
}


} // ONVIF


#include "ONVIF/OSDPosConfigurationExtension.h"


namespace ONVIF {


OSDPosConfigurationExtension::OSDPosConfigurationExtension()
{
}


OSDPosConfigurationExtension::~OSDPosConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/OSDReference.h"


namespace ONVIF {


OSDReference::OSDReference():
	_value()
{
}


OSDReference::OSDReference(const Poco::Optional<std::string>& value):
	_value(value)
{
}


OSDReference::~OSDReference()
{
}


} // ONVIF


#include "ONVIF/OSDTextConfiguration.h"


namespace ONVIF {


OSDTextConfiguration::OSDTextConfiguration():
	_isPersistentText(),
	_type(),
	_dateFormat(),
	_timeFormat(),
	_fontSize(),
	_fontColor(),
	_backgroundColor(),
	_plainText(),
	_extension()
{
}


OSDTextConfiguration::OSDTextConfiguration(Poco::Optional<bool> isPersistentText, const std::string& type, const Poco::Optional<std::string>& dateFormat, const Poco::Optional<std::string>& timeFormat, Poco::Optional<int> fontSize, const Poco::SharedPtr<OSDColor>& fontColor, const Poco::SharedPtr<OSDColor>& backgroundColor, const Poco::Optional<std::string>& plainText, const Poco::SharedPtr<OSDTextConfigurationExtension>& extension):
	_isPersistentText(isPersistentText),
	_type(type),
	_dateFormat(dateFormat),
	_timeFormat(timeFormat),
	_fontSize(fontSize),
	_fontColor(fontColor),
	_backgroundColor(backgroundColor),
	_plainText(plainText),
	_extension(extension)
{
}


OSDTextConfiguration::~OSDTextConfiguration()
{
}


} // ONVIF


#include "ONVIF/OSDTextConfigurationExtension.h"


namespace ONVIF {


OSDTextConfigurationExtension::OSDTextConfigurationExtension()
{
}


OSDTextConfigurationExtension::~OSDTextConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/OSDTextOptions.h"


namespace ONVIF {


OSDTextOptions::OSDTextOptions():
	_type(),
	_fontSizeRange(),
	_dateFormat(),
	_timeFormat(),
	_fontColor(),
	_backgroundColor(),
	_extension()
{
}


OSDTextOptions::OSDTextOptions(const std::vector<std::string>& type, const Poco::SharedPtr<IntRange>& fontSizeRange, const std::vector<std::string>& dateFormat, const std::vector<std::string>& timeFormat, const Poco::SharedPtr<OSDColorOptions>& fontColor, const Poco::SharedPtr<OSDColorOptions>& backgroundColor, const Poco::SharedPtr<OSDTextOptionsExtension>& extension):
	_type(type),
	_fontSizeRange(fontSizeRange),
	_dateFormat(dateFormat),
	_timeFormat(timeFormat),
	_fontColor(fontColor),
	_backgroundColor(backgroundColor),
	_extension(extension)
{
}


OSDTextOptions::~OSDTextOptions()
{
}


} // ONVIF


#include "ONVIF/OSDTextOptionsExtension.h"


namespace ONVIF {


OSDTextOptionsExtension::OSDTextOptionsExtension()
{
}


OSDTextOptionsExtension::~OSDTextOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/OnvifVersion.h"


namespace ONVIF {


OnvifVersion::OnvifVersion():
	_major(),
	_minor()
{
}


OnvifVersion::OnvifVersion(int major, int minor):
	_major(major),
	_minor(minor)
{
}


OnvifVersion::~OnvifVersion()
{
}


} // ONVIF


#include "ONVIF/PTControlDirection.h"


namespace ONVIF {


PTControlDirection::PTControlDirection():
	_eFlip(),
	_reverse(),
	_extension()
{
}


PTControlDirection::PTControlDirection(const Poco::SharedPtr<EFlip>& eFlip, const Poco::SharedPtr<Reverse>& reverse, const Poco::SharedPtr<PTControlDirectionExtension>& extension):
	_eFlip(eFlip),
	_reverse(reverse),
	_extension(extension)
{
}


PTControlDirection::~PTControlDirection()
{
}


} // ONVIF


#include "ONVIF/PTControlDirectionExtension.h"


namespace ONVIF {


PTControlDirectionExtension::PTControlDirectionExtension()
{
}


PTControlDirectionExtension::~PTControlDirectionExtension()
{
}


} // ONVIF


#include "ONVIF/PTControlDirectionOptions.h"


namespace ONVIF {


PTControlDirectionOptions::PTControlDirectionOptions():
	_eFlip(),
	_reverse(),
	_extension()
{
}


PTControlDirectionOptions::PTControlDirectionOptions(const Poco::SharedPtr<EFlipOptions>& eFlip, const Poco::SharedPtr<ReverseOptions>& reverse, const Poco::SharedPtr<PTControlDirectionOptionsExtension>& extension):
	_eFlip(eFlip),
	_reverse(reverse),
	_extension(extension)
{
}


PTControlDirectionOptions::~PTControlDirectionOptions()
{
}


} // ONVIF


#include "ONVIF/PTControlDirectionOptionsExtension.h"


namespace ONVIF {


PTControlDirectionOptionsExtension::PTControlDirectionOptionsExtension()
{
}


PTControlDirectionOptionsExtension::~PTControlDirectionOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/PTZCapabilities.h"


namespace ONVIF {


PTZCapabilities::PTZCapabilities():
	_xAddr()
{
}


PTZCapabilities::PTZCapabilities(const Poco::URI& xAddr):
	_xAddr(xAddr)
{
}


PTZCapabilities::~PTZCapabilities()
{
}


} // ONVIF


#include "ONVIF/PTZConfiguration.h"


namespace ONVIF {


PTZConfiguration::PTZConfiguration():
	_moveRamp(),
	_presetRamp(),
	_presetTourRamp(),
	_nodeToken(),
	_defaultAbsolutePantTiltPositionSpace(),
	_defaultAbsoluteZoomPositionSpace(),
	_defaultRelativePanTiltTranslationSpace(),
	_defaultRelativeZoomTranslationSpace(),
	_defaultContinuousPanTiltVelocitySpace(),
	_defaultContinuousZoomVelocitySpace(),
	_defaultPTZSpeed(),
	_defaultPTZTimeout(),
	_panTiltLimits(),
	_zoomLimits(),
	_extension()
{
}


PTZConfiguration::PTZConfiguration(const std::string& token, const std::string& name, int useCount, Poco::Optional<int> moveRamp, Poco::Optional<int> presetRamp, Poco::Optional<int> presetTourRamp, const std::string& nodeToken, const Poco::Optional<Poco::URI>& defaultAbsolutePantTiltPositionSpace, const Poco::Optional<Poco::URI>& defaultAbsoluteZoomPositionSpace, const Poco::Optional<Poco::URI>& defaultRelativePanTiltTranslationSpace, const Poco::Optional<Poco::URI>& defaultRelativeZoomTranslationSpace, const Poco::Optional<Poco::URI>& defaultContinuousPanTiltVelocitySpace, const Poco::Optional<Poco::URI>& defaultContinuousZoomVelocitySpace, const Poco::SharedPtr<PTZSpeed>& defaultPTZSpeed, const Poco::Optional<std::string>& defaultPTZTimeout, const Poco::SharedPtr<PanTiltLimits>& panTiltLimits, const Poco::SharedPtr<ZoomLimits>& zoomLimits, const Poco::SharedPtr<PTZConfigurationExtension>& extension):
	ConfigurationEntity(token, name, useCount),
	_moveRamp(moveRamp),
	_presetRamp(presetRamp),
	_presetTourRamp(presetTourRamp),
	_nodeToken(nodeToken),
	_defaultAbsolutePantTiltPositionSpace(defaultAbsolutePantTiltPositionSpace),
	_defaultAbsoluteZoomPositionSpace(defaultAbsoluteZoomPositionSpace),
	_defaultRelativePanTiltTranslationSpace(defaultRelativePanTiltTranslationSpace),
	_defaultRelativeZoomTranslationSpace(defaultRelativeZoomTranslationSpace),
	_defaultContinuousPanTiltVelocitySpace(defaultContinuousPanTiltVelocitySpace),
	_defaultContinuousZoomVelocitySpace(defaultContinuousZoomVelocitySpace),
	_defaultPTZSpeed(defaultPTZSpeed),
	_defaultPTZTimeout(defaultPTZTimeout),
	_panTiltLimits(panTiltLimits),
	_zoomLimits(zoomLimits),
	_extension(extension)
{
}


PTZConfiguration::~PTZConfiguration()
{
}


} // ONVIF


#include "ONVIF/PTZConfigurationExtension.h"


namespace ONVIF {


PTZConfigurationExtension::PTZConfigurationExtension():
	_pTControlDirection(),
	_extension()
{
}


PTZConfigurationExtension::PTZConfigurationExtension(const Poco::SharedPtr<PTControlDirection>& pTControlDirection, const Poco::SharedPtr<PTZConfigurationExtension2>& extension):
	_pTControlDirection(pTControlDirection),
	_extension(extension)
{
}


PTZConfigurationExtension::~PTZConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/PTZConfigurationExtension2.h"


namespace ONVIF {


PTZConfigurationExtension2::PTZConfigurationExtension2()
{
}


PTZConfigurationExtension2::~PTZConfigurationExtension2()
{
}


} // ONVIF


#include "ONVIF/PTZConfigurationOptions.h"


namespace ONVIF {


PTZConfigurationOptions::PTZConfigurationOptions():
	_pTZRamps(),
	_spaces(),
	_pTZTimeout(),
	_pTControlDirection(),
	_extension()
{
}


PTZConfigurationOptions::PTZConfigurationOptions(const std::vector<std::vector<int>>& pTZRamps, const PTZSpaces& spaces, const DurationRange& pTZTimeout, const Poco::SharedPtr<PTControlDirectionOptions>& pTControlDirection, const Poco::SharedPtr<PTZConfigurationOptions2>& extension):
	_pTZRamps(pTZRamps),
	_spaces(spaces),
	_pTZTimeout(pTZTimeout),
	_pTControlDirection(pTControlDirection),
	_extension(extension)
{
}


PTZConfigurationOptions::~PTZConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/PTZConfigurationOptions2.h"


namespace ONVIF {


PTZConfigurationOptions2::PTZConfigurationOptions2()
{
}


PTZConfigurationOptions2::~PTZConfigurationOptions2()
{
}


} // ONVIF


#include "ONVIF/PTZFilter.h"


namespace ONVIF {


PTZFilter::PTZFilter():
	_status(),
	_position()
{
}


PTZFilter::PTZFilter(bool status, bool position):
	_status(status),
	_position(position)
{
}


PTZFilter::~PTZFilter()
{
}


} // ONVIF


#include "ONVIF/PTZMoveStatus.h"


namespace ONVIF {


PTZMoveStatus::PTZMoveStatus():
	_panTilt(),
	_zoom()
{
}


PTZMoveStatus::PTZMoveStatus(const Poco::Optional<std::string>& panTilt, const Poco::Optional<std::string>& zoom):
	_panTilt(panTilt),
	_zoom(zoom)
{
}


PTZMoveStatus::~PTZMoveStatus()
{
}


} // ONVIF


#include "ONVIF/PTZNode.h"


namespace ONVIF {


PTZNode::PTZNode():
	_fixedHomePosition(),
	_geoMove(),
	_name(),
	_supportedPTZSpaces(),
	_maximumNumberOfPresets(),
	_homeSupported(),
	_auxiliaryCommands(),
	_extension()
{
}


PTZNode::PTZNode(const std::string& token, Poco::Optional<bool> fixedHomePosition, Poco::Optional<bool> geoMove, const Poco::Optional<std::string>& name, const PTZSpaces& supportedPTZSpaces, int maximumNumberOfPresets, bool homeSupported, const std::vector<std::string>& auxiliaryCommands, const Poco::SharedPtr<PTZNodeExtension>& extension):
	DeviceEntity(token),
	_fixedHomePosition(fixedHomePosition),
	_geoMove(geoMove),
	_name(name),
	_supportedPTZSpaces(supportedPTZSpaces),
	_maximumNumberOfPresets(maximumNumberOfPresets),
	_homeSupported(homeSupported),
	_auxiliaryCommands(auxiliaryCommands),
	_extension(extension)
{
}


PTZNode::~PTZNode()
{
}


} // ONVIF


#include "ONVIF/PTZNodeExtension.h"


namespace ONVIF {


PTZNodeExtension::PTZNodeExtension():
	_supportedPresetTour(),
	_extension()
{
}


PTZNodeExtension::PTZNodeExtension(const Poco::SharedPtr<PTZPresetTourSupported>& supportedPresetTour, const Poco::SharedPtr<PTZNodeExtension2>& extension):
	_supportedPresetTour(supportedPresetTour),
	_extension(extension)
{
}


PTZNodeExtension::~PTZNodeExtension()
{
}


} // ONVIF


#include "ONVIF/PTZNodeExtension2.h"


namespace ONVIF {


PTZNodeExtension2::PTZNodeExtension2()
{
}


PTZNodeExtension2::~PTZNodeExtension2()
{
}


} // ONVIF


#include "ONVIF/PTZPositionFilter.h"


namespace ONVIF {


PTZPositionFilter::PTZPositionFilter():
	_minPosition(),
	_maxPosition(),
	_enterOrExit()
{
}


PTZPositionFilter::PTZPositionFilter(const PTZVector& minPosition, const PTZVector& maxPosition, bool enterOrExit):
	_minPosition(minPosition),
	_maxPosition(maxPosition),
	_enterOrExit(enterOrExit)
{
}


PTZPositionFilter::~PTZPositionFilter()
{
}


} // ONVIF


#include "ONVIF/PTZPreset.h"


namespace ONVIF {


PTZPreset::PTZPreset():
	_token(),
	_name(),
	_pTZPosition()
{
}


PTZPreset::PTZPreset(const Poco::Optional<std::string>& token, const Poco::Optional<std::string>& name, const Poco::SharedPtr<PTZVector>& pTZPosition):
	_token(token),
	_name(name),
	_pTZPosition(pTZPosition)
{
}


PTZPreset::~PTZPreset()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourExtension.h"


namespace ONVIF {


PTZPresetTourExtension::PTZPresetTourExtension()
{
}


PTZPresetTourExtension::~PTZPresetTourExtension()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourOptions.h"


namespace ONVIF {


PTZPresetTourOptions::PTZPresetTourOptions():
	_autoStart(),
	_startingCondition(),
	_tourSpot()
{
}


PTZPresetTourOptions::PTZPresetTourOptions(bool autoStart, const PTZPresetTourStartingConditionOptions& startingCondition, const PTZPresetTourSpotOptions& tourSpot):
	_autoStart(autoStart),
	_startingCondition(startingCondition),
	_tourSpot(tourSpot)
{
}


PTZPresetTourOptions::~PTZPresetTourOptions()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourPresetDetail.h"


namespace ONVIF {


PTZPresetTourPresetDetail::PTZPresetTourPresetDetail():
	_presetToken(),
	_home(),
	_pTZPosition(),
	_typeExtension()
{
}


PTZPresetTourPresetDetail::PTZPresetTourPresetDetail(const Poco::Optional<std::string>& presetToken, Poco::Optional<bool> home, const Poco::SharedPtr<PTZVector>& pTZPosition, const Poco::SharedPtr<PTZPresetTourTypeExtension>& typeExtension):
	_presetToken(presetToken),
	_home(home),
	_pTZPosition(pTZPosition),
	_typeExtension(typeExtension)
{
}


PTZPresetTourPresetDetail::~PTZPresetTourPresetDetail()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourPresetDetailOptions.h"


namespace ONVIF {


PTZPresetTourPresetDetailOptions::PTZPresetTourPresetDetailOptions():
	_presetToken(),
	_home(),
	_panTiltPositionSpace(),
	_zoomPositionSpace(),
	_extension()
{
}


PTZPresetTourPresetDetailOptions::PTZPresetTourPresetDetailOptions(const std::vector<std::string>& presetToken, Poco::Optional<bool> home, const Poco::SharedPtr<Space2DDescription>& panTiltPositionSpace, const Poco::SharedPtr<Space1DDescription>& zoomPositionSpace, const Poco::SharedPtr<PTZPresetTourPresetDetailOptionsExtension>& extension):
	_presetToken(presetToken),
	_home(home),
	_panTiltPositionSpace(panTiltPositionSpace),
	_zoomPositionSpace(zoomPositionSpace),
	_extension(extension)
{
}


PTZPresetTourPresetDetailOptions::~PTZPresetTourPresetDetailOptions()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourPresetDetailOptionsExtension.h"


namespace ONVIF {


PTZPresetTourPresetDetailOptionsExtension::PTZPresetTourPresetDetailOptionsExtension()
{
}


PTZPresetTourPresetDetailOptionsExtension::~PTZPresetTourPresetDetailOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourSpot.h"


namespace ONVIF {


PTZPresetTourSpot::PTZPresetTourSpot():
	_presetDetail(),
	_speed(),
	_stayTime(),
	_extension()
{
}


PTZPresetTourSpot::PTZPresetTourSpot(const PTZPresetTourPresetDetail& presetDetail, const Poco::SharedPtr<PTZSpeed>& speed, const Poco::Optional<std::string>& stayTime, const Poco::SharedPtr<PTZPresetTourSpotExtension>& extension):
	_presetDetail(presetDetail),
	_speed(speed),
	_stayTime(stayTime),
	_extension(extension)
{
}


PTZPresetTourSpot::~PTZPresetTourSpot()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourSpotExtension.h"


namespace ONVIF {


PTZPresetTourSpotExtension::PTZPresetTourSpotExtension()
{
}


PTZPresetTourSpotExtension::~PTZPresetTourSpotExtension()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourSpotOptions.h"


namespace ONVIF {


PTZPresetTourSpotOptions::PTZPresetTourSpotOptions():
	_presetDetail(),
	_stayTime()
{
}


PTZPresetTourSpotOptions::PTZPresetTourSpotOptions(const PTZPresetTourPresetDetailOptions& presetDetail, const DurationRange& stayTime):
	_presetDetail(presetDetail),
	_stayTime(stayTime)
{
}


PTZPresetTourSpotOptions::~PTZPresetTourSpotOptions()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourStartingCondition.h"


namespace ONVIF {


PTZPresetTourStartingCondition::PTZPresetTourStartingCondition():
	_randomPresetOrder(),
	_recurringTime(),
	_recurringDuration(),
	_direction(),
	_extension()
{
}


PTZPresetTourStartingCondition::PTZPresetTourStartingCondition(Poco::Optional<bool> randomPresetOrder, Poco::Optional<int> recurringTime, const Poco::Optional<std::string>& recurringDuration, const Poco::Optional<std::string>& direction, const Poco::SharedPtr<PTZPresetTourStartingConditionExtension>& extension):
	_randomPresetOrder(randomPresetOrder),
	_recurringTime(recurringTime),
	_recurringDuration(recurringDuration),
	_direction(direction),
	_extension(extension)
{
}


PTZPresetTourStartingCondition::~PTZPresetTourStartingCondition()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourStartingConditionExtension.h"


namespace ONVIF {


PTZPresetTourStartingConditionExtension::PTZPresetTourStartingConditionExtension()
{
}


PTZPresetTourStartingConditionExtension::~PTZPresetTourStartingConditionExtension()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourStartingConditionOptions.h"


namespace ONVIF {


PTZPresetTourStartingConditionOptions::PTZPresetTourStartingConditionOptions():
	_recurringTime(),
	_recurringDuration(),
	_direction(),
	_extension()
{
}


PTZPresetTourStartingConditionOptions::PTZPresetTourStartingConditionOptions(const Poco::SharedPtr<IntRange>& recurringTime, const Poco::SharedPtr<DurationRange>& recurringDuration, const std::vector<std::string>& direction, const Poco::SharedPtr<PTZPresetTourStartingConditionOptionsExtension>& extension):
	_recurringTime(recurringTime),
	_recurringDuration(recurringDuration),
	_direction(direction),
	_extension(extension)
{
}


PTZPresetTourStartingConditionOptions::~PTZPresetTourStartingConditionOptions()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourStartingConditionOptionsExtension.h"


namespace ONVIF {


PTZPresetTourStartingConditionOptionsExtension::PTZPresetTourStartingConditionOptionsExtension()
{
}


PTZPresetTourStartingConditionOptionsExtension::~PTZPresetTourStartingConditionOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourStatus.h"


namespace ONVIF {


PTZPresetTourStatus::PTZPresetTourStatus():
	_state(),
	_currentTourSpot(),
	_extension()
{
}


PTZPresetTourStatus::PTZPresetTourStatus(const std::string& state, const Poco::SharedPtr<PTZPresetTourSpot>& currentTourSpot, const Poco::SharedPtr<PTZPresetTourStatusExtension>& extension):
	_state(state),
	_currentTourSpot(currentTourSpot),
	_extension(extension)
{
}


PTZPresetTourStatus::~PTZPresetTourStatus()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourStatusExtension.h"


namespace ONVIF {


PTZPresetTourStatusExtension::PTZPresetTourStatusExtension()
{
}


PTZPresetTourStatusExtension::~PTZPresetTourStatusExtension()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourSupported.h"


namespace ONVIF {


PTZPresetTourSupported::PTZPresetTourSupported():
	_maximumNumberOfPresetTours(),
	_pTZPresetTourOperation(),
	_extension()
{
}


PTZPresetTourSupported::PTZPresetTourSupported(int maximumNumberOfPresetTours, const std::vector<std::string>& pTZPresetTourOperation, const Poco::SharedPtr<PTZPresetTourSupportedExtension>& extension):
	_maximumNumberOfPresetTours(maximumNumberOfPresetTours),
	_pTZPresetTourOperation(pTZPresetTourOperation),
	_extension(extension)
{
}


PTZPresetTourSupported::~PTZPresetTourSupported()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourSupportedExtension.h"


namespace ONVIF {


PTZPresetTourSupportedExtension::PTZPresetTourSupportedExtension()
{
}


PTZPresetTourSupportedExtension::~PTZPresetTourSupportedExtension()
{
}


} // ONVIF


#include "ONVIF/PTZPresetTourTypeExtension.h"


namespace ONVIF {


PTZPresetTourTypeExtension::PTZPresetTourTypeExtension()
{
}


PTZPresetTourTypeExtension::~PTZPresetTourTypeExtension()
{
}


} // ONVIF


#include "ONVIF/PTZSpaces.h"


namespace ONVIF {


PTZSpaces::PTZSpaces():
	_absolutePanTiltPositionSpace(),
	_absoluteZoomPositionSpace(),
	_relativePanTiltTranslationSpace(),
	_relativeZoomTranslationSpace(),
	_continuousPanTiltVelocitySpace(),
	_continuousZoomVelocitySpace(),
	_panTiltSpeedSpace(),
	_zoomSpeedSpace(),
	_extension()
{
}


PTZSpaces::PTZSpaces(const std::vector<Space2DDescription>& absolutePanTiltPositionSpace, const std::vector<Space1DDescription>& absoluteZoomPositionSpace, const std::vector<Space2DDescription>& relativePanTiltTranslationSpace, const std::vector<Space1DDescription>& relativeZoomTranslationSpace, const std::vector<Space2DDescription>& continuousPanTiltVelocitySpace, const std::vector<Space1DDescription>& continuousZoomVelocitySpace, const std::vector<Space1DDescription>& panTiltSpeedSpace, const std::vector<Space1DDescription>& zoomSpeedSpace, const Poco::SharedPtr<PTZSpacesExtension>& extension):
	_absolutePanTiltPositionSpace(absolutePanTiltPositionSpace),
	_absoluteZoomPositionSpace(absoluteZoomPositionSpace),
	_relativePanTiltTranslationSpace(relativePanTiltTranslationSpace),
	_relativeZoomTranslationSpace(relativeZoomTranslationSpace),
	_continuousPanTiltVelocitySpace(continuousPanTiltVelocitySpace),
	_continuousZoomVelocitySpace(continuousZoomVelocitySpace),
	_panTiltSpeedSpace(panTiltSpeedSpace),
	_zoomSpeedSpace(zoomSpeedSpace),
	_extension(extension)
{
}


PTZSpaces::~PTZSpaces()
{
}


} // ONVIF


#include "ONVIF/PTZSpacesExtension.h"


namespace ONVIF {


PTZSpacesExtension::PTZSpacesExtension()
{
}


PTZSpacesExtension::~PTZSpacesExtension()
{
}


} // ONVIF


#include "ONVIF/PTZSpeed.h"


namespace ONVIF {


PTZSpeed::PTZSpeed():
	_panTilt(),
	_zoom()
{
}


PTZSpeed::PTZSpeed(const Poco::SharedPtr<Vector2D>& panTilt, const Poco::SharedPtr<Vector1D>& zoom):
	_panTilt(panTilt),
	_zoom(zoom)
{
}


PTZSpeed::~PTZSpeed()
{
}


} // ONVIF


#include "ONVIF/PTZStatus.h"


namespace ONVIF {


PTZStatus::PTZStatus():
	_position(),
	_moveStatus(),
	_error(),
	_utcTime()
{
}


PTZStatus::PTZStatus(const Poco::SharedPtr<PTZVector>& position, const Poco::SharedPtr<PTZMoveStatus>& moveStatus, const Poco::Optional<std::string>& error, const Poco::DateTime& utcTime):
	_position(position),
	_moveStatus(moveStatus),
	_error(error),
	_utcTime(utcTime)
{
}


PTZStatus::~PTZStatus()
{
}


} // ONVIF


#include "ONVIF/PTZStatusFilterOptions.h"


namespace ONVIF {


PTZStatusFilterOptions::PTZStatusFilterOptions():
	_panTiltStatusSupported(),
	_zoomStatusSupported(),
	_panTiltPositionSupported(),
	_zoomPositionSupported(),
	_extension()
{
}


PTZStatusFilterOptions::PTZStatusFilterOptions(bool panTiltStatusSupported, bool zoomStatusSupported, Poco::Optional<bool> panTiltPositionSupported, Poco::Optional<bool> zoomPositionSupported, const Poco::SharedPtr<PTZStatusFilterOptionsExtension>& extension):
	_panTiltStatusSupported(panTiltStatusSupported),
	_zoomStatusSupported(zoomStatusSupported),
	_panTiltPositionSupported(panTiltPositionSupported),
	_zoomPositionSupported(zoomPositionSupported),
	_extension(extension)
{
}


PTZStatusFilterOptions::~PTZStatusFilterOptions()
{
}


} // ONVIF


#include "ONVIF/PTZStatusFilterOptionsExtension.h"


namespace ONVIF {


PTZStatusFilterOptionsExtension::PTZStatusFilterOptionsExtension()
{
}


PTZStatusFilterOptionsExtension::~PTZStatusFilterOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/PTZVector.h"


namespace ONVIF {


PTZVector::PTZVector():
	_panTilt(),
	_zoom()
{
}


PTZVector::PTZVector(const Poco::SharedPtr<Vector2D>& panTilt, const Poco::SharedPtr<Vector1D>& zoom):
	_panTilt(panTilt),
	_zoom(zoom)
{
}


PTZVector::~PTZVector()
{
}


} // ONVIF


#include "ONVIF/PanTiltLimits.h"


namespace ONVIF {


PanTiltLimits::PanTiltLimits():
	_range()
{
}


PanTiltLimits::PanTiltLimits(const Space2DDescription& range):
	_range(range)
{
}


PanTiltLimits::~PanTiltLimits()
{
}


} // ONVIF


#include "ONVIF/PaneConfiguration.h"


namespace ONVIF {


PaneConfiguration::PaneConfiguration():
	_paneName(),
	_audioOutputToken(),
	_audioSourceToken(),
	_audioEncoderConfiguration(),
	_receiverToken(),
	_token()
{
}


PaneConfiguration::PaneConfiguration(const Poco::Optional<std::string>& paneName, const Poco::Optional<std::string>& audioOutputToken, const Poco::Optional<std::string>& audioSourceToken, const Poco::SharedPtr<AudioEncoderConfiguration>& audioEncoderConfiguration, const Poco::Optional<std::string>& receiverToken, const std::string& token):
	_paneName(paneName),
	_audioOutputToken(audioOutputToken),
	_audioSourceToken(audioSourceToken),
	_audioEncoderConfiguration(audioEncoderConfiguration),
	_receiverToken(receiverToken),
	_token(token)
{
}


PaneConfiguration::~PaneConfiguration()
{
}


} // ONVIF


#include "ONVIF/PaneLayout.h"


namespace ONVIF {


PaneLayout::PaneLayout():
	_pane(),
	_area()
{
}


PaneLayout::PaneLayout(const std::string& pane, const Rectangle& area):
	_pane(pane),
	_area(area)
{
}


PaneLayout::~PaneLayout()
{
}


} // ONVIF


#include "ONVIF/PaneLayoutOptions.h"


namespace ONVIF {


PaneLayoutOptions::PaneLayoutOptions():
	_area(),
	_extension()
{
}


PaneLayoutOptions::PaneLayoutOptions(const std::vector<Rectangle>& area, const Poco::SharedPtr<PaneOptionExtension>& extension):
	_area(area),
	_extension(extension)
{
}


PaneLayoutOptions::~PaneLayoutOptions()
{
}


} // ONVIF


#include "ONVIF/PaneOptionExtension.h"


namespace ONVIF {


PaneOptionExtension::PaneOptionExtension()
{
}


PaneOptionExtension::~PaneOptionExtension()
{
}


} // ONVIF


#include "ONVIF/Polygon.h"


namespace ONVIF {


Polygon::Polygon():
	_point()
{
}


Polygon::Polygon(const std::vector<Vector>& point):
	_point(point)
{
}


Polygon::~Polygon()
{
}


} // ONVIF


#include "ONVIF/PolygonOptions.h"


namespace ONVIF {


PolygonOptions::PolygonOptions():
	_rectangleOnly(),
	_vertexLimits()
{
}


PolygonOptions::PolygonOptions(Poco::Optional<bool> rectangleOnly, const Poco::SharedPtr<IntRange>& vertexLimits):
	_rectangleOnly(rectangleOnly),
	_vertexLimits(vertexLimits)
{
}


PolygonOptions::~PolygonOptions()
{
}


} // ONVIF


#include "ONVIF/Polyline.h"


namespace ONVIF {


Polyline::Polyline():
	_point()
{
}


Polyline::Polyline(const std::vector<Vector>& point):
	_point(point)
{
}


Polyline::~Polyline()
{
}


} // ONVIF


#include "ONVIF/PolylineArray.h"


namespace ONVIF {


PolylineArray::PolylineArray():
	_segment(),
	_extension()
{
}


PolylineArray::PolylineArray(const std::vector<Polyline>& segment, const Poco::SharedPtr<PolylineArrayExtension>& extension):
	_segment(segment),
	_extension(extension)
{
}


PolylineArray::~PolylineArray()
{
}


} // ONVIF


#include "ONVIF/PolylineArrayConfiguration.h"


namespace ONVIF {


PolylineArrayConfiguration::PolylineArrayConfiguration():
	_polylineArray()
{
}


PolylineArrayConfiguration::PolylineArrayConfiguration(const PolylineArray& polylineArray):
	_polylineArray(polylineArray)
{
}


PolylineArrayConfiguration::~PolylineArrayConfiguration()
{
}


} // ONVIF


#include "ONVIF/PolylineArrayExtension.h"


namespace ONVIF {


PolylineArrayExtension::PolylineArrayExtension()
{
}


PolylineArrayExtension::~PolylineArrayExtension()
{
}


} // ONVIF


#include "ONVIF/PrefixedIPv4Address.h"


namespace ONVIF {


PrefixedIPv4Address::PrefixedIPv4Address():
	_address(),
	_prefixLength()
{
}


PrefixedIPv4Address::PrefixedIPv4Address(const std::string& address, int prefixLength):
	_address(address),
	_prefixLength(prefixLength)
{
}


PrefixedIPv4Address::~PrefixedIPv4Address()
{
}


} // ONVIF


#include "ONVIF/PrefixedIPv6Address.h"


namespace ONVIF {


PrefixedIPv6Address::PrefixedIPv6Address():
	_address(),
	_prefixLength()
{
}


PrefixedIPv6Address::PrefixedIPv6Address(const std::string& address, int prefixLength):
	_address(address),
	_prefixLength(prefixLength)
{
}


PrefixedIPv6Address::~PrefixedIPv6Address()
{
}


} // ONVIF


#include "ONVIF/PresetTour.h"


namespace ONVIF {


PresetTour::PresetTour():
	_token(),
	_name(),
	_status(),
	_autoStart(),
	_startingCondition(),
	_tourSpot(),
	_extension()
{
}


PresetTour::PresetTour(const Poco::Optional<std::string>& token, const Poco::Optional<std::string>& name, const PTZPresetTourStatus& status, bool autoStart, const PTZPresetTourStartingCondition& startingCondition, const std::vector<PTZPresetTourSpot>& tourSpot, const Poco::SharedPtr<PTZPresetTourExtension>& extension):
	_token(token),
	_name(name),
	_status(status),
	_autoStart(autoStart),
	_startingCondition(startingCondition),
	_tourSpot(tourSpot),
	_extension(extension)
{
}


PresetTour::~PresetTour()
{
}


} // ONVIF


#include "ONVIF/Profile.h"


namespace ONVIF {


Profile::Profile():
	_token(),
	_fixed(),
	_name(),
	_videoSourceConfiguration(),
	_audioSourceConfiguration(),
	_videoEncoderConfiguration(),
	_audioEncoderConfiguration(),
	_videoAnalyticsConfiguration(),
	_pTZConfiguration(),
	_metadataConfiguration(),
	_extension()
{
}


Profile::Profile(const std::string& token, Poco::Optional<bool> fixed, const std::string& name, const Poco::SharedPtr<VideoSourceConfiguration>& videoSourceConfiguration, const Poco::SharedPtr<AudioSourceConfiguration>& audioSourceConfiguration, const Poco::SharedPtr<VideoEncoderConfiguration>& videoEncoderConfiguration, const Poco::SharedPtr<AudioEncoderConfiguration>& audioEncoderConfiguration, const Poco::SharedPtr<VideoAnalyticsConfiguration>& videoAnalyticsConfiguration, const Poco::SharedPtr<PTZConfiguration>& pTZConfiguration, const Poco::SharedPtr<MetadataConfiguration>& metadataConfiguration, const Poco::SharedPtr<ProfileExtension>& extension):
	_token(token),
	_fixed(fixed),
	_name(name),
	_videoSourceConfiguration(videoSourceConfiguration),
	_audioSourceConfiguration(audioSourceConfiguration),
	_videoEncoderConfiguration(videoEncoderConfiguration),
	_audioEncoderConfiguration(audioEncoderConfiguration),
	_videoAnalyticsConfiguration(videoAnalyticsConfiguration),
	_pTZConfiguration(pTZConfiguration),
	_metadataConfiguration(metadataConfiguration),
	_extension(extension)
{
}


Profile::~Profile()
{
}


} // ONVIF


#include "ONVIF/ProfileCapabilities.h"


namespace ONVIF {


ProfileCapabilities::ProfileCapabilities():
	_maximumNumberOfProfiles()
{
}


ProfileCapabilities::ProfileCapabilities(int maximumNumberOfProfiles):
	_maximumNumberOfProfiles(maximumNumberOfProfiles)
{
}


ProfileCapabilities::~ProfileCapabilities()
{
}


} // ONVIF


#include "ONVIF/ProfileExtension.h"


namespace ONVIF {


ProfileExtension::ProfileExtension():
	_audioOutputConfiguration(),
	_audioDecoderConfiguration(),
	_extension()
{
}


ProfileExtension::ProfileExtension(const Poco::SharedPtr<AudioOutputConfiguration>& audioOutputConfiguration, const Poco::SharedPtr<AudioDecoderConfiguration>& audioDecoderConfiguration, const Poco::SharedPtr<ProfileExtension2>& extension):
	_audioOutputConfiguration(audioOutputConfiguration),
	_audioDecoderConfiguration(audioDecoderConfiguration),
	_extension(extension)
{
}


ProfileExtension::~ProfileExtension()
{
}


} // ONVIF


#include "ONVIF/ProfileExtension2.h"


namespace ONVIF {


ProfileExtension2::ProfileExtension2()
{
}


ProfileExtension2::~ProfileExtension2()
{
}


} // ONVIF


#include "ONVIF/ProfileStatus.h"


namespace ONVIF {


ProfileStatus::ProfileStatus():
	_activeConnections(),
	_extension()
{
}


ProfileStatus::ProfileStatus(const std::vector<ActiveConnection>& activeConnections, const Poco::SharedPtr<ProfileStatusExtension>& extension):
	_activeConnections(activeConnections),
	_extension(extension)
{
}


ProfileStatus::~ProfileStatus()
{
}


} // ONVIF


#include "ONVIF/ProfileStatusExtension.h"


namespace ONVIF {


ProfileStatusExtension::ProfileStatusExtension()
{
}


ProfileStatusExtension::~ProfileStatusExtension()
{
}


} // ONVIF


#include "ONVIF/RealTimeStreamingCapabilities.h"


namespace ONVIF {


RealTimeStreamingCapabilities::RealTimeStreamingCapabilities():
	_rTPMulticast(),
	_rTP_TCP(),
	_rTP_RTSP_TCP(),
	_extension()
{
}


RealTimeStreamingCapabilities::RealTimeStreamingCapabilities(Poco::Optional<bool> rTPMulticast, Poco::Optional<bool> rTP_TCP, Poco::Optional<bool> rTP_RTSP_TCP, const Poco::SharedPtr<RealTimeStreamingCapabilitiesExtension>& extension):
	_rTPMulticast(rTPMulticast),
	_rTP_TCP(rTP_TCP),
	_rTP_RTSP_TCP(rTP_RTSP_TCP),
	_extension(extension)
{
}


RealTimeStreamingCapabilities::~RealTimeStreamingCapabilities()
{
}


} // ONVIF


#include "ONVIF/RealTimeStreamingCapabilitiesExtension.h"


namespace ONVIF {


RealTimeStreamingCapabilitiesExtension::RealTimeStreamingCapabilitiesExtension()
{
}


RealTimeStreamingCapabilitiesExtension::~RealTimeStreamingCapabilitiesExtension()
{
}


} // ONVIF


#include "ONVIF/Receiver.h"


namespace ONVIF {


Receiver::Receiver():
	_token(),
	_configuration()
{
}


Receiver::Receiver(const std::string& token, const ReceiverConfiguration& configuration):
	_token(token),
	_configuration(configuration)
{
}


Receiver::~Receiver()
{
}


} // ONVIF


#include "ONVIF/ReceiverCapabilities.h"


namespace ONVIF {


ReceiverCapabilities::ReceiverCapabilities():
	_xAddr(),
	_rTP_Multicast(),
	_rTP_TCP(),
	_rTP_RTSP_TCP(),
	_supportedReceivers(),
	_maximumRTSPURILength()
{
}


ReceiverCapabilities::ReceiverCapabilities(const Poco::URI& xAddr, bool rTP_Multicast, bool rTP_TCP, bool rTP_RTSP_TCP, int supportedReceivers, int maximumRTSPURILength):
	_xAddr(xAddr),
	_rTP_Multicast(rTP_Multicast),
	_rTP_TCP(rTP_TCP),
	_rTP_RTSP_TCP(rTP_RTSP_TCP),
	_supportedReceivers(supportedReceivers),
	_maximumRTSPURILength(maximumRTSPURILength)
{
}


ReceiverCapabilities::~ReceiverCapabilities()
{
}


} // ONVIF


#include "ONVIF/ReceiverConfiguration.h"


namespace ONVIF {


ReceiverConfiguration::ReceiverConfiguration():
	_mode(),
	_mediaUri(),
	_streamSetup()
{
}


ReceiverConfiguration::ReceiverConfiguration(const std::string& mode, const Poco::URI& mediaUri, const StreamSetup& streamSetup):
	_mode(mode),
	_mediaUri(mediaUri),
	_streamSetup(streamSetup)
{
}


ReceiverConfiguration::~ReceiverConfiguration()
{
}


} // ONVIF


#include "ONVIF/ReceiverStateInformation.h"


namespace ONVIF {


ReceiverStateInformation::ReceiverStateInformation():
	_state(),
	_autoCreated()
{
}


ReceiverStateInformation::ReceiverStateInformation(const std::string& state, bool autoCreated):
	_state(state),
	_autoCreated(autoCreated)
{
}


ReceiverStateInformation::~ReceiverStateInformation()
{
}


} // ONVIF


#include "ONVIF/RecordingCapabilities.h"


namespace ONVIF {


RecordingCapabilities::RecordingCapabilities():
	_xAddr(),
	_receiverSource(),
	_mediaProfileSource(),
	_dynamicRecordings(),
	_dynamicTracks(),
	_maxStringLength()
{
}


RecordingCapabilities::RecordingCapabilities(const Poco::URI& xAddr, bool receiverSource, bool mediaProfileSource, bool dynamicRecordings, bool dynamicTracks, int maxStringLength):
	_xAddr(xAddr),
	_receiverSource(receiverSource),
	_mediaProfileSource(mediaProfileSource),
	_dynamicRecordings(dynamicRecordings),
	_dynamicTracks(dynamicTracks),
	_maxStringLength(maxStringLength)
{
}


RecordingCapabilities::~RecordingCapabilities()
{
}


} // ONVIF


#include "ONVIF/RecordingConfiguration.h"


namespace ONVIF {


RecordingConfiguration::RecordingConfiguration():
	_source(),
	_content(),
	_maximumRetentionTime(),
	_target()
{
}


RecordingConfiguration::RecordingConfiguration(const RecordingSourceInformation& source, const std::string& content, const std::string& maximumRetentionTime, const Poco::SharedPtr<RecordingTargetConfiguration>& target):
	_source(source),
	_content(content),
	_maximumRetentionTime(maximumRetentionTime),
	_target(target)
{
}


RecordingConfiguration::~RecordingConfiguration()
{
}


} // ONVIF


#include "ONVIF/RecordingEncryption.h"


namespace ONVIF {


RecordingEncryption::RecordingEncryption():
	_mode(),
	_kID(),
	_key(),
	_track()
{
}


RecordingEncryption::RecordingEncryption(const std::string& mode, const std::string& kID, const Poco::Optional<std::string>& key, const std::vector<std::string>& track):
	_mode(mode),
	_kID(kID),
	_key(key),
	_track(track)
{
}


RecordingEncryption::~RecordingEncryption()
{
}


} // ONVIF


#include "ONVIF/RecordingEventFilter.h"


namespace ONVIF {


RecordingEventFilter::RecordingEventFilter():
	_filter(),
	_before(),
	_after()
{
}


RecordingEventFilter::RecordingEventFilter(const std::vector<Filter>& filter, const Poco::Optional<std::string>& before, const Poco::Optional<std::string>& after):
	_filter(filter),
	_before(before),
	_after(after)
{
}


RecordingEventFilter::~RecordingEventFilter()
{
}


} // ONVIF


#include "ONVIF/RecordingInformation.h"


namespace ONVIF {


RecordingInformation::RecordingInformation():
	_recordingToken(),
	_source(),
	_earliestRecording(),
	_latestRecording(),
	_content(),
	_track(),
	_recordingStatus()
{
}


RecordingInformation::RecordingInformation(const std::string& recordingToken, const RecordingSourceInformation& source, const Poco::Optional<Poco::DateTime>& earliestRecording, const Poco::Optional<Poco::DateTime>& latestRecording, const std::string& content, const std::vector<TrackInformation>& track, const std::string& recordingStatus):
	_recordingToken(recordingToken),
	_source(source),
	_earliestRecording(earliestRecording),
	_latestRecording(latestRecording),
	_content(content),
	_track(track),
	_recordingStatus(recordingStatus)
{
}


RecordingInformation::~RecordingInformation()
{
}


} // ONVIF


#include "ONVIF/RecordingJobConfiguration.h"


namespace ONVIF {


RecordingJobConfiguration::RecordingJobConfiguration():
	_scheduleToken(),
	_recordingToken(),
	_mode(),
	_priority(),
	_source(),
	_extension(),
	_eventFilter()
{
}


RecordingJobConfiguration::RecordingJobConfiguration(const Poco::Optional<std::string>& scheduleToken, const std::string& recordingToken, const std::string& mode, int priority, const std::vector<RecordingJobSource>& source, const Poco::SharedPtr<RecordingJobConfigurationExtension>& extension, const Poco::SharedPtr<RecordingEventFilter>& eventFilter):
	_scheduleToken(scheduleToken),
	_recordingToken(recordingToken),
	_mode(mode),
	_priority(priority),
	_source(source),
	_extension(extension),
	_eventFilter(eventFilter)
{
}


RecordingJobConfiguration::~RecordingJobConfiguration()
{
}


} // ONVIF


#include "ONVIF/RecordingJobConfigurationExtension.h"


namespace ONVIF {


RecordingJobConfigurationExtension::RecordingJobConfigurationExtension()
{
}


RecordingJobConfigurationExtension::~RecordingJobConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/RecordingJobSource.h"


namespace ONVIF {


RecordingJobSource::RecordingJobSource():
	_sourceToken(),
	_autoCreateReceiver(),
	_tracks(),
	_extension()
{
}


RecordingJobSource::RecordingJobSource(const Poco::SharedPtr<SourceReference>& sourceToken, Poco::Optional<bool> autoCreateReceiver, const std::vector<RecordingJobTrack>& tracks, const Poco::SharedPtr<RecordingJobSourceExtension>& extension):
	_sourceToken(sourceToken),
	_autoCreateReceiver(autoCreateReceiver),
	_tracks(tracks),
	_extension(extension)
{
}


RecordingJobSource::~RecordingJobSource()
{
}


} // ONVIF


#include "ONVIF/RecordingJobSourceExtension.h"


namespace ONVIF {


RecordingJobSourceExtension::RecordingJobSourceExtension()
{
}


RecordingJobSourceExtension::~RecordingJobSourceExtension()
{
}


} // ONVIF


#include "ONVIF/RecordingJobStateInformation.h"


namespace ONVIF {


RecordingJobStateInformation::RecordingJobStateInformation():
	_recordingToken(),
	_state(),
	_sources(),
	_extension()
{
}


RecordingJobStateInformation::RecordingJobStateInformation(const std::string& recordingToken, const std::string& state, const std::vector<RecordingJobStateSource>& sources, const Poco::SharedPtr<RecordingJobStateInformationExtension>& extension):
	_recordingToken(recordingToken),
	_state(state),
	_sources(sources),
	_extension(extension)
{
}


RecordingJobStateInformation::~RecordingJobStateInformation()
{
}


} // ONVIF


#include "ONVIF/RecordingJobStateInformationExtension.h"


namespace ONVIF {


RecordingJobStateInformationExtension::RecordingJobStateInformationExtension()
{
}


RecordingJobStateInformationExtension::~RecordingJobStateInformationExtension()
{
}


} // ONVIF


#include "ONVIF/RecordingJobStateSource.h"


namespace ONVIF {


RecordingJobStateSource::RecordingJobStateSource():
	_sourceToken(),
	_state(),
	_tracks()
{
}


RecordingJobStateSource::RecordingJobStateSource(const SourceReference& sourceToken, const std::string& state, const RecordingJobStateTracks& tracks):
	_sourceToken(sourceToken),
	_state(state),
	_tracks(tracks)
{
}


RecordingJobStateSource::~RecordingJobStateSource()
{
}


} // ONVIF


#include "ONVIF/RecordingJobStateTrack.h"


namespace ONVIF {


RecordingJobStateTrack::RecordingJobStateTrack():
	_sourceTag(),
	_destination(),
	_error(),
	_state()
{
}


RecordingJobStateTrack::RecordingJobStateTrack(const std::string& sourceTag, const std::string& destination, const Poco::Optional<std::string>& error, const std::string& state):
	_sourceTag(sourceTag),
	_destination(destination),
	_error(error),
	_state(state)
{
}


RecordingJobStateTrack::~RecordingJobStateTrack()
{
}


} // ONVIF


#include "ONVIF/RecordingJobStateTracks.h"


namespace ONVIF {


RecordingJobStateTracks::RecordingJobStateTracks():
	_track()
{
}


RecordingJobStateTracks::RecordingJobStateTracks(const std::vector<RecordingJobStateTrack>& track):
	_track(track)
{
}


RecordingJobStateTracks::~RecordingJobStateTracks()
{
}


} // ONVIF


#include "ONVIF/RecordingJobTrack.h"


namespace ONVIF {


RecordingJobTrack::RecordingJobTrack():
	_sourceTag(),
	_destination()
{
}


RecordingJobTrack::RecordingJobTrack(const std::string& sourceTag, const std::string& destination):
	_sourceTag(sourceTag),
	_destination(destination)
{
}


RecordingJobTrack::~RecordingJobTrack()
{
}


} // ONVIF


#include "ONVIF/RecordingSourceInformation.h"


namespace ONVIF {


RecordingSourceInformation::RecordingSourceInformation():
	_sourceId(),
	_name(),
	_location(),
	_description(),
	_address()
{
}


RecordingSourceInformation::RecordingSourceInformation(const Poco::URI& sourceId, const std::string& name, const std::string& location, const std::string& description, const Poco::URI& address):
	_sourceId(sourceId),
	_name(name),
	_location(location),
	_description(description),
	_address(address)
{
}


RecordingSourceInformation::~RecordingSourceInformation()
{
}


} // ONVIF


#include "ONVIF/RecordingSummary.h"


namespace ONVIF {


RecordingSummary::RecordingSummary():
	_dataFrom(),
	_dataUntil(),
	_numberRecordings()
{
}


RecordingSummary::RecordingSummary(const Poco::DateTime& dataFrom, const Poco::DateTime& dataUntil, int numberRecordings):
	_dataFrom(dataFrom),
	_dataUntil(dataUntil),
	_numberRecordings(numberRecordings)
{
}


RecordingSummary::~RecordingSummary()
{
}


} // ONVIF


#include "ONVIF/RecordingTargetConfiguration.h"


namespace ONVIF {


RecordingTargetConfiguration::RecordingTargetConfiguration():
	_storage(),
	_format(),
	_prefix(),
	_postfix(),
	_spanDuration(),
	_segmentDuration(),
	_encryption()
{
}


RecordingTargetConfiguration::RecordingTargetConfiguration(const std::string& storage, const std::string& format, const Poco::Optional<std::string>& prefix, const Poco::Optional<std::string>& postfix, const Poco::Optional<std::string>& spanDuration, const std::string& segmentDuration, const std::vector<RecordingEncryption>& encryption):
	_storage(storage),
	_format(format),
	_prefix(prefix),
	_postfix(postfix),
	_spanDuration(spanDuration),
	_segmentDuration(segmentDuration),
	_encryption(encryption)
{
}


RecordingTargetConfiguration::~RecordingTargetConfiguration()
{
}


} // ONVIF


#include "ONVIF/Rectangle.h"


namespace ONVIF {


Rectangle::Rectangle():
	_bottom(),
	_top(),
	_right(),
	_left()
{
}


Rectangle::Rectangle(Poco::Optional<float> bottom, Poco::Optional<float> top, Poco::Optional<float> right, Poco::Optional<float> left):
	_bottom(bottom),
	_top(top),
	_right(right),
	_left(left)
{
}


Rectangle::~Rectangle()
{
}


} // ONVIF


#include "ONVIF/RelativeFocus.h"


namespace ONVIF {


RelativeFocus::RelativeFocus():
	_distance(),
	_speed()
{
}


RelativeFocus::RelativeFocus(float distance, Poco::Optional<float> speed):
	_distance(distance),
	_speed(speed)
{
}


RelativeFocus::~RelativeFocus()
{
}


} // ONVIF


#include "ONVIF/RelativeFocusOptions.h"


namespace ONVIF {


RelativeFocusOptions::RelativeFocusOptions():
	_distance(),
	_speed()
{
}


RelativeFocusOptions::RelativeFocusOptions(const FloatRange& distance, const FloatRange& speed):
	_distance(distance),
	_speed(speed)
{
}


RelativeFocusOptions::~RelativeFocusOptions()
{
}


} // ONVIF


#include "ONVIF/RelativeFocusOptions20.h"


namespace ONVIF {


RelativeFocusOptions20::RelativeFocusOptions20():
	_distance(),
	_speed()
{
}


RelativeFocusOptions20::RelativeFocusOptions20(const FloatRange& distance, const Poco::SharedPtr<FloatRange>& speed):
	_distance(distance),
	_speed(speed)
{
}


RelativeFocusOptions20::~RelativeFocusOptions20()
{
}


} // ONVIF


#include "ONVIF/RelayOutput.h"


namespace ONVIF {


RelayOutput::RelayOutput():
	_properties()
{
}


RelayOutput::RelayOutput(const std::string& token, const RelayOutputSettings& properties):
	DeviceEntity(token),
	_properties(properties)
{
}


RelayOutput::~RelayOutput()
{
}


} // ONVIF


#include "ONVIF/RelayOutputSettings.h"


namespace ONVIF {


RelayOutputSettings::RelayOutputSettings():
	_mode(),
	_delayTime(),
	_idleState()
{
}


RelayOutputSettings::RelayOutputSettings(const std::string& mode, const std::string& delayTime, const std::string& idleState):
	_mode(mode),
	_delayTime(delayTime),
	_idleState(idleState)
{
}


RelayOutputSettings::~RelayOutputSettings()
{
}


} // ONVIF


#include "ONVIF/RemoteUser.h"


namespace ONVIF {


RemoteUser::RemoteUser():
	_username(),
	_password(),
	_useDerivedPassword()
{
}


RemoteUser::RemoteUser(const std::string& username, const Poco::Optional<std::string>& password, bool useDerivedPassword):
	_username(username),
	_password(password),
	_useDerivedPassword(useDerivedPassword)
{
}


RemoteUser::~RemoteUser()
{
}


} // ONVIF


#include "ONVIF/ReplayCapabilities.h"


namespace ONVIF {


ReplayCapabilities::ReplayCapabilities():
	_xAddr()
{
}


ReplayCapabilities::ReplayCapabilities(const Poco::URI& xAddr):
	_xAddr(xAddr)
{
}


ReplayCapabilities::~ReplayCapabilities()
{
}


} // ONVIF


#include "ONVIF/ReplayConfiguration.h"


namespace ONVIF {


ReplayConfiguration::ReplayConfiguration():
	_sessionTimeout()
{
}


ReplayConfiguration::ReplayConfiguration(const std::string& sessionTimeout):
	_sessionTimeout(sessionTimeout)
{
}


ReplayConfiguration::~ReplayConfiguration()
{
}


} // ONVIF


#include "ONVIF/Reverse.h"


namespace ONVIF {


Reverse::Reverse():
	_mode()
{
}


Reverse::Reverse(const std::string& mode):
	_mode(mode)
{
}


Reverse::~Reverse()
{
}


} // ONVIF


#include "ONVIF/ReverseOptions.h"


namespace ONVIF {


ReverseOptions::ReverseOptions():
	_mode(),
	_extension()
{
}


ReverseOptions::ReverseOptions(const std::vector<std::string>& mode, const Poco::SharedPtr<ReverseOptionsExtension>& extension):
	_mode(mode),
	_extension(extension)
{
}


ReverseOptions::~ReverseOptions()
{
}


} // ONVIF


#include "ONVIF/ReverseOptionsExtension.h"


namespace ONVIF {


ReverseOptionsExtension::ReverseOptionsExtension()
{
}


ReverseOptionsExtension::~ReverseOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/Rotate.h"


namespace ONVIF {


Rotate::Rotate():
	_mode(),
	_degree(),
	_extension()
{
}


Rotate::Rotate(const std::string& mode, Poco::Optional<int> degree, const Poco::SharedPtr<RotateExtension>& extension):
	_mode(mode),
	_degree(degree),
	_extension(extension)
{
}


Rotate::~Rotate()
{
}


} // ONVIF


#include "ONVIF/RotateExtension.h"


namespace ONVIF {


RotateExtension::RotateExtension()
{
}


RotateExtension::~RotateExtension()
{
}


} // ONVIF


#include "ONVIF/RotateOptions.h"


namespace ONVIF {


RotateOptions::RotateOptions():
	_reboot(),
	_mode(),
	_degreeList(),
	_extension()
{
}


RotateOptions::RotateOptions(Poco::Optional<bool> reboot, const std::vector<std::string>& mode, const Poco::SharedPtr<IntItems>& degreeList, const Poco::SharedPtr<RotateOptionsExtension>& extension):
	_reboot(reboot),
	_mode(mode),
	_degreeList(degreeList),
	_extension(extension)
{
}


RotateOptions::~RotateOptions()
{
}


} // ONVIF


#include "ONVIF/RotateOptionsExtension.h"


namespace ONVIF {


RotateOptionsExtension::RotateOptionsExtension()
{
}


RotateOptionsExtension::~RotateOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/RuleEngineConfiguration.h"


namespace ONVIF {


RuleEngineConfiguration::RuleEngineConfiguration():
	_rule(),
	_extension()
{
}


RuleEngineConfiguration::RuleEngineConfiguration(const std::vector<Config>& rule, const Poco::SharedPtr<RuleEngineConfigurationExtension>& extension):
	_rule(rule),
	_extension(extension)
{
}


RuleEngineConfiguration::~RuleEngineConfiguration()
{
}


} // ONVIF


#include "ONVIF/RuleEngineConfigurationExtension.h"


namespace ONVIF {


RuleEngineConfigurationExtension::RuleEngineConfigurationExtension()
{
}


RuleEngineConfigurationExtension::~RuleEngineConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/SceneOrientation.h"


namespace ONVIF {


SceneOrientation::SceneOrientation():
	_mode(),
	_orientation()
{
}


SceneOrientation::SceneOrientation(const std::string& mode, const Poco::Optional<std::string>& orientation):
	_mode(mode),
	_orientation(orientation)
{
}


SceneOrientation::~SceneOrientation()
{
}


} // ONVIF


#include "ONVIF/Scope.h"


namespace ONVIF {


Scope::Scope():
	_scopeDef(),
	_scopeItem()
{
}


Scope::Scope(const std::string& scopeDef, const Poco::URI& scopeItem):
	_scopeDef(scopeDef),
	_scopeItem(scopeItem)
{
}


Scope::~Scope()
{
}


} // ONVIF


#include "ONVIF/SearchCapabilities.h"


namespace ONVIF {


SearchCapabilities::SearchCapabilities():
	_xAddr(),
	_metadataSearch()
{
}


SearchCapabilities::SearchCapabilities(const Poco::URI& xAddr, bool metadataSearch):
	_xAddr(xAddr),
	_metadataSearch(metadataSearch)
{
}


SearchCapabilities::~SearchCapabilities()
{
}


} // ONVIF


#include "ONVIF/SearchScope.h"


namespace ONVIF {


SearchScope::SearchScope():
	_includedSources(),
	_includedRecordings(),
	_recordingInformationFilter(),
	_extension()
{
}


SearchScope::SearchScope(const std::vector<SourceReference>& includedSources, const std::vector<std::string>& includedRecordings, const Poco::Optional<std::string>& recordingInformationFilter, const Poco::SharedPtr<SearchScopeExtension>& extension):
	_includedSources(includedSources),
	_includedRecordings(includedRecordings),
	_recordingInformationFilter(recordingInformationFilter),
	_extension(extension)
{
}


SearchScope::~SearchScope()
{
}


} // ONVIF


#include "ONVIF/SearchScopeExtension.h"


namespace ONVIF {


SearchScopeExtension::SearchScopeExtension()
{
}


SearchScopeExtension::~SearchScopeExtension()
{
}


} // ONVIF


#include "ONVIF/SecurityCapabilities.h"


namespace ONVIF {


SecurityCapabilities::SecurityCapabilities():
	_tLS1_1(),
	_tLS1_2(),
	_onboardKeyGeneration(),
	_accessPolicyConfig(),
	_x_509Token(),
	_sAMLToken(),
	_kerberosToken(),
	_rELToken(),
	_extension()
{
}


SecurityCapabilities::SecurityCapabilities(bool tLS1_1, bool tLS1_2, bool onboardKeyGeneration, bool accessPolicyConfig, bool x_509Token, bool sAMLToken, bool kerberosToken, bool rELToken, const Poco::SharedPtr<SecurityCapabilitiesExtension>& extension):
	_tLS1_1(tLS1_1),
	_tLS1_2(tLS1_2),
	_onboardKeyGeneration(onboardKeyGeneration),
	_accessPolicyConfig(accessPolicyConfig),
	_x_509Token(x_509Token),
	_sAMLToken(sAMLToken),
	_kerberosToken(kerberosToken),
	_rELToken(rELToken),
	_extension(extension)
{
}


SecurityCapabilities::~SecurityCapabilities()
{
}


} // ONVIF


#include "ONVIF/SecurityCapabilitiesExtension.h"


namespace ONVIF {


SecurityCapabilitiesExtension::SecurityCapabilitiesExtension():
	_tLS1_0(),
	_extension()
{
}


SecurityCapabilitiesExtension::SecurityCapabilitiesExtension(bool tLS1_0, const Poco::SharedPtr<SecurityCapabilitiesExtension2>& extension):
	_tLS1_0(tLS1_0),
	_extension(extension)
{
}


SecurityCapabilitiesExtension::~SecurityCapabilitiesExtension()
{
}


} // ONVIF


#include "ONVIF/SecurityCapabilitiesExtension2.h"


namespace ONVIF {


SecurityCapabilitiesExtension2::SecurityCapabilitiesExtension2():
	_dot1X(),
	_supportedEAPMethod(),
	_remoteUserHandling()
{
}


SecurityCapabilitiesExtension2::SecurityCapabilitiesExtension2(bool dot1X, const std::vector<int>& supportedEAPMethod, bool remoteUserHandling):
	_dot1X(dot1X),
	_supportedEAPMethod(supportedEAPMethod),
	_remoteUserHandling(remoteUserHandling)
{
}


SecurityCapabilitiesExtension2::~SecurityCapabilitiesExtension2()
{
}


} // ONVIF


#include "ONVIF/SimpleItem.h"


namespace ONVIF {


SimpleItem::SimpleItem():
	_name(),
	_value()
{
}


SimpleItem::SimpleItem(const std::string& name, const std::string& value):
	_name(name),
	_value(value)
{
}


SimpleItem::~SimpleItem()
{
}


} // ONVIF


#include "ONVIF/SimpleItemDescription.h"


namespace ONVIF {


SimpleItemDescription::SimpleItemDescription():
	_name(),
	_type()
{
}


SimpleItemDescription::SimpleItemDescription(const std::string& name, const std::string& type):
	_name(name),
	_type(type)
{
}


SimpleItemDescription::~SimpleItemDescription()
{
}


} // ONVIF


#include "ONVIF/SourceIdentification.h"


namespace ONVIF {


SourceIdentification::SourceIdentification():
	_name(),
	_token(),
	_extension()
{
}


SourceIdentification::SourceIdentification(const std::string& name, const std::vector<std::string>& token, const Poco::SharedPtr<SourceIdentificationExtension>& extension):
	_name(name),
	_token(token),
	_extension(extension)
{
}


SourceIdentification::~SourceIdentification()
{
}


} // ONVIF


#include "ONVIF/SourceIdentificationExtension.h"


namespace ONVIF {


SourceIdentificationExtension::SourceIdentificationExtension()
{
}


SourceIdentificationExtension::~SourceIdentificationExtension()
{
}


} // ONVIF


#include "ONVIF/SourceReference.h"


namespace ONVIF {


SourceReference::SourceReference():
	_type(Poco::URI("http://www.onvif.org/ver10/schema/Receiver")),
	_token()
{
}


SourceReference::SourceReference(const Poco::Optional<Poco::URI>& type, const std::string& token):
	_type(type),
	_token(token)
{
}


SourceReference::~SourceReference()
{
}


} // ONVIF


#include "ONVIF/Space1DDescription.h"


namespace ONVIF {


Space1DDescription::Space1DDescription():
	_uRI(),
	_xRange()
{
}


Space1DDescription::Space1DDescription(const Poco::URI& uRI, const FloatRange& xRange):
	_uRI(uRI),
	_xRange(xRange)
{
}


Space1DDescription::~Space1DDescription()
{
}


} // ONVIF


#include "ONVIF/Space2DDescription.h"


namespace ONVIF {


Space2DDescription::Space2DDescription():
	_uRI(),
	_xRange(),
	_yRange()
{
}


Space2DDescription::Space2DDescription(const Poco::URI& uRI, const FloatRange& xRange, const FloatRange& yRange):
	_uRI(uRI),
	_xRange(xRange),
	_yRange(yRange)
{
}


Space2DDescription::~Space2DDescription()
{
}


} // ONVIF


#include "ONVIF/SphericalCoordinate.h"


namespace ONVIF {


SphericalCoordinate::SphericalCoordinate():
	_distance(),
	_elevationAngle(),
	_azimuthAngle()
{
}


SphericalCoordinate::SphericalCoordinate(Poco::Optional<float> distance, Poco::Optional<float> elevationAngle, Poco::Optional<float> azimuthAngle):
	_distance(distance),
	_elevationAngle(elevationAngle),
	_azimuthAngle(azimuthAngle)
{
}


SphericalCoordinate::~SphericalCoordinate()
{
}


} // ONVIF


#include "ONVIF/StorageReferencePath.h"


namespace ONVIF {


StorageReferencePath::StorageReferencePath():
	_storageToken(),
	_relativePath(),
	_extension()
{
}


StorageReferencePath::StorageReferencePath(const std::string& storageToken, const Poco::Optional<std::string>& relativePath, const Poco::SharedPtr<StorageReferencePathExtension>& extension):
	_storageToken(storageToken),
	_relativePath(relativePath),
	_extension(extension)
{
}


StorageReferencePath::~StorageReferencePath()
{
}


} // ONVIF


#include "ONVIF/StorageReferencePathExtension.h"


namespace ONVIF {


StorageReferencePathExtension::StorageReferencePathExtension()
{
}


StorageReferencePathExtension::~StorageReferencePathExtension()
{
}


} // ONVIF


#include "ONVIF/StreamSetup.h"


namespace ONVIF {


StreamSetup::StreamSetup():
	_stream(),
	_transport()
{
}


StreamSetup::StreamSetup(const std::string& stream, const Transport& transport):
	_stream(stream),
	_transport(transport)
{
}


StreamSetup::~StreamSetup()
{
}


} // ONVIF


#include "ONVIF/StringItems.h"


namespace ONVIF {


StringItems::StringItems():
	_item()
{
}


StringItems::StringItems(const std::vector<std::string>& item):
	_item(item)
{
}


StringItems::~StringItems()
{
}


} // ONVIF


#include "ONVIF/SubscriptionPolicy.h"


namespace ONVIF {


SubscriptionPolicy::SubscriptionPolicy()
{
}


SubscriptionPolicy::~SubscriptionPolicy()
{
}


} // ONVIF


#include "ONVIF/SupportInformation.h"


namespace ONVIF {


SupportInformation::SupportInformation():
	_binary(),
	_string()
{
}


SupportInformation::SupportInformation(const Poco::SharedPtr<AttachmentData>& binary, const Poco::Optional<std::string>& string):
	_binary(binary),
	_string(string)
{
}


SupportInformation::~SupportInformation()
{
}


} // ONVIF


#include "ONVIF/SupportedAnalyticsModules.h"


namespace ONVIF {


SupportedAnalyticsModules::SupportedAnalyticsModules():
	_limit(),
	_analyticsModuleContentSchemaLocation(),
	_analyticsModuleDescription(),
	_extension()
{
}


SupportedAnalyticsModules::SupportedAnalyticsModules(Poco::Optional<int> limit, const std::vector<Poco::URI>& analyticsModuleContentSchemaLocation, const std::vector<ConfigDescription>& analyticsModuleDescription, const Poco::SharedPtr<SupportedAnalyticsModulesExtension>& extension):
	_limit(limit),
	_analyticsModuleContentSchemaLocation(analyticsModuleContentSchemaLocation),
	_analyticsModuleDescription(analyticsModuleDescription),
	_extension(extension)
{
}


SupportedAnalyticsModules::~SupportedAnalyticsModules()
{
}


} // ONVIF


#include "ONVIF/SupportedAnalyticsModulesExtension.h"


namespace ONVIF {


SupportedAnalyticsModulesExtension::SupportedAnalyticsModulesExtension()
{
}


SupportedAnalyticsModulesExtension::~SupportedAnalyticsModulesExtension()
{
}


} // ONVIF


#include "ONVIF/SupportedRules.h"


namespace ONVIF {


SupportedRules::SupportedRules():
	_limit(),
	_ruleContentSchemaLocation(),
	_ruleDescription(),
	_extension()
{
}


SupportedRules::SupportedRules(Poco::Optional<int> limit, const std::vector<Poco::URI>& ruleContentSchemaLocation, const std::vector<ConfigDescription>& ruleDescription, const Poco::SharedPtr<SupportedRulesExtension>& extension):
	_limit(limit),
	_ruleContentSchemaLocation(ruleContentSchemaLocation),
	_ruleDescription(ruleDescription),
	_extension(extension)
{
}


SupportedRules::~SupportedRules()
{
}


} // ONVIF


#include "ONVIF/SupportedRulesExtension.h"


namespace ONVIF {


SupportedRulesExtension::SupportedRulesExtension()
{
}


SupportedRulesExtension::~SupportedRulesExtension()
{
}


} // ONVIF


#include "ONVIF/SystemCapabilities.h"


namespace ONVIF {


SystemCapabilities::SystemCapabilities():
	_discoveryResolve(),
	_discoveryBye(),
	_remoteDiscovery(),
	_systemBackup(),
	_systemLogging(),
	_firmwareUpgrade(),
	_supportedVersions(),
	_extension()
{
}


SystemCapabilities::SystemCapabilities(bool discoveryResolve, bool discoveryBye, bool remoteDiscovery, bool systemBackup, bool systemLogging, bool firmwareUpgrade, const std::vector<OnvifVersion>& supportedVersions, const Poco::SharedPtr<SystemCapabilitiesExtension>& extension):
	_discoveryResolve(discoveryResolve),
	_discoveryBye(discoveryBye),
	_remoteDiscovery(remoteDiscovery),
	_systemBackup(systemBackup),
	_systemLogging(systemLogging),
	_firmwareUpgrade(firmwareUpgrade),
	_supportedVersions(supportedVersions),
	_extension(extension)
{
}


SystemCapabilities::~SystemCapabilities()
{
}


} // ONVIF


#include "ONVIF/SystemCapabilitiesExtension.h"


namespace ONVIF {


SystemCapabilitiesExtension::SystemCapabilitiesExtension():
	_httpFirmwareUpgrade(),
	_httpSystemBackup(),
	_httpSystemLogging(),
	_httpSupportInformation(),
	_extension()
{
}


SystemCapabilitiesExtension::SystemCapabilitiesExtension(Poco::Optional<bool> httpFirmwareUpgrade, Poco::Optional<bool> httpSystemBackup, Poco::Optional<bool> httpSystemLogging, Poco::Optional<bool> httpSupportInformation, const Poco::SharedPtr<SystemCapabilitiesExtension2>& extension):
	_httpFirmwareUpgrade(httpFirmwareUpgrade),
	_httpSystemBackup(httpSystemBackup),
	_httpSystemLogging(httpSystemLogging),
	_httpSupportInformation(httpSupportInformation),
	_extension(extension)
{
}


SystemCapabilitiesExtension::~SystemCapabilitiesExtension()
{
}


} // ONVIF


#include "ONVIF/SystemCapabilitiesExtension2.h"


namespace ONVIF {


SystemCapabilitiesExtension2::SystemCapabilitiesExtension2()
{
}


SystemCapabilitiesExtension2::~SystemCapabilitiesExtension2()
{
}


} // ONVIF


#include "ONVIF/SystemDateTime.h"


namespace ONVIF {


SystemDateTime::SystemDateTime():
	_dateTimeType(),
	_daylightSavings(),
	_timeZone(),
	_uTCDateTime(),
	_localDateTime(),
	_extension()
{
}


SystemDateTime::SystemDateTime(const std::string& dateTimeType, bool daylightSavings, const Poco::SharedPtr<TimeZone>& timeZone, const Poco::SharedPtr<DateTime>& uTCDateTime, const Poco::SharedPtr<DateTime>& localDateTime, const Poco::SharedPtr<SystemDateTimeExtension>& extension):
	_dateTimeType(dateTimeType),
	_daylightSavings(daylightSavings),
	_timeZone(timeZone),
	_uTCDateTime(uTCDateTime),
	_localDateTime(localDateTime),
	_extension(extension)
{
}


SystemDateTime::~SystemDateTime()
{
}


} // ONVIF


#include "ONVIF/SystemDateTimeExtension.h"


namespace ONVIF {


SystemDateTimeExtension::SystemDateTimeExtension()
{
}


SystemDateTimeExtension::~SystemDateTimeExtension()
{
}


} // ONVIF


#include "ONVIF/SystemLog.h"


namespace ONVIF {


SystemLog::SystemLog():
	_binary(),
	_string()
{
}


SystemLog::SystemLog(const Poco::SharedPtr<AttachmentData>& binary, const Poco::Optional<std::string>& string):
	_binary(binary),
	_string(string)
{
}


SystemLog::~SystemLog()
{
}


} // ONVIF


#include "ONVIF/SystemLogUri.h"


namespace ONVIF {


SystemLogUri::SystemLogUri():
	_type(),
	_uri()
{
}


SystemLogUri::SystemLogUri(const SystemLog& type, const Poco::URI& uri):
	_type(type),
	_uri(uri)
{
}


SystemLogUri::~SystemLogUri()
{
}


} // ONVIF


#include "ONVIF/SystemLogUriList.h"


namespace ONVIF {


SystemLogUriList::SystemLogUriList():
	_systemLog()
{
}


SystemLogUriList::SystemLogUriList(const std::vector<SystemLogUri>& systemLog):
	_systemLog(systemLog)
{
}


SystemLogUriList::~SystemLogUriList()
{
}


} // ONVIF


#include "ONVIF/TLSConfiguration.h"


namespace ONVIF {


TLSConfiguration::TLSConfiguration():
	_certificateID()
{
}


TLSConfiguration::TLSConfiguration(const std::string& certificateID):
	_certificateID(certificateID)
{
}


TLSConfiguration::~TLSConfiguration()
{
}


} // ONVIF


#include "ONVIF/Time.h"


namespace ONVIF {


Time::Time():
	_hour(),
	_minute(),
	_second()
{
}


Time::Time(int hour, int minute, int second):
	_hour(hour),
	_minute(minute),
	_second(second)
{
}


Time::~Time()
{
}


} // ONVIF


#include "ONVIF/TimeZone.h"


namespace ONVIF {


TimeZone::TimeZone():
	_tZ()
{
}


TimeZone::TimeZone(const std::string& tZ):
	_tZ(tZ)
{
}


TimeZone::~TimeZone()
{
}


} // ONVIF


#include "ONVIF/ToneCompensation.h"


namespace ONVIF {


ToneCompensation::ToneCompensation():
	_mode(),
	_level(),
	_extension()
{
}


ToneCompensation::ToneCompensation(const std::string& mode, Poco::Optional<float> level, const Poco::SharedPtr<ToneCompensationExtension>& extension):
	_mode(mode),
	_level(level),
	_extension(extension)
{
}


ToneCompensation::~ToneCompensation()
{
}


} // ONVIF


#include "ONVIF/ToneCompensationExtension.h"


namespace ONVIF {


ToneCompensationExtension::ToneCompensationExtension()
{
}


ToneCompensationExtension::~ToneCompensationExtension()
{
}


} // ONVIF


#include "ONVIF/ToneCompensationOptions.h"


namespace ONVIF {


ToneCompensationOptions::ToneCompensationOptions():
	_mode(),
	_level()
{
}


ToneCompensationOptions::ToneCompensationOptions(const std::vector<std::string>& mode, bool level):
	_mode(mode),
	_level(level)
{
}


ToneCompensationOptions::~ToneCompensationOptions()
{
}


} // ONVIF


#include "ONVIF/TrackAttributes.h"


namespace ONVIF {


TrackAttributes::TrackAttributes():
	_trackInformation(),
	_videoAttributes(),
	_audioAttributes(),
	_metadataAttributes(),
	_extension()
{
}


TrackAttributes::TrackAttributes(const TrackInformation& trackInformation, const Poco::SharedPtr<VideoAttributes>& videoAttributes, const Poco::SharedPtr<AudioAttributes>& audioAttributes, const Poco::SharedPtr<MetadataAttributes>& metadataAttributes, const Poco::SharedPtr<TrackAttributesExtension>& extension):
	_trackInformation(trackInformation),
	_videoAttributes(videoAttributes),
	_audioAttributes(audioAttributes),
	_metadataAttributes(metadataAttributes),
	_extension(extension)
{
}


TrackAttributes::~TrackAttributes()
{
}


} // ONVIF


#include "ONVIF/TrackAttributesExtension.h"


namespace ONVIF {


TrackAttributesExtension::TrackAttributesExtension()
{
}


TrackAttributesExtension::~TrackAttributesExtension()
{
}


} // ONVIF


#include "ONVIF/TrackConfiguration.h"


namespace ONVIF {


TrackConfiguration::TrackConfiguration():
	_trackType(),
	_description()
{
}


TrackConfiguration::TrackConfiguration(const std::string& trackType, const std::string& description):
	_trackType(trackType),
	_description(description)
{
}


TrackConfiguration::~TrackConfiguration()
{
}


} // ONVIF


#include "ONVIF/TrackInformation.h"


namespace ONVIF {


TrackInformation::TrackInformation():
	_trackToken(),
	_trackType(),
	_description(),
	_dataFrom(),
	_dataTo()
{
}


TrackInformation::TrackInformation(const std::string& trackToken, const std::string& trackType, const std::string& description, const Poco::DateTime& dataFrom, const Poco::DateTime& dataTo):
	_trackToken(trackToken),
	_trackType(trackType),
	_description(description),
	_dataFrom(dataFrom),
	_dataTo(dataTo)
{
}


TrackInformation::~TrackInformation()
{
}


} // ONVIF


#include "ONVIF/Transformation.h"


namespace ONVIF {


Transformation::Transformation():
	_translate(),
	_scale(),
	_extension()
{
}


Transformation::Transformation(const Poco::SharedPtr<Vector>& translate, const Poco::SharedPtr<Vector>& scale, const Poco::SharedPtr<TransformationExtension>& extension):
	_translate(translate),
	_scale(scale),
	_extension(extension)
{
}


Transformation::~Transformation()
{
}


} // ONVIF


#include "ONVIF/TransformationExtension.h"


namespace ONVIF {


TransformationExtension::TransformationExtension()
{
}


TransformationExtension::~TransformationExtension()
{
}


} // ONVIF


#include "ONVIF/Transport.h"


namespace ONVIF {


Transport::Transport():
	_protocol(),
	_tunnel()
{
}


Transport::Transport(const std::string& protocol, const Poco::SharedPtr<Transport>& tunnel):
	_protocol(protocol),
	_tunnel(tunnel)
{
}


Transport::~Transport()
{
}


} // ONVIF


#include "ONVIF/User.h"


namespace ONVIF {


User::User():
	_username(),
	_password(),
	_userLevel(),
	_extension()
{
}


User::User(const std::string& username, const Poco::Optional<std::string>& password, const std::string& userLevel, const Poco::SharedPtr<UserExtension>& extension):
	_username(username),
	_password(password),
	_userLevel(userLevel),
	_extension(extension)
{
}


User::~User()
{
}


} // ONVIF


#include "ONVIF/UserExtension.h"


namespace ONVIF {


UserExtension::UserExtension()
{
}


UserExtension::~UserExtension()
{
}


} // ONVIF


#include "ONVIF/Vector.h"


namespace ONVIF {


Vector::Vector():
	_x(),
	_y()
{
}


Vector::Vector(Poco::Optional<float> x, Poco::Optional<float> y):
	_x(x),
	_y(y)
{
}


Vector::~Vector()
{
}


} // ONVIF


#include "ONVIF/Vector1D.h"


namespace ONVIF {


Vector1D::Vector1D():
	_x(),
	_space()
{
}


Vector1D::Vector1D(float x, const Poco::Optional<Poco::URI>& space):
	_x(x),
	_space(space)
{
}


Vector1D::~Vector1D()
{
}


} // ONVIF


#include "ONVIF/Vector2D.h"


namespace ONVIF {


Vector2D::Vector2D():
	_x(),
	_y(),
	_space()
{
}


Vector2D::Vector2D(float x, float y, const Poco::Optional<Poco::URI>& space):
	_x(x),
	_y(y),
	_space(space)
{
}


Vector2D::~Vector2D()
{
}


} // ONVIF


#include "ONVIF/VideoAnalyticsConfiguration.h"


namespace ONVIF {


VideoAnalyticsConfiguration::VideoAnalyticsConfiguration():
	_analyticsEngineConfiguration(),
	_ruleEngineConfiguration()
{
}


VideoAnalyticsConfiguration::VideoAnalyticsConfiguration(const std::string& token, const std::string& name, int useCount, const AnalyticsEngineConfiguration& analyticsEngineConfiguration, const RuleEngineConfiguration& ruleEngineConfiguration):
	ConfigurationEntity(token, name, useCount),
	_analyticsEngineConfiguration(analyticsEngineConfiguration),
	_ruleEngineConfiguration(ruleEngineConfiguration)
{
}


VideoAnalyticsConfiguration::~VideoAnalyticsConfiguration()
{
}


} // ONVIF


#include "ONVIF/VideoAttributes.h"


namespace ONVIF {


VideoAttributes::VideoAttributes():
	_bitrate(),
	_width(),
	_height(),
	_encoding(),
	_framerate()
{
}


VideoAttributes::VideoAttributes(Poco::Optional<int> bitrate, int width, int height, const std::string& encoding, float framerate):
	_bitrate(bitrate),
	_width(width),
	_height(height),
	_encoding(encoding),
	_framerate(framerate)
{
}


VideoAttributes::~VideoAttributes()
{
}


} // ONVIF


#include "ONVIF/VideoDecoderConfigurationOptions.h"


namespace ONVIF {


VideoDecoderConfigurationOptions::VideoDecoderConfigurationOptions():
	_jpegDecOptions(),
	_h264DecOptions(),
	_mpeg4DecOptions(),
	_extension()
{
}


VideoDecoderConfigurationOptions::VideoDecoderConfigurationOptions(const Poco::SharedPtr<JpegDecOptions>& jpegDecOptions, const Poco::SharedPtr<H264DecOptions>& h264DecOptions, const Poco::SharedPtr<Mpeg4DecOptions>& mpeg4DecOptions, const Poco::SharedPtr<VideoDecoderConfigurationOptionsExtension>& extension):
	_jpegDecOptions(jpegDecOptions),
	_h264DecOptions(h264DecOptions),
	_mpeg4DecOptions(mpeg4DecOptions),
	_extension(extension)
{
}


VideoDecoderConfigurationOptions::~VideoDecoderConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/VideoDecoderConfigurationOptionsExtension.h"


namespace ONVIF {


VideoDecoderConfigurationOptionsExtension::VideoDecoderConfigurationOptionsExtension()
{
}


VideoDecoderConfigurationOptionsExtension::~VideoDecoderConfigurationOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/VideoEncoder2Configuration.h"


namespace ONVIF {


VideoEncoder2Configuration::VideoEncoder2Configuration():
	_govLength(),
	_profile(),
	_guaranteedFrameRate(),
	_encoding(),
	_resolution(),
	_rateControl(),
	_multicast(),
	_quality()
{
}


VideoEncoder2Configuration::VideoEncoder2Configuration(const std::string& token, const std::string& name, int useCount, Poco::Optional<int> govLength, const Poco::Optional<std::string>& profile, Poco::Optional<bool> guaranteedFrameRate, const std::string& encoding, const VideoResolution2& resolution, const Poco::SharedPtr<VideoRateControl2>& rateControl, const Poco::SharedPtr<MulticastConfiguration>& multicast, float quality):
	ConfigurationEntity(token, name, useCount),
	_govLength(govLength),
	_profile(profile),
	_guaranteedFrameRate(guaranteedFrameRate),
	_encoding(encoding),
	_resolution(resolution),
	_rateControl(rateControl),
	_multicast(multicast),
	_quality(quality)
{
}


VideoEncoder2Configuration::~VideoEncoder2Configuration()
{
}


} // ONVIF


#include "ONVIF/VideoEncoder2ConfigurationOptions.h"


namespace ONVIF {


VideoEncoder2ConfigurationOptions::VideoEncoder2ConfigurationOptions():
	_govLengthRange(),
	_frameRatesSupported(),
	_profilesSupported(),
	_constantBitRateSupported(),
	_guaranteedFrameRateSupported(),
	_encoding(),
	_qualityRange(),
	_resolutionsAvailable(),
	_bitrateRange()
{
}


VideoEncoder2ConfigurationOptions::VideoEncoder2ConfigurationOptions(const std::vector<std::vector<int>>& govLengthRange, const std::vector<std::vector<float>>& frameRatesSupported, const std::vector<std::vector<std::string>>& profilesSupported, Poco::Optional<bool> constantBitRateSupported, Poco::Optional<bool> guaranteedFrameRateSupported, const std::string& encoding, const FloatRange& qualityRange, const std::vector<VideoResolution2>& resolutionsAvailable, const IntRange& bitrateRange):
	_govLengthRange(govLengthRange),
	_frameRatesSupported(frameRatesSupported),
	_profilesSupported(profilesSupported),
	_constantBitRateSupported(constantBitRateSupported),
	_guaranteedFrameRateSupported(guaranteedFrameRateSupported),
	_encoding(encoding),
	_qualityRange(qualityRange),
	_resolutionsAvailable(resolutionsAvailable),
	_bitrateRange(bitrateRange)
{
}


VideoEncoder2ConfigurationOptions::~VideoEncoder2ConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/VideoEncoderConfiguration.h"


namespace ONVIF {


VideoEncoderConfiguration::VideoEncoderConfiguration():
	_guaranteedFrameRate(),
	_encoding(),
	_resolution(),
	_quality(),
	_rateControl(),
	_mPEG4(),
	_h264(),
	_multicast(),
	_sessionTimeout()
{
}


VideoEncoderConfiguration::VideoEncoderConfiguration(const std::string& token, const std::string& name, int useCount, Poco::Optional<bool> guaranteedFrameRate, const std::string& encoding, const VideoResolution& resolution, float quality, const Poco::SharedPtr<VideoRateControl>& rateControl, const Poco::SharedPtr<Mpeg4Configuration>& mPEG4, const Poco::SharedPtr<H264Configuration>& h264, const MulticastConfiguration& multicast, const std::string& sessionTimeout):
	ConfigurationEntity(token, name, useCount),
	_guaranteedFrameRate(guaranteedFrameRate),
	_encoding(encoding),
	_resolution(resolution),
	_quality(quality),
	_rateControl(rateControl),
	_mPEG4(mPEG4),
	_h264(h264),
	_multicast(multicast),
	_sessionTimeout(sessionTimeout)
{
}


VideoEncoderConfiguration::~VideoEncoderConfiguration()
{
}


} // ONVIF


#include "ONVIF/VideoEncoderConfigurationOptions.h"


namespace ONVIF {


VideoEncoderConfigurationOptions::VideoEncoderConfigurationOptions():
	_guaranteedFrameRateSupported(),
	_qualityRange(),
	_jPEG(),
	_mPEG4(),
	_h264(),
	_extension()
{
}


VideoEncoderConfigurationOptions::VideoEncoderConfigurationOptions(Poco::Optional<bool> guaranteedFrameRateSupported, const IntRange& qualityRange, const Poco::SharedPtr<JpegOptions>& jPEG, const Poco::SharedPtr<Mpeg4Options>& mPEG4, const Poco::SharedPtr<H264Options>& h264, const Poco::SharedPtr<VideoEncoderOptionsExtension>& extension):
	_guaranteedFrameRateSupported(guaranteedFrameRateSupported),
	_qualityRange(qualityRange),
	_jPEG(jPEG),
	_mPEG4(mPEG4),
	_h264(h264),
	_extension(extension)
{
}


VideoEncoderConfigurationOptions::~VideoEncoderConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/VideoEncoderOptionsExtension.h"


namespace ONVIF {


VideoEncoderOptionsExtension::VideoEncoderOptionsExtension():
	_jPEG(),
	_mPEG4(),
	_h264(),
	_extension()
{
}


VideoEncoderOptionsExtension::VideoEncoderOptionsExtension(const Poco::SharedPtr<JpegOptions2>& jPEG, const Poco::SharedPtr<Mpeg4Options2>& mPEG4, const Poco::SharedPtr<H264Options2>& h264, const Poco::SharedPtr<VideoEncoderOptionsExtension2>& extension):
	_jPEG(jPEG),
	_mPEG4(mPEG4),
	_h264(h264),
	_extension(extension)
{
}


VideoEncoderOptionsExtension::~VideoEncoderOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/VideoEncoderOptionsExtension2.h"


namespace ONVIF {


VideoEncoderOptionsExtension2::VideoEncoderOptionsExtension2()
{
}


VideoEncoderOptionsExtension2::~VideoEncoderOptionsExtension2()
{
}


} // ONVIF


#include "ONVIF/VideoOutput.h"


namespace ONVIF {


VideoOutput::VideoOutput():
	_layout(),
	_resolution(),
	_refreshRate(),
	_aspectRatio(),
	_extension()
{
}


VideoOutput::VideoOutput(const std::string& token, const Layout& layout, const Poco::SharedPtr<VideoResolution>& resolution, Poco::Optional<float> refreshRate, Poco::Optional<float> aspectRatio, const Poco::SharedPtr<VideoOutputExtension>& extension):
	DeviceEntity(token),
	_layout(layout),
	_resolution(resolution),
	_refreshRate(refreshRate),
	_aspectRatio(aspectRatio),
	_extension(extension)
{
}


VideoOutput::~VideoOutput()
{
}


} // ONVIF


#include "ONVIF/VideoOutputConfiguration.h"


namespace ONVIF {


VideoOutputConfiguration::VideoOutputConfiguration():
	_outputToken()
{
}


VideoOutputConfiguration::VideoOutputConfiguration(const std::string& token, const std::string& name, int useCount, const std::string& outputToken):
	ConfigurationEntity(token, name, useCount),
	_outputToken(outputToken)
{
}


VideoOutputConfiguration::~VideoOutputConfiguration()
{
}


} // ONVIF


#include "ONVIF/VideoOutputConfigurationOptions.h"


namespace ONVIF {


VideoOutputConfigurationOptions::VideoOutputConfigurationOptions()
{
}


VideoOutputConfigurationOptions::~VideoOutputConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/VideoOutputExtension.h"


namespace ONVIF {


VideoOutputExtension::VideoOutputExtension()
{
}


VideoOutputExtension::~VideoOutputExtension()
{
}


} // ONVIF


#include "ONVIF/VideoRateControl.h"


namespace ONVIF {


VideoRateControl::VideoRateControl():
	_frameRateLimit(),
	_encodingInterval(),
	_bitrateLimit()
{
}


VideoRateControl::VideoRateControl(int frameRateLimit, int encodingInterval, int bitrateLimit):
	_frameRateLimit(frameRateLimit),
	_encodingInterval(encodingInterval),
	_bitrateLimit(bitrateLimit)
{
}


VideoRateControl::~VideoRateControl()
{
}


} // ONVIF


#include "ONVIF/VideoRateControl2.h"


namespace ONVIF {


VideoRateControl2::VideoRateControl2():
	_constantBitRate(),
	_frameRateLimit(),
	_bitrateLimit()
{
}


VideoRateControl2::VideoRateControl2(Poco::Optional<bool> constantBitRate, float frameRateLimit, int bitrateLimit):
	_constantBitRate(constantBitRate),
	_frameRateLimit(frameRateLimit),
	_bitrateLimit(bitrateLimit)
{
}


VideoRateControl2::~VideoRateControl2()
{
}


} // ONVIF


#include "ONVIF/VideoResolution.h"


namespace ONVIF {


VideoResolution::VideoResolution():
	_width(),
	_height()
{
}


VideoResolution::VideoResolution(int width, int height):
	_width(width),
	_height(height)
{
}


VideoResolution::~VideoResolution()
{
}


} // ONVIF


#include "ONVIF/VideoResolution2.h"


namespace ONVIF {


VideoResolution2::VideoResolution2():
	_width(),
	_height()
{
}


VideoResolution2::VideoResolution2(int width, int height):
	_width(width),
	_height(height)
{
}


VideoResolution2::~VideoResolution2()
{
}


} // ONVIF


#include "ONVIF/VideoSource.h"


namespace ONVIF {


VideoSource::VideoSource():
	_framerate(),
	_resolution(),
	_imaging(),
	_extension()
{
}


VideoSource::VideoSource(const std::string& token, float framerate, const VideoResolution& resolution, const Poco::SharedPtr<ImagingSettings>& imaging, const Poco::SharedPtr<VideoSourceExtension>& extension):
	DeviceEntity(token),
	_framerate(framerate),
	_resolution(resolution),
	_imaging(imaging),
	_extension(extension)
{
}


VideoSource::~VideoSource()
{
}


} // ONVIF


#include "ONVIF/VideoSourceConfiguration.h"


namespace ONVIF {


VideoSourceConfiguration::VideoSourceConfiguration():
	_viewMode(),
	_sourceToken(),
	_bounds(),
	_extension()
{
}


VideoSourceConfiguration::VideoSourceConfiguration(const std::string& token, const std::string& name, int useCount, const Poco::Optional<std::string>& viewMode, const std::string& sourceToken, const IntRectangle& bounds, const Poco::SharedPtr<VideoSourceConfigurationExtension>& extension):
	ConfigurationEntity(token, name, useCount),
	_viewMode(viewMode),
	_sourceToken(sourceToken),
	_bounds(bounds),
	_extension(extension)
{
}


VideoSourceConfiguration::~VideoSourceConfiguration()
{
}


} // ONVIF


#include "ONVIF/VideoSourceConfigurationExtension.h"


namespace ONVIF {


VideoSourceConfigurationExtension::VideoSourceConfigurationExtension():
	_rotate(),
	_extension()
{
}


VideoSourceConfigurationExtension::VideoSourceConfigurationExtension(const Poco::SharedPtr<Rotate>& rotate, const Poco::SharedPtr<VideoSourceConfigurationExtension2>& extension):
	_rotate(rotate),
	_extension(extension)
{
}


VideoSourceConfigurationExtension::~VideoSourceConfigurationExtension()
{
}


} // ONVIF


#include "ONVIF/VideoSourceConfigurationExtension2.h"


namespace ONVIF {


VideoSourceConfigurationExtension2::VideoSourceConfigurationExtension2():
	_lensDescription(),
	_sceneOrientation()
{
}


VideoSourceConfigurationExtension2::VideoSourceConfigurationExtension2(const std::vector<LensDescription>& lensDescription, const Poco::SharedPtr<SceneOrientation>& sceneOrientation):
	_lensDescription(lensDescription),
	_sceneOrientation(sceneOrientation)
{
}


VideoSourceConfigurationExtension2::~VideoSourceConfigurationExtension2()
{
}


} // ONVIF


#include "ONVIF/VideoSourceConfigurationOptions.h"


namespace ONVIF {


VideoSourceConfigurationOptions::VideoSourceConfigurationOptions():
	_maximumNumberOfProfiles(),
	_boundsRange(),
	_videoSourceTokensAvailable(),
	_extension()
{
}


VideoSourceConfigurationOptions::VideoSourceConfigurationOptions(Poco::Optional<int> maximumNumberOfProfiles, const IntRectangleRange& boundsRange, const std::vector<std::string>& videoSourceTokensAvailable, const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension>& extension):
	_maximumNumberOfProfiles(maximumNumberOfProfiles),
	_boundsRange(boundsRange),
	_videoSourceTokensAvailable(videoSourceTokensAvailable),
	_extension(extension)
{
}


VideoSourceConfigurationOptions::~VideoSourceConfigurationOptions()
{
}


} // ONVIF


#include "ONVIF/VideoSourceConfigurationOptionsExtension.h"


namespace ONVIF {


VideoSourceConfigurationOptionsExtension::VideoSourceConfigurationOptionsExtension():
	_rotate(),
	_extension()
{
}


VideoSourceConfigurationOptionsExtension::VideoSourceConfigurationOptionsExtension(const Poco::SharedPtr<RotateOptions>& rotate, const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension2>& extension):
	_rotate(rotate),
	_extension(extension)
{
}


VideoSourceConfigurationOptionsExtension::~VideoSourceConfigurationOptionsExtension()
{
}


} // ONVIF


#include "ONVIF/VideoSourceConfigurationOptionsExtension2.h"


namespace ONVIF {


VideoSourceConfigurationOptionsExtension2::VideoSourceConfigurationOptionsExtension2():
	_sceneOrientationMode()
{
}


VideoSourceConfigurationOptionsExtension2::VideoSourceConfigurationOptionsExtension2(const std::vector<std::string>& sceneOrientationMode):
	_sceneOrientationMode(sceneOrientationMode)
{
}


VideoSourceConfigurationOptionsExtension2::~VideoSourceConfigurationOptionsExtension2()
{
}


} // ONVIF


#include "ONVIF/VideoSourceExtension.h"


namespace ONVIF {


VideoSourceExtension::VideoSourceExtension():
	_imaging(),
	_extension()
{
}


VideoSourceExtension::VideoSourceExtension(const Poco::SharedPtr<ImagingSettings20>& imaging, const Poco::SharedPtr<VideoSourceExtension2>& extension):
	_imaging(imaging),
	_extension(extension)
{
}


VideoSourceExtension::~VideoSourceExtension()
{
}


} // ONVIF


#include "ONVIF/VideoSourceExtension2.h"


namespace ONVIF {


VideoSourceExtension2::VideoSourceExtension2()
{
}


VideoSourceExtension2::~VideoSourceExtension2()
{
}


} // ONVIF


#include "ONVIF/WhiteBalance.h"


namespace ONVIF {


WhiteBalance::WhiteBalance():
	_mode(),
	_crGain(),
	_cbGain()
{
}


WhiteBalance::WhiteBalance(const std::string& mode, float crGain, float cbGain):
	_mode(mode),
	_crGain(crGain),
	_cbGain(cbGain)
{
}


WhiteBalance::~WhiteBalance()
{
}


} // ONVIF


#include "ONVIF/WhiteBalance20.h"


namespace ONVIF {


WhiteBalance20::WhiteBalance20():
	_mode(),
	_crGain(),
	_cbGain(),
	_extension()
{
}


WhiteBalance20::WhiteBalance20(const std::string& mode, Poco::Optional<float> crGain, Poco::Optional<float> cbGain, const Poco::SharedPtr<WhiteBalance20Extension>& extension):
	_mode(mode),
	_crGain(crGain),
	_cbGain(cbGain),
	_extension(extension)
{
}


WhiteBalance20::~WhiteBalance20()
{
}


} // ONVIF


#include "ONVIF/WhiteBalance20Extension.h"


namespace ONVIF {


WhiteBalance20Extension::WhiteBalance20Extension()
{
}


WhiteBalance20Extension::~WhiteBalance20Extension()
{
}


} // ONVIF


#include "ONVIF/WhiteBalanceOptions.h"


namespace ONVIF {


WhiteBalanceOptions::WhiteBalanceOptions():
	_mode(),
	_yrGain(),
	_ybGain()
{
}


WhiteBalanceOptions::WhiteBalanceOptions(const std::vector<std::string>& mode, const FloatRange& yrGain, const FloatRange& ybGain):
	_mode(mode),
	_yrGain(yrGain),
	_ybGain(ybGain)
{
}


WhiteBalanceOptions::~WhiteBalanceOptions()
{
}


} // ONVIF


#include "ONVIF/WhiteBalanceOptions20.h"


namespace ONVIF {


WhiteBalanceOptions20::WhiteBalanceOptions20():
	_mode(),
	_yrGain(),
	_ybGain(),
	_extension()
{
}


WhiteBalanceOptions20::WhiteBalanceOptions20(const std::vector<std::string>& mode, const Poco::SharedPtr<FloatRange>& yrGain, const Poco::SharedPtr<FloatRange>& ybGain, const Poco::SharedPtr<WhiteBalanceOptions20Extension>& extension):
	_mode(mode),
	_yrGain(yrGain),
	_ybGain(ybGain),
	_extension(extension)
{
}


WhiteBalanceOptions20::~WhiteBalanceOptions20()
{
}


} // ONVIF


#include "ONVIF/WhiteBalanceOptions20Extension.h"


namespace ONVIF {


WhiteBalanceOptions20Extension::WhiteBalanceOptions20Extension()
{
}


WhiteBalanceOptions20Extension::~WhiteBalanceOptions20Extension()
{
}


} // ONVIF


#include "ONVIF/WideDynamicRange.h"


namespace ONVIF {


WideDynamicRange::WideDynamicRange():
	_mode(),
	_level()
{
}


WideDynamicRange::WideDynamicRange(const std::string& mode, float level):
	_mode(mode),
	_level(level)
{
}


WideDynamicRange::~WideDynamicRange()
{
}


} // ONVIF


#include "ONVIF/WideDynamicRange20.h"


namespace ONVIF {


WideDynamicRange20::WideDynamicRange20():
	_mode(),
	_level()
{
}


WideDynamicRange20::WideDynamicRange20(const std::string& mode, Poco::Optional<float> level):
	_mode(mode),
	_level(level)
{
}


WideDynamicRange20::~WideDynamicRange20()
{
}


} // ONVIF


#include "ONVIF/WideDynamicRangeOptions.h"


namespace ONVIF {


WideDynamicRangeOptions::WideDynamicRangeOptions():
	_mode(),
	_level()
{
}


WideDynamicRangeOptions::WideDynamicRangeOptions(const std::vector<std::string>& mode, const FloatRange& level):
	_mode(mode),
	_level(level)
{
}


WideDynamicRangeOptions::~WideDynamicRangeOptions()
{
}


} // ONVIF


#include "ONVIF/WideDynamicRangeOptions20.h"


namespace ONVIF {


WideDynamicRangeOptions20::WideDynamicRangeOptions20():
	_mode(),
	_level()
{
}


WideDynamicRangeOptions20::WideDynamicRangeOptions20(const std::vector<std::string>& mode, const Poco::SharedPtr<FloatRange>& level):
	_mode(mode),
	_level(level)
{
}


WideDynamicRangeOptions20::~WideDynamicRangeOptions20()
{
}


} // ONVIF


#include "ONVIF/ZoomLimits.h"


namespace ONVIF {


ZoomLimits::ZoomLimits():
	_range()
{
}


ZoomLimits::ZoomLimits(const Space1DDescription& range):
	_range(range)
{
}


ZoomLimits::~ZoomLimits()
{
}


} // ONVIF


#include "W3/SOAP/Envelope/Body.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


Body::Body()
{
}


Body::~Body()
{
}


} } } // W3::SOAP::Envelope


#include "W3/SOAP/Envelope/Detail.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


Detail::Detail()
{
}


Detail::~Detail()
{
}


} } } // W3::SOAP::Envelope


#include "W3/SOAP/Envelope/Envelope.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


Envelope::Envelope():
	_header(),
	_body()
{
}


Envelope::Envelope(const Poco::SharedPtr<Header>& header, const Body& body):
	_header(header),
	_body(body)
{
}


Envelope::~Envelope()
{
}


} } } // W3::SOAP::Envelope


#include "W3/SOAP/Envelope/Fault.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


Fault::Fault():
	_code(),
	_reason(),
	_node(),
	_role(),
	_detail()
{
}


Fault::Fault(const Faultcode& code, const Faultreason& reason, const Poco::Optional<Poco::URI>& node, const Poco::Optional<Poco::URI>& role, const Poco::SharedPtr<Detail>& detail):
	_code(code),
	_reason(reason),
	_node(node),
	_role(role),
	_detail(detail)
{
}


Fault::~Fault()
{
}


} } } // W3::SOAP::Envelope


#include "W3/SOAP/Envelope/Faultcode.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


Faultcode::Faultcode():
	_value(),
	_subcode()
{
}


Faultcode::Faultcode(const std::string& value, const Poco::SharedPtr<Subcode>& subcode):
	_value(value),
	_subcode(subcode)
{
}


Faultcode::~Faultcode()
{
}


} } } // W3::SOAP::Envelope


#include "W3/SOAP/Envelope/Faultreason.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


Faultreason::Faultreason():
	_text()
{
}


Faultreason::Faultreason(const std::vector<Reasontext>& text):
	_text(text)
{
}


Faultreason::~Faultreason()
{
}


} } } // W3::SOAP::Envelope


#include "W3/SOAP/Envelope/Header.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


Header::Header()
{
}


Header::~Header()
{
}


} } } // W3::SOAP::Envelope


#include "W3/SOAP/Envelope/NotUnderstood.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


NotUnderstood::NotUnderstood():
	_qname()
{
}


NotUnderstood::NotUnderstood(const std::string& qname):
	_qname(qname)
{
}


NotUnderstood::~NotUnderstood()
{
}


} } } // W3::SOAP::Envelope


#include "W3/SOAP/Envelope/Reasontext.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


Reasontext::Reasontext():
	_value()
{
}


Reasontext::Reasontext(const Poco::Optional<std::string>& value):
	_value(value)
{
}


Reasontext::~Reasontext()
{
}


} } } // W3::SOAP::Envelope


#include "W3/SOAP/Envelope/Subcode.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


Subcode::Subcode():
	_value(),
	_subcode()
{
}


Subcode::Subcode(const std::string& value, const Poco::SharedPtr<Subcode>& subcode):
	_value(value),
	_subcode(subcode)
{
}


Subcode::~Subcode()
{
}


} } } // W3::SOAP::Envelope


#include "W3/SOAP/Envelope/SupportedEnv.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


SupportedEnv::SupportedEnv():
	_qname()
{
}


SupportedEnv::SupportedEnv(const std::string& qname):
	_qname(qname)
{
}


SupportedEnv::~SupportedEnv()
{
}


} } } // W3::SOAP::Envelope


#include "W3/SOAP/Envelope/Upgrade.h"


namespace W3 {
namespace SOAP {
namespace Envelope {


Upgrade::Upgrade():
	_supportedEnvelope()
{
}


Upgrade::Upgrade(const std::vector<SupportedEnv>& supportedEnvelope):
	_supportedEnvelope(supportedEnvelope)
{
}


Upgrade::~Upgrade()
{
}


} } } // W3::SOAP::Envelope


#include "W3/XOP/Include.h"


namespace W3 {
namespace XOP {


Include::Include():
	_href()
{
}


Include::Include(const Poco::URI& href):
	_href(href)
{
}


Include::~Include()
{
}


} } // W3::XOP


#include "W3/XMLMIME/Base64Binary.h"


namespace W3 {
namespace XMLMIME {


Base64Binary::Base64Binary():
	_value()
{
}


Base64Binary::Base64Binary(const Poco::Optional<std::vector<char>>& value):
	_value(value)
{
}


Base64Binary::~Base64Binary()
{
}


} } // W3::XMLMIME


#include "W3/XMLMIME/HexBinary.h"


namespace W3 {
namespace XMLMIME {


HexBinary::HexBinary():
	_value()
{
}


HexBinary::HexBinary(const Poco::Optional<std::string>& value):
	_value(value)
{
}


HexBinary::~HexBinary()
{
}


} } // W3::XMLMIME


#include "W3/Addressing/AttributedAny.h"


namespace W3 {
namespace Addressing {


AttributedAny::AttributedAny()
{
}


AttributedAny::~AttributedAny()
{
}


} } // W3::Addressing


#include "W3/Addressing/AttributedQName.h"


namespace W3 {
namespace Addressing {


AttributedQName::AttributedQName():
	_value()
{
}


AttributedQName::AttributedQName(const Poco::Optional<std::string>& value):
	_value(value)
{
}


AttributedQName::~AttributedQName()
{
}


} } // W3::Addressing


#include "W3/Addressing/AttributedURI.h"


namespace W3 {
namespace Addressing {


AttributedURI::AttributedURI():
	_value()
{
}


AttributedURI::AttributedURI(const Poco::Optional<Poco::URI>& value):
	_value(value)
{
}


AttributedURI::~AttributedURI()
{
}


} } // W3::Addressing


#include "W3/Addressing/AttributedUnsignedLong.h"


namespace W3 {
namespace Addressing {


AttributedUnsignedLong::AttributedUnsignedLong():
	_value()
{
}


AttributedUnsignedLong::AttributedUnsignedLong(Poco::Optional<Poco::UInt64> value):
	_value(value)
{
}


AttributedUnsignedLong::~AttributedUnsignedLong()
{
}


} } // W3::Addressing


#include "W3/Addressing/EndpointReference.h"


namespace W3 {
namespace Addressing {


EndpointReference::EndpointReference():
	_address(),
	_referenceParameters(),
	_metadata()
{
}


EndpointReference::EndpointReference(const AttributedURI& address, const Poco::SharedPtr<ReferenceParameters>& referenceParameters, const Poco::SharedPtr<Metadata>& metadata):
	_address(address),
	_referenceParameters(referenceParameters),
	_metadata(metadata)
{
}


EndpointReference::~EndpointReference()
{
}


} } // W3::Addressing


#include "W3/Addressing/Metadata.h"


namespace W3 {
namespace Addressing {


Metadata::Metadata()
{
}


Metadata::~Metadata()
{
}


} } // W3::Addressing


#include "W3/Addressing/ProblemAction.h"


namespace W3 {
namespace Addressing {


ProblemAction::ProblemAction():
	_action(),
	_soapAction()
{
}


ProblemAction::ProblemAction(const Poco::SharedPtr<AttributedURI>& action, const Poco::Optional<Poco::URI>& soapAction):
	_action(action),
	_soapAction(soapAction)
{
}


ProblemAction::~ProblemAction()
{
}


} } // W3::Addressing


#include "W3/Addressing/ReferenceParameters.h"


namespace W3 {
namespace Addressing {


ReferenceParameters::ReferenceParameters()
{
}


ReferenceParameters::~ReferenceParameters()
{
}


} } // W3::Addressing


#include "W3/Addressing/RelatesTo.h"


namespace W3 {
namespace Addressing {


RelatesTo::RelatesTo():
	_value()
{
}


RelatesTo::RelatesTo(const Poco::Optional<Poco::URI>& value):
	_value(value)
{
}


RelatesTo::~RelatesTo()
{
}


} } // W3::Addressing


