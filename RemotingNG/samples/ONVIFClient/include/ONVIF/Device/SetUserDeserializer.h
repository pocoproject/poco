//
// SetUserDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SetUser_INCLUDED
#define TypeDeserializer_ONVIF_Device_SetUser_INCLUDED


#include "ONVIF/Device/SetUser.h"
#include "ONVIF/UserDeserializer.h"
#include "ONVIF/UserSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SetUser>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SetUser& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SetUser& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"User"s};
		bool ret = false;
		std::vector<ONVIF::User> gen_user;
		ret = TypeDeserializer<std::vector<ONVIF::User>>::deserialize(REMOTING__NAMES[0], true, deser, gen_user);
		if (ret) value.setUser(gen_user);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SetUser_INCLUDED

