//
// VideoEncoder2ConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoEncoder2Configuration_INCLUDED
#define TypeSerializer_ONVIF_VideoEncoder2Configuration_INCLUDED


#include "ONVIF/MulticastConfigurationDeserializer.h"
#include "ONVIF/MulticastConfigurationSerializer.h"
#include "ONVIF/VideoEncoder2Configuration.h"
#include "ONVIF/VideoRateControl2Deserializer.h"
#include "ONVIF/VideoRateControl2Serializer.h"
#include "ONVIF/VideoResolution2Deserializer.h"
#include "ONVIF/VideoResolution2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoEncoder2Configuration>
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
		
		static const std::string REMOTING__NAMES[] = {"GovLength"s,"Profile"s,"GuaranteedFrameRate"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2]);
	}

	static void serialize(const std::string& name, const ONVIF::VideoEncoder2Configuration& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::VideoEncoder2Configuration& value, Serializer& ser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getToken(), ser);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GovLength"s,"Profile"s,"GuaranteedFrameRate"s};
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[0], value.getGovLength(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getProfile(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getGuaranteedFrameRate(), ser);
	}

	static void serializeImpl(const ONVIF::VideoEncoder2Configuration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::ConfigurationEntity
		{
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"Name"s,"UseCount"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getName(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], value.getUseCount(), ser);
		}
		
		static const std::string REMOTING__NAMES[] = {"Encoding"s,"Resolution"s,"RateControl"s,"Multicast"s,"Quality"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getEncoding(), ser);
		TypeSerializer<ONVIF::VideoResolution2>::serialize(REMOTING__NAMES[1], value.getResolution(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoRateControl2>>::serialize(REMOTING__NAMES[2], value.getRateControl(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::MulticastConfiguration>>::serialize(REMOTING__NAMES[3], value.getMulticast(), ser);
		TypeSerializer<float>::serialize(REMOTING__NAMES[4], value.getQuality(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoEncoder2Configuration_INCLUDED

