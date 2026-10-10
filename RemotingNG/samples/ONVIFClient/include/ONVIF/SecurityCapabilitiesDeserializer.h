//
// SecurityCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SecurityCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_SecurityCapabilities_INCLUDED


#include "ONVIF/SecurityCapabilities.h"
#include "ONVIF/SecurityCapabilitiesExtensionDeserializer.h"
#include "ONVIF/SecurityCapabilitiesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SecurityCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SecurityCapabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::SecurityCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TLS1.1"s,"TLS1.2"s,"OnboardKeyGeneration"s,"AccessPolicyConfig"s,"X.509Token"s,"SAMLToken"s,"KerberosToken"s,"RELToken"s,"Extension"s};
		bool ret = false;
		bool gen_tLS1_1;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_tLS1_1);
		if (ret) value.setTLS1_1(gen_tLS1_1);
		bool gen_tLS1_2;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_tLS1_2);
		if (ret) value.setTLS1_2(gen_tLS1_2);
		bool gen_onboardKeyGeneration;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_onboardKeyGeneration);
		if (ret) value.setOnboardKeyGeneration(gen_onboardKeyGeneration);
		bool gen_accessPolicyConfig;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[3], true, deser, gen_accessPolicyConfig);
		if (ret) value.setAccessPolicyConfig(gen_accessPolicyConfig);
		bool gen_x_509Token;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[4], true, deser, gen_x_509Token);
		if (ret) value.setX_509Token(gen_x_509Token);
		bool gen_sAMLToken;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[5], true, deser, gen_sAMLToken);
		if (ret) value.setSAMLToken(gen_sAMLToken);
		bool gen_kerberosToken;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[6], true, deser, gen_kerberosToken);
		if (ret) value.setKerberosToken(gen_kerberosToken);
		bool gen_rELToken;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[7], true, deser, gen_rELToken);
		if (ret) value.setRELToken(gen_rELToken);
		Poco::SharedPtr<ONVIF::SecurityCapabilitiesExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SecurityCapabilitiesExtension>>::deserialize(REMOTING__NAMES[8], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SecurityCapabilities_INCLUDED

