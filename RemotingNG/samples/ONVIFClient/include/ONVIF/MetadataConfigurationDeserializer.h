//
// MetadataConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MetadataConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_MetadataConfiguration_INCLUDED


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
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MetadataConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MetadataConfiguration& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::MetadataConfiguration& value)
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
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CompressionType"s,"GeoLocation"s,"ShapePolygon"s};
		bool ret = false;
		Poco::Optional<std::string> gen_compressionType;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_compressionType);
		if (ret) value.setCompressionType(gen_compressionType);
		Poco::Optional<bool> gen_geoLocation;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_geoLocation);
		if (ret) value.setGeoLocation(gen_geoLocation);
		Poco::Optional<bool> gen_shapePolygon;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_shapePolygon);
		if (ret) value.setShapePolygon(gen_shapePolygon);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::MetadataConfiguration& value)
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
		
		static const std::string REMOTING__NAMES[] = {"PTZStatus"s,"Events"s,"Analytics"s,"Multicast"s,"SessionTimeout"s,"AnalyticsEngineConfiguration"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::PTZFilter> gen_pTZStatus;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZFilter>>::deserialize(REMOTING__NAMES[0], false, deser, gen_pTZStatus);
		if (ret) value.setPTZStatus(gen_pTZStatus);
		Poco::SharedPtr<ONVIF::EventSubscription> gen_events;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::EventSubscription>>::deserialize(REMOTING__NAMES[1], false, deser, gen_events);
		if (ret) value.setEvents(gen_events);
		Poco::Optional<bool> gen_analytics;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_analytics);
		if (ret) value.setAnalytics(gen_analytics);
		ONVIF::MulticastConfiguration gen_multicast;
		ret = TypeDeserializer<ONVIF::MulticastConfiguration>::deserialize(REMOTING__NAMES[3], true, deser, gen_multicast);
		if (ret) value.setMulticast(gen_multicast);
		std::string gen_sessionTimeout;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[4], true, deser, gen_sessionTimeout);
		if (ret) value.setSessionTimeout(gen_sessionTimeout);
		Poco::SharedPtr<ONVIF::AnalyticsEngineConfiguration> gen_analyticsEngineConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AnalyticsEngineConfiguration>>::deserialize(REMOTING__NAMES[5], false, deser, gen_analyticsEngineConfiguration);
		if (ret) value.setAnalyticsEngineConfiguration(gen_analyticsEngineConfiguration);
		Poco::SharedPtr<ONVIF::MetadataConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MetadataConfigurationExtension>>::deserialize(REMOTING__NAMES[6], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], true);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CompressionType"s,"GeoLocation"s,"ShapePolygon"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MetadataConfiguration_INCLUDED

