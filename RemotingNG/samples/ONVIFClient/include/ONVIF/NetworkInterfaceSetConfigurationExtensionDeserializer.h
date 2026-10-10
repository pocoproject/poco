//
// NetworkInterfaceSetConfigurationExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_NetworkInterfaceSetConfigurationExtension_INCLUDED
#define TypeDeserializer_ONVIF_NetworkInterfaceSetConfigurationExtension_INCLUDED


#include "ONVIF/Dot11ConfigurationDeserializer.h"
#include "ONVIF/Dot11ConfigurationSerializer.h"
#include "ONVIF/Dot3ConfigurationDeserializer.h"
#include "ONVIF/Dot3ConfigurationSerializer.h"
#include "ONVIF/NetworkInterfaceSetConfigurationExtension.h"
#include "ONVIF/NetworkInterfaceSetConfigurationExtension2Deserializer.h"
#include "ONVIF/NetworkInterfaceSetConfigurationExtension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::NetworkInterfaceSetConfigurationExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::NetworkInterfaceSetConfigurationExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::NetworkInterfaceSetConfigurationExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dot3"s,"Dot11"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::Dot3Configuration> gen_dot3;
		ret = TypeDeserializer<std::vector<ONVIF::Dot3Configuration>>::deserialize(REMOTING__NAMES[0], false, deser, gen_dot3);
		if (ret) value.setDot3(gen_dot3);
		std::vector<ONVIF::Dot11Configuration> gen_dot11;
		ret = TypeDeserializer<std::vector<ONVIF::Dot11Configuration>>::deserialize(REMOTING__NAMES[1], false, deser, gen_dot11);
		if (ret) value.setDot11(gen_dot11);
		Poco::SharedPtr<ONVIF::NetworkInterfaceSetConfigurationExtension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkInterfaceSetConfigurationExtension2>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_NetworkInterfaceSetConfigurationExtension_INCLUDED

