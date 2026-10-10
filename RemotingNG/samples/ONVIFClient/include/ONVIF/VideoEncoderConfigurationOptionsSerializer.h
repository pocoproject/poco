//
// VideoEncoderConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoEncoderConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_VideoEncoderConfigurationOptions_INCLUDED


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
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoEncoderConfigurationOptions>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GuaranteedFrameRateSupported"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::VideoEncoderConfigurationOptions& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::VideoEncoderConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GuaranteedFrameRateSupported"s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getGuaranteedFrameRateSupported(), ser);
	}

	static void serializeImpl(const ONVIF::VideoEncoderConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"QualityRange"s,"JPEG"s,"MPEG4"s,"H264"s,"Extension"s,""s};
		TypeSerializer<ONVIF::IntRange>::serialize(REMOTING__NAMES[0], value.getQualityRange(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::JpegOptions>>::serialize(REMOTING__NAMES[1], value.getJPEG(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Mpeg4Options>>::serialize(REMOTING__NAMES[2], value.getMPEG4(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::H264Options>>::serialize(REMOTING__NAMES[3], value.getH264(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoEncoderOptionsExtension>>::serialize(REMOTING__NAMES[4], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoEncoderConfigurationOptions_INCLUDED

