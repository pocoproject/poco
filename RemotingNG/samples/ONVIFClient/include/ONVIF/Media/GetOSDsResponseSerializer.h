//
// GetOSDsResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Media_GetOSDsResponse_INCLUDED
#define TypeSerializer_ONVIF_Media_GetOSDsResponse_INCLUDED


#include "ONVIF/Media/GetOSDsResponse.h"
#include "ONVIF/OSDConfigurationDeserializer.h"
#include "ONVIF/OSDConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Media::GetOSDsResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Media::GetOSDsResponse& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Media::GetOSDsResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"OSDs"s,""s};
		TypeSerializer<std::vector<ONVIF::OSDConfiguration>>::serialize(REMOTING__NAMES[0], value.getOSDs(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Media_GetOSDsResponse_INCLUDED

