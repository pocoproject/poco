//
// IPv4ConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_IPv4Configuration_INCLUDED
#define TypeDeserializer_ONVIF_IPv4Configuration_INCLUDED


#include "ONVIF/IPv4Configuration.h"
#include "ONVIF/PrefixedIPv4AddressDeserializer.h"
#include "ONVIF/PrefixedIPv4AddressSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::IPv4Configuration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::IPv4Configuration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::IPv4Configuration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Manual"s,"LinkLocal"s,"FromDHCP"s,"DHCP"s};
		bool ret = false;
		std::vector<ONVIF::PrefixedIPv4Address> gen_manual;
		ret = TypeDeserializer<std::vector<ONVIF::PrefixedIPv4Address>>::deserialize(REMOTING__NAMES[0], false, deser, gen_manual);
		if (ret) value.setManual(gen_manual);
		Poco::SharedPtr<ONVIF::PrefixedIPv4Address> gen_linkLocal;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PrefixedIPv4Address>>::deserialize(REMOTING__NAMES[1], false, deser, gen_linkLocal);
		if (ret) value.setLinkLocal(gen_linkLocal);
		Poco::SharedPtr<ONVIF::PrefixedIPv4Address> gen_fromDHCP;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PrefixedIPv4Address>>::deserialize(REMOTING__NAMES[2], false, deser, gen_fromDHCP);
		if (ret) value.setFromDHCP(gen_fromDHCP);
		bool gen_dHCP;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[3], true, deser, gen_dHCP);
		if (ret) value.setDHCP(gen_dHCP);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_IPv4Configuration_INCLUDED

