//
// VideoEncoderOptionsExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoEncoderOptionsExtension_INCLUDED
#define TypeDeserializer_ONVIF_VideoEncoderOptionsExtension_INCLUDED


#include "ONVIF/H264Options2Deserializer.h"
#include "ONVIF/H264Options2Serializer.h"
#include "ONVIF/JpegOptions2Deserializer.h"
#include "ONVIF/JpegOptions2Serializer.h"
#include "ONVIF/Mpeg4Options2Deserializer.h"
#include "ONVIF/Mpeg4Options2Serializer.h"
#include "ONVIF/VideoEncoderOptionsExtension.h"
#include "ONVIF/VideoEncoderOptionsExtension2Deserializer.h"
#include "ONVIF/VideoEncoderOptionsExtension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoEncoderOptionsExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoEncoderOptionsExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoEncoderOptionsExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"JPEG"s,"MPEG4"s,"H264"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::JpegOptions2> gen_jPEG;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::JpegOptions2>>::deserialize(REMOTING__NAMES[0], false, deser, gen_jPEG);
		if (ret) value.setJPEG(gen_jPEG);
		Poco::SharedPtr<ONVIF::Mpeg4Options2> gen_mPEG4;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Mpeg4Options2>>::deserialize(REMOTING__NAMES[1], false, deser, gen_mPEG4);
		if (ret) value.setMPEG4(gen_mPEG4);
		Poco::SharedPtr<ONVIF::H264Options2> gen_h264;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::H264Options2>>::deserialize(REMOTING__NAMES[2], false, deser, gen_h264);
		if (ret) value.setH264(gen_h264);
		Poco::SharedPtr<ONVIF::VideoEncoderOptionsExtension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoEncoderOptionsExtension2>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoEncoderOptionsExtension_INCLUDED

