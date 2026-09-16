//
// NetworkInterfaceDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_NetworkInterface_INCLUDED
#define TypeDeserializer_ONVIF_NetworkInterface_INCLUDED


#include "ONVIF/IPv4NetworkInterfaceDeserializer.h"
#include "ONVIF/IPv4NetworkInterfaceSerializer.h"
#include "ONVIF/IPv6NetworkInterfaceDeserializer.h"
#include "ONVIF/IPv6NetworkInterfaceSerializer.h"
#include "ONVIF/NetworkInterface.h"
#include "ONVIF/NetworkInterfaceExtensionDeserializer.h"
#include "ONVIF/NetworkInterfaceExtensionSerializer.h"
#include "ONVIF/NetworkInterfaceInfoDeserializer.h"
#include "ONVIF/NetworkInterfaceInfoSerializer.h"
#include "ONVIF/NetworkInterfaceLinkDeserializer.h"
#include "ONVIF/NetworkInterfaceLinkSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::NetworkInterface>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::NetworkInterface& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::NetworkInterface& value)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s};
		bool ret = false;
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES__ONVIF__DEVICEENTITY[0], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
		}
		
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::NetworkInterface& value)
	{
		using namespace std::string_literals;
		
		// ONVIF::DeviceEntity
		{
		}
		
		static const std::string REMOTING__NAMES[] = {"Enabled"s,"Info"s,"Link"s,"IPv4"s,"IPv6"s,"Extension"s};
		bool ret = false;
		bool gen_enabled;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_enabled);
		if (ret) value.setEnabled(gen_enabled);
		Poco::SharedPtr<ONVIF::NetworkInterfaceInfo> gen_info;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkInterfaceInfo>>::deserialize(REMOTING__NAMES[1], false, deser, gen_info);
		if (ret) value.setInfo(gen_info);
		Poco::SharedPtr<ONVIF::NetworkInterfaceLink> gen_link;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkInterfaceLink>>::deserialize(REMOTING__NAMES[2], false, deser, gen_link);
		if (ret) value.setLink(gen_link);
		Poco::SharedPtr<ONVIF::IPv4NetworkInterface> gen_iPv4;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IPv4NetworkInterface>>::deserialize(REMOTING__NAMES[3], false, deser, gen_iPv4);
		if (ret) value.setIPv4(gen_iPv4);
		Poco::SharedPtr<ONVIF::IPv6NetworkInterface> gen_iPv6;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IPv6NetworkInterface>>::deserialize(REMOTING__NAMES[4], false, deser, gen_iPv6);
		if (ret) value.setIPv6(gen_iPv6);
		Poco::SharedPtr<ONVIF::NetworkInterfaceExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkInterfaceExtension>>::deserialize(REMOTING__NAMES[5], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES__ONVIF__DEVICEENTITY[1], REMOTING__NAMES__ONVIF__DEVICEENTITY[0], true);
		}
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_NetworkInterface_INCLUDED

