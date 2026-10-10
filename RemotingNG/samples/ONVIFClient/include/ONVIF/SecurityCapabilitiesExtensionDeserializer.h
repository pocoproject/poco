//
// SecurityCapabilitiesExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SecurityCapabilitiesExtension_INCLUDED
#define TypeDeserializer_ONVIF_SecurityCapabilitiesExtension_INCLUDED


#include "ONVIF/SecurityCapabilitiesExtension.h"
#include "ONVIF/SecurityCapabilitiesExtension2Deserializer.h"
#include "ONVIF/SecurityCapabilitiesExtension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SecurityCapabilitiesExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SecurityCapabilitiesExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::SecurityCapabilitiesExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TLS1.0"s,"Extension"s};
		bool ret = false;
		bool gen_tLS1_0;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_tLS1_0);
		if (ret) value.setTLS1_0(gen_tLS1_0);
		Poco::SharedPtr<ONVIF::SecurityCapabilitiesExtension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SecurityCapabilitiesExtension2>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SecurityCapabilitiesExtension_INCLUDED

