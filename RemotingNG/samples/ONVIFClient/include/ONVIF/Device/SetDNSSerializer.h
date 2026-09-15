//
// SetDNSSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_SetDNS_INCLUDED
#define TypeSerializer_ONVIF_Device_SetDNS_INCLUDED


#include "ONVIF/Device/SetDNS.h"
#include "ONVIF/IPAddressDeserializer.h"
#include "ONVIF/IPAddressSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::SetDNS>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::SetDNS& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Device::SetDNS& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FromDHCP"s,"SearchDomain"s,"DNSManual"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getFromDHCP(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[1], value.getSearchDomain(), ser);
		TypeSerializer<std::vector<ONVIF::IPAddress>>::serialize(REMOTING__NAMES[2], value.getDNSManual(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_SetDNS_INCLUDED

