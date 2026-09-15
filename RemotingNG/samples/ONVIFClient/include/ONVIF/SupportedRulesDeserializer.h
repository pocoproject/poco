//
// SupportedRulesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SupportedRules_INCLUDED
#define TypeDeserializer_ONVIF_SupportedRules_INCLUDED


#include "ONVIF/ConfigDescriptionDeserializer.h"
#include "ONVIF/ConfigDescriptionSerializer.h"
#include "ONVIF/SupportedRules.h"
#include "ONVIF/SupportedRulesExtensionDeserializer.h"
#include "ONVIF/SupportedRulesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SupportedRules>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SupportedRules& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::SupportedRules& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Limit"s};
		bool ret = false;
		Poco::Optional<int> gen_limit;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_limit);
		if (ret) value.setLimit(gen_limit);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::SupportedRules& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RuleContentSchemaLocation"s,"RuleDescription"s,"Extension"s};
		bool ret = false;
		std::vector<Poco::URI> gen_ruleContentSchemaLocation;
		ret = TypeDeserializer<std::vector<Poco::URI>>::deserialize(REMOTING__NAMES[0], false, deser, gen_ruleContentSchemaLocation);
		if (ret) value.setRuleContentSchemaLocation(gen_ruleContentSchemaLocation);
		std::vector<ONVIF::ConfigDescription> gen_ruleDescription;
		ret = TypeDeserializer<std::vector<ONVIF::ConfigDescription>>::deserialize(REMOTING__NAMES[1], false, deser, gen_ruleDescription);
		if (ret) value.setRuleDescription(gen_ruleDescription);
		Poco::SharedPtr<ONVIF::SupportedRulesExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SupportedRulesExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Limit"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SupportedRules_INCLUDED

