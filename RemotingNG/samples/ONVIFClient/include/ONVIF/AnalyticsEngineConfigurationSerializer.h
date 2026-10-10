//
// AnalyticsEngineConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AnalyticsEngineConfiguration_INCLUDED
#define TypeSerializer_ONVIF_AnalyticsEngineConfiguration_INCLUDED


#include "ONVIF/AnalyticsEngineConfiguration.h"
#include "ONVIF/AnalyticsEngineConfigurationExtensionDeserializer.h"
#include "ONVIF/AnalyticsEngineConfigurationExtensionSerializer.h"
#include "ONVIF/ConfigDeserializer.h"
#include "ONVIF/ConfigSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AnalyticsEngineConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::AnalyticsEngineConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::AnalyticsEngineConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AnalyticsModule"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::Config>>::serialize(REMOTING__NAMES[0], value.getAnalyticsModule(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AnalyticsEngineConfigurationExtension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AnalyticsEngineConfiguration_INCLUDED

