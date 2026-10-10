//
// DeviceIOCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_DeviceIOCapabilities_INCLUDED
#define TypeSerializer_ONVIF_DeviceIOCapabilities_INCLUDED


#include "ONVIF/DeviceIOCapabilities.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::DeviceIOCapabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::DeviceIOCapabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::DeviceIOCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"VideoSources"s,"VideoOutputs"s,"AudioSources"s,"AudioOutputs"s,"RelayOutputs"s,""s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getXAddr(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[1], value.getVideoSources(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[2], value.getVideoOutputs(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[3], value.getAudioSources(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[4], value.getAudioOutputs(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[5], value.getRelayOutputs(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_DeviceIOCapabilities_INCLUDED

