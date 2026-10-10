//
// NetworkCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_NetworkCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_Device_NetworkCapabilities_INCLUDED


#include "ONVIF/Device/NetworkCapabilities.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::NetworkCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::NetworkCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::Device::NetworkCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IPFilter"s,"ZeroConfiguration"s,"IPVersion6"s,"DynDNS"s,"Dot11Configuration"s,"Dot1XConfigurations"s,"HostnameFromDHCP"s,"NTP"s,"DHCPv6"s};
		bool ret = false;
		Poco::Optional<bool> gen_iPFilter;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_iPFilter);
		if (ret) value.setIPFilter(gen_iPFilter);
		Poco::Optional<bool> gen_zeroConfiguration;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_zeroConfiguration);
		if (ret) value.setZeroConfiguration(gen_zeroConfiguration);
		Poco::Optional<bool> gen_iPVersion6;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_iPVersion6);
		if (ret) value.setIPVersion6(gen_iPVersion6);
		Poco::Optional<bool> gen_dynDNS;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[3], false, deser, gen_dynDNS);
		if (ret) value.setDynDNS(gen_dynDNS);
		Poco::Optional<bool> gen_dot11Configuration;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[4], false, deser, gen_dot11Configuration);
		if (ret) value.setDot11Configuration(gen_dot11Configuration);
		Poco::Optional<int> gen_dot1XConfigurations;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[5], false, deser, gen_dot1XConfigurations);
		if (ret) value.setDot1XConfigurations(gen_dot1XConfigurations);
		Poco::Optional<bool> gen_hostnameFromDHCP;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[6], false, deser, gen_hostnameFromDHCP);
		if (ret) value.setHostnameFromDHCP(gen_hostnameFromDHCP);
		Poco::Optional<int> gen_nTP;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[7], false, deser, gen_nTP);
		if (ret) value.setNTP(gen_nTP);
		Poco::Optional<bool> gen_dHCPv6;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[8], false, deser, gen_dHCPv6);
		if (ret) value.setDHCPv6(gen_dHCPv6);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::NetworkCapabilities& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IPFilter"s,"ZeroConfiguration"s,"IPVersion6"s,"DynDNS"s,"Dot11Configuration"s,"Dot1XConfigurations"s,"HostnameFromDHCP"s,"NTP"s,"DHCPv6"s,"http://www.onvif.org/ver10/device/wsdl"s};
		deser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[2], false);
		deser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[3], false);
		deser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[4], false);
		deser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[5], false);
		deser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[6], false);
		deser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[7], false);
		deser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[8], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_NetworkCapabilities_INCLUDED

