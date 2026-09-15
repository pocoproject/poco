//
// NotificationMessageHolderDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSN_B2_NotificationMessageHolder_INCLUDED
#define TypeDeserializer_OASIS_WSN_B2_NotificationMessageHolder_INCLUDED


#include "OASIS/WSN/B2/MessageDeserializer.h"
#include "OASIS/WSN/B2/MessageSerializer.h"
#include "OASIS/WSN/B2/NotificationMessageHolder.h"
#include "OASIS/WSN/B2/TopicExpressionDeserializer.h"
#include "OASIS/WSN/B2/TopicExpressionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "W3/Addressing/EndpointReferenceDeserializer.h"
#include "W3/Addressing/EndpointReferenceSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSN::B2::NotificationMessageHolder>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSN::B2::NotificationMessageHolder& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://docs.oasis-open.org/wsn/b-2"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, OASIS::WSN::B2::NotificationMessageHolder& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SubscriptionReference"s,"Topic"s,"ProducerReference"s,"Message"s};
		bool ret = false;
		Poco::SharedPtr<W3::Addressing::EndpointReference> gen_subscriptionReference;
		ret = TypeDeserializer<Poco::SharedPtr<W3::Addressing::EndpointReference>>::deserialize(REMOTING__NAMES[0], false, deser, gen_subscriptionReference);
		if (ret) value.setSubscriptionReference(gen_subscriptionReference);
		Poco::SharedPtr<OASIS::WSN::B2::TopicExpression> gen_topic;
		ret = TypeDeserializer<Poco::SharedPtr<OASIS::WSN::B2::TopicExpression>>::deserialize(REMOTING__NAMES[1], false, deser, gen_topic);
		if (ret) value.setTopic(gen_topic);
		Poco::SharedPtr<W3::Addressing::EndpointReference> gen_producerReference;
		ret = TypeDeserializer<Poco::SharedPtr<W3::Addressing::EndpointReference>>::deserialize(REMOTING__NAMES[2], false, deser, gen_producerReference);
		if (ret) value.setProducerReference(gen_producerReference);
		OASIS::WSN::B2::Message gen_message;
		ret = TypeDeserializer<OASIS::WSN::B2::Message>::deserialize(REMOTING__NAMES[3], true, deser, gen_message);
		if (ret) value.setMessage(gen_message);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSN_B2_NotificationMessageHolder_INCLUDED

