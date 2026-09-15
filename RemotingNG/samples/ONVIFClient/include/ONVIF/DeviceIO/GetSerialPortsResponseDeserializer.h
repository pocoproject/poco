//
// GetSerialPortsResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_GetSerialPortsResponse_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_GetSerialPortsResponse_INCLUDED


#include "ONVIF/DeviceIO/GetSerialPortsResponse.h"
#include "ONVIF/DeviceIO/SerialPortDeserializer.h"
#include "ONVIF/DeviceIO/SerialPortSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::GetSerialPortsResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::GetSerialPortsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/deviceIO/wsdl"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::GetSerialPortsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SerialPort"s};
		bool ret = false;
		std::vector<ONVIF::DeviceIO::SerialPort> gen_serialPort;
		ret = TypeDeserializer<std::vector<ONVIF::DeviceIO::SerialPort>>::deserialize(REMOTING__NAMES[0], false, deser, gen_serialPort);
		if (ret) value.setSerialPort(gen_serialPort);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_GetSerialPortsResponse_INCLUDED

