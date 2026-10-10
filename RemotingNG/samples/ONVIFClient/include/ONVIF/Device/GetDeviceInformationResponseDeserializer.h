//
// GetDeviceInformationResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetDeviceInformationResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetDeviceInformationResponse_INCLUDED


#include "ONVIF/Device/GetDeviceInformationResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetDeviceInformationResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetDeviceInformationResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetDeviceInformationResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Manufacturer"s,"Model"s,"FirmwareVersion"s,"SerialNumber"s,"HardwareId"s};
		bool ret = false;
		std::string gen_manufacturer;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_manufacturer);
		if (ret) value.setManufacturer(gen_manufacturer);
		std::string gen_model;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_model);
		if (ret) value.setModel(gen_model);
		std::string gen_firmwareVersion;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[2], true, deser, gen_firmwareVersion);
		if (ret) value.setFirmwareVersion(gen_firmwareVersion);
		std::string gen_serialNumber;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[3], true, deser, gen_serialNumber);
		if (ret) value.setSerialNumber(gen_serialNumber);
		std::string gen_hardwareId;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[4], true, deser, gen_hardwareId);
		if (ret) value.setHardwareId(gen_hardwareId);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetDeviceInformationResponse_INCLUDED

