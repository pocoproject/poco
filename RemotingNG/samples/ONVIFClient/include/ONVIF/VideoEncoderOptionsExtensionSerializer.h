//
// VideoEncoderOptionsExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoEncoderOptionsExtension_INCLUDED
#define TypeSerializer_ONVIF_VideoEncoderOptionsExtension_INCLUDED


#include "ONVIF/H264Options2Deserializer.h"
#include "ONVIF/H264Options2Serializer.h"
#include "ONVIF/JpegOptions2Deserializer.h"
#include "ONVIF/JpegOptions2Serializer.h"
#include "ONVIF/Mpeg4Options2Deserializer.h"
#include "ONVIF/Mpeg4Options2Serializer.h"
#include "ONVIF/VideoEncoderOptionsExtension.h"
#include "ONVIF/VideoEncoderOptionsExtension2Deserializer.h"
#include "ONVIF/VideoEncoderOptionsExtension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoEncoderOptionsExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::VideoEncoderOptionsExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::VideoEncoderOptionsExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"JPEG"s,"MPEG4"s,"H264"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::JpegOptions2>>::serialize(REMOTING__NAMES[0], value.getJPEG(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Mpeg4Options2>>::serialize(REMOTING__NAMES[1], value.getMPEG4(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::H264Options2>>::serialize(REMOTING__NAMES[2], value.getH264(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoEncoderOptionsExtension2>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoEncoderOptionsExtension_INCLUDED

