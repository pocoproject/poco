//
// NetworkCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_NetworkCapabilities_INCLUDED
#define TypeSerializer_ONVIF_Device_NetworkCapabilities_INCLUDED


#include "ONVIF/Device/NetworkCapabilities.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::NetworkCapabilities>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IPFilter"s,"ZeroConfiguration"s,"IPVersion6"s,"DynDNS"s,"Dot11Configuration"s,"Dot1XConfigurations"s,"HostnameFromDHCP"s,"NTP"s,"DHCPv6"s,"http://www.onvif.org/ver10/device/wsdl"s};
		ser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[3]);
		ser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[4]);
		ser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[5]);
		ser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[6]);
		ser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[7]);
		ser.pushAttribute(REMOTING__NAMES[9], REMOTING__NAMES[8]);
	}

	static void serialize(const std::string& name, const ONVIF::Device::NetworkCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::Device::NetworkCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IPFilter"s,"ZeroConfiguration"s,"IPVersion6"s,"DynDNS"s,"Dot11Configuration"s,"Dot1XConfigurations"s,"HostnameFromDHCP"s,"NTP"s,"DHCPv6"s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getIPFilter(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getZeroConfiguration(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getIPVersion6(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[3], value.getDynDNS(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[4], value.getDot11Configuration(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[5], value.getDot1XConfigurations(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[6], value.getHostnameFromDHCP(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[7], value.getNTP(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[8], value.getDHCPv6(), ser);
	}

	static void serializeImpl(const ONVIF::Device::NetworkCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_NetworkCapabilities_INCLUDED

