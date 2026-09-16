//
// PaneConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PaneConfiguration_INCLUDED
#define TypeSerializer_ONVIF_PaneConfiguration_INCLUDED


#include "ONVIF/AudioEncoderConfigurationDeserializer.h"
#include "ONVIF/AudioEncoderConfigurationSerializer.h"
#include "ONVIF/PaneConfiguration.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PaneConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::PaneConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PaneConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PaneName"s,"AudioOutputToken"s,"AudioSourceToken"s,"AudioEncoderConfiguration"s,"ReceiverToken"s,"Token"s,""s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getPaneName(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getAudioOutputToken(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getAudioSourceToken(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AudioEncoderConfiguration>>::serialize(REMOTING__NAMES[3], value.getAudioEncoderConfiguration(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[4], value.getReceiverToken(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[5], value.getToken(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PaneConfiguration_INCLUDED

