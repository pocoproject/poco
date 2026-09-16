//
// GetOSDOptionsResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Media_GetOSDOptionsResponse_INCLUDED
#define TypeSerializer_ONVIF_Media_GetOSDOptionsResponse_INCLUDED


#include "ONVIF/Media/GetOSDOptionsResponse.h"
#include "ONVIF/OSDConfigurationOptionsDeserializer.h"
#include "ONVIF/OSDConfigurationOptionsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Media::GetOSDOptionsResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Media::GetOSDOptionsResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/media/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::Media::GetOSDOptionsResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"OSDOptions"s,""s};
		TypeSerializer<ONVIF::OSDConfigurationOptions>::serialize(REMOTING__NAMES[0], value.getOSDOptions(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Media_GetOSDOptionsResponse_INCLUDED

