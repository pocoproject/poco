//
// AudioEncoder2ConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AudioEncoder2Configuration_INCLUDED
#define TypeSerializer_ONVIF_AudioEncoder2Configuration_INCLUDED


#include "ONVIF/AudioEncoder2Configuration.h"
#include "ONVIF/MulticastConfigurationDeserializer.h"
#include "ONVIF/MulticastConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AudioEncoder2Configuration>
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

	static void serialize(const std::string& name, const ONVIF::AudioEncoder2Configuration& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::AudioEncoder2Configuration& value, Serializer& ser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getToken(), ser);
		}
		
	}

	static void serializeImpl(const ONVIF::AudioEncoder2Configuration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::ConfigurationEntity
		{
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"Name"s,"UseCount"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getName(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], value.getUseCount(), ser);
		}
		
		static const std::string REMOTING__NAMES[] = {"Encoding"s,"Multicast"s,"Bitrate"s,"SampleRate"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getEncoding(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::MulticastConfiguration>>::serialize(REMOTING__NAMES[1], value.getMulticast(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[2], value.getBitrate(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[3], value.getSampleRate(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AudioEncoder2Configuration_INCLUDED

