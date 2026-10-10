//
// EventSubscriptionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_EventSubscription_INCLUDED
#define TypeDeserializer_ONVIF_EventSubscription_INCLUDED


#include "OASIS/WSN/B2/FilterDeserializer.h"
#include "OASIS/WSN/B2/FilterSerializer.h"
#include "ONVIF/EventSubscription.h"
#include "ONVIF/SubscriptionPolicyDeserializer.h"
#include "ONVIF/SubscriptionPolicySerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::EventSubscription>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::EventSubscription& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::EventSubscription& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Filter"s,"SubscriptionPolicy"s};
		bool ret = false;
		Poco::SharedPtr<OASIS::WSN::B2::Filter> gen_filter;
		ret = TypeDeserializer<Poco::SharedPtr<OASIS::WSN::B2::Filter>>::deserialize(REMOTING__NAMES[0], false, deser, gen_filter);
		if (ret) value.setFilter(gen_filter);
		Poco::SharedPtr<ONVIF::SubscriptionPolicy> gen_subscriptionPolicy;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SubscriptionPolicy>>::deserialize(REMOTING__NAMES[1], false, deser, gen_subscriptionPolicy);
		if (ret) value.setSubscriptionPolicy(gen_subscriptionPolicy);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_EventSubscription_INCLUDED

