//
// ImagingOptions20ExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ImagingOptions20Extension_INCLUDED
#define TypeSerializer_ONVIF_ImagingOptions20Extension_INCLUDED


#include "ONVIF/ImageStabilizationOptionsDeserializer.h"
#include "ONVIF/ImageStabilizationOptionsSerializer.h"
#include "ONVIF/ImagingOptions20Extension.h"
#include "ONVIF/ImagingOptions20Extension2Deserializer.h"
#include "ONVIF/ImagingOptions20Extension2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ImagingOptions20Extension>
{
public:
	static void serialize(const std::string& name, const ONVIF::ImagingOptions20Extension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ImagingOptions20Extension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ImageStabilization"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::ImageStabilizationOptions>>::serialize(REMOTING__NAMES[0], value.getImageStabilization(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ImagingOptions20Extension2>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ImagingOptions20Extension_INCLUDED

