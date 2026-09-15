//
// SetNetworkInterfacesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SetNetworkInterfaces_INCLUDED
#define TypeDeserializer_ONVIF_Device_SetNetworkInterfaces_INCLUDED


#include "ONVIF/Device/SetNetworkInterfaces.h"
#include "ONVIF/NetworkInterfaceSetConfigurationDeserializer.h"
#include "ONVIF/NetworkInterfaceSetConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SetNetworkInterfaces>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SetNetworkInterfaces& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SetNetworkInterfaces& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"InterfaceToken"s,"NetworkInterface"s};
		bool ret = false;
		std::string gen_interfaceToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_interfaceToken);
		if (ret) value.setInterfaceToken(gen_interfaceToken);
		ONVIF::NetworkInterfaceSetConfiguration gen_networkInterface;
		ret = TypeDeserializer<ONVIF::NetworkInterfaceSetConfiguration>::deserialize(REMOTING__NAMES[1], true, deser, gen_networkInterface);
		if (ret) value.setNetworkInterface(gen_networkInterface);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SetNetworkInterfaces_INCLUDED

