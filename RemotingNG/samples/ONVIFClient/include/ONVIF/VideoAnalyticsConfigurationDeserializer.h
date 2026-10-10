//
// VideoAnalyticsConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoAnalyticsConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_VideoAnalyticsConfiguration_INCLUDED


#include "ONVIF/AnalyticsEngineConfigurationDeserializer.h"
#include "ONVIF/AnalyticsEngineConfigurationSerializer.h"
#include "ONVIF/RuleEngineConfigurationDeserializer.h"
#include "ONVIF/RuleEngineConfigurationSerializer.h"
#include "ONVIF/VideoAnalyticsConfiguration.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoAnalyticsConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoAnalyticsConfiguration& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::VideoAnalyticsConfiguration& value)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s};
		bool ret = false;
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
		}
		
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoAnalyticsConfiguration& value)
	{
		using namespace std::string_literals;
		
		// ONVIF::ConfigurationEntity
		{
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"Name"s,"UseCount"s};
		bool ret = false;
		std::string gen_name;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], true, deser, gen_name);
		if (ret) value.setName(gen_name);
		int gen_useCount;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], true, deser, gen_useCount);
		if (ret) value.setUseCount(gen_useCount);
		}
		
		static const std::string REMOTING__NAMES[] = {"AnalyticsEngineConfiguration"s,"RuleEngineConfiguration"s};
		bool ret = false;
		ONVIF::AnalyticsEngineConfiguration gen_analyticsEngineConfiguration;
		ret = TypeDeserializer<ONVIF::AnalyticsEngineConfiguration>::deserialize(REMOTING__NAMES[0], true, deser, gen_analyticsEngineConfiguration);
		if (ret) value.setAnalyticsEngineConfiguration(gen_analyticsEngineConfiguration);
		ONVIF::RuleEngineConfiguration gen_ruleEngineConfiguration;
		ret = TypeDeserializer<ONVIF::RuleEngineConfiguration>::deserialize(REMOTING__NAMES[1], true, deser, gen_ruleEngineConfiguration);
		if (ret) value.setRuleEngineConfiguration(gen_ruleEngineConfiguration);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], true);
		}
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoAnalyticsConfiguration_INCLUDED

