//
// UserDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_User_INCLUDED
#define TypeDeserializer_ONVIF_User_INCLUDED


#include "ONVIF/User.h"
#include "ONVIF/UserExtensionDeserializer.h"
#include "ONVIF/UserExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::User>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::User& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::User& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Username"s,"Password"s,"UserLevel"s,"Extension"s};
		bool ret = false;
		std::string gen_username;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_username);
		if (ret) value.setUsername(gen_username);
		Poco::Optional<std::string> gen_password;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_password);
		if (ret) value.setPassword(gen_password);
		std::string gen_userLevel;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[2], true, deser, gen_userLevel);
		if (ret) value.setUserLevel(gen_userLevel);
		Poco::SharedPtr<ONVIF::UserExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::UserExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_User_INCLUDED

