//
// SetPasswordComplexityConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SetPasswordComplexityConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_Device_SetPasswordComplexityConfiguration_INCLUDED


#include "ONVIF/Device/SetPasswordComplexityConfiguration.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SetPasswordComplexityConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SetPasswordComplexityConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SetPasswordComplexityConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MinLen"s,"Uppercase"s,"Number"s,"SpecialChars"s,"BlockUsernameOccurrence"s,"PolicyConfigurationLocked"s};
		bool ret = false;
		Poco::Optional<int> gen_minLen;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_minLen);
		if (ret) value.setMinLen(gen_minLen);
		Poco::Optional<int> gen_uppercase;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[1], false, deser, gen_uppercase);
		if (ret) value.setUppercase(gen_uppercase);
		Poco::Optional<int> gen_number;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[2], false, deser, gen_number);
		if (ret) value.setNumber(gen_number);
		Poco::Optional<int> gen_specialChars;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[3], false, deser, gen_specialChars);
		if (ret) value.setSpecialChars(gen_specialChars);
		Poco::Optional<bool> gen_blockUsernameOccurrence;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[4], false, deser, gen_blockUsernameOccurrence);
		if (ret) value.setBlockUsernameOccurrence(gen_blockUsernameOccurrence);
		Poco::Optional<bool> gen_policyConfigurationLocked;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[5], false, deser, gen_policyConfigurationLocked);
		if (ret) value.setPolicyConfigurationLocked(gen_policyConfigurationLocked);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SetPasswordComplexityConfiguration_INCLUDED

