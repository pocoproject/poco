//
// VideoDecoderConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoDecoderConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_VideoDecoderConfigurationOptions_INCLUDED


#include "ONVIF/H264DecOptionsDeserializer.h"
#include "ONVIF/H264DecOptionsSerializer.h"
#include "ONVIF/JpegDecOptionsDeserializer.h"
#include "ONVIF/JpegDecOptionsSerializer.h"
#include "ONVIF/Mpeg4DecOptionsDeserializer.h"
#include "ONVIF/Mpeg4DecOptionsSerializer.h"
#include "ONVIF/VideoDecoderConfigurationOptions.h"
#include "ONVIF/VideoDecoderConfigurationOptionsExtensionDeserializer.h"
#include "ONVIF/VideoDecoderConfigurationOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoDecoderConfigurationOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::VideoDecoderConfigurationOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::VideoDecoderConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"JpegDecOptions"s,"H264DecOptions"s,"Mpeg4DecOptions"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::JpegDecOptions>>::serialize(REMOTING__NAMES[0], value.getJpegDecOptions(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::H264DecOptions>>::serialize(REMOTING__NAMES[1], value.getH264DecOptions(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Mpeg4DecOptions>>::serialize(REMOTING__NAMES[2], value.getMpeg4DecOptions(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoDecoderConfigurationOptionsExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoDecoderConfigurationOptions_INCLUDED

