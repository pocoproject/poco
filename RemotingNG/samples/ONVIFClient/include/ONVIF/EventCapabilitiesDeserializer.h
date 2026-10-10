//
// EventCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_EventCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_EventCapabilities_INCLUDED


#include "ONVIF/EventCapabilities.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::EventCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::EventCapabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::EventCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"WSSubscriptionPolicySupport"s,"WSPullPointSupport"s,"WSPausableSubscriptionManagerInterfaceSupport"s};
		bool ret = false;
		Poco::URI gen_xAddr;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_xAddr);
		if (ret) value.setXAddr(gen_xAddr);
		bool gen_wSSubscriptionPolicySupport;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_wSSubscriptionPolicySupport);
		if (ret) value.setWSSubscriptionPolicySupport(gen_wSSubscriptionPolicySupport);
		bool gen_wSPullPointSupport;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_wSPullPointSupport);
		if (ret) value.setWSPullPointSupport(gen_wSPullPointSupport);
		bool gen_wSPausableSubscriptionManagerInterfaceSupport;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[3], true, deser, gen_wSPausableSubscriptionManagerInterfaceSupport);
		if (ret) value.setWSPausableSubscriptionManagerInterfaceSupport(gen_wSPausableSubscriptionManagerInterfaceSupport);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_EventCapabilities_INCLUDED

