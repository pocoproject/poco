//
// SecurityCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SecurityCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_Device_SecurityCapabilities_INCLUDED


#include "ONVIF/Device/SecurityCapabilities.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SecurityCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SecurityCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::Device::SecurityCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TLS1.0"s,"TLS1.1"s,"TLS1.2"s,"OnboardKeyGeneration"s,"AccessPolicyConfig"s,"DefaultAccessPolicy"s,"Dot1X"s,"RemoteUserHandling"s,"X.509Token"s,"SAMLToken"s,"KerberosToken"s,"UsernameToken"s,"HttpDigest"s,"RELToken"s,"SupportedEAPMethods"s,"MaxUsers"s,"MaxUserNameLength"s,"MaxPasswordLength"s,"SecurityPolicies"s,"MaxPasswordHistory"s,"HashingAlgorithms"s};
		bool ret = false;
		Poco::Optional<bool> gen_tLS1_0;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_tLS1_0);
		if (ret) value.setTLS1_0(gen_tLS1_0);
		Poco::Optional<bool> gen_tLS1_1;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_tLS1_1);
		if (ret) value.setTLS1_1(gen_tLS1_1);
		Poco::Optional<bool> gen_tLS1_2;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_tLS1_2);
		if (ret) value.setTLS1_2(gen_tLS1_2);
		Poco::Optional<bool> gen_onboardKeyGeneration;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[3], false, deser, gen_onboardKeyGeneration);
		if (ret) value.setOnboardKeyGeneration(gen_onboardKeyGeneration);
		Poco::Optional<bool> gen_accessPolicyConfig;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[4], false, deser, gen_accessPolicyConfig);
		if (ret) value.setAccessPolicyConfig(gen_accessPolicyConfig);
		Poco::Optional<bool> gen_defaultAccessPolicy;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[5], false, deser, gen_defaultAccessPolicy);
		if (ret) value.setDefaultAccessPolicy(gen_defaultAccessPolicy);
		Poco::Optional<bool> gen_dot1X;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[6], false, deser, gen_dot1X);
		if (ret) value.setDot1X(gen_dot1X);
		Poco::Optional<bool> gen_remoteUserHandling;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[7], false, deser, gen_remoteUserHandling);
		if (ret) value.setRemoteUserHandling(gen_remoteUserHandling);
		Poco::Optional<bool> gen_x_509Token;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[8], false, deser, gen_x_509Token);
		if (ret) value.setX_509Token(gen_x_509Token);
		Poco::Optional<bool> gen_sAMLToken;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[9], false, deser, gen_sAMLToken);
		if (ret) value.setSAMLToken(gen_sAMLToken);
		Poco::Optional<bool> gen_kerberosToken;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[10], false, deser, gen_kerberosToken);
		if (ret) value.setKerberosToken(gen_kerberosToken);
		Poco::Optional<bool> gen_usernameToken;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[11], false, deser, gen_usernameToken);
		if (ret) value.setUsernameToken(gen_usernameToken);
		Poco::Optional<bool> gen_httpDigest;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[12], false, deser, gen_httpDigest);
		if (ret) value.setHttpDigest(gen_httpDigest);
		Poco::Optional<bool> gen_rELToken;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[13], false, deser, gen_rELToken);
		if (ret) value.setRELToken(gen_rELToken);
		std::vector<std::vector<int>> gen_supportedEAPMethods;
		ret = TypeDeserializer<std::vector<std::vector<int>>>::deserialize(REMOTING__NAMES[14], false, deser, gen_supportedEAPMethods);
		if (ret) value.setSupportedEAPMethods(gen_supportedEAPMethods);
		Poco::Optional<int> gen_maxUsers;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[15], false, deser, gen_maxUsers);
		if (ret) value.setMaxUsers(gen_maxUsers);
		Poco::Optional<int> gen_maxUserNameLength;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[16], false, deser, gen_maxUserNameLength);
		if (ret) value.setMaxUserNameLength(gen_maxUserNameLength);
		Poco::Optional<int> gen_maxPasswordLength;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[17], false, deser, gen_maxPasswordLength);
		if (ret) value.setMaxPasswordLength(gen_maxPasswordLength);
		std::vector<std::vector<std::string>> gen_securityPolicies;
		ret = TypeDeserializer<std::vector<std::vector<std::string>>>::deserialize(REMOTING__NAMES[18], false, deser, gen_securityPolicies);
		if (ret) value.setSecurityPolicies(gen_securityPolicies);
		Poco::Optional<int> gen_maxPasswordHistory;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[19], false, deser, gen_maxPasswordHistory);
		if (ret) value.setMaxPasswordHistory(gen_maxPasswordHistory);
		std::vector<std::vector<std::string>> gen_hashingAlgorithms;
		ret = TypeDeserializer<std::vector<std::vector<std::string>>>::deserialize(REMOTING__NAMES[20], false, deser, gen_hashingAlgorithms);
		if (ret) value.setHashingAlgorithms(gen_hashingAlgorithms);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SecurityCapabilities& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TLS1.0"s,"TLS1.1"s,"TLS1.2"s,"OnboardKeyGeneration"s,"AccessPolicyConfig"s,"DefaultAccessPolicy"s,"Dot1X"s,"RemoteUserHandling"s,"X.509Token"s,"SAMLToken"s,"KerberosToken"s,"UsernameToken"s,"HttpDigest"s,"RELToken"s,"SupportedEAPMethods"s,"MaxUsers"s,"MaxUserNameLength"s,"MaxPasswordLength"s,"SecurityPolicies"s,"MaxPasswordHistory"s,"HashingAlgorithms"s,"http://www.onvif.org/ver10/device/wsdl"s};
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[2], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[3], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[4], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[5], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[6], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[7], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[8], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[9], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[10], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[11], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[12], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[13], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[14], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[15], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[16], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[17], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[18], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[19], false);
		deser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[20], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SecurityCapabilities_INCLUDED

