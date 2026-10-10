//
// AnalyticsCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AnalyticsCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_AnalyticsCapabilities_INCLUDED


#include "ONVIF/AnalyticsCapabilities.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AnalyticsCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AnalyticsCapabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AnalyticsCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"RuleSupport"s,"AnalyticsModuleSupport"s};
		bool ret = false;
		Poco::URI gen_xAddr;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_xAddr);
		if (ret) value.setXAddr(gen_xAddr);
		bool gen_ruleSupport;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_ruleSupport);
		if (ret) value.setRuleSupport(gen_ruleSupport);
		bool gen_analyticsModuleSupport;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_analyticsModuleSupport);
		if (ret) value.setAnalyticsModuleSupport(gen_analyticsModuleSupport);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AnalyticsCapabilities_INCLUDED

