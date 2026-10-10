//
// GetVideoEncoderConfigurationsResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Media_GetVideoEncoderConfigurationsResponse_INCLUDED
#define TypeSerializer_ONVIF_Media_GetVideoEncoderConfigurationsResponse_INCLUDED


#include "ONVIF/Media/GetVideoEncoderConfigurationsResponse.h"
#include "ONVIF/VideoEncoderConfigurationDeserializer.h"
#include "ONVIF/VideoEncoderConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Media::GetVideoEncoderConfigurationsResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Media::GetVideoEncoderConfigurationsResponse& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Media::GetVideoEncoderConfigurationsResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Configurations"s,""s};
		TypeSerializer<std::vector<ONVIF::VideoEncoderConfiguration>>::serialize(REMOTING__NAMES[0], value.getConfigurations(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Media_GetVideoEncoderConfigurationsResponse_INCLUDED

