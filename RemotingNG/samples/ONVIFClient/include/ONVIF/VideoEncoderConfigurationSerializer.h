//
// VideoEncoderConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoEncoderConfiguration_INCLUDED
#define TypeSerializer_ONVIF_VideoEncoderConfiguration_INCLUDED


#include "ONVIF/H264ConfigurationDeserializer.h"
#include "ONVIF/H264ConfigurationSerializer.h"
#include "ONVIF/Mpeg4ConfigurationDeserializer.h"
#include "ONVIF/Mpeg4ConfigurationSerializer.h"
#include "ONVIF/MulticastConfigurationDeserializer.h"
#include "ONVIF/MulticastConfigurationSerializer.h"
#include "ONVIF/VideoEncoderConfiguration.h"
#include "ONVIF/VideoRateControlDeserializer.h"
#include "ONVIF/VideoRateControlSerializer.h"
#include "ONVIF/VideoResolutionDeserializer.h"
#include "ONVIF/VideoResolutionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoEncoderConfiguration>
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
		
		static const std::string REMOTING__NAMES[] = {"GuaranteedFrameRate"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::VideoEncoderConfiguration& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::VideoEncoderConfiguration& value, Serializer& ser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getToken(), ser);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GuaranteedFrameRate"s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getGuaranteedFrameRate(), ser);
	}

	static void serializeImpl(const ONVIF::VideoEncoderConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::ConfigurationEntity
		{
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"Name"s,"UseCount"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getName(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], value.getUseCount(), ser);
		}
		
		static const std::string REMOTING__NAMES[] = {"Encoding"s,"Resolution"s,"Quality"s,"RateControl"s,"MPEG4"s,"H264"s,"Multicast"s,"SessionTimeout"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getEncoding(), ser);
		TypeSerializer<ONVIF::VideoResolution>::serialize(REMOTING__NAMES[1], value.getResolution(), ser);
		TypeSerializer<float>::serialize(REMOTING__NAMES[2], value.getQuality(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoRateControl>>::serialize(REMOTING__NAMES[3], value.getRateControl(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Mpeg4Configuration>>::serialize(REMOTING__NAMES[4], value.getMPEG4(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::H264Configuration>>::serialize(REMOTING__NAMES[5], value.getH264(), ser);
		TypeSerializer<ONVIF::MulticastConfiguration>::serialize(REMOTING__NAMES[6], value.getMulticast(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[7], value.getSessionTimeout(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoEncoderConfiguration_INCLUDED

