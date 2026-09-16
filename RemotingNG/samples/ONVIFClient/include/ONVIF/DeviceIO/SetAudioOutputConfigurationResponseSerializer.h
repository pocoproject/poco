//
// SetAudioOutputConfigurationResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_DeviceIO_SetAudioOutputConfigurationResponse_INCLUDED
#define TypeSerializer_ONVIF_DeviceIO_SetAudioOutputConfigurationResponse_INCLUDED


#include "ONVIF/DeviceIO/SetAudioOutputConfigurationResponse.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::DeviceIO::SetAudioOutputConfigurationResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::DeviceIO::SetAudioOutputConfigurationResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/deviceIO/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::DeviceIO::SetAudioOutputConfigurationResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_DeviceIO_SetAudioOutputConfigurationResponse_INCLUDED

