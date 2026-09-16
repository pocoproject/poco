//
// AnalyticsEngineSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AnalyticsEngine_INCLUDED
#define TypeSerializer_ONVIF_AnalyticsEngine_INCLUDED


#include "ONVIF/AnalyticsDeviceEngineConfigurationDeserializer.h"
#include "ONVIF/AnalyticsDeviceEngineConfigurationSerializer.h"
#include "ONVIF/AnalyticsEngine.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AnalyticsEngine>
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
		
	}

	static void serialize(const std::string& name, const ONVIF::AnalyticsEngine& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::AnalyticsEngine& value, Serializer& ser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getToken(), ser);
		}
		
	}

	static void serializeImpl(const ONVIF::AnalyticsEngine& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::ConfigurationEntity
		{
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"Name"s,"UseCount"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getName(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], value.getUseCount(), ser);
		}
		
		static const std::string REMOTING__NAMES[] = {"AnalyticsEngineConfiguration"s,""s};
		TypeSerializer<ONVIF::AnalyticsDeviceEngineConfiguration>::serialize(REMOTING__NAMES[0], value.getAnalyticsEngineConfiguration(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AnalyticsEngine_INCLUDED

