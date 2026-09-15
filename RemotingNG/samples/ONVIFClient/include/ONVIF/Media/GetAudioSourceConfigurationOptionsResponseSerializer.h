//
// GetAudioSourceConfigurationOptionsResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Media_GetAudioSourceConfigurationOptionsResponse_INCLUDED
#define TypeSerializer_ONVIF_Media_GetAudioSourceConfigurationOptionsResponse_INCLUDED


#include "ONVIF/AudioSourceConfigurationOptionsDeserializer.h"
#include "ONVIF/AudioSourceConfigurationOptionsSerializer.h"
#include "ONVIF/Media/GetAudioSourceConfigurationOptionsResponse.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Media::GetAudioSourceConfigurationOptionsResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Media::GetAudioSourceConfigurationOptionsResponse& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Media::GetAudioSourceConfigurationOptionsResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Options"s,""s};
		TypeSerializer<ONVIF::AudioSourceConfigurationOptions>::serialize(REMOTING__NAMES[0], value.getOptions(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Media_GetAudioSourceConfigurationOptionsResponse_INCLUDED

