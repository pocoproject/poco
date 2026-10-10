//
// SetDPAddressesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SetDPAddresses_INCLUDED
#define TypeDeserializer_ONVIF_Device_SetDPAddresses_INCLUDED


#include "ONVIF/Device/SetDPAddresses.h"
#include "ONVIF/NetworkHostDeserializer.h"
#include "ONVIF/NetworkHostSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SetDPAddresses>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SetDPAddresses& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SetDPAddresses& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DPAddress"s};
		bool ret = false;
		std::vector<ONVIF::NetworkHost> gen_dPAddress;
		ret = TypeDeserializer<std::vector<ONVIF::NetworkHost>>::deserialize(REMOTING__NAMES[0], false, deser, gen_dPAddress);
		if (ret) value.setDPAddress(gen_dPAddress);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SetDPAddresses_INCLUDED

