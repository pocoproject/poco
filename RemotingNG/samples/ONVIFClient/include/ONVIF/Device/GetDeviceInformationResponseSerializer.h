//
// GetDeviceInformationResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_GetDeviceInformationResponse_INCLUDED
#define TypeSerializer_ONVIF_Device_GetDeviceInformationResponse_INCLUDED


#include "ONVIF/Device/GetDeviceInformationResponse.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::GetDeviceInformationResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::GetDeviceInformationResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::Device::GetDeviceInformationResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Manufacturer"s,"Model"s,"FirmwareVersion"s,"SerialNumber"s,"HardwareId"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getManufacturer(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getModel(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[2], value.getFirmwareVersion(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[3], value.getSerialNumber(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[4], value.getHardwareId(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_GetDeviceInformationResponse_INCLUDED

