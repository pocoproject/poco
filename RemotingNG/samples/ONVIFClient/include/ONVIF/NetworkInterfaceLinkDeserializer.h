//
// NetworkInterfaceLinkDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_NetworkInterfaceLink_INCLUDED
#define TypeDeserializer_ONVIF_NetworkInterfaceLink_INCLUDED


#include "ONVIF/NetworkInterfaceConnectionSettingDeserializer.h"
#include "ONVIF/NetworkInterfaceConnectionSettingSerializer.h"
#include "ONVIF/NetworkInterfaceLink.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::NetworkInterfaceLink>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::NetworkInterfaceLink& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::NetworkInterfaceLink& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AdminSettings"s,"OperSettings"s,"InterfaceType"s};
		bool ret = false;
		ONVIF::NetworkInterfaceConnectionSetting gen_adminSettings;
		ret = TypeDeserializer<ONVIF::NetworkInterfaceConnectionSetting>::deserialize(REMOTING__NAMES[0], true, deser, gen_adminSettings);
		if (ret) value.setAdminSettings(gen_adminSettings);
		ONVIF::NetworkInterfaceConnectionSetting gen_operSettings;
		ret = TypeDeserializer<ONVIF::NetworkInterfaceConnectionSetting>::deserialize(REMOTING__NAMES[1], true, deser, gen_operSettings);
		if (ret) value.setOperSettings(gen_operSettings);
		int gen_interfaceType;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_interfaceType);
		if (ret) value.setInterfaceType(gen_interfaceType);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_NetworkInterfaceLink_INCLUDED

