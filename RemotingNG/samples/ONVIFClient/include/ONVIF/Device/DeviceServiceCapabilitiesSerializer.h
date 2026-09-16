//
// DeviceServiceCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_DeviceServiceCapabilities_INCLUDED
#define TypeSerializer_ONVIF_Device_DeviceServiceCapabilities_INCLUDED


#include "ONVIF/Device/DeviceServiceCapabilities.h"
#include "ONVIF/Device/MiscCapabilitiesDeserializer.h"
#include "ONVIF/Device/MiscCapabilitiesSerializer.h"
#include "ONVIF/Device/NetworkCapabilitiesDeserializer.h"
#include "ONVIF/Device/NetworkCapabilitiesSerializer.h"
#include "ONVIF/Device/SecurityCapabilitiesDeserializer.h"
#include "ONVIF/Device/SecurityCapabilitiesSerializer.h"
#include "ONVIF/Device/SystemCapabilitiesDeserializer.h"
#include "ONVIF/Device/SystemCapabilitiesSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::DeviceServiceCapabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::DeviceServiceCapabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Device::DeviceServiceCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Network"s,"Security"s,"System"s,"Misc"s,""s};
		TypeSerializer<ONVIF::Device::NetworkCapabilities>::serialize(REMOTING__NAMES[0], value.getNetwork(), ser);
		TypeSerializer<ONVIF::Device::SecurityCapabilities>::serialize(REMOTING__NAMES[1], value.getSecurity(), ser);
		TypeSerializer<ONVIF::Device::SystemCapabilities>::serialize(REMOTING__NAMES[2], value.getSystem(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Device::MiscCapabilities>>::serialize(REMOTING__NAMES[3], value.getMisc(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_DeviceServiceCapabilities_INCLUDED

