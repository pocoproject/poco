//
// SetSerialPortConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_DeviceIO_SetSerialPortConfiguration_INCLUDED
#define TypeSerializer_ONVIF_DeviceIO_SetSerialPortConfiguration_INCLUDED


#include "ONVIF/DeviceIO/SerialPortConfigurationDeserializer.h"
#include "ONVIF/DeviceIO/SerialPortConfigurationSerializer.h"
#include "ONVIF/DeviceIO/SetSerialPortConfiguration.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::DeviceIO::SetSerialPortConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::DeviceIO::SetSerialPortConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::DeviceIO::SetSerialPortConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SerialPortConfiguration"s,"ForcePersistance"s,""s};
		TypeSerializer<ONVIF::DeviceIO::SerialPortConfiguration>::serialize(REMOTING__NAMES[0], value.getSerialPortConfiguration(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getForcePersistance(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_DeviceIO_SetSerialPortConfiguration_INCLUDED

