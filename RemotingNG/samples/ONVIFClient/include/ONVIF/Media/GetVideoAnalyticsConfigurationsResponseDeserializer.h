//
// GetVideoAnalyticsConfigurationsResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_GetVideoAnalyticsConfigurationsResponse_INCLUDED
#define TypeDeserializer_ONVIF_Media_GetVideoAnalyticsConfigurationsResponse_INCLUDED


#include "ONVIF/Media/GetVideoAnalyticsConfigurationsResponse.h"
#include "ONVIF/VideoAnalyticsConfigurationDeserializer.h"
#include "ONVIF/VideoAnalyticsConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::GetVideoAnalyticsConfigurationsResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::GetVideoAnalyticsConfigurationsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/media/wsdl"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::GetVideoAnalyticsConfigurationsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Configurations"s};
		bool ret = false;
		std::vector<ONVIF::VideoAnalyticsConfiguration> gen_configurations;
		ret = TypeDeserializer<std::vector<ONVIF::VideoAnalyticsConfiguration>>::deserialize(REMOTING__NAMES[0], false, deser, gen_configurations);
		if (ret) value.setConfigurations(gen_configurations);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_GetVideoAnalyticsConfigurationsResponse_INCLUDED

