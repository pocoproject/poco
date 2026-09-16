//
// NetworkCapabilitiesExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_NetworkCapabilitiesExtension_INCLUDED
#define TypeDeserializer_ONVIF_NetworkCapabilitiesExtension_INCLUDED


#include "ONVIF/NetworkCapabilitiesExtension.h"
#include "ONVIF/NetworkCapabilitiesExtension2Deserializer.h"
#include "ONVIF/NetworkCapabilitiesExtension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::NetworkCapabilitiesExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::NetworkCapabilitiesExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::NetworkCapabilitiesExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dot11Configuration"s,"Extension"s};
		bool ret = false;
		Poco::Optional<bool> gen_dot11Configuration;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_dot11Configuration);
		if (ret) value.setDot11Configuration(gen_dot11Configuration);
		Poco::SharedPtr<ONVIF::NetworkCapabilitiesExtension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkCapabilitiesExtension2>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_NetworkCapabilitiesExtension_INCLUDED

