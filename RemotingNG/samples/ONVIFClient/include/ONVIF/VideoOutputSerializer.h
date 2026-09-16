//
// VideoOutputSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoOutput_INCLUDED
#define TypeSerializer_ONVIF_VideoOutput_INCLUDED


#include "ONVIF/LayoutDeserializer.h"
#include "ONVIF/LayoutSerializer.h"
#include "ONVIF/VideoOutput.h"
#include "ONVIF/VideoOutputExtensionDeserializer.h"
#include "ONVIF/VideoOutputExtensionSerializer.h"
#include "ONVIF/VideoResolutionDeserializer.h"
#include "ONVIF/VideoResolutionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoOutput>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES__ONVIF__DEVICEENTITY[1], REMOTING__NAMES__ONVIF__DEVICEENTITY[0]);
		}
		
	}

	static void serialize(const std::string& name, const ONVIF::VideoOutput& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::VideoOutput& value, Serializer& ser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__DEVICEENTITY[0], value.getToken(), ser);
		}
		
	}

	static void serializeImpl(const ONVIF::VideoOutput& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::DeviceEntity
		{
		}
		
		static const std::string REMOTING__NAMES[] = {"Layout"s,"Resolution"s,"RefreshRate"s,"AspectRatio"s,"Extension"s,""s};
		TypeSerializer<ONVIF::Layout>::serialize(REMOTING__NAMES[0], value.getLayout(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoResolution>>::serialize(REMOTING__NAMES[1], value.getResolution(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[2], value.getRefreshRate(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[3], value.getAspectRatio(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoOutputExtension>>::serialize(REMOTING__NAMES[4], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoOutput_INCLUDED

