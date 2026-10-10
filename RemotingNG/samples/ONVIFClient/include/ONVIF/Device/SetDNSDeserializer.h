//
// SetDNSDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SetDNS_INCLUDED
#define TypeDeserializer_ONVIF_Device_SetDNS_INCLUDED


#include "ONVIF/Device/SetDNS.h"
#include "ONVIF/IPAddressDeserializer.h"
#include "ONVIF/IPAddressSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SetDNS>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SetDNS& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SetDNS& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FromDHCP"s,"SearchDomain"s,"DNSManual"s};
		bool ret = false;
		bool gen_fromDHCP;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_fromDHCP);
		if (ret) value.setFromDHCP(gen_fromDHCP);
		std::vector<std::string> gen_searchDomain;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_searchDomain);
		if (ret) value.setSearchDomain(gen_searchDomain);
		std::vector<ONVIF::IPAddress> gen_dNSManual;
		ret = TypeDeserializer<std::vector<ONVIF::IPAddress>>::deserialize(REMOTING__NAMES[2], false, deser, gen_dNSManual);
		if (ret) value.setDNSManual(gen_dNSManual);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SetDNS_INCLUDED

