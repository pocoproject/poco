//
// CapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_DeviceIO_Capabilities_INCLUDED
#define TypeSerializer_ONVIF_DeviceIO_Capabilities_INCLUDED


#include "ONVIF/DeviceIO/Capabilities.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::DeviceIO::Capabilities>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"VideoSources"s,"VideoOutputs"s,"AudioSources"s,"AudioOutputs"s,"RelayOutputs"s,"SerialPorts"s,"DigitalInputs"s,"DigitalInputOptions"s,"http://www.onvif.org/ver10/deviceIO/wsdl"s};
		ser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[3]);
		ser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[4]);
		ser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[5]);
		ser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[6]);
		ser.pushAttribute(REMOTING__NAMES[8], REMOTING__NAMES[7]);
	}

	static void serialize(const std::string& name, const ONVIF::DeviceIO::Capabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/deviceIO/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::DeviceIO::Capabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"VideoSources"s,"VideoOutputs"s,"AudioSources"s,"AudioOutputs"s,"RelayOutputs"s,"SerialPorts"s,"DigitalInputs"s,"DigitalInputOptions"s};
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[0], value.getVideoSources(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[1], value.getVideoOutputs(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[2], value.getAudioSources(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[3], value.getAudioOutputs(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[4], value.getRelayOutputs(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[5], value.getSerialPorts(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[6], value.getDigitalInputs(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[7], value.getDigitalInputOptions(), ser);
	}

	static void serializeImpl(const ONVIF::DeviceIO::Capabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_DeviceIO_Capabilities_INCLUDED

