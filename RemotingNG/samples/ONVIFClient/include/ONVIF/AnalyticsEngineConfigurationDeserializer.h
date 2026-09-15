//
// AnalyticsEngineConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AnalyticsEngineConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_AnalyticsEngineConfiguration_INCLUDED


#include "ONVIF/AnalyticsEngineConfiguration.h"
#include "ONVIF/AnalyticsEngineConfigurationExtensionDeserializer.h"
#include "ONVIF/AnalyticsEngineConfigurationExtensionSerializer.h"
#include "ONVIF/ConfigDeserializer.h"
#include "ONVIF/ConfigSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AnalyticsEngineConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AnalyticsEngineConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AnalyticsEngineConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AnalyticsModule"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::Config> gen_analyticsModule;
		ret = TypeDeserializer<std::vector<ONVIF::Config>>::deserialize(REMOTING__NAMES[0], false, deser, gen_analyticsModule);
		if (ret) value.setAnalyticsModule(gen_analyticsModule);
		Poco::SharedPtr<ONVIF::AnalyticsEngineConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AnalyticsEngineConfigurationExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AnalyticsEngineConfiguration_INCLUDED

