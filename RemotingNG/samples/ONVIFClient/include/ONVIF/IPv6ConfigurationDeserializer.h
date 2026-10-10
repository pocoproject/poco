//
// IPv6ConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_IPv6Configuration_INCLUDED
#define TypeDeserializer_ONVIF_IPv6Configuration_INCLUDED


#include "ONVIF/IPv6Configuration.h"
#include "ONVIF/IPv6ConfigurationExtensionDeserializer.h"
#include "ONVIF/IPv6ConfigurationExtensionSerializer.h"
#include "ONVIF/PrefixedIPv6AddressDeserializer.h"
#include "ONVIF/PrefixedIPv6AddressSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::IPv6Configuration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::IPv6Configuration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::IPv6Configuration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AcceptRouterAdvert"s,"DHCP"s,"Manual"s,"LinkLocal"s,"FromDHCP"s,"FromRA"s,"Extension"s};
		bool ret = false;
		Poco::Optional<bool> gen_acceptRouterAdvert;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_acceptRouterAdvert);
		if (ret) value.setAcceptRouterAdvert(gen_acceptRouterAdvert);
		std::string gen_dHCP;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_dHCP);
		if (ret) value.setDHCP(gen_dHCP);
		std::vector<ONVIF::PrefixedIPv6Address> gen_manual;
		ret = TypeDeserializer<std::vector<ONVIF::PrefixedIPv6Address>>::deserialize(REMOTING__NAMES[2], false, deser, gen_manual);
		if (ret) value.setManual(gen_manual);
		std::vector<ONVIF::PrefixedIPv6Address> gen_linkLocal;
		ret = TypeDeserializer<std::vector<ONVIF::PrefixedIPv6Address>>::deserialize(REMOTING__NAMES[3], false, deser, gen_linkLocal);
		if (ret) value.setLinkLocal(gen_linkLocal);
		std::vector<ONVIF::PrefixedIPv6Address> gen_fromDHCP;
		ret = TypeDeserializer<std::vector<ONVIF::PrefixedIPv6Address>>::deserialize(REMOTING__NAMES[4], false, deser, gen_fromDHCP);
		if (ret) value.setFromDHCP(gen_fromDHCP);
		std::vector<ONVIF::PrefixedIPv6Address> gen_fromRA;
		ret = TypeDeserializer<std::vector<ONVIF::PrefixedIPv6Address>>::deserialize(REMOTING__NAMES[5], false, deser, gen_fromRA);
		if (ret) value.setFromRA(gen_fromRA);
		Poco::SharedPtr<ONVIF::IPv6ConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IPv6ConfigurationExtension>>::deserialize(REMOTING__NAMES[6], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_IPv6Configuration_INCLUDED

