//
// VideoDecoderConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoDecoderConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_VideoDecoderConfigurationOptions_INCLUDED


#include "ONVIF/H264DecOptionsDeserializer.h"
#include "ONVIF/H264DecOptionsSerializer.h"
#include "ONVIF/JpegDecOptionsDeserializer.h"
#include "ONVIF/JpegDecOptionsSerializer.h"
#include "ONVIF/Mpeg4DecOptionsDeserializer.h"
#include "ONVIF/Mpeg4DecOptionsSerializer.h"
#include "ONVIF/VideoDecoderConfigurationOptions.h"
#include "ONVIF/VideoDecoderConfigurationOptionsExtensionDeserializer.h"
#include "ONVIF/VideoDecoderConfigurationOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoDecoderConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoDecoderConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoDecoderConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"JpegDecOptions"s,"H264DecOptions"s,"Mpeg4DecOptions"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::JpegDecOptions> gen_jpegDecOptions;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::JpegDecOptions>>::deserialize(REMOTING__NAMES[0], false, deser, gen_jpegDecOptions);
		if (ret) value.setJpegDecOptions(gen_jpegDecOptions);
		Poco::SharedPtr<ONVIF::H264DecOptions> gen_h264DecOptions;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::H264DecOptions>>::deserialize(REMOTING__NAMES[1], false, deser, gen_h264DecOptions);
		if (ret) value.setH264DecOptions(gen_h264DecOptions);
		Poco::SharedPtr<ONVIF::Mpeg4DecOptions> gen_mpeg4DecOptions;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Mpeg4DecOptions>>::deserialize(REMOTING__NAMES[2], false, deser, gen_mpeg4DecOptions);
		if (ret) value.setMpeg4DecOptions(gen_mpeg4DecOptions);
		Poco::SharedPtr<ONVIF::VideoDecoderConfigurationOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoDecoderConfigurationOptionsExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoDecoderConfigurationOptions_INCLUDED

