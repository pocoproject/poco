//
// VideoSourceExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoSourceExtension_INCLUDED
#define TypeSerializer_ONVIF_VideoSourceExtension_INCLUDED


#include "ONVIF/ImagingSettings20Deserializer.h"
#include "ONVIF/ImagingSettings20Serializer.h"
#include "ONVIF/VideoSourceExtension.h"
#include "ONVIF/VideoSourceExtension2Deserializer.h"
#include "ONVIF/VideoSourceExtension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoSourceExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::VideoSourceExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::VideoSourceExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Imaging"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::ImagingSettings20>>::serialize(REMOTING__NAMES[0], value.getImaging(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoSourceExtension2>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoSourceExtension_INCLUDED

