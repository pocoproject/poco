//
// SecurityCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_SecurityCapabilities_INCLUDED
#define TypeSerializer_ONVIF_Device_SecurityCapabilities_INCLUDED


#include "ONVIF/Device/SecurityCapabilities.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::SecurityCapabilities>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TLS1.0"s,"TLS1.1"s,"TLS1.2"s,"OnboardKeyGeneration"s,"AccessPolicyConfig"s,"DefaultAccessPolicy"s,"Dot1X"s,"RemoteUserHandling"s,"X.509Token"s,"SAMLToken"s,"KerberosToken"s,"UsernameToken"s,"HttpDigest"s,"RELToken"s,"SupportedEAPMethods"s,"MaxUsers"s,"MaxUserNameLength"s,"MaxPasswordLength"s,"SecurityPolicies"s,"MaxPasswordHistory"s,"HashingAlgorithms"s,"http://www.onvif.org/ver10/device/wsdl"s};
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[3]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[4]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[5]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[6]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[7]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[8]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[9]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[10]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[11]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[12]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[13]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[14]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[15]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[16]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[17]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[18]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[19]);
		ser.pushAttribute(REMOTING__NAMES[21], REMOTING__NAMES[20]);
	}

	static void serialize(const std::string& name, const ONVIF::Device::SecurityCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::Device::SecurityCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TLS1.0"s,"TLS1.1"s,"TLS1.2"s,"OnboardKeyGeneration"s,"AccessPolicyConfig"s,"DefaultAccessPolicy"s,"Dot1X"s,"RemoteUserHandling"s,"X.509Token"s,"SAMLToken"s,"KerberosToken"s,"UsernameToken"s,"HttpDigest"s,"RELToken"s,"SupportedEAPMethods"s,"MaxUsers"s,"MaxUserNameLength"s,"MaxPasswordLength"s,"SecurityPolicies"s,"MaxPasswordHistory"s,"HashingAlgorithms"s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getTLS1_0(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getTLS1_1(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getTLS1_2(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[3], value.getOnboardKeyGeneration(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[4], value.getAccessPolicyConfig(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[5], value.getDefaultAccessPolicy(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[6], value.getDot1X(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[7], value.getRemoteUserHandling(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[8], value.getX_509Token(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[9], value.getSAMLToken(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[10], value.getKerberosToken(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[11], value.getUsernameToken(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[12], value.getHttpDigest(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[13], value.getRELToken(), ser);
		TypeSerializer<std::vector<std::vector<int>>>::serialize(REMOTING__NAMES[14], value.getSupportedEAPMethods(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[15], value.getMaxUsers(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[16], value.getMaxUserNameLength(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[17], value.getMaxPasswordLength(), ser);
		TypeSerializer<std::vector<std::vector<std::string>>>::serialize(REMOTING__NAMES[18], value.getSecurityPolicies(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[19], value.getMaxPasswordHistory(), ser);
		TypeSerializer<std::vector<std::vector<std::string>>>::serialize(REMOTING__NAMES[20], value.getHashingAlgorithms(), ser);
	}

	static void serializeImpl(const ONVIF::Device::SecurityCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_SecurityCapabilities_INCLUDED

