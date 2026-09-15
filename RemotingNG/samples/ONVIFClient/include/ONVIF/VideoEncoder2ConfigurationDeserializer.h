//
// VideoEncoder2ConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoEncoder2Configuration_INCLUDED
#define TypeDeserializer_ONVIF_VideoEncoder2Configuration_INCLUDED


#include "ONVIF/MulticastConfigurationDeserializer.h"
#include "ONVIF/MulticastConfigurationSerializer.h"
#include "ONVIF/VideoEncoder2Configuration.h"
#include "ONVIF/VideoRateControl2Deserializer.h"
#include "ONVIF/VideoRateControl2Serializer.h"
#include "ONVIF/VideoResolution2Deserializer.h"
#include "ONVIF/VideoResolution2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoEncoder2Configuration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoEncoder2Configuration& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::VideoEncoder2Configuration& value)
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
		
		static const std::string REMOTING__NAMES[] = {"GovLength"s,"Profile"s,"GuaranteedFrameRate"s};
		bool ret = false;
		Poco::Optional<int> gen_govLength;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_govLength);
		if (ret) value.setGovLength(gen_govLength);
		Poco::Optional<std::string> gen_profile;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_profile);
		if (ret) value.setProfile(gen_profile);
		Poco::Optional<bool> gen_guaranteedFrameRate;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_guaranteedFrameRate);
		if (ret) value.setGuaranteedFrameRate(gen_guaranteedFrameRate);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoEncoder2Configuration& value)
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
		
		static const std::string REMOTING__NAMES[] = {"Encoding"s,"Resolution"s,"RateControl"s,"Multicast"s,"Quality"s};
		bool ret = false;
		std::string gen_encoding;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_encoding);
		if (ret) value.setEncoding(gen_encoding);
		ONVIF::VideoResolution2 gen_resolution;
		ret = TypeDeserializer<ONVIF::VideoResolution2>::deserialize(REMOTING__NAMES[1], true, deser, gen_resolution);
		if (ret) value.setResolution(gen_resolution);
		Poco::SharedPtr<ONVIF::VideoRateControl2> gen_rateControl;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoRateControl2>>::deserialize(REMOTING__NAMES[2], false, deser, gen_rateControl);
		if (ret) value.setRateControl(gen_rateControl);
		Poco::SharedPtr<ONVIF::MulticastConfiguration> gen_multicast;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MulticastConfiguration>>::deserialize(REMOTING__NAMES[3], false, deser, gen_multicast);
		if (ret) value.setMulticast(gen_multicast);
		float gen_quality;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[4], true, deser, gen_quality);
		if (ret) value.setQuality(gen_quality);
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
		
		static const std::string REMOTING__NAMES[] = {"GovLength"s,"Profile"s,"GuaranteedFrameRate"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoEncoder2Configuration_INCLUDED

