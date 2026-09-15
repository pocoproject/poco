//
// MetadataConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_MetadataConfiguration_INCLUDED
#define TypeSerializer_ONVIF_MetadataConfiguration_INCLUDED


#include "ONVIF/AnalyticsEngineConfigurationDeserializer.h"
#include "ONVIF/AnalyticsEngineConfigurationSerializer.h"
#include "ONVIF/EventSubscriptionDeserializer.h"
#include "ONVIF/EventSubscriptionSerializer.h"
#include "ONVIF/MetadataConfiguration.h"
#include "ONVIF/MetadataConfigurationExtensionDeserializer.h"
#include "ONVIF/MetadataConfigurationExtensionSerializer.h"
#include "ONVIF/MulticastConfigurationDeserializer.h"
#include "ONVIF/MulticastConfigurationSerializer.h"
#include "ONVIF/PTZFilterDeserializer.h"
#include "ONVIF/PTZFilterSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::MetadataConfiguration>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0]);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CompressionType"s,"GeoLocation"s,"ShapePolygon"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2]);
	}

	static void serialize(const std::string& name, const ONVIF::MetadataConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::MetadataConfiguration& value, Serializer& ser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getToken(), ser);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CompressionType"s,"GeoLocation"s,"ShapePolygon"s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getCompressionType(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getGeoLocation(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getShapePolygon(), ser);
	}

	static void serializeImpl(const ONVIF::MetadataConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::ConfigurationEntity
		{
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"Name"s,"UseCount"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getName(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], value.getUseCount(), ser);
		}
		
		static const std::string REMOTING__NAMES[] = {"PTZStatus"s,"Events"s,"Analytics"s,"Multicast"s,"SessionTimeout"s,"AnalyticsEngineConfiguration"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZFilter>>::serialize(REMOTING__NAMES[0], value.getPTZStatus(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::EventSubscription>>::serialize(REMOTING__NAMES[1], value.getEvents(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getAnalytics(), ser);
		TypeSerializer<ONVIF::MulticastConfiguration>::serialize(REMOTING__NAMES[3], value.getMulticast(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[4], value.getSessionTimeout(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AnalyticsEngineConfiguration>>::serialize(REMOTING__NAMES[5], value.getAnalyticsEngineConfiguration(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::MetadataConfigurationExtension>>::serialize(REMOTING__NAMES[6], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_MetadataConfiguration_INCLUDED

