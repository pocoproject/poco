//
// NetworkHostDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_NetworkHost_INCLUDED
#define TypeDeserializer_ONVIF_NetworkHost_INCLUDED


#include "ONVIF/NetworkHost.h"
#include "ONVIF/NetworkHostDeserializer.h"
#include "ONVIF/NetworkHostExtensionDeserializer.h"
#include "ONVIF/NetworkHostExtensionSerializer.h"
#include "ONVIF/NetworkHostSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::NetworkHost>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::NetworkHost& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::NetworkHost& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"IPv4Address"s,"IPv6Address"s,"DNSname"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::NetworkHost> gen_type;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkHost>>::deserialize(REMOTING__NAMES[0], true, deser, gen_type);
		if (ret) value.setType(gen_type);
		Poco::Optional<std::string> gen_iPv4Address;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_iPv4Address);
		if (ret) value.setIPv4Address(gen_iPv4Address);
		Poco::Optional<std::string> gen_iPv6Address;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_iPv6Address);
		if (ret) value.setIPv6Address(gen_iPv6Address);
		Poco::Optional<std::string> gen_dNSname;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[3], false, deser, gen_dNSname);
		if (ret) value.setDNSname(gen_dNSname);
		Poco::SharedPtr<ONVIF::NetworkHostExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkHostExtension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_NetworkHost_INCLUDED

