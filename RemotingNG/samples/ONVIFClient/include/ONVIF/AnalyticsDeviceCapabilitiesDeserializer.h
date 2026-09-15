//
// AnalyticsDeviceCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AnalyticsDeviceCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_AnalyticsDeviceCapabilities_INCLUDED


#include "ONVIF/AnalyticsDeviceCapabilities.h"
#include "ONVIF/AnalyticsDeviceExtensionDeserializer.h"
#include "ONVIF/AnalyticsDeviceExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AnalyticsDeviceCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AnalyticsDeviceCapabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AnalyticsDeviceCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"RuleSupport"s,"Extension"s};
		bool ret = false;
		Poco::URI gen_xAddr;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_xAddr);
		if (ret) value.setXAddr(gen_xAddr);
		Poco::Optional<bool> gen_ruleSupport;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_ruleSupport);
		if (ret) value.setRuleSupport(gen_ruleSupport);
		Poco::SharedPtr<ONVIF::AnalyticsDeviceExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AnalyticsDeviceExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AnalyticsDeviceCapabilities_INCLUDED

