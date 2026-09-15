//
// NetworkZeroConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_NetworkZeroConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_NetworkZeroConfiguration_INCLUDED


#include "ONVIF/NetworkZeroConfiguration.h"
#include "ONVIF/NetworkZeroConfigurationExtensionDeserializer.h"
#include "ONVIF/NetworkZeroConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::NetworkZeroConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::NetworkZeroConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::NetworkZeroConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"InterfaceToken"s,"Enabled"s,"Addresses"s,"Extension"s};
		bool ret = false;
		std::string gen_interfaceToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_interfaceToken);
		if (ret) value.setInterfaceToken(gen_interfaceToken);
		bool gen_enabled;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_enabled);
		if (ret) value.setEnabled(gen_enabled);
		std::vector<std::string> gen_addresses;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_addresses);
		if (ret) value.setAddresses(gen_addresses);
		Poco::SharedPtr<ONVIF::NetworkZeroConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkZeroConfigurationExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_NetworkZeroConfiguration_INCLUDED

