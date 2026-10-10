//
// SecurityCapabilitiesExtension2Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SecurityCapabilitiesExtension2_INCLUDED
#define TypeDeserializer_ONVIF_SecurityCapabilitiesExtension2_INCLUDED


#include "ONVIF/SecurityCapabilitiesExtension2.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SecurityCapabilitiesExtension2>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SecurityCapabilitiesExtension2& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::SecurityCapabilitiesExtension2& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dot1X"s,"SupportedEAPMethod"s,"RemoteUserHandling"s};
		bool ret = false;
		bool gen_dot1X;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_dot1X);
		if (ret) value.setDot1X(gen_dot1X);
		std::vector<int> gen_supportedEAPMethod;
		ret = TypeDeserializer<std::vector<int>>::deserialize(REMOTING__NAMES[1], false, deser, gen_supportedEAPMethod);
		if (ret) value.setSupportedEAPMethod(gen_supportedEAPMethod);
		bool gen_remoteUserHandling;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_remoteUserHandling);
		if (ret) value.setRemoteUserHandling(gen_remoteUserHandling);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SecurityCapabilitiesExtension2_INCLUDED

