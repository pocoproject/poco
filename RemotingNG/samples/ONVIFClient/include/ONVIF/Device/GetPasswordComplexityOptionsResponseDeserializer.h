//
// GetPasswordComplexityOptionsResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetPasswordComplexityOptionsResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetPasswordComplexityOptionsResponse_INCLUDED


#include "ONVIF/Device/GetPasswordComplexityOptionsResponse.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetPasswordComplexityOptionsResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetPasswordComplexityOptionsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetPasswordComplexityOptionsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MinLenRange"s,"UppercaseRange"s,"NumberRange"s,"SpecialCharsRange"s,"BlockUsernameOccurrenceSupported"s,"PolicyConfigurationLockSupported"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::IntRange> gen_minLenRange;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IntRange>>::deserialize(REMOTING__NAMES[0], false, deser, gen_minLenRange);
		if (ret) value.setMinLenRange(gen_minLenRange);
		Poco::SharedPtr<ONVIF::IntRange> gen_uppercaseRange;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IntRange>>::deserialize(REMOTING__NAMES[1], false, deser, gen_uppercaseRange);
		if (ret) value.setUppercaseRange(gen_uppercaseRange);
		Poco::SharedPtr<ONVIF::IntRange> gen_numberRange;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IntRange>>::deserialize(REMOTING__NAMES[2], false, deser, gen_numberRange);
		if (ret) value.setNumberRange(gen_numberRange);
		Poco::SharedPtr<ONVIF::IntRange> gen_specialCharsRange;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IntRange>>::deserialize(REMOTING__NAMES[3], false, deser, gen_specialCharsRange);
		if (ret) value.setSpecialCharsRange(gen_specialCharsRange);
		Poco::Optional<bool> gen_blockUsernameOccurrenceSupported;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[4], false, deser, gen_blockUsernameOccurrenceSupported);
		if (ret) value.setBlockUsernameOccurrenceSupported(gen_blockUsernameOccurrenceSupported);
		Poco::Optional<bool> gen_policyConfigurationLockSupported;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[5], false, deser, gen_policyConfigurationLockSupported);
		if (ret) value.setPolicyConfigurationLockSupported(gen_policyConfigurationLockSupported);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetPasswordComplexityOptionsResponse_INCLUDED

