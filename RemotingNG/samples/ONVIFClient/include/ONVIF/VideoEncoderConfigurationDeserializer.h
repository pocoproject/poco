//
// VideoEncoderConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoEncoderConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_VideoEncoderConfiguration_INCLUDED


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
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoEncoderConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoEncoderConfiguration& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::VideoEncoderConfiguration& value)
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
		
		static const std::string REMOTING__NAMES[] = {"GuaranteedFrameRate"s};
		bool ret = false;
		Poco::Optional<bool> gen_guaranteedFrameRate;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_guaranteedFrameRate);
		if (ret) value.setGuaranteedFrameRate(gen_guaranteedFrameRate);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoEncoderConfiguration& value)
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
		
		static const std::string REMOTING__NAMES[] = {"Encoding"s,"Resolution"s,"Quality"s,"RateControl"s,"MPEG4"s,"H264"s,"Multicast"s,"SessionTimeout"s};
		bool ret = false;
		std::string gen_encoding;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_encoding);
		if (ret) value.setEncoding(gen_encoding);
		ONVIF::VideoResolution gen_resolution;
		ret = TypeDeserializer<ONVIF::VideoResolution>::deserialize(REMOTING__NAMES[1], true, deser, gen_resolution);
		if (ret) value.setResolution(gen_resolution);
		float gen_quality;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[2], true, deser, gen_quality);
		if (ret) value.setQuality(gen_quality);
		Poco::SharedPtr<ONVIF::VideoRateControl> gen_rateControl;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoRateControl>>::deserialize(REMOTING__NAMES[3], false, deser, gen_rateControl);
		if (ret) value.setRateControl(gen_rateControl);
		Poco::SharedPtr<ONVIF::Mpeg4Configuration> gen_mPEG4;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Mpeg4Configuration>>::deserialize(REMOTING__NAMES[4], false, deser, gen_mPEG4);
		if (ret) value.setMPEG4(gen_mPEG4);
		Poco::SharedPtr<ONVIF::H264Configuration> gen_h264;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::H264Configuration>>::deserialize(REMOTING__NAMES[5], false, deser, gen_h264);
		if (ret) value.setH264(gen_h264);
		ONVIF::MulticastConfiguration gen_multicast;
		ret = TypeDeserializer<ONVIF::MulticastConfiguration>::deserialize(REMOTING__NAMES[6], true, deser, gen_multicast);
		if (ret) value.setMulticast(gen_multicast);
		std::string gen_sessionTimeout;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[7], true, deser, gen_sessionTimeout);
		if (ret) value.setSessionTimeout(gen_sessionTimeout);
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
		
		static const std::string REMOTING__NAMES[] = {"GuaranteedFrameRate"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoEncoderConfiguration_INCLUDED

