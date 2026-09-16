//
// ImagingStatus20Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ImagingStatus20_INCLUDED
#define TypeSerializer_ONVIF_ImagingStatus20_INCLUDED


#include "ONVIF/FocusStatus20Deserializer.h"
#include "ONVIF/FocusStatus20Serializer.h"
#include "ONVIF/ImagingStatus20.h"
#include "ONVIF/ImagingStatus20ExtensionDeserializer.h"
#include "ONVIF/ImagingStatus20ExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ImagingStatus20>
{
public:
	static void serialize(const std::string& name, const ONVIF::ImagingStatus20& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ImagingStatus20& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FocusStatus20"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::FocusStatus20>>::serialize(REMOTING__NAMES[0], value.getFocusStatus20(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ImagingStatus20Extension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ImagingStatus20_INCLUDED

