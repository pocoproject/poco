//
// EngineConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_EngineConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_EngineConfiguration_INCLUDED


#include "ONVIF/AnalyticsEngineInputInfoDeserializer.h"
#include "ONVIF/AnalyticsEngineInputInfoSerializer.h"
#include "ONVIF/EngineConfiguration.h"
#include "ONVIF/VideoAnalyticsConfigurationDeserializer.h"
#include "ONVIF/VideoAnalyticsConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::EngineConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::EngineConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::EngineConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"VideoAnalyticsConfiguration"s,"AnalyticsEngineInputInfo"s};
		bool ret = false;
		ONVIF::VideoAnalyticsConfiguration gen_videoAnalyticsConfiguration;
		ret = TypeDeserializer<ONVIF::VideoAnalyticsConfiguration>::deserialize(REMOTING__NAMES[0], true, deser, gen_videoAnalyticsConfiguration);
		if (ret) value.setVideoAnalyticsConfiguration(gen_videoAnalyticsConfiguration);
		ONVIF::AnalyticsEngineInputInfo gen_analyticsEngineInputInfo;
		ret = TypeDeserializer<ONVIF::AnalyticsEngineInputInfo>::deserialize(REMOTING__NAMES[1], true, deser, gen_analyticsEngineInputInfo);
		if (ret) value.setAnalyticsEngineInputInfo(gen_analyticsEngineInputInfo);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_EngineConfiguration_INCLUDED

