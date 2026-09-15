//
// UserCredentialDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_UserCredential_INCLUDED
#define TypeDeserializer_ONVIF_Device_UserCredential_INCLUDED


#include "ONVIF/Device/Extension_1Deserializer.h"
#include "ONVIF/Device/Extension_1Serializer.h"
#include "ONVIF/Device/UserCredential.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::UserCredential>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::UserCredential& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::UserCredential& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"UserName"s,"Password"s,"Extension"s};
		bool ret = false;
		std::string gen_userName;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_userName);
		if (ret) value.setUserName(gen_userName);
		Poco::Optional<std::string> gen_password;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_password);
		if (ret) value.setPassword(gen_password);
		Poco::SharedPtr<ONVIF::Device::Extension_1> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Device::Extension_1>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_UserCredential_INCLUDED

