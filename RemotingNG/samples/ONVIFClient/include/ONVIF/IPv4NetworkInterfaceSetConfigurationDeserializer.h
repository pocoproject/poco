//
// IPv4NetworkInterfaceSetConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_IPv4NetworkInterfaceSetConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_IPv4NetworkInterfaceSetConfiguration_INCLUDED


#include "ONVIF/IPv4NetworkInterfaceSetConfiguration.h"
#include "ONVIF/PrefixedIPv4AddressDeserializer.h"
#include "ONVIF/PrefixedIPv4AddressSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::IPv4NetworkInterfaceSetConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::IPv4NetworkInterfaceSetConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::IPv4NetworkInterfaceSetConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Enabled"s,"Manual"s,"DHCP"s};
		bool ret = false;
		Poco::Optional<bool> gen_enabled;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_enabled);
		if (ret) value.setEnabled(gen_enabled);
		std::vector<ONVIF::PrefixedIPv4Address> gen_manual;
		ret = TypeDeserializer<std::vector<ONVIF::PrefixedIPv4Address>>::deserialize(REMOTING__NAMES[1], false, deser, gen_manual);
		if (ret) value.setManual(gen_manual);
		Poco::Optional<bool> gen_dHCP;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_dHCP);
		if (ret) value.setDHCP(gen_dHCP);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_IPv4NetworkInterfaceSetConfiguration_INCLUDED

