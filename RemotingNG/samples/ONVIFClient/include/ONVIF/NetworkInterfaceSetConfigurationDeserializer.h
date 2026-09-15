//
// NetworkInterfaceSetConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_NetworkInterfaceSetConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_NetworkInterfaceSetConfiguration_INCLUDED


#include "ONVIF/IPv4NetworkInterfaceSetConfigurationDeserializer.h"
#include "ONVIF/IPv4NetworkInterfaceSetConfigurationSerializer.h"
#include "ONVIF/IPv6NetworkInterfaceSetConfigurationDeserializer.h"
#include "ONVIF/IPv6NetworkInterfaceSetConfigurationSerializer.h"
#include "ONVIF/NetworkInterfaceConnectionSettingDeserializer.h"
#include "ONVIF/NetworkInterfaceConnectionSettingSerializer.h"
#include "ONVIF/NetworkInterfaceSetConfiguration.h"
#include "ONVIF/NetworkInterfaceSetConfigurationExtensionDeserializer.h"
#include "ONVIF/NetworkInterfaceSetConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::NetworkInterfaceSetConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::NetworkInterfaceSetConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::NetworkInterfaceSetConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Enabled"s,"Link"s,"MTU"s,"IPv4"s,"IPv6"s,"Extension"s};
		bool ret = false;
		Poco::Optional<bool> gen_enabled;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_enabled);
		if (ret) value.setEnabled(gen_enabled);
		Poco::SharedPtr<ONVIF::NetworkInterfaceConnectionSetting> gen_link;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkInterfaceConnectionSetting>>::deserialize(REMOTING__NAMES[1], false, deser, gen_link);
		if (ret) value.setLink(gen_link);
		Poco::Optional<int> gen_mTU;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[2], false, deser, gen_mTU);
		if (ret) value.setMTU(gen_mTU);
		Poco::SharedPtr<ONVIF::IPv4NetworkInterfaceSetConfiguration> gen_iPv4;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IPv4NetworkInterfaceSetConfiguration>>::deserialize(REMOTING__NAMES[3], false, deser, gen_iPv4);
		if (ret) value.setIPv4(gen_iPv4);
		Poco::SharedPtr<ONVIF::IPv6NetworkInterfaceSetConfiguration> gen_iPv6;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IPv6NetworkInterfaceSetConfiguration>>::deserialize(REMOTING__NAMES[4], false, deser, gen_iPv6);
		if (ret) value.setIPv6(gen_iPv6);
		Poco::SharedPtr<ONVIF::NetworkInterfaceSetConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkInterfaceSetConfigurationExtension>>::deserialize(REMOTING__NAMES[5], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_NetworkInterfaceSetConfiguration_INCLUDED

