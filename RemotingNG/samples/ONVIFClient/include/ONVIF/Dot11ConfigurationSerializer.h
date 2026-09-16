//
// Dot11ConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Dot11Configuration_INCLUDED
#define TypeSerializer_ONVIF_Dot11Configuration_INCLUDED


#include "ONVIF/Dot11Configuration.h"
#include "ONVIF/Dot11SecurityConfigurationDeserializer.h"
#include "ONVIF/Dot11SecurityConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Dot11Configuration>
{
public:
	static void serialize(const std::string& name, const ONVIF::Dot11Configuration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Dot11Configuration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SSID"s,"Mode"s,"Alias"s,"Priority"s,"Security"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getSSID(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getMode(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[2], value.getAlias(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[3], value.getPriority(), ser);
		TypeSerializer<ONVIF::Dot11SecurityConfiguration>::serialize(REMOTING__NAMES[4], value.getSecurity(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Dot11Configuration_INCLUDED

