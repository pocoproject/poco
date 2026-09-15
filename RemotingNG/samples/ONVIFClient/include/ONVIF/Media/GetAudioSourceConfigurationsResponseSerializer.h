//
// GetAudioSourceConfigurationsResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Media_GetAudioSourceConfigurationsResponse_INCLUDED
#define TypeSerializer_ONVIF_Media_GetAudioSourceConfigurationsResponse_INCLUDED


#include "ONVIF/AudioSourceConfigurationDeserializer.h"
#include "ONVIF/AudioSourceConfigurationSerializer.h"
#include "ONVIF/Media/GetAudioSourceConfigurationsResponse.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Media::GetAudioSourceConfigurationsResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Media::GetAudioSourceConfigurationsResponse& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Media::GetAudioSourceConfigurationsResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Configurations"s,""s};
		TypeSerializer<std::vector<ONVIF::AudioSourceConfiguration>>::serialize(REMOTING__NAMES[0], value.getConfigurations(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Media_GetAudioSourceConfigurationsResponse_INCLUDED

