//
// GetPasswordComplexityOptionsResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_GetPasswordComplexityOptionsResponse_INCLUDED
#define TypeSerializer_ONVIF_Device_GetPasswordComplexityOptionsResponse_INCLUDED


#include "ONVIF/Device/GetPasswordComplexityOptionsResponse.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::GetPasswordComplexityOptionsResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::GetPasswordComplexityOptionsResponse& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Device::GetPasswordComplexityOptionsResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MinLenRange"s,"UppercaseRange"s,"NumberRange"s,"SpecialCharsRange"s,"BlockUsernameOccurrenceSupported"s,"PolicyConfigurationLockSupported"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::IntRange>>::serialize(REMOTING__NAMES[0], value.getMinLenRange(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IntRange>>::serialize(REMOTING__NAMES[1], value.getUppercaseRange(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IntRange>>::serialize(REMOTING__NAMES[2], value.getNumberRange(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IntRange>>::serialize(REMOTING__NAMES[3], value.getSpecialCharsRange(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[4], value.getBlockUsernameOccurrenceSupported(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[5], value.getPolicyConfigurationLockSupported(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_GetPasswordComplexityOptionsResponse_INCLUDED

