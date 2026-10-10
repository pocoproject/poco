//
// GetAudioSourcesResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Media_GetAudioSourcesResponse_INCLUDED
#define TypeSerializer_ONVIF_Media_GetAudioSourcesResponse_INCLUDED


#include "ONVIF/AudioSourceDeserializer.h"
#include "ONVIF/AudioSourceSerializer.h"
#include "ONVIF/Media/GetAudioSourcesResponse.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Media::GetAudioSourcesResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Media::GetAudioSourcesResponse& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Media::GetAudioSourcesResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AudioSources"s,""s};
		TypeSerializer<std::vector<ONVIF::AudioSource>>::serialize(REMOTING__NAMES[0], value.getAudioSources(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Media_GetAudioSourcesResponse_INCLUDED

