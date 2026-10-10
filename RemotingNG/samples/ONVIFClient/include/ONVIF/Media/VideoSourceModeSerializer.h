//
// VideoSourceModeSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Media_VideoSourceMode_INCLUDED
#define TypeSerializer_ONVIF_Media_VideoSourceMode_INCLUDED


#include "ONVIF/Media/VideoSourceMode.h"
#include "ONVIF/Media/VideoSourceModeExtensionDeserializer.h"
#include "ONVIF/Media/VideoSourceModeExtensionSerializer.h"
#include "ONVIF/VideoResolutionDeserializer.h"
#include "ONVIF/VideoResolutionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Media::VideoSourceMode>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"Enabled"s,"http://www.onvif.org/ver10/media/wsdl"s};
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1]);
	}

	static void serialize(const std::string& name, const ONVIF::Media::VideoSourceMode& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/media/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::Media::VideoSourceMode& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"Enabled"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getToken(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getEnabled(), ser);
	}

	static void serializeImpl(const ONVIF::Media::VideoSourceMode& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MaxFramerate"s,"MaxResolution"s,"Encodings"s,"Reboot"s,"Description"s,"Extension"s,""s};
		TypeSerializer<float>::serialize(REMOTING__NAMES[0], value.getMaxFramerate(), ser);
		TypeSerializer<ONVIF::VideoResolution>::serialize(REMOTING__NAMES[1], value.getMaxResolution(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[2], value.getEncodings(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[3], value.getReboot(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[4], value.getDescription(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Media::VideoSourceModeExtension>>::serialize(REMOTING__NAMES[5], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Media_VideoSourceMode_INCLUDED

