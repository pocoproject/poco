//
// VideoEncoderConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoEncoderConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_VideoEncoderConfigurationOptions_INCLUDED


#include "ONVIF/H264OptionsDeserializer.h"
#include "ONVIF/H264OptionsSerializer.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/JpegOptionsDeserializer.h"
#include "ONVIF/JpegOptionsSerializer.h"
#include "ONVIF/Mpeg4OptionsDeserializer.h"
#include "ONVIF/Mpeg4OptionsSerializer.h"
#include "ONVIF/VideoEncoderConfigurationOptions.h"
#include "ONVIF/VideoEncoderOptionsExtensionDeserializer.h"
#include "ONVIF/VideoEncoderOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoEncoderConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoEncoderConfigurationOptions& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::VideoEncoderConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GuaranteedFrameRateSupported"s};
		bool ret = false;
		Poco::Optional<bool> gen_guaranteedFrameRateSupported;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_guaranteedFrameRateSupported);
		if (ret) value.setGuaranteedFrameRateSupported(gen_guaranteedFrameRateSupported);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoEncoderConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"QualityRange"s,"JPEG"s,"MPEG4"s,"H264"s,"Extension"s};
		bool ret = false;
		ONVIF::IntRange gen_qualityRange;
		ret = TypeDeserializer<ONVIF::IntRange>::deserialize(REMOTING__NAMES[0], true, deser, gen_qualityRange);
		if (ret) value.setQualityRange(gen_qualityRange);
		Poco::SharedPtr<ONVIF::JpegOptions> gen_jPEG;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::JpegOptions>>::deserialize(REMOTING__NAMES[1], false, deser, gen_jPEG);
		if (ret) value.setJPEG(gen_jPEG);
		Poco::SharedPtr<ONVIF::Mpeg4Options> gen_mPEG4;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Mpeg4Options>>::deserialize(REMOTING__NAMES[2], false, deser, gen_mPEG4);
		if (ret) value.setMPEG4(gen_mPEG4);
		Poco::SharedPtr<ONVIF::H264Options> gen_h264;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::H264Options>>::deserialize(REMOTING__NAMES[3], false, deser, gen_h264);
		if (ret) value.setH264(gen_h264);
		Poco::SharedPtr<ONVIF::VideoEncoderOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoEncoderOptionsExtension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GuaranteedFrameRateSupported"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoEncoderConfigurationOptions_INCLUDED

