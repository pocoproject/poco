//
// AnalyticsDeviceEngineConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AnalyticsDeviceEngineConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_AnalyticsDeviceEngineConfiguration_INCLUDED


#include "ONVIF/AnalyticsDeviceEngineConfiguration.h"
#include "ONVIF/AnalyticsDeviceEngineConfigurationExtensionDeserializer.h"
#include "ONVIF/AnalyticsDeviceEngineConfigurationExtensionSerializer.h"
#include "ONVIF/EngineConfigurationDeserializer.h"
#include "ONVIF/EngineConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AnalyticsDeviceEngineConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AnalyticsDeviceEngineConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AnalyticsDeviceEngineConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"EngineConfiguration"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::EngineConfiguration> gen_engineConfiguration;
		ret = TypeDeserializer<std::vector<ONVIF::EngineConfiguration>>::deserialize(REMOTING__NAMES[0], true, deser, gen_engineConfiguration);
		if (ret) value.setEngineConfiguration(gen_engineConfiguration);
		Poco::SharedPtr<ONVIF::AnalyticsDeviceEngineConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AnalyticsDeviceEngineConfigurationExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AnalyticsDeviceEngineConfiguration_INCLUDED

