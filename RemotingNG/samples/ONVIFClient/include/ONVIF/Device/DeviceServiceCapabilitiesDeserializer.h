//
// DeviceServiceCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_DeviceServiceCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_Device_DeviceServiceCapabilities_INCLUDED


#include "ONVIF/Device/DeviceServiceCapabilities.h"
#include "ONVIF/Device/MiscCapabilitiesDeserializer.h"
#include "ONVIF/Device/MiscCapabilitiesSerializer.h"
#include "ONVIF/Device/NetworkCapabilitiesDeserializer.h"
#include "ONVIF/Device/NetworkCapabilitiesSerializer.h"
#include "ONVIF/Device/SecurityCapabilitiesDeserializer.h"
#include "ONVIF/Device/SecurityCapabilitiesSerializer.h"
#include "ONVIF/Device/SystemCapabilitiesDeserializer.h"
#include "ONVIF/Device/SystemCapabilitiesSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::DeviceServiceCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::DeviceServiceCapabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::DeviceServiceCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Network"s,"Security"s,"System"s,"Misc"s};
		bool ret = false;
		ONVIF::Device::NetworkCapabilities gen_network;
		ret = TypeDeserializer<ONVIF::Device::NetworkCapabilities>::deserialize(REMOTING__NAMES[0], true, deser, gen_network);
		if (ret) value.setNetwork(gen_network);
		ONVIF::Device::SecurityCapabilities gen_security;
		ret = TypeDeserializer<ONVIF::Device::SecurityCapabilities>::deserialize(REMOTING__NAMES[1], true, deser, gen_security);
		if (ret) value.setSecurity(gen_security);
		ONVIF::Device::SystemCapabilities gen_system;
		ret = TypeDeserializer<ONVIF::Device::SystemCapabilities>::deserialize(REMOTING__NAMES[2], true, deser, gen_system);
		if (ret) value.setSystem(gen_system);
		Poco::SharedPtr<ONVIF::Device::MiscCapabilities> gen_misc;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Device::MiscCapabilities>>::deserialize(REMOTING__NAMES[3], false, deser, gen_misc);
		if (ret) value.setMisc(gen_misc);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_DeviceServiceCapabilities_INCLUDED

