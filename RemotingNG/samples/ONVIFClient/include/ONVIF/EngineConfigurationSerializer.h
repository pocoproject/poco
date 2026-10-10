//
// EngineConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_EngineConfiguration_INCLUDED
#define TypeSerializer_ONVIF_EngineConfiguration_INCLUDED


#include "ONVIF/AnalyticsEngineInputInfoDeserializer.h"
#include "ONVIF/AnalyticsEngineInputInfoSerializer.h"
#include "ONVIF/EngineConfiguration.h"
#include "ONVIF/VideoAnalyticsConfigurationDeserializer.h"
#include "ONVIF/VideoAnalyticsConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::EngineConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::EngineConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::EngineConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"VideoAnalyticsConfiguration"s,"AnalyticsEngineInputInfo"s,""s};
		TypeSerializer<ONVIF::VideoAnalyticsConfiguration>::serialize(REMOTING__NAMES[0], value.getVideoAnalyticsConfiguration(), ser);
		TypeSerializer<ONVIF::AnalyticsEngineInputInfo>::serialize(REMOTING__NAMES[1], value.getAnalyticsEngineInputInfo(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_EngineConfiguration_INCLUDED

