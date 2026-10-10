//
// SetVideoAnalyticsConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_SetVideoAnalyticsConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_Media_SetVideoAnalyticsConfiguration_INCLUDED


#include "ONVIF/Media/SetVideoAnalyticsConfiguration.h"
#include "ONVIF/VideoAnalyticsConfigurationDeserializer.h"
#include "ONVIF/VideoAnalyticsConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::SetVideoAnalyticsConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::SetVideoAnalyticsConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::SetVideoAnalyticsConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Configuration"s,"ForcePersistence"s};
		bool ret = false;
		ONVIF::VideoAnalyticsConfiguration gen_configuration;
		ret = TypeDeserializer<ONVIF::VideoAnalyticsConfiguration>::deserialize(REMOTING__NAMES[0], true, deser, gen_configuration);
		if (ret) value.setConfiguration(gen_configuration);
		bool gen_forcePersistence;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_forcePersistence);
		if (ret) value.setForcePersistence(gen_forcePersistence);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_SetVideoAnalyticsConfiguration_INCLUDED

