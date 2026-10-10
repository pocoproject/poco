//
// GetPasswordComplexityConfigurationResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_GetPasswordComplexityConfigurationResponse_INCLUDED
#define TypeSerializer_ONVIF_Device_GetPasswordComplexityConfigurationResponse_INCLUDED


#include "ONVIF/Device/GetPasswordComplexityConfigurationResponse.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::GetPasswordComplexityConfigurationResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::GetPasswordComplexityConfigurationResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::Device::GetPasswordComplexityConfigurationResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MinLen"s,"Uppercase"s,"Number"s,"SpecialChars"s,"BlockUsernameOccurrence"s,"PolicyConfigurationLocked"s,""s};
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[0], value.getMinLen(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[1], value.getUppercase(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[2], value.getNumber(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[3], value.getSpecialChars(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[4], value.getBlockUsernameOccurrence(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[5], value.getPolicyConfigurationLocked(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_GetPasswordComplexityConfigurationResponse_INCLUDED

