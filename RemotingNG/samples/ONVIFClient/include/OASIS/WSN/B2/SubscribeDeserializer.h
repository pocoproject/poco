//
// SubscribeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSN_B2_Subscribe_INCLUDED
#define TypeDeserializer_OASIS_WSN_B2_Subscribe_INCLUDED


#include "OASIS/WSN/B2/FilterDeserializer.h"
#include "OASIS/WSN/B2/FilterSerializer.h"
#include "OASIS/WSN/B2/Subscribe.h"
#include "OASIS/WSN/B2/SubscriptionPolicyDeserializer.h"
#include "OASIS/WSN/B2/SubscriptionPolicySerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "W3/Addressing/EndpointReferenceDeserializer.h"
#include "W3/Addressing/EndpointReferenceSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSN::B2::Subscribe>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSN::B2::Subscribe& value)
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

	static void deserializeImpl(Deserializer& deser, OASIS::WSN::B2::Subscribe& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ConsumerReference"s,"Filter"s,"InitialTerminationTime"s,"SubscriptionPolicy"s};
		bool ret = false;
		W3::Addressing::EndpointReference gen_consumerReference;
		ret = TypeDeserializer<W3::Addressing::EndpointReference>::deserialize(REMOTING__NAMES[0], true, deser, gen_consumerReference);
		if (ret) value.setConsumerReference(gen_consumerReference);
		Poco::SharedPtr<OASIS::WSN::B2::Filter> gen_filter;
		ret = TypeDeserializer<Poco::SharedPtr<OASIS::WSN::B2::Filter>>::deserialize(REMOTING__NAMES[1], false, deser, gen_filter);
		if (ret) value.setFilter(gen_filter);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, ""s);
		Poco::Nullable<Poco::DateTime> gen_initialTerminationTime;
		ret = TypeDeserializer<Poco::Nullable<Poco::DateTime>>::deserialize(REMOTING__NAMES[2], false, deser, gen_initialTerminationTime);
		if (ret) value.setInitialTerminationTime(gen_initialTerminationTime);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		Poco::SharedPtr<OASIS::WSN::B2::SubscriptionPolicy> gen_subscriptionPolicy;
		ret = TypeDeserializer<Poco::SharedPtr<OASIS::WSN::B2::SubscriptionPolicy>>::deserialize(REMOTING__NAMES[3], false, deser, gen_subscriptionPolicy);
		if (ret) value.setSubscriptionPolicy(gen_subscriptionPolicy);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSN_B2_Subscribe_INCLUDED

