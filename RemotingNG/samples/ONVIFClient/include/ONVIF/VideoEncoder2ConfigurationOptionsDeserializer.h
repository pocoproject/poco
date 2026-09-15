//
// VideoEncoder2ConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoEncoder2ConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_VideoEncoder2ConfigurationOptions_INCLUDED


#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/VideoEncoder2ConfigurationOptions.h"
#include "ONVIF/VideoResolution2Deserializer.h"
#include "ONVIF/VideoResolution2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoEncoder2ConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoEncoder2ConfigurationOptions& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::VideoEncoder2ConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GovLengthRange"s,"FrameRatesSupported"s,"ProfilesSupported"s,"ConstantBitRateSupported"s,"GuaranteedFrameRateSupported"s};
		bool ret = false;
		std::vector<std::vector<int>> gen_govLengthRange;
		ret = TypeDeserializer<std::vector<std::vector<int>>>::deserialize(REMOTING__NAMES[0], false, deser, gen_govLengthRange);
		if (ret) value.setGovLengthRange(gen_govLengthRange);
		std::vector<std::vector<float>> gen_frameRatesSupported;
		ret = TypeDeserializer<std::vector<std::vector<float>>>::deserialize(REMOTING__NAMES[1], false, deser, gen_frameRatesSupported);
		if (ret) value.setFrameRatesSupported(gen_frameRatesSupported);
		std::vector<std::vector<std::string>> gen_profilesSupported;
		ret = TypeDeserializer<std::vector<std::vector<std::string>>>::deserialize(REMOTING__NAMES[2], false, deser, gen_profilesSupported);
		if (ret) value.setProfilesSupported(gen_profilesSupported);
		Poco::Optional<bool> gen_constantBitRateSupported;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[3], false, deser, gen_constantBitRateSupported);
		if (ret) value.setConstantBitRateSupported(gen_constantBitRateSupported);
		Poco::Optional<bool> gen_guaranteedFrameRateSupported;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[4], false, deser, gen_guaranteedFrameRateSupported);
		if (ret) value.setGuaranteedFrameRateSupported(gen_guaranteedFrameRateSupported);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoEncoder2ConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Encoding"s,"QualityRange"s,"ResolutionsAvailable"s,"BitrateRange"s};
		bool ret = false;
		std::string gen_encoding;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_encoding);
		if (ret) value.setEncoding(gen_encoding);
		ONVIF::FloatRange gen_qualityRange;
		ret = TypeDeserializer<ONVIF::FloatRange>::deserialize(REMOTING__NAMES[1], true, deser, gen_qualityRange);
		if (ret) value.setQualityRange(gen_qualityRange);
		std::vector<ONVIF::VideoResolution2> gen_resolutionsAvailable;
		ret = TypeDeserializer<std::vector<ONVIF::VideoResolution2>>::deserialize(REMOTING__NAMES[2], true, deser, gen_resolutionsAvailable);
		if (ret) value.setResolutionsAvailable(gen_resolutionsAvailable);
		ONVIF::IntRange gen_bitrateRange;
		ret = TypeDeserializer<ONVIF::IntRange>::deserialize(REMOTING__NAMES[3], true, deser, gen_bitrateRange);
		if (ret) value.setBitrateRange(gen_bitrateRange);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GovLengthRange"s,"FrameRatesSupported"s,"ProfilesSupported"s,"ConstantBitRateSupported"s,"GuaranteedFrameRateSupported"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[2], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[3], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[4], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoEncoder2ConfigurationOptions_INCLUDED

