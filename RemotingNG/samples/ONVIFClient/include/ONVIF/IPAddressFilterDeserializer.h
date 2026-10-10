//
// IPAddressFilterDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_IPAddressFilter_INCLUDED
#define TypeDeserializer_ONVIF_IPAddressFilter_INCLUDED


#include "ONVIF/IPAddressFilter.h"
#include "ONVIF/IPAddressFilterDeserializer.h"
#include "ONVIF/IPAddressFilterExtensionDeserializer.h"
#include "ONVIF/IPAddressFilterExtensionSerializer.h"
#include "ONVIF/IPAddressFilterSerializer.h"
#include "ONVIF/PrefixedIPv4AddressDeserializer.h"
#include "ONVIF/PrefixedIPv4AddressSerializer.h"
#include "ONVIF/PrefixedIPv6AddressDeserializer.h"
#include "ONVIF/PrefixedIPv6AddressSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::IPAddressFilter>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::IPAddressFilter& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::IPAddressFilter& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"IPv4Address"s,"IPv6Address"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::IPAddressFilter> gen_type;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IPAddressFilter>>::deserialize(REMOTING__NAMES[0], true, deser, gen_type);
		if (ret) value.setType(gen_type);
		std::vector<ONVIF::PrefixedIPv4Address> gen_iPv4Address;
		ret = TypeDeserializer<std::vector<ONVIF::PrefixedIPv4Address>>::deserialize(REMOTING__NAMES[1], false, deser, gen_iPv4Address);
		if (ret) value.setIPv4Address(gen_iPv4Address);
		std::vector<ONVIF::PrefixedIPv6Address> gen_iPv6Address;
		ret = TypeDeserializer<std::vector<ONVIF::PrefixedIPv6Address>>::deserialize(REMOTING__NAMES[2], false, deser, gen_iPv6Address);
		if (ret) value.setIPv6Address(gen_iPv6Address);
		Poco::SharedPtr<ONVIF::IPAddressFilterExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IPAddressFilterExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_IPAddressFilter_INCLUDED

